using Microsoft.VisualBasic;
using System;
using System.Threading;
using static System.Windows.Forms.LinkLabel;
using static System.Windows.Forms.VisualStyles.VisualStyleElement;

namespace PICkitS;

public class USBWrite {
	public delegate void GUINotifierLockUp();

	private const byte EVENT_TIMER_RESET_V = 32;

	private static Thread m_write_thread;

	private static Thread m_clear_status_errors;

	private static volatile bool m_we_are_in_write_loop;

	private static volatile bool m_we_had_an_error_writing;

	private static Mutex m_cbuf1_avail_bytes_mutex = new Mutex(initiallyOwned: false);

	private static AutoResetEvent m_write_thread_has_started_up_event;

	private static AutoResetEvent m_ready_to_write_event;

	private static AutoResetEvent m_have_written_event;

	private static Mutex m_last_script_array_mutex;

	private static object m_priv_write_buffer_lock = new object();

	private static object m_priv_write_script_lock = new object();

	private static byte[] m_command_byte_array;

	private static byte[] m_last_script_array;

	private static byte m_last_script_array_byte_count;

	private static byte m_cbuf1_avail_bytes;

	private static volatile bool m_data_buffer_is_empty;

	public static int m_universal_timeout = 3000;

	internal static volatile bool m_use_script_timeout = true;

	public static event GUINotifierLockUp Tell_Host_PKSA_Needs_Reset;

	public static void Initialize_Write_Objects() {
		m_we_are_in_write_loop = false;
		m_we_had_an_error_writing = false;
		m_cbuf1_avail_bytes = 0;
		m_data_buffer_is_empty = true;
		m_command_byte_array = new byte[65];
		m_last_script_array = new byte[20480];
		Array.Clear(m_last_script_array, 0, m_last_script_array.Length);
		Array.Clear(m_command_byte_array, 0, m_command_byte_array.Length);
		m_last_script_array_byte_count = 0;
		m_command_byte_array[0] = 0;
		m_command_byte_array[1] = 1;
		m_command_byte_array[2] = 0;
		m_command_byte_array[3] = 0;
		m_command_byte_array[4] = 0;
		m_write_thread_has_started_up_event = new AutoResetEvent(initialState: false);
		m_ready_to_write_event = new AutoResetEvent(initialState: false);
		m_have_written_event = new AutoResetEvent(initialState: false);
		m_priv_write_buffer_lock = new object();
		m_priv_write_script_lock = new object();
		m_last_script_array_mutex = new Mutex(initiallyOwned: false);
	}

	public static bool Update_Status_Packet() {
		bool result = false;
		if (Utilities.m_flags.HID_read_handle != IntPtr.Zero && USBRead.Read_Thread_Is_Active()) {
			Utilities.m_flags.g_status_packet_data_update_event.Reset();
			if (Send_Status_Request() && Utilities.m_flags.g_status_packet_data_update_event.WaitOne(2000, exitContext: false)) {
				result = true;
			}
		}

		return result;
	}

	public static bool Update_Special_Status_Packet() {
		bool result = false;
		if (Utilities.m_flags.HID_read_handle != IntPtr.Zero && USBRead.Read_Thread_Is_Active()) {
			Utilities.m_flags.g_special_status_request_event.Reset();
			if (Send_Special_Status_Request() && Utilities.m_flags.g_special_status_request_event.WaitOne(2000, exitContext: false)) {
				result = true;
			}
		}

		return result;
	}

	private static bool there_is_room_in_cbuf1(byte p_num_bytes_to_write) {
		bool result = false;
		int num = 0;
		if ((uint)p_num_bytes_to_write > 255u) {
			return result;
		}

		if (p_num_bytes_to_write > m_cbuf1_avail_bytes) {
			while (p_num_bytes_to_write >= m_cbuf1_avail_bytes && num++ < 6) {
				m_cbuf1_avail_bytes_mutex.WaitOne();
				m_cbuf1_avail_bytes = this_many_bytes_are_actually_available_in_cbuf1();
				m_cbuf1_avail_bytes_mutex.ReleaseMutex();
				if (p_num_bytes_to_write < m_cbuf1_avail_bytes) {
					result = true;
					break;
				}

				Thread.Sleep(100);
			}
		} else {
			result = true;
		}

		return result;
	}

