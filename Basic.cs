using Microsoft.VisualBasic;
using System;
using System.Threading;
using static System.Windows.Forms.VisualStyles.VisualStyleElement;

namespace PICkitS;

public class Basic {
	private static Mutex m_reset_cb = new Mutex(initiallyOwned: false);

	private static Thread m_flash_led;

	private static Thread m_reset_control_block;

	internal static int m_i2cs_read_wait_time = 200;

	internal static int m_i2cs_receive_wait_time = 200;

	internal static int m_spi_receive_wait_time = 200;

	public static uint Retrieve_USART_Data_Byte_Count() {
		return USBRead.Retrieve_Data_Byte_Count();
	}

	public static bool Retrieve_USART_Data(uint p_byte_count, ref byte[] p_data_array) {
		bool result = false;
		if (USBRead.Retrieve_Data(ref p_data_array, p_byte_count)) {
			result = true;
		}

		return result;
	}

	public static bool Initialize_PICkitSerial() {
		IntPtr p_ReadHandle = IntPtr.Zero;
		IntPtr p_WriteHandle = IntPtr.Zero;
		Guid p_HidGuid = Guid.Empty;
		string p_devicepath = "";
		ushort p_PoductID = 54;
		ushort p_VendorID = 1240;
		ushort p_num_devices_attached = 0;
		bool flag = USB.Get_This_Device(p_VendorID, p_PoductID, 0, ref p_ReadHandle, ref p_WriteHandle, ref p_devicepath, p_pass_ptr_to_handle: false, ref p_HidGuid, ref p_num_devices_attached);
		if (flag) {
			flag = USBRead.Kick_Off_Read_Thread();
			if (flag) {
				flag = USBWrite.kick_off_write_thread();
			}
		}

		return flag;
	}

	public static bool Initialize_PICkitSerial(ushort USBIndex) {
		IntPtr p_ReadHandle = IntPtr.Zero;
		IntPtr p_WriteHandle = IntPtr.Zero;
		Guid p_HidGuid = Guid.Empty;
		string p_devicepath = "";
		ushort p_PoductID = 54;
		ushort p_VendorID = 1240;
		ushort p_num_devices_attached = 0;
		bool flag = USB.Get_This_Device(p_VendorID, p_PoductID, USBIndex, ref p_ReadHandle, ref p_WriteHandle, ref p_devicepath, p_pass_ptr_to_handle: false, ref p_HidGuid, ref p_num_devices_attached);
		if (flag) {
			flag = USBRead.Kick_Off_Read_Thread();
			if (flag) {
				flag = USBWrite.kick_off_write_thread();
			}
		}

		return flag;
	}

	public static bool Initialize_MyDevice(ushort USBIndex, ushort ProductID) {
		IntPtr p_ReadHandle = IntPtr.Zero;
		IntPtr p_WriteHandle = IntPtr.Zero;
		Guid p_HidGuid = Guid.Empty;
		string p_devicepath = "";
		ushort p_VendorID = 1240;
		ushort p_num_devices_attached = 0;
		bool flag = USB.Get_This_Device(p_VendorID, ProductID, USBIndex, ref p_ReadHandle, ref p_WriteHandle, ref p_devicepath, p_pass_ptr_to_handle: false, ref p_HidGuid, ref p_num_devices_attached);
		if (flag) {
			flag = USBRead.Kick_Off_Read_Thread();
			if (flag) {
				flag = USBWrite.kick_off_write_thread();
				if (ProductID == 80) {
					Utilities.g_comm_mode = Utilities.COMM_MODE.MTOUCH2;
				}
			}
		}

		return flag;
	}

	public static ushort How_Many_PICkitSerials_Are_Attached() {
		IntPtr p_ReadHandle = IntPtr.Zero;
		IntPtr p_WriteHandle = IntPtr.Zero;
		Guid p_HidGuid = Guid.Empty;
		string p_devicepath = "";
		ushort p_PoductID = 54;
		ushort p_VendorID = 1240;
		return USB.Count_Attached_PKSA(p_VendorID, p_PoductID, 29, ref p_ReadHandle, ref p_WriteHandle, ref p_devicepath, p_pass_ptr_to_handle: false, ref p_HidGuid);
	}

