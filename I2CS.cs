using Microsoft.VisualBasic;
using System;
using System.Threading;

namespace PICkitS;

public class I2CS {
	public delegate void GUINotifierReceive(byte slave_addr);

	public delegate void GUINotifierRead(byte slave_addr, ushort byte_count, ref byte[] data);

	public delegate void GUINotifierWrite(byte slave_addr, ushort byte_count, ref byte[] data);

	public delegate void GUINotifierError();

	private const int m_slave_array_size = 1000;

	private const ushort SLAVE_ADDR_RESET = 512;

	private static Mutex m_read_mutex = new Mutex(initiallyOwned: false);

	private static Thread m_read;

	private static Mutex m_receive_mutex = new Mutex(initiallyOwned: false);

	private static Thread m_receive;

	private static Mutex m_write_mutex = new Mutex(initiallyOwned: false);

	private static Thread m_write;

	private static byte[] m_slave_data_array = new byte[1000];

	private static uint m_slave_array_index = 0u;

	internal static ushort m_last_slave_addr_received = 512;

	internal static ushort m_previous_slave_addr_received = 512;

	internal static byte m_start_data_addr_received = 0;

	internal static bool m_slave_address_was_just_set = false;

	internal static bool m_master_is_waiting_for_data = false;

	internal static bool m_stop_command_issued = false;

	public static event GUINotifierWrite Write;

	public static event GUINotifierRead Read;

	public static event GUINotifierReceive Receive;

	public static event GUINotifierError Error;

	internal static void issue_error() {
		Basic.Reset_Control_Block();
		I2CS.Error();
	}

	internal static void issue_event() {
		if (Utilities.g_i2cs_mode == Utilities.I2CS_MODE.INTERACTIVE) {
			bool flag = false;
			if (m_last_slave_addr_received == m_previous_slave_addr_received + 1 && m_master_is_waiting_for_data && !m_stop_command_issued) {
				issue_read_command();
				flag = true;
			} else if (m_previous_slave_addr_received == 512 && m_last_slave_addr_received % 2 != 0 && m_master_is_waiting_for_data && !m_stop_command_issued) {
				issue_receive_command();
				flag = true;
			} else if (m_previous_slave_addr_received == 512 && m_last_slave_addr_received % 2 == 0 && !m_master_is_waiting_for_data && m_stop_command_issued) {
				issue_write_command();
				flag = true;
			} else if (m_last_slave_addr_received != m_previous_slave_addr_received + 1 && !m_master_is_waiting_for_data && m_previous_slave_addr_received != 512 && m_slave_address_was_just_set && !m_stop_command_issued) {
				issue_write_command();
				USBRead.Clear_Data_Array(0u);
				USBRead.Clear_Raw_Data_Array();
				flag = true;
			} else if (m_previous_slave_addr_received != 512 && m_last_slave_addr_received != m_previous_slave_addr_received + 1 && m_last_slave_addr_received % 2 != 0 && m_slave_address_was_just_set && !m_stop_command_issued) {
				issue_read_command();
				USBRead.Clear_Data_Array(0u);
				USBRead.Clear_Raw_Data_Array();
				flag = true;
			} else if (m_previous_slave_addr_received != 512 && m_last_slave_addr_received % 2 == 0 && m_slave_address_was_just_set && !m_stop_command_issued) {
				issue_receive_command();
				USBRead.Clear_Data_Array(0u);
				USBRead.Clear_Raw_Data_Array();
				flag = true;
			}
		}
	}

	internal static void reset_buffers() {
		USBRead.Clear_Data_Array(0u);
		USBRead.Clear_Raw_Data_Array();
		m_last_slave_addr_received = 512;
		m_previous_slave_addr_received = 512;
	}

	private static void issue_read_command() {
		m_slave_array_index = USBRead.m_cbuf2_data_array_index;
		for (int i = 0; i < m_slave_array_index; i++) {
			m_slave_data_array[i] = USBRead.m_cbuf2_data_array[i];
		}

		m_read = new Thread(fire_and_forget_read);
		m_read.IsBackground = true;
		m_read.Start();
	}

	private static void issue_receive_command() {
		m_receive = new Thread(fire_and_forget_receive);
		m_receive.IsBackground = true;
		m_receive.Start();
	}

	private static void issue_write_command() {
		m_slave_array_index = USBRead.m_cbuf2_data_array_index;
		for (int i = 0; i < m_slave_array_index; i++) {
			m_slave_data_array[i] = USBRead.m_cbuf2_data_array[i];
		}

		m_write = new Thread(fire_and_forget_write);
		m_write.IsBackground = true;
		m_write.Start();
	}

