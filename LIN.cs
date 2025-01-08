using Microsoft.VisualBasic;
using System;
using System.Diagnostics;
using System.Threading;
using static System.Windows.Forms.VisualStyles.VisualStyleElement;

namespace PICkitS;

public class LIN {
	public delegate void GUINotifierOR(byte masterid, byte[] data, byte length, byte error, ushort baud, double time);

	public delegate void GUINotifierOA(byte masterid, byte[] data, byte length, byte error, ushort baud, double time);

	internal enum OPMODE {
		LISTEN,
		TRANSMIT,
		DISPLAY_ALL
	}

	internal struct FRAMEINFO {
		internal volatile byte FrameID;

		internal volatile byte[] FrameData;

		internal volatile byte bytecount;

		internal volatile ushort baud;

		internal double time;

		internal long frame_timeout_time;
	}

	internal struct SLAVE_PROFILE_ID {
		internal byte FrameID;

		internal byte ByteCount;

		internal byte[] Data;
	}

	internal struct BUILD_STATE {
		internal volatile bool we_have_transmitted;

		internal volatile bool we_are_building_a_frame;

		internal volatile bool we_have_an_id;

		internal volatile bool we_had_a_status_error;

		internal volatile bool we_timed_out;

		internal volatile bool next_frame_header_received;

		internal volatile bool transmit_data_byte_count_zero;
	}

	internal struct WORKING_FRAME {
		internal FRAMEINFO FrameInfo;

		internal BUILD_STATE BuildState;
	}

	internal const int MAX_NUM_DATA_BYTES = 9;

	internal const int FRAME_ARRAY_COUNT = 256;

	private static long[] m_time = new long[11];

	public static Stopwatch m_stopwatch;

	internal static System.Threading.Timer m_FrameStartTimer;

	private static AutoResetEvent m_working_frame_is_done;

	internal static AutoResetEvent m_slave_profile_id_read;

	private static byte m_OnReceive_error = 0;

	internal static Thread m_reset_timer;

	private static volatile bool m_we_are_finishing_a_frame = false;

	internal static OPMODE m_opmode;

	internal static int FRAME_TIMEOUT = 100;

	internal static double m_interbyte_timeout = 0.01;

	internal static bool m_next_frame_is_first_frame = true;

	internal static bool m_use_baud_rate_timeout = false;

	internal static ushort m_last_master_baud_rate = 0;

	internal static volatile bool m_autobaud_is_on = true;

	internal static WORKING_FRAME m_working_frame = default(WORKING_FRAME);

	internal static FRAMEINFO[] m_Frames = new FRAMEINFO[256];

	internal static SLAVE_PROFILE_ID m_slave_profile_id;

	public static event GUINotifierOR OnReceive;

	public static event GUINotifierOA OnAnswer;

	public static bool Configure_PICkitSerial_For_LIN() {
		bool result = false;
		if (Basic.Configure_PICkitSerial(10, p_reset: true)) {
			Get_LIN_BAUD_Rate();
			result = true;
		}

		return result;
	}

	public static bool Configure_PICkitSerial_For_LIN_No_Autobaud() {
		bool result = false;
		if (Basic.Configure_PICkitSerial(19, p_reset: true)) {
			Get_LIN_BAUD_Rate();
			result = true;
		}

		return result;
	}