	public static ushort How_Many_Of_MyDevices_Are_Attached(ushort ProductID) {
		IntPtr p_ReadHandle = IntPtr.Zero;
		IntPtr p_WriteHandle = IntPtr.Zero;
		Guid p_HidGuid = Guid.Empty;
		string p_devicepath = "";
		ushort p_VendorID = 1240;
		return USB.Count_Attached_PKSA(p_VendorID, ProductID, 29, ref p_ReadHandle, ref p_WriteHandle, ref p_devicepath, p_pass_ptr_to_handle: false, ref p_HidGuid);
	}

	public static void Terminate_Comm_Threads() {
		USBRead.Kill_Read_Thread();
		USBWrite.Kill_Write_Thread();
	}

	public static bool ReEstablish_Comm_Threads() {
		if (USBRead.Kick_Off_Read_Thread()) {
			return USBWrite.kick_off_write_thread();
		}

		return false;
	}

	public static bool Configure_PICkitSerial_For_I2C() {
		return Configure_PICkitSerial(1, p_reset: true);
	}

	public static bool Configure_PICkitSerial_For_LIN() {
		return Configure_PICkitSerial(10, p_reset: true);
	}

	public static bool Configure_PICkitSerial(int p_mode, bool p_reset) {
		bool result = false;
		string p_str = "";
		string p_result_str = "";
		byte[] p_data = new byte[65];
		byte[] p_status_packet_data = new byte[65];
		if (Utilities.m_flags.HID_read_handle != IntPtr.Zero) {
			Array.Clear(p_data, 0, p_data.Length);
			Array.Clear(p_status_packet_data, 0, p_status_packet_data.Length);
			Mode.update_status_packet_data(p_mode, ref p_status_packet_data);
			USBWrite.configure_outbound_control_block_packet(ref p_data, ref p_str, ref p_status_packet_data);
			result = USBWrite.write_and_verify_config_block(ref p_data, ref p_result_str, p_reset, ref p_str);
		}

		return result;
	}

	public static void Cleanup() {
		if (USBRead.Read_Thread_Is_Active()) {
			USBRead.Kill_Read_Thread();
			Thread.Sleep(500);
		}

		USBWrite.Kill_Write_Thread();
		USBWrite.Dispose_Of_Write_Objects();
		Utilities.CloseHandle(Utilities.m_flags.HID_write_handle);
		Utilities.CloseHandle(Utilities.m_flags.HID_read_handle);
		Utilities.m_flags.g_status_packet_data_update_event.Close();
		Utilities.m_flags.g_data_arrived_event.Close();
		Utilities.m_flags.g_bl_data_arrived_event.Close();
		Utilities.m_flags.g_status_packet_mutex.Close();
		Utilities.m_flags.g_PKSA_has_completed_script.Close();
		USBRead.m_usb_packet_mutex.Close();
		USBRead.m_cbuf2_data_array_mutex.Close();
	}

	public static bool Send_I2CWrite_Cmd(byte p_slave_addr, byte p_start_data_addr, byte p_num_bytes_to_write, ref byte[] p_data_array, ref string p_script_view) {
		bool flag = false;
		if (p_num_bytes_to_write > 253) {
			return false;
		}

		int num = 0;
		byte[] p_send_byte_array = new byte[300];
		Array.Clear(p_send_byte_array, 0, p_send_byte_array.Length);
		p_send_byte_array[0] = 0;
		p_send_byte_array[1] = 3;
		p_send_byte_array[2] = (byte)(8 + p_num_bytes_to_write);
		p_send_byte_array[3] = 129;
		p_send_byte_array[4] = 132;
		p_send_byte_array[5] = (byte)(2 + p_num_bytes_to_write);
		p_send_byte_array[6] = p_slave_addr;
		p_send_byte_array[7] = p_start_data_addr;
		for (num = 0; num < p_num_bytes_to_write; num++) {
			p_send_byte_array[num + 8] = p_data_array[num];
		}

		p_send_byte_array[num + 8] = 130;
		p_send_byte_array[num + 9] = 31;
		p_send_byte_array[num + 10] = 119;
		p_send_byte_array[num + 11] = 0;
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

		p_script_view += "[P_]";
		USBRead.Clear_Data_Array(0u);
		USBRead.Clear_Raw_Data_Array();
		return USBWrite.Send_Script_To_PICkitS(ref p_send_byte_array);
	}