	private static byte this_many_bytes_are_actually_available_in_cbuf1() {
		byte result = 0;
		if (Update_Special_Status_Packet()) {
			Utilities.m_flags.g_status_packet_mutex.WaitOne();
			result = Constants.STATUS_PACKET_DATA[54];
			Utilities.m_flags.g_status_packet_mutex.ReleaseMutex();
		}

		return result;
	}

	public static bool kick_off_write_thread() {
		bool flag = true;
		if (!m_we_are_in_write_loop) {
			m_write_thread = new Thread(Write_USB_Thread);
			m_write_thread.IsBackground = true;
			m_write_thread.Start();
			return m_write_thread_has_started_up_event.WaitOne(5000, exitContext: false);
		}

		return false;
	}

	public static bool There_Was_A_Write_Error() {
		return m_we_had_an_error_writing;
	}

	private static bool We_Are_Done_Writing_Data() {
		return m_data_buffer_is_empty;
	}

	public static bool Transaction_Is_Complete() {
		bool result = false;
		if (We_Are_Done_Writing_Data() && !USBRead.m_read_thread_is_processing_a_USB_packet && Update_Status_Packet()) {
			Utilities.m_flags.g_status_packet_mutex.WaitOne();
			if ((Constants.STATUS_PACKET_DATA[37] & 1) == 0 && Constants.STATUS_PACKET_DATA[55] == 0) {
				result = true;
			}

			Utilities.m_flags.g_status_packet_mutex.ReleaseMutex();
		}

		return result;
	}

	public static bool Write_Thread_Is_Active() {
		return m_we_are_in_write_loop;
	}

	public static void Dispose_Of_Write_Objects() {
		m_write_thread_has_started_up_event.Close();
		m_ready_to_write_event.Close();
		m_have_written_event.Close();
		m_cbuf1_avail_bytes_mutex.Close();
	}

	public static void Kill_Write_Thread() {
		if (m_write_thread != null && m_write_thread.IsAlive) {
			m_we_are_in_write_loop = false;
			m_write_thread.Join();
		}
	}

	public static bool Send_Data_Packet_To_PICkitS(ref byte[] p_data) {
		int num = 0;
		bool result = true;
		lock (m_priv_write_buffer_lock) {
			if (m_we_are_in_write_loop && Utilities.m_flags.HID_read_handle != IntPtr.Zero) {
				for (num = 0; num < Utilities.m_flags.write_buffer.Length; num++) {
					Utilities.m_flags.write_buffer[num] = p_data[num];
				}

				m_ready_to_write_event.Set();
				if (!m_have_written_event.WaitOne(3000, exitContext: false)) {
					issue_Tell_Host_PKSA_Needs_Reset();
				}

				if (There_Was_A_Write_Error()) {
					result = false;
				}
			} else {
				_ = $"Error writing to USB device";
				result = false;
			}
		}

		return result;
	}

	private static void issue_Tell_Host_PKSA_Needs_Reset() {
		if (USBWrite.Tell_Host_PKSA_Needs_Reset != null) {
			USBWrite.Tell_Host_PKSA_Needs_Reset();
		}
	}

	public static void Write_USB_Thread() {
		m_we_are_in_write_loop = true;
		m_write_thread_has_started_up_event.Set();
		bool flag = false;
		int numBytesWritten = 0;
		while (m_we_are_in_write_loop) {
			if (m_ready_to_write_event.WaitOne(500, exitContext: false)) {
				flag = Utilities.WriteFile(Utilities.m_flags.HID_write_handle, Utilities.m_flags.write_buffer, Utilities.m_flags.orbl, ref numBytesWritten, 0);
				m_have_written_event.Set();
				if (!flag || numBytesWritten != Utilities.m_flags.orbl) {
					m_we_had_an_error_writing = true;
				} else {
					m_we_had_an_error_writing = false;
				}
			}
		}
	}

	public static bool Send_Status_Request() {
		m_command_byte_array[0] = 0;
		m_command_byte_array[1] = 1;
		m_command_byte_array[2] = 2;
		m_command_byte_array[3] = 0;
		m_command_byte_array[4] = 0;
		bool flag = Send_Data_Packet_To_PICkitS(ref m_command_byte_array);
		if (!flag) {
			flag = false;
		}

		return flag;
	}