	public static bool Configure_PICkitSerial_For_LIN(bool p_chip_select_hi, bool p_receive_enable, bool p_autobaud) {
		bool flag = false;
		if (Utilities.m_flags.HID_read_handle != IntPtr.Zero && Basic.Configure_PICkitSerial(10, p_reset: true)) {
			string p_str = "";
			string p_result_str = "";
			byte[] p_data = new byte[65];
			byte[] p_status_packet = new byte[65];
			if (Utilities.m_flags.HID_read_handle != IntPtr.Zero) {
				Array.Clear(p_data, 0, p_data.Length);
				Array.Clear(p_status_packet, 0, p_status_packet.Length);
				if (!Basic.Get_Status_Packet(ref p_status_packet)) {
					return false;
				}

				if (p_chip_select_hi) {
					p_status_packet[23] |= 8;
				} else {
					p_status_packet[23] &= 247;
				}

				if (p_receive_enable) {
					p_status_packet[23] |= 64;
				} else {
					p_status_packet[23] &= 191;
				}

				if (p_autobaud) {
					p_status_packet[23] |= 128;
				} else {
					p_status_packet[23] &= 127;
				}

				USBWrite.configure_outbound_control_block_packet(ref p_data, ref p_str, ref p_status_packet);
				flag = USBWrite.write_and_verify_config_block(ref p_data, ref p_result_str, p_perform_warm_and_cold_reset: true, ref p_str);
				if (flag) {
					Get_LIN_BAUD_Rate();
				}
			}
		}

		return flag;
	}

	public static bool Get_LIN_Options(ref bool p_chip_select_hi, ref bool p_receive_enable, ref bool p_autobaud) {
		byte[] p_status_packet = new byte[65];
		bool result = false;
		if (Utilities.m_flags.HID_read_handle != IntPtr.Zero) {
			Array.Clear(p_status_packet, 0, p_status_packet.Length);
			if (Basic.Get_Status_Packet(ref p_status_packet)) {
				p_chip_select_hi = (p_status_packet[23] & 8) > 0;
				p_receive_enable = (p_status_packet[23] & 0x40) > 0;
				p_autobaud = (p_status_packet[23] & 0x80) > 0;
				result = true;
			}
		}

		return result;
	}

	public static bool Set_LIN_Options(bool p_chip_select_hi, bool p_receive_enable, bool p_autobaud) {
		bool result = false;
		string p_str = "";
		string p_result_str = "";
		byte[] p_data = new byte[65];
		byte[] p_status_packet = new byte[65];
		if (Utilities.m_flags.HID_read_handle != IntPtr.Zero) {
			Array.Clear(p_data, 0, p_data.Length);
			Array.Clear(p_status_packet, 0, p_status_packet.Length);
			if (!Basic.Get_Status_Packet(ref p_status_packet)) {
				return false;
			}

			if (p_chip_select_hi) {
				p_status_packet[23] |= 8;
			} else {
				p_status_packet[23] &= 247;
			}

			if (p_receive_enable) {
				p_status_packet[23] |= 64;
			} else {
				p_status_packet[23] &= 191;
			}

			if (p_autobaud) {
				p_status_packet[23] |= 128;
			} else {
				p_status_packet[23] &= 127;
			}

			USBWrite.configure_outbound_control_block_packet(ref p_data, ref p_str, ref p_status_packet);
			result = USBWrite.write_and_verify_LIN_config_block(ref p_data, ref p_result_str, p_perform_warm_and_cold_reset: true, ref p_str);
		}

		return result;
	}

	public static bool Set_OnReceive_Timeout(int Timeout) {
		if (Timeout == 65535) {
			if (set_OnReceive_timeout_from_baud_rate()) {
				m_use_baud_rate_timeout = true;
				return true;
			}

			Timeout = 0;
			return false;
		}

		m_use_baud_rate_timeout = false;
		FRAME_TIMEOUT = Timeout;
		return true;
	}

	public static int Get_OnReceive_Timeout() {
		return FRAME_TIMEOUT;
	}

	public static bool OnReceive_Timeout_Is_Baud_Dependent() {
		return m_use_baud_rate_timeout;
	}

	public static bool SetModeListen() {
		m_opmode = OPMODE.LISTEN;
		return true;
	}

	public static bool SetModeTransmit() {
		m_opmode = OPMODE.TRANSMIT;
		return true;
	}

	public static bool SetModeDisplayAll() {
		m_opmode = OPMODE.DISPLAY_ALL;
		return true;
	}

	public static bool Transmit_mode_Is_Set() {
		if (m_opmode == OPMODE.TRANSMIT) {
			return true;
		}

		return false;
	}

	public static bool Listen_mode_Is_Set() {
		if (m_opmode == OPMODE.LISTEN) {
			return true;
		}

		return false;
	}

