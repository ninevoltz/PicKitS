using Microsoft.VisualBasic.ApplicationServices;
using System;

namespace PICkitS;

public class I2CM {
	public static bool Configure_PICkitSerial_For_I2CMaster() {
		return Basic.Configure_PICkitSerial(1, p_reset: true);
	}

	public static bool Configure_PICkitSerial_For_I2CMaster(bool p_aux1_def, bool p_aux2_def, bool p_aux1_dir, bool p_aux2_dir, bool p_enable_pu, double p_voltage) {
		bool result = false;
		int num = 0;
		if (Utilities.m_flags.HID_read_handle != IntPtr.Zero) {
			if (p_voltage < 0.0 || p_voltage > 5.0) {
				return result;
			}

			if (Basic.Configure_PICkitSerial(1, p_reset: true)) {
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

					if (p_aux1_def) {
						p_status_packet[28] |= 1;
					} else {
						p_status_packet[28] &= 254;
					}

					if (p_aux2_def) {
						p_status_packet[28] |= 2;
					} else {
						p_status_packet[28] &= 253;
					}

					if (p_aux1_dir) {
						p_status_packet[28] |= 4;
					} else {
						p_status_packet[28] &= 251;
					}

					if (p_aux2_dir) {
						p_status_packet[28] |= 8;
					} else {
						p_status_packet[28] &= 247;
					}

					if (p_enable_pu) {
						p_status_packet[16] |= 16;
					} else {
						p_status_packet[16] &= 239;
					}

					num = (int)Math.Round((p_voltage * 1000.0 + 43.53) / 21.191);
					p_status_packet[19] = (byte)num;
					p_status_packet[20] = (byte)(num / 4);
					p_status_packet[16] |= 32;
					p_status_packet[16] |= 64;
					USBWrite.configure_outbound_control_block_packet(ref p_data, ref p_str, ref p_status_packet);
					result = USBWrite.write_and_verify_config_block(ref p_data, ref p_result_str, p_perform_warm_and_cold_reset: true, ref p_str);
				}
			}
		}