	public static bool Send_Special_Status_Request() {
		m_command_byte_array[0] = 0;
		m_command_byte_array[1] = 11;
		m_command_byte_array[2] = 171;
		m_command_byte_array[3] = 0;
		m_command_byte_array[4] = 0;
		bool flag = Send_Data_Packet_To_PICkitS(ref m_command_byte_array);
		if (!flag) {
			flag = false;
		}

		return flag;
	}

	public static bool Send_Warm_Reset_Cmd() {
		USBRead.m_EVENT_TIME_ROLLOVER = 0.0;
		USBRead.m_RUNNING_TIME = 0.0;
		m_command_byte_array[0] = 0;
		m_command_byte_array[1] = 1;
		m_command_byte_array[2] = 1;
		m_command_byte_array[3] = 0;
		m_command_byte_array[4] = 0;
		bool result = Send_Data_Packet_To_PICkitS(ref m_command_byte_array);
		Update_Status_Packet();
		return result;
	}

	public static bool Send_Cold_Reset_Cmd() {
		m_command_byte_array[0] = 0;
		m_command_byte_array[1] = 1;
		m_command_byte_array[2] = 0;
		m_command_byte_array[3] = 0;
		m_command_byte_array[4] = 0;
		bool result = Send_Data_Packet_To_PICkitS(ref m_command_byte_array);
		Update_Status_Packet();
		return result;
	}

	public static bool Send_CtrlBlk2EE_Cmd() {
		m_command_byte_array[0] = 0;
		m_command_byte_array[1] = 1;
		m_command_byte_array[2] = 3;
		m_command_byte_array[3] = 0;
		m_command_byte_array[4] = 0;
		bool result = Send_Data_Packet_To_PICkitS(ref m_command_byte_array);
		Update_Status_Packet();
		return result;
	}

	public static bool Send_EE2CtrlBlk_Cmd() {
		m_command_byte_array[0] = 0;
		m_command_byte_array[1] = 1;
		m_command_byte_array[2] = 4;
		m_command_byte_array[3] = 0;
		m_command_byte_array[4] = 0;
		bool result = Send_Data_Packet_To_PICkitS(ref m_command_byte_array);
		Update_Status_Packet();
		return result;
	}

	public static bool Send_FlushCbuf2_Cmd() {
		m_command_byte_array[0] = 0;
		m_command_byte_array[1] = 1;
		m_command_byte_array[2] = 5;
		m_command_byte_array[3] = 0;
		m_command_byte_array[4] = 0;
		bool result = Send_Data_Packet_To_PICkitS(ref m_command_byte_array);
		Update_Status_Packet();
		return result;
	}

	public static bool Send_CommReset_Cmd() {
		m_command_byte_array[0] = 0;
		m_command_byte_array[1] = 1;
		m_command_byte_array[2] = 6;
		m_command_byte_array[3] = 0;
		m_command_byte_array[4] = 0;
		bool result = Send_Data_Packet_To_PICkitS(ref m_command_byte_array);
		Update_Status_Packet();
		return result;
	}

	public static void Send_CommClear_Cmd() {
		m_command_byte_array[0] = 0;
		m_command_byte_array[1] = 1;
		m_command_byte_array[2] = 7;
		m_command_byte_array[3] = 0;
		m_command_byte_array[4] = 0;
		Send_Data_Packet_To_PICkitS(ref m_command_byte_array);
		Update_Status_Packet();
	}

	public static void Clear_Status_Errors() {
		m_clear_status_errors = new Thread(Send_CommClear_Cmd);
		m_clear_status_errors.IsBackground = true;
		m_clear_status_errors.Start();
	}

	public static bool Send_Event_Timer_Reset_Cmd() {
		byte[] p_data = new byte[65];
		p_data[0] = 0;
		p_data[1] = 3;
		p_data[2] = 1;
		p_data[3] = 32;
		return Send_Data_Packet_To_PICkitS(ref p_data);
	}

	public static bool Clear_CBUF(byte p_buffer) {
		if (p_buffer < 1 || p_buffer > 3) {
			return false;
		}

		byte[] p_data = new byte[65];
		p_data[0] = 0;
		p_data[1] = (byte)(p_buffer + 7);
		return Send_Data_Packet_To_PICkitS(ref p_data);
	}