	public static bool DisplayAll_mode_Is_Set() {
		if (m_opmode == OPMODE.DISPLAY_ALL) {
			return true;
		}

		return false;
	}

	internal static void initialize_LIN_frames() {
		m_stopwatch = new Stopwatch();
		m_opmode = OPMODE.LISTEN;
		m_working_frame.FrameInfo.FrameData = new byte[9];
		reset_working_frame();
		Reset_LIN_Frame_Buffers();
		m_FrameStartTimer = new System.Threading.Timer(frame_has_timed_out, null, -1, -1);
		m_working_frame.BuildState.we_have_transmitted = false;
		m_working_frame.BuildState.transmit_data_byte_count_zero = false;
		m_working_frame_is_done = new AutoResetEvent(initialState: false);
		m_slave_profile_id_read = new AutoResetEvent(initialState: false);
		m_slave_profile_id.ByteCount = 0;
		m_slave_profile_id.FrameID = 0;
		m_slave_profile_id.Data = new byte[255];
	}

	internal static void reset_LIN_timeout() {
		m_reset_timer = new Thread(reset_lin_timer);
		m_reset_timer.Start();
		m_reset_timer.Join();
	}

	private static void reset_lin_timer() {
		m_FrameStartTimer.Change(FRAME_TIMEOUT, -1);
		m_stopwatch.Reset();
		m_stopwatch.Start();
	}

	public static void Reset_LIN_Frame_Buffers() {
		for (int i = 0; i < m_Frames.Length; i++) {
			m_Frames[i].FrameID = (byte)i;
			m_Frames[i].FrameData = new byte[9];
			m_Frames[i].bytecount = 0;
			m_Frames[i].baud = 0;
			m_Frames[i].time = 0.0;
			m_Frames[i].frame_timeout_time = 0L;
			for (int j = 0; j < m_Frames[i].FrameData.Length; j++) {
				m_Frames[i].FrameData[j] = 0;
			}
		}
	}

	public static void Reset_Timer() {
		m_next_frame_is_first_frame = true;
	}

	private static void frame_has_timed_out(object state) {
		while (m_stopwatch.ElapsedMilliseconds < FRAME_TIMEOUT) {
			Thread.Sleep(10);
		}

		m_working_frame.FrameInfo.frame_timeout_time = m_stopwatch.ElapsedMilliseconds;
		if (!m_we_are_finishing_a_frame) {
			if (m_working_frame.FrameInfo.FrameID == 0) {
				_ = USBRead.m_raw_cbuf2_data_array[USBRead.m_cb2_array_tag_index];
			}

			m_working_frame.BuildState.we_timed_out = true;
			finish_this_frame();
		}
	}

	internal static void reset_working_frame() {
		for (int i = 0; i < m_working_frame.FrameInfo.FrameData.Length; i++) {
			m_working_frame.FrameInfo.FrameData[i] = 0;
		}

		m_working_frame.FrameInfo.FrameID = 0;
		m_working_frame.FrameInfo.bytecount = 0;
		m_working_frame.FrameInfo.baud = 0;
		m_working_frame.FrameInfo.time = 0.0;
		m_working_frame.FrameInfo.frame_timeout_time = 0L;
		m_working_frame.BuildState.we_had_a_status_error = false;
		m_working_frame.BuildState.we_are_building_a_frame = false;
		m_working_frame.BuildState.we_have_an_id = false;
		m_working_frame.BuildState.we_timed_out = false;
		m_working_frame.BuildState.next_frame_header_received = false;
		m_working_frame.BuildState.we_have_transmitted = false;
		m_working_frame.BuildState.transmit_data_byte_count_zero = false;
	}

	private static bool this_is_a_valid_frame() {
		return true;
	}

	private static bool this_frame_is_different_than_last() {
		if (m_Frames[m_working_frame.FrameInfo.FrameID].bytecount != m_working_frame.FrameInfo.bytecount) {
			return true;
		}

		if (m_Frames[m_working_frame.FrameInfo.FrameID].baud != m_working_frame.FrameInfo.baud) {
			return true;
		}

		for (int i = 0; i < m_working_frame.FrameInfo.FrameData.Length; i++) {
			if (m_Frames[m_working_frame.FrameInfo.FrameID].FrameData[i] != m_working_frame.FrameInfo.FrameData[i]) {
				return true;
			}
		}

		return false;
	}

