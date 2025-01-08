using System;
using System.Diagnostics;
using System.IO;

namespace PICkitS;

public class Device {
	public static bool Initialize_PICkitSerial() {
		return Basic.Initialize_PICkitSerial();
	}

	public static bool Initialize_PICkitSerial(ushort USBIndex) {
		return Basic.Initialize_PICkitSerial(USBIndex);
	}

	public static bool Initialize_MyDevice(ushort USBIndex, ushort ProductID) {
		return Basic.Initialize_MyDevice(USBIndex, ProductID);
	}

	public static bool Find_ThisDevice(ushort VendorID, ushort ProductID) {
		IntPtr p_ReadHandle = IntPtr.Zero;
		IntPtr p_WriteHandle = IntPtr.Zero;
		Guid p_HidGuid = Guid.Empty;
		string p_devicepath = "";
		ushort p_num_devices_attached = 0;
		bool flag = USB.Get_This_Device(VendorID, ProductID, 0, ref p_ReadHandle, ref p_WriteHandle, ref p_devicepath, p_pass_ptr_to_handle: false, ref p_HidGuid, ref p_num_devices_attached);
		if (flag) {
			flag = USBRead.Kick_Off_Read_Thread();
			if (flag) {
				flag = USBWrite.kick_off_write_thread();
			}
		}

		return flag;
	}

	public static ushort How_Many_PICkitSerials_Are_Attached() {
		return Basic.How_Many_PICkitSerials_Are_Attached();
	}

	public static ushort How_Many_Of_MyDevices_Are_Attached(ushort ProductID) {
		return Basic.How_Many_Of_MyDevices_Are_Attached(ProductID);
	}

	public static void Terminate_Comm_Threads() {
		Basic.Terminate_Comm_Threads();
	}

	public static bool ReEstablish_Comm_Threads() {
		return Basic.ReEstablish_Comm_Threads();
	}

	public static void Cleanup() {
		Basic.Cleanup();
	}

	public static bool There_Is_A_Status_Error(ref uint p_error) {
		return Basic.There_Is_A_Status_Error(ref p_error);
	}

	public static int Get_Script_Timeout() {
		return Basic.Get_Script_Timeout();
	}

	public static void Set_Script_Timeout(int p_time) {
		Basic.Set_Script_Timeout(p_time);
	}

	public static bool Get_Status_Packet(ref byte[] p_status_packet) {
		return Basic.Get_Status_Packet(ref p_status_packet);
	}

	public static void Reset_Control_Block() {
		Basic.Reset_Control_Block();
	}

	public static void Clear_Status_Errors() {
		USBWrite.Clear_Status_Errors();
	}

	public static bool Clear_Comm_Errors() {
		bool result = false;
		I2CS.reset_buffers();
		if (USBWrite.Send_CommReset_Cmd() && USBWrite.Send_Warm_Reset_Cmd()) {
			result = true;
		}

		return result;
	}

	public static void Flash_LED1_For_2_Seconds() {
		Basic.Flash_LED1_For_2_Seconds();
	}

	public static bool Set_Buffer_Flush_Parameters(bool p_flush_on_count, bool p_flush_on_time, byte p_flush_byte_count, double p_flush_interval) {
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

			if (p_flush_on_count) {
				p_status_packet[7] |= 64;
			} else {
				p_status_packet[7] &= 191;
			}

			if (p_flush_on_time) {
				p_status_packet[7] |= 128;
			} else {
				p_status_packet[7] &= 127;
			}

			p_status_packet[10] = p_flush_byte_count;
			byte b = (byte)Math.Round(p_flush_interval / 0.409);
			p_status_packet[11] = b;
			USBWrite.configure_outbound_control_block_packet(ref p_data, ref p_str, ref p_status_packet);
			result = USBWrite.write_and_verify_config_block(ref p_data, ref p_result_str, p_perform_warm_and_cold_reset: false, ref p_str);
		}

		return result;
	}

	public static bool Get_Buffer_Flush_Parameters(ref bool p_flush_on_count, ref bool p_flush_on_time, ref byte p_flush_byte_count, ref double p_flush_interval) {
		bool result = false;
		byte[] p_status_packet = new byte[65];
		if (Utilities.m_flags.HID_read_handle != IntPtr.Zero) {
			Array.Clear(p_status_packet, 0, p_status_packet.Length);
			if (Basic.Get_Status_Packet(ref p_status_packet)) {
				p_flush_on_count = (p_status_packet[7] & 0x40) > 0;
				p_flush_on_time = (p_status_packet[7] & 0x80) > 0;
				p_flush_byte_count = p_status_packet[10];
				p_flush_interval = (double)(int)p_status_packet[11] * 0.409;
				if (p_flush_interval == 0.0) {
					p_flush_interval = 0.409;
				}

				result = true;
			}
		}

		return result;
	}

	public static bool Set_Buffer_Flush_Time(double p_time) {
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

			byte b = (byte)Math.Round(p_time / 0.409);
			p_status_packet[11] = b;
			USBWrite.configure_outbound_control_block_packet(ref p_data, ref p_str, ref p_status_packet);
			result = USBWrite.write_and_verify_config_block(ref p_data, ref p_result_str, p_perform_warm_and_cold_reset: false, ref p_str);
		}

		return result;
	}

	public static double Get_Buffer_Flush_Time() {
		double num = 9999.0;
		byte[] p_status_packet = new byte[65];
		if (Utilities.m_flags.HID_read_handle != IntPtr.Zero) {
			Array.Clear(p_status_packet, 0, p_status_packet.Length);
			if (!Basic.Get_Status_Packet(ref p_status_packet)) {
				return 9999.0;
			}

			num = (double)(int)p_status_packet[11] * 0.409;
			if (num == 0.0) {
				num = 0.409;
			}
		}

		return num;
	}

	public static void Set_Script_Timeout_Option(bool p_use_timeout) {
		USBWrite.m_use_script_timeout = p_use_timeout;
	}

	public static bool Get_Script_Timeout_Option() {
		return USBWrite.m_use_script_timeout;
	}

	public static bool Get_PKSA_FW_Version(ref ushort p_version, ref string p_str_fw_ver) {
		bool result = false;
		byte[] p_status_packet = new byte[65];
		if (Basic.Get_Status_Packet(ref p_status_packet)) {
			p_version = (ushort)((p_status_packet[4] << 8) + p_status_packet[3]);
			p_str_fw_ver = $"0x{p_version:X4}";
			result = true;
		}

		return result;
	}

	public static bool Get_PickitS_DLL_Version(ref string p_version) {
		bool result = false;
		string currentDirectory = Directory.GetCurrentDirectory();
		currentDirectory += "\\PICkitS.dll";
		if (File.Exists(currentDirectory)) {
			result = true;
			FileVersionInfo versionInfo = FileVersionInfo.GetVersionInfo(currentDirectory);
			p_version = versionInfo.FileVersion;
			versionInfo = null;
		}

		return result;
	}
}