		return result;
	}

	public static bool Write(byte p_slave_addr, byte p_start_data_addr, byte p_num_bytes_to_write, ref byte[] p_data_array, ref string p_script_view) {
		return Basic.Send_I2CWrite_Cmd(p_slave_addr, p_start_data_addr, p_num_bytes_to_write, ref p_data_array, ref p_script_view);
	}

	public static bool Write(byte p_slave_addr, byte p_command1, byte p_command2, byte p_num_bytes_to_write, ref byte[] p_data_array, ref string p_script_view) {
		return Basic.Send_I2CWrite_Word_Cmd(p_slave_addr, p_command1, p_command2, p_num_bytes_to_write, ref p_data_array, ref p_script_view);
	}

	public static bool Write_Using_PEC(byte p_slave_addr, byte p_start_data_addr, byte p_num_bytes_to_write, ref byte[] p_data_array, ref byte p_PEC, ref string p_script_view) {
		bool flag = false;
		if (p_num_bytes_to_write > 253) {
			return false;
		}

		int num = 0;
		byte[] p_send_byte_array = new byte[300];
		byte p_start_crc = 0;
		p_start_crc = Utilities.calculate_crc8(p_slave_addr, p_start_crc);
		p_start_crc = Utilities.calculate_crc8(p_start_data_addr, p_start_crc);
		for (num = 0; num < p_num_bytes_to_write; num++) {
			p_start_crc = Utilities.calculate_crc8(p_data_array[num], p_start_crc);
		}

		Array.Clear(p_send_byte_array, 0, p_send_byte_array.Length);
		p_send_byte_array[0] = 0;
		p_send_byte_array[1] = 3;
		p_send_byte_array[2] = (byte)(9 + p_num_bytes_to_write);
		p_send_byte_array[3] = 129;
		p_send_byte_array[4] = 132;
		p_send_byte_array[5] = (byte)(3 + p_num_bytes_to_write);
		p_send_byte_array[6] = p_slave_addr;
		p_send_byte_array[7] = p_start_data_addr;
		for (num = 0; num < p_num_bytes_to_write; num++) {
			p_send_byte_array[num + 8] = p_data_array[num];
		}

		p_send_byte_array[num + 8] = p_start_crc;
		p_send_byte_array[num + 9] = 130;
		p_send_byte_array[num + 10] = 31;
		p_send_byte_array[num + 11] = 119;
		p_send_byte_array[num + 12] = 0;
		p_script_view = "[S_][W_]";
		string text = $"[{p_send_byte_array[5]:X2}]";
		p_script_view += text;
		text = $"[{p_send_byte_array[6]:X2}]";
		p_script_view += text;
		text = $"[{p_send_byte_array[7]:X2}]";
		p_script_view += text;
		for (num = 0; num < p_num_bytes_to_write; num++) {
			text = $"[{p_send_byte_array[num + 8]:X2}]";
			p_script_view += text;
		}

		text = $"[{p_start_crc:X2}]";
		p_script_view += text;
		p_script_view += "[P_]";
		USBRead.Clear_Data_Array(0u);
		USBRead.Clear_Raw_Data_Array();
		return USBWrite.Send_Script_To_PICkitS(ref p_send_byte_array);
	}

	public static bool Read(byte p_slave_addr, byte p_command1, byte p_command2, byte p_num_bytes_to_read, ref byte[] p_data_array, ref string p_script_view) {
		return Basic.Send_I2CRead_Word_Cmd(p_slave_addr, p_command1, p_command2, p_num_bytes_to_read, ref p_data_array, ref p_script_view);
	}

	public static bool Read(byte p_slave_addr, byte p_start_data_addr, byte p_num_bytes_to_read, ref byte[] p_data_array, ref string p_script_view) {
		return Basic.Send_I2CRead_Cmd(p_slave_addr, p_start_data_addr, p_num_bytes_to_read, ref p_data_array, ref p_script_view);
	}

	public static bool Receive(byte p_slave_addr, byte p_num_bytes_to_read, ref byte[] p_data_array, ref string p_script_view) {
		return Basic.Send_I2C_SimpleRead_Cmd(p_slave_addr, p_num_bytes_to_read, ref p_data_array, ref p_script_view);
	}

	public static bool Set_I2C_Bit_Rate(double p_Bit_Rate) {
		bool result = false;
		byte b = 0;
		string p_str = "";
		string p_result_str = "";
		byte[] p_data = new byte[65];
		byte[] p_status_packet = new byte[65];
		if (p_Bit_Rate < 39.1 || p_Bit_Rate > 5000.0) {
			return result;
		}

		if (Utilities.m_flags.HID_read_handle != IntPtr.Zero) {
			Array.Clear(p_data, 0, p_data.Length);
			Array.Clear(p_status_packet, 0, p_status_packet.Length);
			if (!Basic.Get_Status_Packet(ref p_status_packet)) {
				return false;
			}

			b = (byte)(Math.Round(20000.0 / p_Bit_Rate / 4.0) - 1.0);
			p_status_packet[30] = b;
			USBWrite.configure_outbound_control_block_packet(ref p_data, ref p_str, ref p_status_packet);
			result = USBWrite.write_and_verify_config_block(ref p_data, ref p_result_str, p_perform_warm_and_cold_reset: true, ref p_str);
		}

		return result;
	}

	public static double Get_I2C_Bit_Rate() {
		byte[] p_status_packet = new byte[65];
		double result = 0.0;
		if (Utilities.m_flags.HID_read_handle != IntPtr.Zero) {
			Array.Clear(p_status_packet, 0, p_status_packet.Length);
			if (Basic.Get_Status_Packet(ref p_status_packet)) {
				result = calculate_baud_rate(p_status_packet[51]);
			}
		}

		return result;
	}

	private static double calculate_baud_rate(ushort p_baud) {
		double num = 20.0 / (4.0 * ((double)(int)p_baud + 1.0));
		return num * 1000.0;
	}

	public static void Set_Read_Wait_Time(int p_time) {
		Basic.m_i2cs_read_wait_time = p_time;
	}

	public static int Get_Read_Wait_Time() {
		return Basic.m_i2cs_read_wait_time;
	}

	public static void Set_Receive_Wait_Time(int p_time) {
		Basic.m_i2cs_receive_wait_time = p_time;
	}

	public static int Get_Receive_Wait_Time() {
		return Basic.m_i2cs_receive_wait_time;
	}

	public static bool Get_Source_Voltage(ref double p_voltage, ref bool p_PKSA_power) {
		return USART.Get_Source_Voltage(ref p_voltage, ref p_PKSA_power);
	}

	public static bool Set_Source_Voltage(double p_voltage) {
		return USART.Set_Source_Voltage(p_voltage);
	}

	public static bool Tell_PKSA_To_Use_External_Voltage_Source() {
		return USART.Tell_PKSA_To_Use_External_Voltage_Source();
	}

	public static bool Get_Aux_Status(ref bool p_aux1_state, ref bool p_aux2_state, ref bool p_aux1_dir, ref bool p_aux2_dir) {
		return USART.Get_Aux_Status(ref p_aux1_state, ref p_aux2_state, ref p_aux1_dir, ref p_aux2_dir);
	}

	public static bool Set_Aux1_Direction(bool p_dir) {
		return USART.Set_Aux1_Direction(p_dir);
	}

	public static bool Set_Aux2_Direction(bool p_dir) {
		return USART.Set_Aux2_Direction(p_dir);
	}

	public static bool Set_Aux1_State(bool p_state) {
		return USART.Set_Aux1_State(p_state);
	}

	public static bool Set_Aux2_State(bool p_state) {
		return USART.Set_Aux2_State(p_state);
	}

	public static bool Set_Pullup_State(bool p_enable) {
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

			if (p_enable) {
				p_status_packet[16] |= 16;
			} else {
				p_status_packet[16] &= 239;
			}

			USBWrite.configure_outbound_control_block_packet(ref p_data, ref p_str, ref p_status_packet);
			result = USBWrite.write_and_verify_config_block(ref p_data, ref p_result_str, p_perform_warm_and_cold_reset: false, ref p_str);
		}

		return result;
	}
}