	private static void copy_this_frame_into_array() {
		m_Frames[m_working_frame.FrameInfo.FrameID].bytecount = m_working_frame.FrameInfo.bytecount;
		m_Frames[m_working_frame.FrameInfo.FrameID].baud = m_working_frame.FrameInfo.baud;
		m_Frames[m_working_frame.FrameInfo.FrameID].time = m_working_frame.FrameInfo.time;
		for (int i = 0; i < m_working_frame.FrameInfo.FrameData.Length; i++) {
			m_Frames[m_working_frame.FrameInfo.FrameID].FrameData[i] = m_working_frame.FrameInfo.FrameData[i];
		}
	}

	internal static void finalize_working_frame() {
		if (!m_we_are_finishing_a_frame) {
			finish_this_frame();
		}
	}

	private static void finish_this_frame() {
		m_we_are_finishing_a_frame = true;
		m_working_frame.BuildState.we_are_building_a_frame = false;
		m_FrameStartTimer.Change(-1, -1);
		bool flag = false;
		if (this_is_a_valid_frame()) {
			if (m_next_frame_is_first_frame) {
				m_working_frame.FrameInfo.time = 0.0;
				m_next_frame_is_first_frame = false;
				flag = !USBRead.reset_timer_params();
			}

			if (m_working_frame.FrameInfo.baud != 0) {
				m_working_frame.FrameInfo.baud = calculate_baud_rate(m_working_frame.FrameInfo.baud);
			} else {
				m_working_frame.FrameInfo.baud = m_last_master_baud_rate;
			}

			m_OnReceive_error = 0;
			uint p_error = 0u;
			if (Status.There_Is_A_Status_Error(ref p_error)) {
				m_OnReceive_error = 4;
				Device.Clear_Status_Errors();
			}

			if (m_OnReceive_error == 0 && m_working_frame.BuildState.we_had_a_status_error) {
				m_OnReceive_error = 5;
			} else if (m_working_frame.BuildState.we_timed_out) {
				m_OnReceive_error = 1;
			} else if (flag) {
				m_OnReceive_error = 3;
			} else if (m_working_frame.BuildState.next_frame_header_received) {
				m_OnReceive_error = 6;
			}

			if ((this_frame_is_different_than_last() && m_opmode == OPMODE.LISTEN) || m_opmode == OPMODE.DISPLAY_ALL) {
				if (m_working_frame.BuildState.we_have_transmitted && m_working_frame.BuildState.transmit_data_byte_count_zero) {
					LIN.OnAnswer(m_working_frame.FrameInfo.FrameID, m_working_frame.FrameInfo.FrameData, m_working_frame.FrameInfo.bytecount, m_OnReceive_error, m_working_frame.FrameInfo.baud, m_working_frame.FrameInfo.time);
				} else if ((!m_working_frame.BuildState.we_have_transmitted || !m_working_frame.BuildState.transmit_data_byte_count_zero) && LIN.OnReceive != null) {
					if (m_OnReceive_error == 6) {
						m_OnReceive_error = 0;
					}

					LIN.OnReceive(m_working_frame.FrameInfo.FrameID, m_working_frame.FrameInfo.FrameData, m_working_frame.FrameInfo.bytecount, m_OnReceive_error, m_working_frame.FrameInfo.baud, m_working_frame.FrameInfo.time);
				}
			}

			copy_this_frame_into_array();
		}

		reset_working_frame();
		m_working_frame_is_done.Set();
		m_we_are_finishing_a_frame = false;
	}

	internal static void send_on_answer(byte p_id, double p_time, ref byte[] p_data) {
		if (LIN.OnAnswer != null) {
			LIN.OnAnswer(p_id, p_data, 9, 0, 0, p_time);
		}
	}