	public static bool Send_I2CWrite_Word_Cmd(byte p_slave_addr, byte p_command1, byte p_command2, byte p_num_bytes_to_write, ref byte[] p_data_array, ref string p_script_view) {
		bool flag = false;
		if (p_num_bytes_to_write > 253) {
			return false;
		}

		int num = 0;
		byte[] p_send_byte_array = new byte[300];
		Array.Clear(p_send_byte_array, 0, p_send_byte_array.Length);
		p_send_byte_array[0] = 0;
		p_send_byte_array[1] = 3;
		p_send_byte_array[2] = (byte)(9 + p_num_bytes_to_write);
		p_send_byte_array[3] = 129;
		p_send_byte_array[4] = 132;
		p_send_byte_array[5] = (byte)(3 + p_num_bytes_to_write);
		p_send_byte_array[6] = p_slave_addr;
		p_send_byte_array[7] = p_command1;
		p_send_byte_array[8] = p_command2;
		for (num = 0; num < p_num_bytes_to_write; num++) {
			p_send_byte_array[num + 9] = p_data_array[num];
		}

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
		text = $"[{p_send_byte_array[8]:X2}]";
		p_script_view += text;
		for (num = 0; num < p_num_bytes_to_write; num++) {
			text = $"[{p_send_byte_array[num + 9]:X2}]";
			p_script_view += text;
		}

		p_script_view += "[P_]";
		USBRead.Clear_Data_Array(0u);
		USBRead.Clear_Raw_Data_Array();
		return USBWrite.Send_Script_To_PICkitS(ref p_send_byte_array);
	}

	public static bool Send_SPI_Send_Cmd(byte p_byte_count, ref byte[] p_data, bool p_first_cmd, bool p_last_cmd, ref string p_script_view) {
		bool flag = false;
		byte[] p_send_byte_array = new byte[255];
		int num = 3;
		int num2 = 0;
		string text = "";
		if (p_byte_count > 246) {
			return false;
		}

		p_script_view = "";
		Array.Clear(p_send_byte_array, 0, p_send_byte_array.Length);
		p_send_byte_array[0] = 0;
		p_send_byte_array[1] = 3;
		if (p_first_cmd) {
			p_send_byte_array[num++] = 139;
			p_script_view = "[CSON]";
		}

		p_send_byte_array[num++] = 133;
		p_send_byte_array[num++] = p_byte_count;
		p_script_view += "[DO]";
		text = $"[{p_byte_count:X2}]";
		p_script_view += text;
		for (num2 = 0; num2 < p_byte_count; num2++) {
			p_send_byte_array[num++] = p_data[num2];
			text = $"[{p_data[num2]:X2}]";
			p_script_view += text;
		}

		if (p_last_cmd) {
			p_send_byte_array[num++] = 140;
			p_script_view += "[CSOF]";
		}

		p_send_byte_array[2] = (byte)(num - 1);
		p_send_byte_array[num++] = 31;
		p_send_byte_array[num++] = 119;
		p_send_byte_array[num] = 0;
		USBRead.Clear_Data_Array(0u);
		USBRead.Clear_Raw_Data_Array();
		return USBWrite.Send_Script_To_PICkitS(ref p_send_byte_array);
	}

	public static bool Send_USART_Cmd(byte p_byte_count, ref byte[] p_data, ref string p_script_view) {
		bool flag = false;
		byte[] p_send_byte_array = new byte[255];
		int num = 5;
		int num2 = 0;
		string text = "";
		p_script_view = "";
		if (p_byte_count > 247) {
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
			p_send_byte_array[num++] = p_data[num2];
			text = $"[{p_data[num2]:X2}]";
			p_script_view += text;
		}

		p_send_byte_array[num++] = 31;
		p_send_byte_array[num++] = 119;
		p_send_byte_array[num] = 0;
		return USBWrite.Send_Script_To_PICkitS(ref p_send_byte_array);
	}