	public static bool Send_CtrlBlkWrite_Cmd(ref byte[] p_data_array) {
		byte[] p_data = new byte[65];
		int num = 0;
		Array.Clear(p_data, 0, p_data.Length);
		p_data[0] = 0;
		p_data[1] = 2;
		for (num = 0; num < 24; num++) {
			p_data[num + 2] = p_data_array[num];
		}

		p_data[num] = 0;
		Send_Cold_Reset_Cmd();
		bool result = Send_Data_Packet_To_PICkitS(ref p_data);
		Send_Warm_Reset_Cmd();
		Update_Status_Packet();
		return result;
	}

	public static bool Send_LED_State_Cmd(int p_LED_num, byte p_value) {
		byte[] p_data = new byte[65];
		Array.Clear(p_data, 0, p_data.Length);
		p_data[1] = 3;
		p_data[2] = 2;
		switch (p_LED_num) {
			case 1:
			p_data[3] = 18;
			break;
			case 2:
			p_data[3] = 19;
			break;
			default:
			return false;
		}

		p_data[4] = p_value;
		p_data[5] = 0;
		bool result = Send_Data_Packet_To_PICkitS(ref p_data);
		Update_Status_Packet();
		return result;
	}

	public static void Get_Last_Script_Sent(ref byte[] p_array, byte p_byte_count) {
		m_last_script_array_mutex.WaitOne();
		for (int i = 0; i < p_byte_count; i++) {
			p_array[i] = m_last_script_array[i];
		}

		m_last_script_array_mutex.ReleaseMutex();
	}

	public static byte Get_Last_Script_ByteCount() {
		return m_last_script_array_byte_count;
	}

	public static bool Send_Script_To_PICkitS(ref byte[] p_send_byte_array) {
		lock (m_priv_write_script_lock) {
			m_data_buffer_is_empty = false;
			bool result = false;
			int num = 0;
			uint num2 = 0u;
			uint num3 = 0u;
			byte b = 0;
			uint num4 = 0u;
			uint num5 = p_send_byte_array[2];
			uint num6 = 0u;
			uint num7 = 0u;
			byte[] p_data = new byte[65];
			m_last_script_array_mutex.WaitOne();
			m_last_script_array_byte_count = (byte)(num5 + 2);
			Array.Clear(m_last_script_array, 0, m_last_script_array.Length);
			for (int i = 0; i < m_last_script_array_byte_count; i++) {
				m_last_script_array[i] = p_send_byte_array[i + 1];
			}

			Utilities.m_flags.g_PKSA_has_completed_script.Reset();
			m_last_script_array_mutex.ReleaseMutex();
			num2 = num5 / 62;
			b = (byte)(num5 % 62);
			if (b != 0) {
				num2++;
			}

			for (num = 0; num < num2; num++) {
				Array.Clear(p_data, 0, p_data.Length);
				p_data[0] = p_send_byte_array[0];
				p_data[1] = p_send_byte_array[1];
				if (num2 != 1) {
					num7 = 3u;
					if (num == num2 - 1 && b != 0) {
						num6 = (uint)(b + 4);
						p_data[2] = b;
					} else {
						num6 = 65u;
						p_data[2] = 62;
					}
				} else {
					num6 = num5 + 4;
					num7 = 2u;
				}

				for (num3 = num7; num3 < num6; num3++) {
					p_data[num3] = p_send_byte_array[num4 + num3];
				}

				if (num2 != 1) {
					num4 += 62;
					if (num == num2 - 1) {
						num4++;
					}
				}

				if (p_data[1] == 3) {
					if (!there_is_room_in_cbuf1(p_data[2])) {
						return false;
					}

					m_cbuf1_avail_bytes_mutex.WaitOne();
					m_cbuf1_avail_bytes -= p_data[2];
					m_cbuf1_avail_bytes_mutex.ReleaseMutex();
					if (!Send_Data_Packet_To_PICkitS(ref p_data)) {
						return false;
					}

					if (There_Was_A_Write_Error()) {
						return false;
					}
				} else {
					if (!Send_Data_Packet_To_PICkitS(ref p_data)) {
						return false;
					}

					if (There_Was_A_Write_Error()) {
						return false;
					}
				}
			}

			if (p_data[1] == 3) {
				if (m_use_script_timeout) {
					if (Utilities.m_flags.g_PKSA_has_completed_script.WaitOne(m_universal_timeout, exitContext: false) && Update_Status_Packet()) {
						uint p_error = 0u;
						if (!Status.There_Is_A_Status_Error(ref p_error)) {
							result = true;
						}
					}
				} else {
					result = true;
				}
			} else {
				result = true;
			}

			m_data_buffer_is_empty = true;
			return result;
		}
	}