	private static void fire_and_forget_read() {
		m_read_mutex.WaitOne();
		if (I2CS.Read != null) {
			I2CS.Read((byte)m_last_slave_addr_received, (ushort)m_slave_array_index, ref m_slave_data_array);
		}

		m_read_mutex.ReleaseMutex();
	}

	private static void fire_and_forget_receive() {
		m_receive_mutex.WaitOne();
		if (I2CS.Receive != null) {
			I2CS.Receive((byte)m_last_slave_addr_received);
		}

		m_receive_mutex.ReleaseMutex();
	}

	private static void fire_and_forget_write() {
		m_write_mutex.WaitOne();
		if (I2CS.Write != null) {
			I2CS.Write((byte)m_last_slave_addr_received, (ushort)m_slave_array_index, ref m_slave_data_array);
		}

		m_write_mutex.ReleaseMutex();
	}

	public static bool Configure_PICkitSerial_For_I2CSlave_Default_Mode(byte p_slave_addr, byte p_slave_mask, byte p_read_byte_0_data, byte p_read_bytes_1_N_data) {
		bool result = false;
		if (Basic.Configure_PICkitSerial(7, p_reset: true)) {
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

				p_status_packet[23] = 0;
				p_status_packet[27] = p_read_bytes_1_N_data;
				p_status_packet[28] = p_read_byte_0_data;
				p_status_packet[29] = p_slave_addr;
				p_status_packet[30] = p_slave_mask;
				USBWrite.configure_outbound_control_block_packet(ref p_data, ref p_str, ref p_status_packet);
				result = USBWrite.write_and_verify_config_block(ref p_data, ref p_result_str, p_perform_warm_and_cold_reset: true, ref p_str);
			}
		}