	public static bool Send_I2CRead_Cmd(byte p_slave_addr, byte p_start_data_addr, byte p_num_bytes_to_read, ref byte[] p_data_array, ref string p_script_view) {
		bool flag = false;
		if (p_num_bytes_to_read == 0) {
			return false;
		}

		byte[] p_send_byte_array = new byte[300];
		Array.Clear(p_send_byte_array, 0, p_send_byte_array.Length);
		p_send_byte_array[0] = 0;
		p_send_byte_array[1] = 3;
		p_send_byte_array[2] = 14;
		p_send_byte_array[3] = 129;
		p_send_byte_array[4] = 132;
		p_send_byte_array[5] = 2;
		p_send_byte_array[6] = p_slave_addr;
		p_send_byte_array[7] = p_start_data_addr;
		p_send_byte_array[8] = 131;
		p_send_byte_array[9] = 132;
		p_send_byte_array[10] = 1;
		p_send_byte_array[11] = (byte)(p_slave_addr + 1);
		p_send_byte_array[12] = 137;
		p_send_byte_array[13] = p_num_bytes_to_read;
		p_send_byte_array[14] = 130;
		p_send_byte_array[15] = 31;
		p_send_byte_array[16] = 119;
		p_send_byte_array[17] = 0;
		p_script_view = "[S_][W_][02]";
		string text = $"[{p_send_byte_array[6]:X2}]";
		p_script_view += text;
		text = $"[{p_send_byte_array[7]:X2}]";
		p_script_view += text;
		p_script_view += "[RS][W_][01]";
		text = $"[{p_send_byte_array[11]:X2}]";
		p_script_view += text;
		p_script_view += "[RN]";
		text = $"[{p_send_byte_array[13]:X2}]";
		p_script_view += text;
		p_script_view += "[P_]";
		USBRead.Clear_Data_Array(p_num_bytes_to_read);
		USBRead.Clear_Raw_Data_Array();
		if (USBWrite.Send_Script_To_PICkitS(ref p_send_byte_array)) {
			if (Utilities.m_flags.g_data_arrived_event.WaitOne(m_i2cs_read_wait_time, exitContext: false)) {
				if (USBRead.Retrieve_Data(ref p_data_array, p_num_bytes_to_read)) {
					return true;
				}

				return false;
			}

			return false;
		}

		return false;
	}

	public static bool Send_I2CRead_Word_Cmd(byte p_slave_addr, byte p_command1, byte p_command2, byte p_num_bytes_to_read, ref byte[] p_data_array, ref string p_script_view) {
		bool flag = false;
		if (p_num_bytes_to_read == 0) {
			return false;
		}

		byte[] p_send_byte_array = new byte[300];
		Array.Clear(p_send_byte_array, 0, p_send_byte_array.Length);
		p_send_byte_array[0] = 0;
		p_send_byte_array[1] = 3;
		p_send_byte_array[2] = 15;
		p_send_byte_array[3] = 129;
		p_send_byte_array[4] = 132;
		p_send_byte_array[5] = 3;
		p_send_byte_array[6] = p_slave_addr;
		p_send_byte_array[7] = p_command1;
		p_send_byte_array[8] = p_command2;
		p_send_byte_array[9] = 131;
		p_send_byte_array[10] = 132;
		p_send_byte_array[11] = 1;
		p_send_byte_array[12] = (byte)(p_slave_addr + 1);
		p_send_byte_array[13] = 137;
		p_send_byte_array[14] = p_num_bytes_to_read;
		p_send_byte_array[15] = 130;
		p_send_byte_array[16] = 31;
		p_send_byte_array[17] = 119;
		p_send_byte_array[18] = 0;
		p_script_view = "[S_][W_][03]";
		string text = $"[{p_send_byte_array[6]:X2}]";
		p_script_view += text;
		text = $"[{p_send_byte_array[7]:X2}]";
		p_script_view += text;
		text = $"[{p_send_byte_array[8]:X2}]";
		p_script_view += text;
		p_script_view += "[RS][W_][01]";
		text = $"[{p_send_byte_array[12]:X2}]";
		p_script_view += text;
		p_script_view += "[RN]";
		text = $"[{p_send_byte_array[14]:X2}]";
		p_script_view += text;
		p_script_view += "[P_]";
		USBRead.Clear_Data_Array(p_num_bytes_to_read);
		USBRead.Clear_Raw_Data_Array();
		if (USBWrite.Send_Script_To_PICkitS(ref p_send_byte_array)) {
			if (Utilities.m_flags.g_data_arrived_event.WaitOne(m_i2cs_read_wait_time, exitContext: false)) {
				if (USBRead.Retrieve_Data(ref p_data_array, p_num_bytes_to_read)) {
					return true;
				}

				return false;
			}

			return false;
		}

		return false;
	}