	public static bool Transmit(byte MasterID, byte[] Data, byte DataByteCount, ref string ErrorString) {
		int num = 0;
		bool result = false;
		byte[] p_send_byte_array = new byte[30];
		Array.Clear(p_send_byte_array, 0, p_send_byte_array.Length);
		if (DataByteCount > 9) {
			ErrorString = "DataByteCount cannot exceed 9.";
			return result;
		}

		p_send_byte_array[0] = 0;
		p_send_byte_array[1] = 3;
		p_send_byte_array[2] = (byte)(DataByteCount + 5);
		p_send_byte_array[3] = 132;
		p_send_byte_array[4] = (byte)(DataByteCount + 1);
		p_send_byte_array[5] = MasterID;
		p_send_byte_array[6] = 31;
		p_send_byte_array[7] = 119;
		p_send_byte_array[8] = 0;
		if (DataByteCount == 0) {
			m_working_frame.BuildState.transmit_data_byte_count_zero = true;
			m_working_frame.BuildState.we_have_transmitted = true;
			m_working_frame_is_done.Reset();
			result = USBWrite.Send_Script_To_PICkitS(ref p_send_byte_array);
			if (!result) {
				ErrorString = "Error sending script.";
				return false;
			}

			if (!m_working_frame_is_done.WaitOne(6000, exitContext: false)) {
				ErrorString = "No data returned";
				return false;
			}
		} else {
			for (num = 0; num < DataByteCount; num++) {
				p_send_byte_array[num + 6] = Data[num];
			}

			p_send_byte_array[num + 6] = 31;
			p_send_byte_array[num + 7] = 119;
			p_send_byte_array[num + 8] = 0;
			m_working_frame.BuildState.transmit_data_byte_count_zero = false;
			m_working_frame.BuildState.we_have_transmitted = true;
			m_working_frame_is_done.Reset();
			result = USBWrite.Send_Script_To_PICkitS(ref p_send_byte_array);
			if (!result) {
				ErrorString = "Error sending script.";
				return false;
			}
		}

		return result;
	}

	public static bool Change_LIN_BAUD_Rate(ushort Baud) {
		bool flag = false;
		int num = 0;
		string p_str = "";
		string p_result_str = "";
		byte[] p_data = new byte[65];
		byte[] p_status_packet = new byte[65];
		if (Utilities.m_flags.HID_read_handle != IntPtr.Zero) {
			Array.Clear(p_data, 0, p_data.Length);
			Array.Clear(p_status_packet, 0, p_status_packet.Length);
			if (!Basic.Get_Status_Packet(ref p_status_packet)) {
				return false;
			}

			num = (int)Math.Round(20000000.0 / (double)(int)Baud / 4.0) - 1;
			p_status_packet[29] = (byte)num;
			p_status_packet[30] = (byte)(num >> 8);
			USBWrite.configure_outbound_control_block_packet(ref p_data, ref p_str, ref p_status_packet);
			flag = USBWrite.write_and_verify_LIN_config_block(ref p_data, ref p_result_str, p_perform_warm_and_cold_reset: true, ref p_str);
		}

		if (flag) {
			Get_LIN_BAUD_Rate();
		}

		return flag;
	}

	public static ushort Get_LIN_BAUD_Rate() {
		byte[] p_status_packet = new byte[65];
		ushort result = 0;
		if (Utilities.m_flags.HID_read_handle != IntPtr.Zero) {
			Array.Clear(p_status_packet, 0, p_status_packet.Length);
			if (Basic.Get_Status_Packet(ref p_status_packet)) {
				result = (m_last_master_baud_rate = calculate_baud_rate((ushort)(p_status_packet[50] + (p_status_packet[51] << 8))));
			}
		}

		return result;
	}

	private static bool set_OnReceive_timeout_from_baud_rate() {
		byte[] p_status_packet = new byte[65];
		if (Utilities.m_flags.HID_read_handle != IntPtr.Zero) {
			Array.Clear(p_status_packet, 0, p_status_packet.Length);
			if (!Basic.Get_Status_Packet(ref p_status_packet)) {
				return false;
			}

			int p_baud = calculate_baud_rate((ushort)(p_status_packet[50] + (p_status_packet[51] << 8)));
			calculate_new_baud_dependent_onreceive_timeout(p_baud);
		}

		return true;
	}