		return result;
	}

	public static bool Configure_PICkitSerial_For_I2CSlave_Interactive_Mode(byte p_slave_addr, byte p_slave_mask) {
		bool result = false;
		string p_str = "";
		string p_result_str = "";
		byte b = 0;
		byte b2 = 0;
		byte[] p_data = new byte[65];
		byte[] p_status_packet = new byte[65];
		if (!Basic.Get_Status_Packet(ref p_status_packet)) {
			return false;
		}

		b = p_status_packet[28];
		b2 = p_status_packet[27];
		if (Basic.Configure_PICkitSerial(7, p_reset: true) && Utilities.m_flags.HID_read_handle != IntPtr.Zero) {
			Array.Clear(p_data, 0, p_data.Length);
			Array.Clear(p_status_packet, 0, p_status_packet.Length);
			if (!Basic.Get_Status_Packet(ref p_status_packet)) {
				return false;
			}

			p_status_packet[23] = 1;
			p_status_packet[27] = b2;
			p_status_packet[28] = b;
			p_status_packet[29] = p_slave_addr;
			p_status_packet[30] = p_slave_mask;
			USBWrite.configure_outbound_control_block_packet(ref p_data, ref p_str, ref p_status_packet);
			result = USBWrite.write_and_verify_config_block(ref p_data, ref p_result_str, p_perform_warm_and_cold_reset: true, ref p_str);
		}

		return result;
	}

	public static bool Set_I2CSlave_Address_and_Mask(byte p_slave_addr, byte p_slave_mask) {
		bool result = false;
		string p_str = "";
		string p_result_str = "";
		byte[] p_data = new byte[65];
		byte[] p_status_packet = new byte[65];
		if (!Basic.Get_Status_Packet(ref p_status_packet)) {
			return false;
		}

		if (Utilities.m_flags.HID_read_handle != IntPtr.Zero) {
			Array.Clear(p_data, 0, p_data.Length);
			Array.Clear(p_status_packet, 0, p_status_packet.Length);
			if (!Basic.Get_Status_Packet(ref p_status_packet)) {
				return false;
			}

			p_status_packet[29] = p_slave_addr;
			p_status_packet[30] = p_slave_mask;
			USBWrite.configure_outbound_control_block_packet(ref p_data, ref p_str, ref p_status_packet);
			result = USBWrite.write_and_verify_config_block(ref p_data, ref p_result_str, p_perform_warm_and_cold_reset: true, ref p_str);
		}

		return result;
	}

	public static bool Get_I2CSlave_Address_and_Mask(ref byte p_slave_addr, ref byte p_slave_mask) {
		byte[] p_status_packet = new byte[65];
		if (!Basic.Get_Status_Packet(ref p_status_packet)) {
			return false;
		}

		p_slave_addr = p_status_packet[29];
		p_slave_mask = p_status_packet[30];
		return true;
	}

	public static bool Send_Bytes(byte p_num_bytes_to_write, ref byte[] p_data_array, ref string p_script_view) {
		if (p_num_bytes_to_write > 253) {
			return false;
		}

		int num = 0;
		byte[] p_send_byte_array = new byte[300];
		Array.Clear(p_send_byte_array, 0, p_send_byte_array.Length);
		p_send_byte_array[0] = 0;
		p_send_byte_array[1] = 3;
		p_send_byte_array[2] = (byte)(4 + p_num_bytes_to_write);
		p_send_byte_array[3] = 193;
		p_send_byte_array[4] = p_num_bytes_to_write;
		for (num = 0; num < p_num_bytes_to_write; num++) {
			p_send_byte_array[num + 5] = p_data_array[num];
		}

		p_send_byte_array[num + 5] = 31;
		p_send_byte_array[num + 6] = 119;
		p_send_byte_array[num + 7] = 0;
		p_script_view = "[SB]";
		string text = $"[{p_send_byte_array[4]:X2}]";
		p_script_view += text;
		for (num = 0; num < p_num_bytes_to_write; num++) {
			text = $"[{p_send_byte_array[num + 5]:X2}]";
			p_script_view += text;
		}

		USBRead.Clear_Data_Array(0u);
		USBRead.Clear_Raw_Data_Array();
		return USBWrite.Send_Script_To_PICkitS(ref p_send_byte_array);
	}

	public static bool Configure_PICkitSerial_For_I2CSlave_Auto_Mode(byte p_slave_addr, byte p_slave_mask, byte p_array_byte_count, ref byte[] p_profile_array, ref string p_result_str) {
		bool flag = false;
		byte[] p_data = new byte[65];
		byte[] p_send_byte_array = new byte[255];
		byte[] p_status_packet_data = new byte[65];
		bool result = false;
		string p_str = "";
		int num = 0;
		Array.Clear(p_data, 0, p_data.Length);
		Array.Clear(p_status_packet_data, 0, p_status_packet_data.Length);
		Mode.update_status_packet_data(7, ref p_status_packet_data);
		p_status_packet_data[23] = 2;
		p_status_packet_data[29] = p_slave_addr;
		p_status_packet_data[30] = p_slave_mask;
		USBWrite.configure_outbound_control_block_packet(ref p_data, ref p_str, ref p_status_packet_data);
		USBWrite.Send_Cold_Reset_Cmd();
		flag = USBWrite.Send_Data_Packet_To_PICkitS(ref p_data);
		Array.Clear(p_send_byte_array, 0, p_send_byte_array.Length);
		p_send_byte_array[0] = 0;
		p_send_byte_array[1] = 5;
		p_send_byte_array[2] = p_array_byte_count;
		for (int i = 3; i < p_array_byte_count + 3; i++) {
			p_send_byte_array[i] = p_profile_array[i - 3];
		}

		USBWrite.Send_Script_To_PICkitS(ref p_send_byte_array);
		USBWrite.Send_Warm_Reset_Cmd();
		if (flag) {
			if (USBWrite.Update_Status_Packet()) {
				Utilities.m_flags.g_status_packet_mutex.WaitOne();
				for (num = 7; num < 31; num++) {
					if (Constants.STATUS_PACKET_DATA[num] != p_data[num - 5]) {
						p_result_str = $"Byte {num - 7} failed verification in config block write.\n Value reads {Constants.STATUS_PACKET_DATA[num]:X2}, but should be {p_data[num - 5]:X2}.";
						break;
					}
				}

				if (num == 31) {
					result = true;
					p_result_str = $"PICkit Serial Analyzer correctly updated.";
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

	public static byte Get_Slave_Addresses_That_Will_Acknowledge(byte p_slave_addr, byte p_slave_mask, ref byte[] p_addr_array, ref string p_addr_str) {
		byte result = 0;
		p_addr_str = "";
		byte b = (byte)(p_slave_mask & 0x3Eu);
		byte b2 = (byte)(p_slave_addr & ~b);
		for (int i = 0; i <= 255; i++) {
			byte b3 = (byte)(i & ~b);
			if (b3 == b2) {
				p_addr_array[result++] = (byte)i;
				p_addr_str += $"{i:X2} ";
			}
		}

		return result;
	}
}