	public static void configure_outbound_control_block_packet(ref byte[] p_data, ref string p_str, ref byte[] p_status_packet_data) {
		int num = 0;
		string text = "";
		p_str = "";
		p_data[0] = 0;
		p_data[1] = 2;
		for (num = 2; num < 31; num++) {
			text = $"{p_status_packet_data[7 + num - 2]:X2} ";
			p_str += text;
			p_data[num] = p_status_packet_data[7 + num - 2];
		}

		p_data[num] = 0;
		if ((p_status_packet_data[23] & 0x80) == 128) {
			LIN.m_autobaud_is_on = true;
		} else {
			LIN.m_autobaud_is_on = false;
		}
	}

	public static bool write_and_verify_config_block(ref byte[] p_control_block_data, ref string p_result_str, bool p_perform_warm_and_cold_reset, ref string p_cb_data_str) {
		bool flag = false;
		bool result = false;
		int num = 0;
		if (p_perform_warm_and_cold_reset) {
			Send_Cold_Reset_Cmd();
		}

		flag = Send_Data_Packet_To_PICkitS(ref p_control_block_data);
		if (p_perform_warm_and_cold_reset) {
			Send_Warm_Reset_Cmd();
		}

		if (flag) {
			if (Update_Status_Packet()) {
				Utilities.m_flags.g_status_packet_mutex.WaitOne();
				for (num = 7; num < 31; num++) {
					if (Constants.STATUS_PACKET_DATA[num] != p_control_block_data[num - 5]) {
						p_result_str = $"Byte {num - 7} failed verification in config block write.\n Value reads {Constants.STATUS_PACKET_DATA[num]:X2}, but should be {p_control_block_data[num - 5]:X2}.";
						break;
					}
				}

				if (num == 31) {
					result = true;
					p_result_str = $"PICkit Serial Analyzer correctly updated.";
					p_cb_data_str = "";
					for (num = 7; num < 31; num++) {
						p_cb_data_str += $"{Constants.STATUS_PACKET_DATA[num]:X2} ";
					}
				}

				Utilities.m_flags.g_status_packet_mutex.ReleaseMutex();
			} else {
				p_result_str = $"Error requesting config verification - Config Block may not be updated correctly";
			}
		} else {
			p_result_str = $"Error sending config packet - Config Block may not be updated correctly";
		}

		return result;
	}

	public static bool write_and_verify_LIN_config_block(ref byte[] p_control_block_data, ref string p_result_str, bool p_perform_warm_and_cold_reset, ref string p_cb_data_str) {
		bool flag = false;
		bool result = false;
		int num = 0;
		flag = Send_Data_Packet_To_PICkitS(ref p_control_block_data);
		Device.Clear_Status_Errors();
		if (flag) {
			if (Update_Status_Packet()) {
				Utilities.m_flags.g_status_packet_mutex.WaitOne();
				for (num = 7; num < 31; num++) {
					if (Constants.STATUS_PACKET_DATA[num] != p_control_block_data[num - 5]) {
						p_result_str = $"Byte {num - 7} failed verification in config block write.\n Value reads {Constants.STATUS_PACKET_DATA[num]:X2}, but should be {p_control_block_data[num - 5]:X2}.";
						break;
					}
				}

				if (num == 31) {
					result = true;
					p_result_str = $"PICkit Serial Analyzer correctly updated.";
					p_cb_data_str = "";
					for (num = 7; num < 31; num++) {
						p_cb_data_str += $"{Constants.STATUS_PACKET_DATA[num]:X2} ";
					}
				}

				Utilities.m_flags.g_status_packet_mutex.ReleaseMutex();
			} else {
				p_result_str = $"Error requesting config verification - Config Block may not be updated correctly";
			}
		} else {
			p_result_str = $"Error sending config packet - Config Block may not be updated correctly";
		}

		return result;
	}
}