	public static bool Send_I2C_SimpleRead_Cmd(byte p_slave_addr, byte p_num_bytes_to_read, ref byte[] p_data_array, ref string p_script_view) {
		bool result = false;
		if (p_num_bytes_to_read == 0) {
			return false;
		}

		byte[] p_send_byte_array = new byte[300];
		Array.Clear(p_send_byte_array, 0, p_send_byte_array.Length);
		p_send_byte_array[0] = 0;
		p_send_byte_array[1] = 3;
		p_send_byte_array[2] = 9;
		p_send_byte_array[3] = 129;
		p_send_byte_array[4] = 132;
		p_send_byte_array[5] = 1;
		p_send_byte_array[6] = p_slave_addr;
		p_send_byte_array[7] = 137;
		p_send_byte_array[8] = p_num_bytes_to_read;
		p_send_byte_array[9] = 130;
		p_send_byte_array[10] = 31;
		p_send_byte_array[11] = 119;
		p_send_byte_array[12] = 0;
		p_script_view = "[S_][W_][01]";
		string text = $"[{p_send_byte_array[6]:X2}]";
		p_script_view += text;
		p_script_view += "[RN]";
		text = $"[{p_send_byte_array[8]:X2}]";
		p_script_view += text;
		p_script_view += "[P_]";
		USBRead.Clear_Data_Array(p_num_bytes_to_read);
		USBRead.Clear_Raw_Data_Array();
		if (USBWrite.Send_Script_To_PICkitS(ref p_send_byte_array) && Utilities.m_flags.g_data_arrived_event.WaitOne(m_i2cs_receive_wait_time, exitContext: false) && USBRead.Retrieve_Data(ref p_data_array, p_num_bytes_to_read)) {
			result = true;
		}

		return result;
	}

	public static bool Send_SPI_Receive_Cmd(byte p_num_bytes_to_read, ref byte[] p_data_array, bool p_first_cmd, bool p_last_cmd, ref string p_script_view) {
		bool result = false;
		byte[] p_send_byte_array = new byte[255];
		int num = 3;
		p_script_view = "";
		string text = "";
		Array.Clear(p_send_byte_array, 0, p_send_byte_array.Length);
		p_send_byte_array[0] = 0;
		p_send_byte_array[1] = 3;
		if (p_first_cmd) {
			p_send_byte_array[num++] = 139;
			p_script_view = "[CSON]";
		}

		p_send_byte_array[num++] = 132;
		p_send_byte_array[num++] = p_num_bytes_to_read;
		p_script_view += "[DI]";
		text = $"[{p_num_bytes_to_read:X2}]";
		p_script_view += text;
		if (p_last_cmd) {
			p_send_byte_array[num++] = 140;
			p_script_view += "[CSOF]";
		}

		p_send_byte_array[2] = (byte)(num - 1);
		p_send_byte_array[num++] = 31;
		p_send_byte_array[num++] = 119;
		p_send_byte_array[num] = 0;
		USBRead.Clear_Data_Array(p_num_bytes_to_read);
		USBRead.Clear_Raw_Data_Array();
		if (USBWrite.Send_Script_To_PICkitS(ref p_send_byte_array) && Utilities.m_flags.g_data_arrived_event.WaitOne(m_spi_receive_wait_time, exitContext: false) && USBRead.Retrieve_Data(ref p_data_array, p_num_bytes_to_read)) {
			result = true;
		}

		return result;
	}