	internal static void calculate_new_baud_dependent_onreceive_timeout(int p_baud) {
		double num = 11.0 / (double)p_baud * 10.0 * 1.5;
		FRAME_TIMEOUT = (int)(num * 1000.0);
	}

	private static ushort calculate_baud_rate(ushort p_baud) {
		double num = 20.0 / (4.0 * ((double)(int)p_baud + 1.0));
		int num2 = (int)Math.Round(num * 1000000.0);
		return (ushort)num2;
	}

	private static bool Toggle_AutoBaud_Set(bool p_turn_autobaudset_on, ref ushort p_baud, ref string p_error_detail) {
		bool flag = false;
		int num = 0;
		string p_str = "";
		string p_result_str = "";
		byte[] p_data = new byte[65];
		byte[] p_status_packet = new byte[65];
		p_error_detail = "";
		if (Utilities.m_flags.HID_read_handle != IntPtr.Zero) {
			Array.Clear(p_data, 0, p_data.Length);
			Array.Clear(p_status_packet, 0, p_status_packet.Length);
			if (!Basic.Get_Status_Packet(ref p_status_packet)) {
				p_error_detail = "Could not poll PKSA for status.";
				return false;
			}

			p_baud = calculate_baud_rate((ushort)(p_status_packet[29] + (p_status_packet[30] << 8)));
			if (p_turn_autobaudset_on) {
				p_status_packet[23] |= 128;
			} else {
				p_status_packet[23] &= 127;
			}

			USBWrite.configure_outbound_control_block_packet(ref p_data, ref p_str, ref p_status_packet);
			while (!flag && num < 3) {
				num++;
				flag = USBWrite.write_and_verify_LIN_config_block(ref p_data, ref p_result_str, p_perform_warm_and_cold_reset: false, ref p_str);
				p_error_detail += p_result_str;
			}
		}

		return flag;
	}

	public static bool Configure_PICkitSerial_For_LINSlave_Mode(byte p_array_byte_count, ref byte[] p_profile_array, ref string p_result_str, bool p_autobaud, ref int p_error_code) {
		bool result = false;
		byte[] p_data = new byte[65];
		byte[] p_send_byte_array = new byte[255];
		byte[] p_status_packet_data = new byte[65];
		bool result2 = false;
		byte p_used_bytes = 0;
		byte p_unused_bytes = 0;
		string p_str = "";
		int num = 0;
		p_error_code = 0;
		Array.Clear(p_data, 0, p_data.Length);
		Array.Clear(p_status_packet_data, 0, p_status_packet_data.Length);
		byte b = Number_Of_Bytes_In_CBUF3(ref p_used_bytes, ref p_unused_bytes);
		if (p_array_byte_count > b) {
			p_result_str = $"Byte count of {p_array_byte_count} greater than allowed value of {b}.";
			p_error_code = 1;
			return result;
		}

		USBWrite.Clear_CBUF(3);
		if (p_autobaud) {
			Mode.update_status_packet_data(10, ref p_status_packet_data);
		} else {
			Mode.update_status_packet_data(19, ref p_status_packet_data);
		}

		USBWrite.configure_outbound_control_block_packet(ref p_data, ref p_str, ref p_status_packet_data);
		USBWrite.Send_Cold_Reset_Cmd();
		result = USBWrite.Send_Data_Packet_To_PICkitS(ref p_data);
		USBWrite.Send_Warm_Reset_Cmd();
		Array.Clear(p_send_byte_array, 0, p_send_byte_array.Length);
		p_send_byte_array[0] = 0;
		p_send_byte_array[1] = 5;
		p_send_byte_array[2] = p_array_byte_count;
		for (int i = 3; i < p_array_byte_count + 3; i++) {
			p_send_byte_array[i] = p_profile_array[i - 3];
		}

		USBWrite.Send_Script_To_PICkitS(ref p_send_byte_array);
		p_status_packet_data[23] |= 32;
		USBWrite.configure_outbound_control_block_packet(ref p_data, ref p_str, ref p_status_packet_data);
		if (USBWrite.Send_Data_Packet_To_PICkitS(ref p_data)) {
			if (USBWrite.Update_Status_Packet()) {
				Utilities.m_flags.g_status_packet_mutex.WaitOne();
				for (num = 7; num < 31; num++) {
					if (Constants.STATUS_PACKET_DATA[num] != p_data[num - 5]) {
						p_error_code = 3;
						p_result_str = $"Byte {num - 7} failed verification in config block write.\n Value reads {Constants.STATUS_PACKET_DATA[num]:X2}, but should be {p_data[num - 5]:X2}.";
						break;
					}
				}

				if (num == 31) {
					result2 = true;
					p_result_str = $"PICkit Serial Analyzer correctly updated.";
				}

				Utilities.m_flags.g_status_packet_mutex.ReleaseMutex();
			} else {
				p_result_str = $"Error requesting config verification - Config Block may not be updated correctly";
			}
		} else {
			p_error_code = 2;
			p_result_str = $"Error sending config packet - Config Block may not be updated correctly";
		}

		return result2;
	}

