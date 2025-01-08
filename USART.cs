using System;

namespace PICkitS;

public class USART {
	public static bool Configure_PICkitSerial_For_USARTAsync() {
		return Basic.Configure_PICkitSerial(4, p_reset: true);
	}

	public static bool Configure_PICkitSerial_For_USARTAsync(bool p_aux1_def, bool p_aux2_def, bool p_aux1_dir, bool p_aux2_dir, bool p_rcv_dis, double p_voltage) {
		bool result = false;
		int num = 0;
		if (Basic.Configure_PICkitSerial(4, p_reset: true)) {
			string p_str = "";
			string p_result_str = "";
			byte[] p_data = new byte[65];
			byte[] p_status_packet = new byte[65];
			if (Utilities.m_flags.HID_read_handle != IntPtr.Zero) {
				if (p_voltage < 0.0 || p_voltage > 5.0) {
					return result;
				}

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

				if (p_rcv_dis) {
					p_status_packet[24] |= 4;
				} else {
					p_status_packet[24] &= 251;
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

		return result;
	}

	public static bool Configure_PICkitSerial_For_USARTSyncMaster(bool p_aux1_def, bool p_aux2_def, bool p_aux1_dir, bool p_aux2_dir, bool p_clock_pol, double p_voltage) {
		bool result = false;
		int num = 0;
		if (Basic.Configure_PICkitSerial(5, p_reset: true)) {
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

				if (p_clock_pol) {
					p_status_packet[24] |= 1;
				} else {
					p_status_packet[24] &= 254;
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

		return result;
	}

	public static bool Configure_PICkitSerial_For_USARTSyncMaster() {
		return Basic.Configure_PICkitSerial(5, p_reset: true);
	}

	public static bool Configure_PICkitSerial_For_USARTSyncSlave() {
		return Basic.Configure_PICkitSerial(6, p_reset: true);
	}

	public static uint Retrieve_Data_Byte_Count() {
		return USBRead.Retrieve_Data_Byte_Count();
	}

	public static bool Retrieve_Data(uint p_byte_count, ref byte[] p_data_array) {
		bool result = false;
		if (USBRead.Retrieve_Data(ref p_data_array, p_byte_count)) {
			result = true;
		}

		return result;
	}

	public static bool Send_Data(byte p_byte_count, ref byte[] p_data_array, ref string p_script_view) {
		bool flag = false;
		byte[] p_send_byte_array = new byte[310];
		int num = 5;
		int num2 = 0;
		string text = "";
		p_script_view = "";
		if (p_byte_count > 251) {
			return false;
		}

		Array.Clear(p_send_byte_array, 0, p_send_byte_array.Length);
		p_send_byte_array[0] = 0;
		p_send_byte_array[1] = 3;
		p_send_byte_array[2] = (byte)(p_byte_count + 4);
		p_send_byte_array[3] = 130;
		p_send_byte_array[4] = p_byte_count;
		text = $"[{p_byte_count:X2}]";
		for (num2 = 0; num2 < p_byte_count; num2++) {
			p_send_byte_array[num++] = p_data_array[num2];
			text = $"[{p_data_array[num2]:X2}]";
			p_script_view += text;
		}

		p_send_byte_array[num++] = 31;
		p_send_byte_array[num++] = 119;
		p_send_byte_array[num] = 0;
		return USBWrite.Send_Script_To_PICkitS(ref p_send_byte_array);
	}

	public static bool Set_Baud_Rate(ushort p_baud) {
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

			int num = (int)Math.Round(20000000.0 / (double)(int)p_baud / 4.0) - 1;
			p_status_packet[29] = (byte)num;
			p_status_packet[30] = (byte)(num >> 8);
			USBWrite.configure_outbound_control_block_packet(ref p_data, ref p_str, ref p_status_packet);
			result = USBWrite.write_and_verify_config_block(ref p_data, ref p_result_str, p_perform_warm_and_cold_reset: true, ref p_str);
		}

		return result;
	}

	public static ushort Get_Baud_Rate() {
		byte[] p_status_packet = new byte[65];
		ushort result = 0;
		if (Utilities.m_flags.HID_read_handle != IntPtr.Zero) {
			Array.Clear(p_status_packet, 0, p_status_packet.Length);
			if (Basic.Get_Status_Packet(ref p_status_packet)) {
				result = calculate_baud_rate((ushort)(p_status_packet[50] + (p_status_packet[51] << 8)));
			}
		}

		return result;
	}

	private static ushort calculate_baud_rate(ushort p_baud) {
		double num = 20.0 / (4.0 * ((double)(int)p_baud + 1.0));
		int num2 = (int)Math.Round(num * 1000000.0);
		return (ushort)num2;
	}

	public static bool Get_Source_Voltage(ref double p_voltage, ref bool p_PKSA_power) {
		byte[] p_status_packet = new byte[65];
		bool result = false;
		if (Utilities.m_flags.HID_read_handle != IntPtr.Zero) {
			Array.Clear(p_status_packet, 0, p_status_packet.Length);
			if (Basic.Get_Status_Packet(ref p_status_packet)) {
				p_voltage = (double)(int)p_status_packet[39] * 5.0 / 255.0;
				p_PKSA_power = (p_status_packet[16] & 0x20) > 0;
				result = true;
			}
		}

		return result;
	}

	public static bool Set_Source_Voltage(double p_voltage) {
		bool result = false;
		int num = 0;
		string p_str = "";
		string p_result_str = "";
		byte[] p_data = new byte[65];
		byte[] p_status_packet = new byte[65];
		if (Utilities.m_flags.HID_read_handle != IntPtr.Zero) {
			if (p_voltage < 0.0 || p_voltage > 5.0) {
				return result;
			}

			Array.Clear(p_data, 0, p_data.Length);
			Array.Clear(p_status_packet, 0, p_status_packet.Length);
			if (!Basic.Get_Status_Packet(ref p_status_packet)) {
				return false;
			}

			num = (int)Math.Round((p_voltage * 1000.0 + 43.53) / 21.191);
			p_status_packet[19] = (byte)num;
			p_status_packet[20] = (byte)(num / 4);
			p_status_packet[16] |= 32;
			p_status_packet[16] |= 64;
			USBWrite.configure_outbound_control_block_packet(ref p_data, ref p_str, ref p_status_packet);
			result = USBWrite.write_and_verify_config_block(ref p_data, ref p_result_str, p_perform_warm_and_cold_reset: false, ref p_str);
		}

		return result;
	}

	public static bool Tell_PKSA_To_Use_External_Voltage_Source() {
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

			p_status_packet[16] &= 223;
			USBWrite.configure_outbound_control_block_packet(ref p_data, ref p_str, ref p_status_packet);
			result = USBWrite.write_and_verify_config_block(ref p_data, ref p_result_str, p_perform_warm_and_cold_reset: false, ref p_str);
		}

		return result;
	}

	public static bool Get_Aux_Status(ref bool p_aux1_state, ref bool p_aux2_state, ref bool p_aux1_dir, ref bool p_aux2_dir) {
		byte[] p_status_packet = new byte[65];
		bool result = false;
		if (Utilities.m_flags.HID_read_handle != IntPtr.Zero) {
			Array.Clear(p_status_packet, 0, p_status_packet.Length);
			if (Basic.Get_Status_Packet(ref p_status_packet)) {
				p_aux1_state = (p_status_packet[48] & 1) > 0;
				p_aux2_state = (p_status_packet[48] & 2) > 0;
				p_aux1_dir = (p_status_packet[48] & 4) > 0;
				p_aux2_dir = (p_status_packet[48] & 8) > 0;
				result = true;
			}
		}

		return result;
	}

	public static bool Set_Aux1_Direction(bool p_dir) {
		bool flag = false;
		byte[] p_send_byte_array = new byte[16];
		Array.Clear(p_send_byte_array, 0, p_send_byte_array.Length);
		p_send_byte_array[0] = 0;
		p_send_byte_array[1] = 3;
		p_send_byte_array[2] = 3;
		if (p_dir) {
			p_send_byte_array[3] = 147;
		} else {
			p_send_byte_array[3] = 146;
		}

		p_send_byte_array[4] = 31;
		p_send_byte_array[5] = 119;
		p_send_byte_array[6] = 0;
		return USBWrite.Send_Script_To_PICkitS(ref p_send_byte_array);
	}

	public static bool Set_Aux2_Direction(bool p_dir) {
		bool flag = false;
		byte[] p_send_byte_array = new byte[16];
		Array.Clear(p_send_byte_array, 0, p_send_byte_array.Length);
		p_send_byte_array[0] = 0;
		p_send_byte_array[1] = 3;
		p_send_byte_array[2] = 3;
		if (p_dir) {
			p_send_byte_array[3] = 153;
		} else {
			p_send_byte_array[3] = 152;
		}

		p_send_byte_array[4] = 31;
		p_send_byte_array[5] = 119;
		p_send_byte_array[6] = 0;
		return USBWrite.Send_Script_To_PICkitS(ref p_send_byte_array);
	}

	public static bool Set_Aux1_State(bool p_state) {
		bool flag = false;
		byte[] p_send_byte_array = new byte[16];
		Array.Clear(p_send_byte_array, 0, p_send_byte_array.Length);
		p_send_byte_array[0] = 0;
		p_send_byte_array[1] = 3;
		p_send_byte_array[2] = 3;
		if (p_state) {
			p_send_byte_array[3] = 145;
		} else {
			p_send_byte_array[3] = 144;
		}

		p_send_byte_array[4] = 31;
		p_send_byte_array[5] = 119;
		p_send_byte_array[6] = 0;
		return USBWrite.Send_Script_To_PICkitS(ref p_send_byte_array);
	}

	public static bool Set_Aux2_State(bool p_state) {
		bool flag = false;
		byte[] p_send_byte_array = new byte[16];
		Array.Clear(p_send_byte_array, 0, p_send_byte_array.Length);
		p_send_byte_array[0] = 0;
		p_send_byte_array[1] = 3;
		p_send_byte_array[2] = 3;
		if (p_state) {
			p_send_byte_array[3] = 151;
		} else {
			p_send_byte_array[3] = 150;
		}

		p_send_byte_array[4] = 31;
		p_send_byte_array[5] = 119;
		p_send_byte_array[6] = 0;
		return USBWrite.Send_Script_To_PICkitS(ref p_send_byte_array);
	}
}