	public static bool There_Is_A_Status_Error(ref uint p_error) {
		return Status.There_Is_A_Status_Error(ref p_error);
	}

	public static int Get_Script_Timeout() {
		return USBWrite.m_universal_timeout;
	}

	public static void Set_Script_Timeout(int p_time) {
		USBWrite.m_universal_timeout = p_time;
	}

	public static bool Get_Status_Packet(ref byte[] p_status_packet) {
		bool result = false;
		if (USBWrite.Update_Status_Packet()) {
			Utilities.m_flags.g_status_packet_mutex.WaitOne();
			for (int i = 0; i < Constants.STATUS_PACKET_DATA.Length; i++) {
				p_status_packet[i] = Constants.STATUS_PACKET_DATA[i];
			}

			Utilities.m_flags.g_status_packet_mutex.ReleaseMutex();
			result = true;
		}

		return result;
	}

	public static void Reset_Control_Block() {
		m_reset_control_block = new Thread(Reset_Control_Block_Thread);
		m_reset_control_block.IsBackground = true;
		m_reset_control_block.Start();
	}

	private static void Reset_Control_Block_Thread() {
		m_reset_cb.WaitOne();
		string p_str = "";
		byte[] p_status_packet_data = new byte[65];
		byte[] p_data = new byte[65];
		string p_cb_data_str = "";
		Array.Clear(p_status_packet_data, 0, p_status_packet_data.Length);
		Array.Clear(p_data, 0, p_data.Length);
		I2CS.reset_buffers();
		if (USBWrite.Update_Status_Packet()) {
			Utilities.m_flags.g_status_packet_mutex.WaitOne();
			for (int i = 0; i < p_status_packet_data.Length; i++) {
				p_status_packet_data[i] = Constants.STATUS_PACKET_DATA[i];
			}

			Utilities.m_flags.g_status_packet_mutex.ReleaseMutex();
			USBWrite.configure_outbound_control_block_packet(ref p_data, ref p_str, ref p_status_packet_data);
			USBWrite.write_and_verify_config_block(ref p_data, ref p_str, p_perform_warm_and_cold_reset: true, ref p_cb_data_str);
		}

		m_reset_cb.ReleaseMutex();
	}

	public static void Flash_LED1_For_2_Seconds() {
		m_flash_led = new Thread(flash_the_busy_led);
		m_flash_led.IsBackground = true;
		m_flash_led.Start();
	}

	private static void flash_the_busy_led() {
		byte p_value = 193;
		string p_str = "";
		string p_result_str = "";
		byte[] p_data = new byte[65];
		byte[] p_status_packet = new byte[65];
		if (!(Utilities.m_flags.HID_read_handle != IntPtr.Zero)) {
			return;
		}

		Array.Clear(p_data, 0, p_data.Length);
		Array.Clear(p_status_packet, 0, p_status_packet.Length);
		if (Get_Status_Packet(ref p_status_packet)) {
			p_status_packet[7] |= 32;
			USBWrite.configure_outbound_control_block_packet(ref p_data, ref p_str, ref p_status_packet);
			USBWrite.write_and_verify_config_block(ref p_data, ref p_result_str, p_perform_warm_and_cold_reset: true, ref p_str);
			if (USBWrite.Send_LED_State_Cmd(1, p_value)) {
				p_status_packet[7] &= 223;
				Thread.Sleep(2000);
				USBWrite.configure_outbound_control_block_packet(ref p_data, ref p_str, ref p_status_packet);
				USBWrite.write_and_verify_config_block(ref p_data, ref p_result_str, p_perform_warm_and_cold_reset: true, ref p_str);
			}
		}
	}
}