	public static bool Add_LIN_Slave_Profile_To_PKS(byte p_array_byte_count, ref byte[] p_profile_array, ref string p_result_str, ref int p_error_code) {
		bool result = false;
		byte[] p_data = new byte[65];
		byte[] p_send_byte_array = new byte[255];
		byte[] p_status_packet = new byte[65];
		bool result2 = false;
		byte p_used_bytes = 0;
		byte p_unused_bytes = 0;
		string p_str = "";
		int num = 0;
		p_error_code = 0;
		Array.Clear(p_data, 0, p_data.Length);
		Array.Clear(p_status_packet, 0, p_status_packet.Length);
		byte b = Number_Of_Bytes_In_CBUF3(ref p_used_bytes, ref p_unused_bytes);
		if (p_array_byte_count > b) {
			p_result_str = $"Byte count of {p_array_byte_count} greater than allowed value of {b}.";
			p_error_code = 1;
			return result;
		}

		USBWrite.Clear_CBUF(3);
		if (!Basic.Get_Status_Packet(ref p_status_packet)) {
			p_result_str = $"Error reading status packet.";
			p_error_code = 2;
			return false;
		}

		p_status_packet[23] |= 32;
		USBWrite.configure_outbound_control_block_packet(ref p_data, ref p_str, ref p_status_packet);
		result = USBWrite.Send_Data_Packet_To_PICkitS(ref p_data);
		Array.Clear(p_send_byte_array, 0, p_send_byte_array.Length);
		p_send_byte_array[0] = 0;
		p_send_byte_array[1] = 5;
		p_send_byte_array[2] = p_array_byte_count;
		for (int i = 3; i < p_array_byte_count + 3; i++) {
			p_send_byte_array[i] = p_profile_array[i - 3];
		}

		bool flag = USBWrite.Send_Script_To_PICkitS(ref p_send_byte_array);
		if (result && flag) {
			if (USBWrite.Update_Status_Packet()) {
				Utilities.m_flags.g_status_packet_mutex.WaitOne();
				for (num = 7; num < 31; num++) {
					if (Constants.STATUS_PACKET_DATA[num] != p_data[num - 5]) {
						p_error_code = 3;
						p_result_str = $"Byte {num - 7} failed verification in config block write.\n Value reads {Constants.STATUS_PACKET_DATA[num]:X2}, but should be {p_data[num - 5]:X2}.";
						break;
					}
				}

				if (num == 31) {
					result2 = true;
					p_result_str = $"PICkit Serial Analyzer correctly updated.";
				}

				Utilities.m_flags.g_status_packet_mutex.ReleaseMutex();
			} else {
				p_result_str = $"Error requesting config verification - Config Block may not be updated correctly";
			}
		} else {
			p_error_code = 2;
			p_result_str = $"Error sending config packet - Config Block may not be updated correctly";
		}

		return result2;
	}

	public static byte Number_Of_Bytes_In_CBUF3(ref byte p_used_bytes, ref byte p_unused_bytes) {
		byte result = 0;
		if (USBWrite.Update_Special_Status_Packet()) {
			Utilities.m_flags.g_status_packet_mutex.WaitOne();
			result = (byte)(Constants.STATUS_PACKET_DATA[57] + Constants.STATUS_PACKET_DATA[58]);
			p_used_bytes = Constants.STATUS_PACKET_DATA[57];
			p_unused_bytes = Constants.STATUS_PACKET_DATA[58];
			Utilities.m_flags.g_status_packet_mutex.ReleaseMutex();
		}

		return result;
	}

	public static bool Read_Slave_Profile(byte p_masterid, ref byte[] p_data, byte p_expected_byte_count, ref byte p_actual_byte_count, ref byte p_error_code) {
		bool result = false;
		p_error_code = 0;
		byte[] p_send_byte_array = new byte[30];
		Array.Clear(p_send_byte_array, 0, p_send_byte_array.Length);
		p_send_byte_array[0] = 0;
		p_send_byte_array[1] = 3;
		p_send_byte_array[2] = 4;
		p_send_byte_array[3] = 135;
		p_send_byte_array[4] = p_masterid;
		p_send_byte_array[5] = 31;
		p_send_byte_array[6] = 119;
		p_send_byte_array[7] = 0;
		Array.Clear(m_slave_profile_id.Data, 0, m_slave_profile_id.Data.Length);
		if (USBWrite.Send_Script_To_PICkitS(ref p_send_byte_array)) {
			if (m_slave_profile_id_read.WaitOne(2000, exitContext: false)) {
				if (p_masterid == m_slave_profile_id.FrameID) {
					p_actual_byte_count = m_slave_profile_id.ByteCount;
					if (p_expected_byte_count >= m_slave_profile_id.ByteCount) {
						for (int i = 0; i < m_slave_profile_id.ByteCount; i++) {
							p_data[i] = m_slave_profile_id.Data[i];
						}

						result = true;
					} else {
						p_error_code = 3;
					}
				} else {
					p_error_code = 4;
				}
			} else {
				p_error_code = 1;
			}
		} else {
			p_error_code = 2;
		}

		return result;
	}

	public static bool Write_Slave_Profile(byte p_masterid, ref byte[] p_data, byte p_byte_count, ref byte p_error_code) {
		bool result = false;
		p_error_code = 0;
		byte[] p_send_byte_array = new byte[255];
		byte[] p_data2 = new byte[255];
		byte p_error_code2 = 0;
		byte p_actual_byte_count = 0;
		byte b = 0;
		if (p_byte_count > 244) {
			p_error_code = 3;
			return false;
		}

		Array.Clear(p_send_byte_array, 0, p_send_byte_array.Length);
		p_send_byte_array[0] = 0;
		p_send_byte_array[1] = 3;
		p_send_byte_array[2] = (byte)(p_byte_count + 5);
		p_send_byte_array[3] = 134;
		p_send_byte_array[4] = p_masterid;
		p_send_byte_array[5] = p_byte_count;
		for (b = 0; b < p_byte_count; b++) {
			p_send_byte_array[6 + b] = p_data[b];
		}

		p_send_byte_array[b + 6] = 31;
		p_send_byte_array[b + 7] = 119;
		p_send_byte_array[b + 8] = 0;
		if (USBWrite.Send_Script_To_PICkitS(ref p_send_byte_array)) {
			if (!Read_Slave_Profile(p_masterid, ref p_data2, p_byte_count, ref p_actual_byte_count, ref p_error_code2)) {
				p_error_code = 1;
			} else {
				for (b = 0; b < p_byte_count; b++) {
					if (p_data2[b] != p_data[b]) {
						p_error_code = 1;
						return false;
					}
				}

				result = true;
			}
		} else {
			p_error_code = 2;
		}

		return result;
	}
}