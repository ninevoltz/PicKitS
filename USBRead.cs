using System;
using System.Threading;

namespace PICkitS;

public class USBRead {
	public delegate void DataNotifier();

	public delegate void USBNotifier();

	private const byte TAG_EOD_READ = 128;

	private const byte TAG_FW_VER = 129;

	private const byte TAG_CTRL_BLK_READ = 130;

	private const byte TAG_STATUS_BLOCK = 131;

	private const byte TAG_STATUS_CBUF = 132;

	private const byte TAG_CBUF_1_READ = 133;

	private const byte TAG_CBUF_2_READ = 134;

	private const byte TAG_CBUF_3_READ = 135;

	private const byte TAG_PACKET_ID = 136;

	public const byte TAG_COMMON_DATA_BYTE_V = 16;

	public const byte TAG_COMMON_DATA_BYTES_V = 17;

	public const byte TAG_COMMON_EVENT_MACRO_LOOP_V = 18;

	public const byte TAG_COMMON_EVENT_TIME_V = 19;

	public const byte TAG_COMMON_EVENT_TIME_ROLLOVER_V = 20;

	public const byte TAG_COMMON_EVENT_MACRO_DONE_V = 21;

	public const byte TAG_COMMON_EVENT_MACRO_ROLLOVER_V = 22;

	public const byte TAG_COMMON_EVENT_MACRO_ABORT_V = 23;

	public const byte TAG_COMMON_EVENT_TIMEOUT_AB0_V = 24;

	public const byte TAG_COMMON_EVENT_TIMEOUT_AB1_V = 25;

	public const byte TAG_COMMON_EVENT_STATUS_ERROR_V = 26;

	public const byte TAG_COMMON_EVENT_END_OF_SCRIPT_V = 27;

	public const byte TAG_COMMON_MARKER_V = 28;

	public const byte TAG_I2C_EVENT_START_TX_V = 128;

	public const byte TAG_I2C_EVENT_STOP_TX_V = 129;

	public const byte TAG_I2C_EVENT_RESTART_TX_V = 130;

	public const byte TAG_I2C_EVENT_ACK_TX_V = 131;

	public const byte TAG_I2C_EVENT_NACK_TX_V = 132;

	public const byte TAG_I2C_EVENT_ACK_RX_V = 133;

	public const byte TAG_I2C_EVENT_NACK_RX_V = 134;

	public const byte TAG_I2C_EVENT_BYTE_TX_V = 135;

	public const byte TAG_I2C_EVENT_BYTE_RX_V = 136;

	public const byte TAG_I2C_EVENT_XACT_ERR_V = 137;

	public const byte TAG_I2C_EVENT_STATUS_ERR_V = 138;

	public const byte TAG_I2CS_EVENT_ADDR_RX_V = 192;

	public const byte TAG_I2CS_EVENT_DATA_RX_V = 193;

	public const byte TAG_I2CS_EVENT_DATA_TX_V = 194;

	public const byte TAG_I2CS_EVENT_ACK_RCV_V = 195;

	public const byte TAG_I2CS_EVENT_NACK_RCV_V = 196;

	public const byte TAG_I2CS_EVENT_STOP_V = 198;

	public const byte TAG_I2CS_EVENT_STATUS_ERROR_V = 199;

	public const byte TAG_I2CS_EVENT_DATA_RQ_V = 200;

	public const byte TAG_I2CS_EVENT_REG_READ_V = 201;

	public const byte TAG_I2CS_EVENT_REG_WRITE_V = 202;

	public const byte TAG_I2CS_EVENT_REG_DATA_V = 203;

	public const byte TAG_SPI_EVENT_BYTE_TX_V = 128;

	public const byte TAG_SPI_EVENT_BYTE_RX_V = 129;

	public const byte TAG_SPI_EVENT_STATUS_ERROR_V = 130;

	public const byte TAG_USART_EVENT_BYTE_TX_V = 128;

	public const byte TAG_USART_EVENT_BYTE_RX_V = 129;

	public const byte TAG_USART_EVENT_STATUS_ERROR_V = 130;

	public const byte TAG_USART_EVENT_BREAK_TX_V = 131;

	public const byte TAG_LIN_EVENT_BYTE_RX_V = 128;

	public const byte TAG_LIN_EVENT_BYTE_TX_V = 129;

	public const byte TAG_LIN_EVENT_STATUS_ERR_V = 130;

	public const byte TAG_LIN_EVENT_BREAK_RX_V = 131;

	public const byte TAG_LIN_EVENT_BREAK_TX_V = 132;

	public const byte TAG_LIN_EVENT_AUTO_BAUD_V = 133;

	public const byte TAG_LIN_EVENT_CHECKSUM_ERR_V = 134;

	public const byte TAG_LIN_EVENT_ID_PARITY_ERR_V = 135;

	public const byte TAG_LIN_EVENT_SLAVE_PROFILE_ID_DATA_V = 136;

	public static bool m_ready_to_notify = false;

	public static bool m_OK_to_send_data = true;

	internal static double m_EVENT_TIME_ROLLOVER = 0.0;

	internal static double m_RUNNING_TIME = 0.0;

	private static double m_EVENT_TIME_CONSTANT = 0.000409;

	private static double m_EVENT_TIME_ROLLOVER_CONSTANT = 26.8;

	private static bool m_grab_next_time_marker = false;

	private static bool m_special_status_requested = false;

	private static volatile bool m_we_are_in_read_loop = false;

	private static Thread m_read_thread;

	internal static Mutex m_cbuf2_data_array_mutex = new Mutex(initiallyOwned: false);

	internal static Mutex m_usb_packet_mutex = new Mutex(initiallyOwned: false);

	public static EventWaitHandle m_packet_is_ready = null;

	public static AutoResetEvent m_packet_is_copied = null;

	internal static byte[] m_raw_cbuf2_data_array = new byte[20480];

	internal static byte[] m_cbuf2_data_array = new byte[20480];

	internal static volatile uint m_cbuf2_data_array_index;

	private static volatile uint m_required_byte_count;

	private static volatile uint m_raw_cbuf2_data_array_index;

	private static volatile uint m_cbuf2_packet_byte_count;

	private static volatile bool m_process_data;

	public static volatile bool m_read_thread_is_processing_a_USB_packet;

	internal static uint m_cb2_array_tag_index;

	private static bool m_we_need_next_packet_to_continue;

	private static bool m_user_has_created_synchro_objects = false;

	public static event DataNotifier DataAvailable;

	public static event USBNotifier USBDataAvailable;

	public static void Initialize_Read_Objects() {
		m_we_are_in_read_loop = false;
		Array.Clear(m_raw_cbuf2_data_array, 0, m_raw_cbuf2_data_array.Length);
		Array.Clear(m_cbuf2_data_array, 0, m_cbuf2_data_array.Length);
		m_cbuf2_data_array_index = 0u;
		m_raw_cbuf2_data_array_index = 0u;
		m_cb2_array_tag_index = 0u;
		m_cbuf2_packet_byte_count = 0u;
		m_required_byte_count = 0u;
		m_process_data = true;
		m_we_need_next_packet_to_continue = false;
	}

	public static void Dispose_Of_Read_Objects() {
		m_cbuf2_data_array_mutex.Close();
		m_usb_packet_mutex.Close();
		m_packet_is_ready.Close();
	}

	public static bool Read_Thread_Is_Active() {
		return m_we_are_in_read_loop;
	}

	public static void Kill_Read_Thread() {
		m_we_are_in_read_loop = false;
		if (Utilities.g_comm_mode == Utilities.COMM_MODE.MTOUCH2) {
			mTouch2.Send_MT2_RD_STATUS_Command();
		} else {
			USBWrite.Send_Status_Request();
		}
	}

	public static void DLL_Should_Process_Data() {
		m_process_data = true;
	}

	public static void DLL_Should_Not_Process_Data() {
		m_process_data = false;
	}

	public static bool Kick_Off_Read_Thread() {
		bool result = true;
		if (!m_we_are_in_read_loop) {
			m_read_thread = new Thread(Read_USB_Thread);
			m_read_thread.IsBackground = true;
			m_read_thread.Start();
		} else {
			result = false;
		}

		return result;
	}

	public static void Read_USB_Thread() {
		bool flag = true;
		int pNumberOfBytesRead = 0;
		m_we_are_in_read_loop = true;
		while (m_we_are_in_read_loop) {
			m_read_thread_is_processing_a_USB_packet = false;
			m_usb_packet_mutex.WaitOne();
			Array.Clear(Utilities.m_flags.read_buffer, 0, Utilities.m_flags.read_buffer.Length);
			flag = Utilities.ReadFile(Utilities.m_flags.HID_read_handle, Utilities.m_flags.read_buffer, Utilities.m_flags.irbl, ref pNumberOfBytesRead, 0);
			if (Utilities.m_flags.g_need_to_copy_bl_data) {
				for (int i = 0; i < Utilities.m_flags.read_buffer.Length; i++) {
					Utilities.m_flags.bl_buffer[i] = Utilities.m_flags.read_buffer[i];
				}

				Utilities.m_flags.g_bl_data_arrived_event.Set();
				Utilities.m_flags.g_need_to_copy_bl_data = false;
			}

			m_read_thread_is_processing_a_USB_packet = true;
			m_usb_packet_mutex.ReleaseMutex();
			if (pNumberOfBytesRead != Utilities.m_flags.irbl || !flag || !m_we_are_in_read_loop) {
				break;
			}

			if (m_user_has_created_synchro_objects) {
				m_packet_is_ready.Set();
				m_packet_is_copied.WaitOne();
			}

			if (USBRead.USBDataAvailable != null && m_OK_to_send_data) {
				USBRead.USBDataAvailable();
				m_packet_is_copied.WaitOne();
			}

			if (m_process_data) {
				m_usb_packet_mutex.WaitOne();
				process_this_packet(ref Utilities.m_flags.read_buffer);
				m_usb_packet_mutex.ReleaseMutex();
			}
		}
	}

	public static void Create_Single_Sync_object() {
		m_packet_is_copied = new AutoResetEvent(initialState: false);
	}

	public static ushort Get_USB_IRBL() {
		return Utilities.m_flags.irbl;
	}

	public static void Create_Synchronization_Object(ref string p_ready_to_copy) {
		string text = DateTime.Now.ToLongTimeString();
		string text2 = "PacketReady" + text;
		m_packet_is_ready = new EventWaitHandle(initialState: false, EventResetMode.AutoReset, text2);
		m_packet_is_copied = new AutoResetEvent(initialState: false);
		p_ready_to_copy = text2;
		m_user_has_created_synchro_objects = true;
	}

	public static void Get_USB_Data_Packet(ref byte[] p_data) {
		if (Utilities.m_flags.read_buffer[64] == 0) {
			Utilities.m_flags.read_buffer[64] = 0;
		}

		m_usb_packet_mutex.WaitOne();
		for (int i = 0; i < Utilities.m_flags.read_buffer.Length; i++) {
			p_data[i] = Utilities.m_flags.read_buffer[i];
		}

		m_usb_packet_mutex.ReleaseMutex();
		m_packet_is_copied.Set();
	}

	private static bool process_this_packet(ref byte[] p_packet) {
		bool flag = false;
		m_usb_packet_mutex.WaitOne();
		byte b = 1;
		bool flag2 = false;
		_ = p_packet[1];
		while ((uint)b < 64u) {
			switch (p_packet[b]) {
				case 128:
				flag2 = true;
				break;
				case 129:
				_ = p_packet[b];
				process_this_group(ref p_packet, b);
				b += 3;
				break;
				case 130:
				_ = p_packet[b];
				process_this_group(ref p_packet, b);
				b += 25;
				break;
				case 131:
				_ = p_packet[b];
				process_this_group(ref p_packet, b);
				b += 21;
				break;
				case 132:
				_ = p_packet[b];
				process_this_group(ref p_packet, b);
				b += 7;
				break;
				case 133:
				case 134:
				case 135:
				_ = p_packet[b];
				process_this_group(ref p_packet, b);
				b += (byte)(2 + p_packet[b + 1]);
				break;
				case 136:
				_ = p_packet[b];
				process_this_group(ref p_packet, b);
				b += 2;
				break;
				default:
				flag = true;
				break;
			}

			if (flag2) {
				break;
			}

			if (flag || (uint)b > 66u) {
				flag = true;
				break;
			}
		}

		m_usb_packet_mutex.ReleaseMutex();
		return flag;
	}

	private static bool process_this_group(ref byte[] p_data, int p_index) {
		bool result = true;
		int num = 0;
		switch (p_data[p_index]) {
			case 128:
			case 133:
			case 135:
			case 136:
			if (p_data[p_index + 1] != 0) {
				m_special_status_requested = true;
			}

			break;
			case 134:
			process_cbuf2_data(ref p_data, p_index);
			break;
			case 129:
			Utilities.m_flags.g_status_packet_mutex.WaitOne();
			Constants.STATUS_PACKET_DATA[3] = p_data[p_index + 1];
			Constants.STATUS_PACKET_DATA[4] = p_data[p_index + 2];
			Utilities.m_flags.g_status_packet_mutex.ReleaseMutex();
			break;
			case 130:
			Utilities.m_flags.g_status_packet_mutex.WaitOne();
			for (num = 1; num <= 24; num++) {
				Constants.STATUS_PACKET_DATA[6 + num] = p_data[p_index + num];
			}

			Utilities.m_flags.g_status_packet_mutex.ReleaseMutex();
			break;
			case 131:
			Utilities.m_flags.g_status_packet_mutex.WaitOne();
			for (num = 1; num <= 20; num++) {
				Constants.STATUS_PACKET_DATA[31 + num] = p_data[p_index + num];
			}

			Utilities.m_flags.g_status_packet_mutex.ReleaseMutex();
			Utilities.Set_Comm_Mode(p_data[38], p_data[23]);
			break;
			case 132:
			Utilities.m_flags.g_status_packet_mutex.WaitOne();
			for (num = 1; num <= 6; num++) {
				Constants.STATUS_PACKET_DATA[52 + num] = p_data[p_index + num];
			}

			Utilities.m_flags.g_status_packet_mutex.ReleaseMutex();
			Utilities.m_flags.g_status_packet_data_update_event.Set();
			if (m_special_status_requested) {
				m_special_status_requested = false;
				Utilities.m_flags.g_special_status_request_event.Set();
			}

			break;
			default:
			result = false;
			break;
		}

		return result;
	}

	private static void process_cbuf2_data(ref byte[] p_data, int p_index) {
		m_cbuf2_packet_byte_count = p_data[p_index + 1];
		if (m_cbuf2_packet_byte_count == 0) {
			return;
		}

		if (!m_we_need_next_packet_to_continue) {
			Clear_Raw_Data_Array();
			if (Utilities.g_comm_mode == Utilities.COMM_MODE.LIN) {
				Clear_Data_Array(0u);
			}
		}

		for (int i = p_index + 2; i < p_index + 2 + m_cbuf2_packet_byte_count; i++) {
			if (m_raw_cbuf2_data_array_index >= m_raw_cbuf2_data_array.Length) {
				break;
			}

			m_raw_cbuf2_data_array[m_raw_cbuf2_data_array_index++] = p_data[i];
		}

		process_m_cbuf2_array_data();
	}

	private static bool process_m_cbuf2_array_data() {
		bool result = false;
		bool p_error = false;
		m_we_need_next_packet_to_continue = false;
		while (m_cb2_array_tag_index < m_raw_cbuf2_data_array_index && !p_error && !m_we_need_next_packet_to_continue) {
			if ((m_raw_cbuf2_data_array[m_cb2_array_tag_index] & 0x10) == 16) {
				process_common_data(ref p_error);
				continue;
			}

			if ((m_raw_cbuf2_data_array[m_cb2_array_tag_index] & 0x80) != 128 && Utilities.g_comm_mode != Utilities.COMM_MODE.MTOUCH2) {
				p_error = true;
				continue;
			}

			switch (Utilities.g_comm_mode) {
				case Utilities.COMM_MODE.I2C_M:
				process_i2c_data(ref p_error);
				break;
				case Utilities.COMM_MODE.SPI_M:
				case Utilities.COMM_MODE.SPI_S:
				case Utilities.COMM_MODE.UWIRE:
				process_spi_data(ref p_error);
				break;
				case Utilities.COMM_MODE.USART_A:
				case Utilities.COMM_MODE.USART_SM:
				case Utilities.COMM_MODE.USART_SS:
				process_usart_data(ref p_error);
				break;
				case Utilities.COMM_MODE.LIN:
				process_lin_data(ref p_error);
				break;
				case Utilities.COMM_MODE.I2C_S:
				process_i2c_slave_data(ref p_error);
				break;
				case Utilities.COMM_MODE.MTOUCH2:
				process_mtouch2_data(ref p_error);
				break;
				default:
				p_error = true;
				break;
			}
		}

		if (USBRead.DataAvailable != null && m_ready_to_notify && m_OK_to_send_data) {
			USBRead.DataAvailable();
			m_ready_to_notify = false;
		}

		return result;
	}

	private static void process_mtouch2_data(ref bool p_error) {
		switch (m_raw_cbuf2_data_array[m_cb2_array_tag_index]) {
			case 65:
			if (m_raw_cbuf2_data_array_index - m_cb2_array_tag_index >= 16) {
				mTouch2.m_sensor_status_mutex.WaitOne();
				mTouch2.m_data_status.comm_fw_ver = (ushort)(m_raw_cbuf2_data_array[m_raw_cbuf2_data_array_index + 2] + (m_raw_cbuf2_data_array[m_raw_cbuf2_data_array_index + 3] << 8));
				mTouch2.m_data_status.touch_fw_ver = (ushort)(m_raw_cbuf2_data_array[m_raw_cbuf2_data_array_index + 4] + (m_raw_cbuf2_data_array[m_raw_cbuf2_data_array_index + 5] << 8));
				mTouch2.m_data_status.hardware_id = m_raw_cbuf2_data_array[m_raw_cbuf2_data_array_index + 6];
				mTouch2.m_data_status.max_num_sensors = m_raw_cbuf2_data_array[m_raw_cbuf2_data_array_index + 7];
				mTouch2.m_data_status.broadcast_group_id = m_raw_cbuf2_data_array[m_raw_cbuf2_data_array_index + 8];
				mTouch2.m_data_status.broadcast_enable_flags.trip = (m_raw_cbuf2_data_array[m_raw_cbuf2_data_array_index + 9] & 1) > 0;
				mTouch2.m_data_status.broadcast_enable_flags.guardband = (m_raw_cbuf2_data_array[m_raw_cbuf2_data_array_index + 9] & 2) > 0;
				mTouch2.m_data_status.broadcast_enable_flags.raw = (m_raw_cbuf2_data_array[m_raw_cbuf2_data_array_index + 9] & 4) > 0;
				mTouch2.m_data_status.broadcast_enable_flags.avg = (m_raw_cbuf2_data_array[m_raw_cbuf2_data_array_index + 9] & 8) > 0;
				mTouch2.m_data_status.broadcast_enable_flags.detect_flags = (m_raw_cbuf2_data_array[m_raw_cbuf2_data_array_index + 9] & 0x10) > 0;
				mTouch2.m_data_status.broadcast_enable_flags.status = (m_raw_cbuf2_data_array[m_raw_cbuf2_data_array_index + 9] & 0x80) > 0;
				mTouch2.m_data_status.time_interval = (ushort)(m_raw_cbuf2_data_array[m_raw_cbuf2_data_array_index + 10] + (m_raw_cbuf2_data_array[m_raw_cbuf2_data_array_index + 11] << 8));
				mTouch2.m_sensor_status_mutex.ReleaseMutex();
				mTouch2.m_status_data_is_ready.Set();
				m_cb2_array_tag_index += 16u;
			} else {
				m_we_need_next_packet_to_continue = true;
			}

			break;
			case 66:
			if (m_raw_cbuf2_data_array_index - m_cb2_array_tag_index >= 7) {
				int num9 = 0;
				mTouch2.m_sensor_data_mutex.WaitOne();
				for (uint num10 = m_cb2_array_tag_index + 2; num10 < m_cb2_array_tag_index + 2 + 5; num10++) {
					mTouch2.m_detect_values[num9++] = m_raw_cbuf2_data_array[num10];
				}

				mTouch2.m_sensor_data_mutex.ReleaseMutex();
				mTouch2.m_detect_data_is_ready.Set();
				m_cb2_array_tag_index += 7u;
			} else {
				m_we_need_next_packet_to_continue = true;
			}

			break;
			case 67:
			if (m_raw_cbuf2_data_array_index - m_cb2_array_tag_index >= m_raw_cbuf2_data_array[m_cb2_array_tag_index + 2] + 2) {
				for (int i = 0; i < m_raw_cbuf2_data_array[m_cb2_array_tag_index + 2] + 1; i++) {
					mTouch2.m_user_sensor_values[i] = m_raw_cbuf2_data_array[m_cb2_array_tag_index + 2 + i];
				}

				mTouch2.m_user_sensor_values_are_ready.Set();
				m_cb2_array_tag_index += (uint)(m_raw_cbuf2_data_array[m_cb2_array_tag_index + 2] + 3);
			} else {
				m_we_need_next_packet_to_continue = true;
			}

			break;
			case 68:
			case 69:
			case 70:
			case 71:
			case 72:
			case 73:
			if (m_raw_cbuf2_data_array_index - m_cb2_array_tag_index >= m_raw_cbuf2_data_array[m_cb2_array_tag_index + 2] + 3) {
				int num = 0;
				int num2 = m_raw_cbuf2_data_array[m_cb2_array_tag_index + 2] / 2;
				mTouch2.m_current_sensor_id = m_raw_cbuf2_data_array[m_cb2_array_tag_index + 1];
				mTouch2.m_num_current_sensors = (byte)num2;
				switch (m_raw_cbuf2_data_array[m_cb2_array_tag_index]) {
					case 68: {
						mTouch2.m_sensor_data_mutex.WaitOne();
						for (uint num7 = m_cb2_array_tag_index + 3; num7 < m_cb2_array_tag_index + 3 + num2 * 2; num7 += 2) {
							mTouch2.m_trp_values[num++] = (ushort)(m_raw_cbuf2_data_array[num7] + (m_raw_cbuf2_data_array[num7 + 1] << 8));
						}

						mTouch2.m_sensor_data_mutex.ReleaseMutex();
						mTouch2.m_trip_data_is_ready.Set();
						break;
					}
					case 69: {
						mTouch2.m_sensor_data_mutex.WaitOne();
						for (uint num5 = m_cb2_array_tag_index + 3; num5 < m_cb2_array_tag_index + 3 + num2 * 2; num5 += 2) {
							mTouch2.m_gdb_values[num++] = (ushort)(m_raw_cbuf2_data_array[num5] + (m_raw_cbuf2_data_array[num5 + 1] << 8));
						}

						mTouch2.m_sensor_data_mutex.ReleaseMutex();
						mTouch2.m_gdb_data_is_ready.Set();
						break;
					}
					case 70: {
						mTouch2.m_sensor_data_mutex.WaitOne();
						for (uint num8 = m_cb2_array_tag_index + 3; num8 < m_cb2_array_tag_index + 3 + num2 * 2; num8 += 2) {
							mTouch2.m_raw_values[num++] = (ushort)(m_raw_cbuf2_data_array[num8] + (m_raw_cbuf2_data_array[num8 + 1] << 8));
						}

						mTouch2.m_sensor_data_mutex.ReleaseMutex();
						break;
					}
					case 71: {
						mTouch2.m_sensor_data_mutex.WaitOne();
						for (uint num4 = m_cb2_array_tag_index + 3; num4 < m_cb2_array_tag_index + 3 + num2 * 2; num4 += 2) {
							mTouch2.m_avg_values[num++] = (ushort)(m_raw_cbuf2_data_array[num4] + (m_raw_cbuf2_data_array[num4 + 1] << 8));
						}

						mTouch2.m_sensor_data_mutex.ReleaseMutex();
						break;
					}
					case 72: {
						mTouch2.m_sensor_data_mutex.WaitOne();
						for (uint num6 = m_cb2_array_tag_index + 3; num6 < m_cb2_array_tag_index + 3 + num2 * 2; num6 += 2) {
							mTouch2.m_au1_values[num++] = (ushort)(m_raw_cbuf2_data_array[num6] + (m_raw_cbuf2_data_array[num6 + 1] << 8));
						}

						mTouch2.m_sensor_data_mutex.ReleaseMutex();
						break;
					}
					case 73: {
						mTouch2.m_sensor_data_mutex.WaitOne();
						for (uint num3 = m_cb2_array_tag_index + 3; num3 < m_cb2_array_tag_index + 3 + num2 * 2; num3 += 2) {
							mTouch2.m_au2_values[num++] = (ushort)(m_raw_cbuf2_data_array[num3] + (m_raw_cbuf2_data_array[num3 + 1] << 8));
						}

						mTouch2.m_sensor_data_mutex.ReleaseMutex();
						break;
					}
				}

				m_cb2_array_tag_index += (uint)(m_raw_cbuf2_data_array[m_cb2_array_tag_index + 2] + 3);
			} else {
				m_we_need_next_packet_to_continue = true;
			}

			break;
		}
	}

	private static void process_lin_data(ref bool p_error) {
		switch (m_raw_cbuf2_data_array[m_cb2_array_tag_index]) {
			case 131:
			if (!LIN.m_working_frame.BuildState.we_timed_out) {
				if (LIN.m_working_frame.BuildState.we_are_building_a_frame) {
					LIN.m_working_frame.BuildState.next_frame_header_received = true;
					LIN.finalize_working_frame();
				}

				LIN.m_working_frame.BuildState.we_are_building_a_frame = true;
				m_grab_next_time_marker = true;
				LIN.reset_LIN_timeout();
			}

			m_cb2_array_tag_index++;
			break;
			case 132:
			m_cb2_array_tag_index++;
			break;
			case 134:
			if (m_raw_cbuf2_data_array_index - m_cb2_array_tag_index >= 2) {
				m_cb2_array_tag_index += 2u;
			} else {
				m_we_need_next_packet_to_continue = true;
			}

			break;
			case 135:
			if (m_raw_cbuf2_data_array_index - m_cb2_array_tag_index >= 2) {
				m_cb2_array_tag_index += 2u;
			} else {
				m_we_need_next_packet_to_continue = true;
			}

			break;
			case 136: {
				LIN.m_slave_profile_id.FrameID = m_raw_cbuf2_data_array[m_cb2_array_tag_index + 1];
				LIN.m_slave_profile_id.ByteCount = m_raw_cbuf2_data_array[m_cb2_array_tag_index + 2];
				for (int i = 0; i < LIN.m_slave_profile_id.ByteCount; i++) {
					LIN.m_slave_profile_id.Data[i] = m_raw_cbuf2_data_array[m_cb2_array_tag_index + 3 + i];
				}

				m_cb2_array_tag_index += (uint)(m_raw_cbuf2_data_array[m_cb2_array_tag_index + 2] + 3);
				LIN.m_slave_profile_id_read.Set();
				break;
			}
			case 128:
			if (m_raw_cbuf2_data_array_index - m_cb2_array_tag_index >= 2) {
				m_cb2_array_tag_index += 2u;
			} else {
				m_we_need_next_packet_to_continue = true;
			}

			break;
			case 129:
			if (m_raw_cbuf2_data_array_index - m_cb2_array_tag_index >= 2) {
				m_cb2_array_tag_index += 2u;
			} else {
				m_we_need_next_packet_to_continue = true;
			}

			break;
			case 130:
			LIN.m_working_frame.BuildState.we_had_a_status_error = true;
			if (!LIN.m_working_frame.BuildState.we_timed_out) {
				LIN.finalize_working_frame();
				Device.Clear_Status_Errors();
			}

			if (m_raw_cbuf2_data_array_index - m_cb2_array_tag_index >= 2) {
				m_cb2_array_tag_index += 2u;
			} else {
				m_we_need_next_packet_to_continue = true;
			}

			break;
			case 133:
			if (LIN.m_working_frame.BuildState.we_are_building_a_frame) {
				LIN.m_working_frame.FrameInfo.baud = (ushort)(m_raw_cbuf2_data_array[m_cb2_array_tag_index + 1] + (m_raw_cbuf2_data_array[m_cb2_array_tag_index + 2] << 8));
				if (LIN.m_use_baud_rate_timeout) {
					LIN.calculate_new_baud_dependent_onreceive_timeout(LIN.m_working_frame.FrameInfo.baud);
				}
			}

			if (m_raw_cbuf2_data_array_index - m_cb2_array_tag_index >= 3) {
				m_cb2_array_tag_index += 3u;
			} else {
				m_we_need_next_packet_to_continue = true;
			}

			break;
			default:
			p_error = true;
			break;
		}
	}

	private static void process_i2c_data(ref bool p_error) {
		switch (m_raw_cbuf2_data_array[m_cb2_array_tag_index]) {
			case 128:
			m_cb2_array_tag_index++;
			break;
			case 129:
			m_cb2_array_tag_index++;
			break;
			case 130:
			m_cb2_array_tag_index++;
			break;
			case 131:
			m_cb2_array_tag_index++;
			break;
			case 132:
			m_cb2_array_tag_index++;
			break;
			case 133:
			m_cb2_array_tag_index++;
			break;
			case 134:
			m_cb2_array_tag_index++;
			break;
			case 135:
			case 136:
			case 137:
			case 138:
			if (m_raw_cbuf2_data_array_index - m_cb2_array_tag_index >= 2) {
				m_cb2_array_tag_index += 2u;
			} else {
				m_we_need_next_packet_to_continue = true;
			}

			break;
			default:
			p_error = true;
			break;
		}
	}

	private static void process_i2c_slave_data(ref bool p_error) {
		I2CS.m_slave_address_was_just_set = false;
		I2CS.m_master_is_waiting_for_data = false;
		I2CS.m_stop_command_issued = false;
		switch (m_raw_cbuf2_data_array[m_cb2_array_tag_index]) {
			case 195:
			m_cb2_array_tag_index++;
			break;
			case 196:
			m_cb2_array_tag_index++;
			break;
			case 200:
			m_cb2_array_tag_index++;
			I2CS.m_master_is_waiting_for_data = true;
			I2CS.issue_event();
			break;
			case 198:
			m_cb2_array_tag_index++;
			I2CS.m_stop_command_issued = true;
			I2CS.issue_event();
			I2CS.reset_buffers();
			break;
			case 192:
			case 193:
			case 194:
			case 199:
			if (m_raw_cbuf2_data_array_index - m_cb2_array_tag_index >= 2) {
				switch (m_raw_cbuf2_data_array[m_cb2_array_tag_index]) {
					case 192:
					I2CS.m_previous_slave_addr_received = I2CS.m_last_slave_addr_received;
					I2CS.m_last_slave_addr_received = m_raw_cbuf2_data_array[m_cb2_array_tag_index + 1];
					I2CS.m_slave_address_was_just_set = true;
					I2CS.issue_event();
					break;
					case 193:
					add_data_to_cbuf2_data_array(ref m_raw_cbuf2_data_array, m_cb2_array_tag_index + 1, 1);
					break;
					case 199:
					I2CS.issue_error();
					break;
				}

				m_cb2_array_tag_index += 2u;
			} else {
				m_we_need_next_packet_to_continue = true;
			}

			break;
			case 201:
			case 202:
			if (m_raw_cbuf2_data_array_index - m_cb2_array_tag_index >= 3) {
				switch (m_raw_cbuf2_data_array[m_cb2_array_tag_index]) {
					default:
					m_cb2_array_tag_index += 3u;
					break;
				}
			} else {
				m_we_need_next_packet_to_continue = true;
			}

			break;
			case 203:
			m_cb2_array_tag_index += (uint)(m_raw_cbuf2_data_array[m_cb2_array_tag_index + 3] + 4);
			break;
			default:
			p_error = true;
			break;
		}
	}

	private static void process_usart_data(ref bool p_error) {
		switch (m_raw_cbuf2_data_array[m_cb2_array_tag_index]) {
			case 128:
			case 129:
			case 130:
			if (m_raw_cbuf2_data_array_index - m_cb2_array_tag_index >= 2) {
				m_cb2_array_tag_index += 2u;
			} else {
				m_we_need_next_packet_to_continue = true;
			}

			break;
			case 131:
			m_cb2_array_tag_index++;
			break;
			default:
			p_error = true;
			break;
		}
	}

	private static void process_spi_data(ref bool p_error) {
		switch (m_raw_cbuf2_data_array[m_cb2_array_tag_index]) {
			case 128:
			case 129:
			case 130:
			if (m_raw_cbuf2_data_array_index - m_cb2_array_tag_index >= 2) {
				m_cb2_array_tag_index += 2u;
			} else {
				m_we_need_next_packet_to_continue = true;
			}

			break;
			default:
			p_error = true;
			break;
		}
	}

	private static void process_common_data(ref bool p_error) {
		switch (m_raw_cbuf2_data_array[m_cb2_array_tag_index]) {
			case 16:
			case 18:
			case 26:
			case 28:
			if (m_raw_cbuf2_data_array_index - m_cb2_array_tag_index >= 2) {
				switch (m_raw_cbuf2_data_array[m_cb2_array_tag_index]) {
					case 16:
					add_data_to_cbuf2_data_array(ref m_raw_cbuf2_data_array, m_cb2_array_tag_index + 1, 1);
					if (Utilities.g_comm_mode != Utilities.COMM_MODE.LIN || LIN.m_working_frame.BuildState.we_timed_out || !LIN.m_working_frame.BuildState.we_are_building_a_frame) {
						break;
					}

					if (!LIN.m_working_frame.BuildState.we_have_an_id) {
						LIN.m_working_frame.FrameInfo.FrameID = m_raw_cbuf2_data_array[m_cb2_array_tag_index + 1];
						LIN.m_working_frame.BuildState.we_have_an_id = true;
						break;
					}

					if (LIN.m_working_frame.FrameInfo.bytecount < 9) {
						LIN.m_working_frame.FrameInfo.FrameData[LIN.m_working_frame.FrameInfo.bytecount++] = m_raw_cbuf2_data_array[m_cb2_array_tag_index + 1];
					}

					if (LIN.m_working_frame.FrameInfo.bytecount == 9) {
						LIN.finalize_working_frame();
					}

					break;
					case 28:
					if (m_raw_cbuf2_data_array[m_cb2_array_tag_index + 1] == 119) {
						Utilities.m_flags.g_PKSA_has_completed_script.Set();
					}

					break;
				}

				m_cb2_array_tag_index += 2u;
			} else {
				m_we_need_next_packet_to_continue = true;
			}

			break;
			case 17:
			if (m_raw_cbuf2_data_array_index - m_cb2_array_tag_index - 2 >= m_raw_cbuf2_data_array[m_cb2_array_tag_index + 1]) {
				add_data_to_cbuf2_data_array(ref m_raw_cbuf2_data_array, m_cb2_array_tag_index + 2, m_raw_cbuf2_data_array[m_cb2_array_tag_index + 1]);
				m_cb2_array_tag_index += (uint)(2 + m_raw_cbuf2_data_array[m_cb2_array_tag_index + 1]);
			} else {
				m_we_need_next_packet_to_continue = true;
			}

			break;
			case 19:
			if (m_raw_cbuf2_data_array_index - m_cb2_array_tag_index >= 3) {
				if (Utilities.g_comm_mode == Utilities.COMM_MODE.LIN && LIN.m_working_frame.BuildState.we_are_building_a_frame && m_grab_next_time_marker) {
					double num = (double)((m_raw_cbuf2_data_array[m_cb2_array_tag_index + 2] << 8) + m_raw_cbuf2_data_array[m_cb2_array_tag_index + 1]) * m_EVENT_TIME_CONSTANT;
					m_RUNNING_TIME = m_EVENT_TIME_ROLLOVER + num;
					LIN.m_working_frame.FrameInfo.time = m_RUNNING_TIME;
					m_grab_next_time_marker = false;
				}

				m_cb2_array_tag_index += 3u;
			} else {
				m_we_need_next_packet_to_continue = true;
			}

			break;
			case 21:
			if (m_raw_cbuf2_data_array_index - m_cb2_array_tag_index >= 3) {
				m_cb2_array_tag_index += 3u;
			} else {
				m_we_need_next_packet_to_continue = true;
			}

			break;
			case 20:
			m_EVENT_TIME_ROLLOVER += m_EVENT_TIME_ROLLOVER_CONSTANT;
			m_cb2_array_tag_index++;
			break;
			case 22:
			m_cb2_array_tag_index++;
			break;
			case 24:
			m_cb2_array_tag_index++;
			break;
			case 23:
			m_cb2_array_tag_index++;
			break;
			case 25:
			m_cb2_array_tag_index++;
			break;
			case 27:
			m_cb2_array_tag_index++;
			break;
			default:
			p_error = true;
			break;
		}
	}

	private static void add_data_to_cbuf2_data_array(ref byte[] p_source, uint p_index, int p_num_bytes) {
		int num = 0;
		m_cbuf2_data_array_mutex.WaitOne();
		while (m_cbuf2_data_array_index < m_cbuf2_data_array.Length && num < p_num_bytes) {
			m_cbuf2_data_array[m_cbuf2_data_array_index] = p_source[p_index + num];
			num++;
			m_cbuf2_data_array_index++;
		}

		m_cbuf2_data_array_mutex.ReleaseMutex();
		if (m_cbuf2_data_array_index >= m_required_byte_count && m_required_byte_count != 0) {
			Utilities.m_flags.g_data_arrived_event.Set();
		}

		m_ready_to_notify = true;
	}

	public static uint Retrieve_Data_Byte_Count() {
		return m_cbuf2_data_array_index;
	}

	public static uint Retrieve_Raw_Data_Byte_Count() {
		return m_raw_cbuf2_data_array_index;
	}

	public static void DataAvailable_Should_Fire(bool p_value) {
		m_OK_to_send_data = p_value;
	}

	public static void Clear_Data_Array(uint p_required_byte_count) {
		m_cbuf2_data_array_mutex.WaitOne();
		Array.Clear(m_cbuf2_data_array, 0, m_cbuf2_data_array.Length);
		m_cbuf2_data_array_index = 0u;
		m_required_byte_count = p_required_byte_count;
		m_cbuf2_data_array_mutex.ReleaseMutex();
		Utilities.m_flags.g_data_arrived_event.Reset();
	}

	public static void Clear_Raw_Data_Array() {
		Array.Clear(m_raw_cbuf2_data_array, 0, m_raw_cbuf2_data_array.Length);
		m_raw_cbuf2_data_array_index = 0u;
		m_cb2_array_tag_index = 0u;
	}

	public static bool Retrieve_Data(ref byte[] p_data_array, uint p_num_bytes) {
		if (p_num_bytes > m_cbuf2_data_array_index) {
			return false;
		}

		m_cbuf2_data_array_mutex.WaitOne();
		for (int i = 0; i < p_num_bytes; i++) {
			p_data_array[i] = m_cbuf2_data_array[i];
		}

		for (int j = 0; j < m_cbuf2_data_array_index - p_num_bytes; j++) {
			m_cbuf2_data_array[j] = m_cbuf2_data_array[p_num_bytes + j];
		}

		m_cbuf2_data_array_index -= p_num_bytes;
		Array.Clear(m_cbuf2_data_array, (int)m_cbuf2_data_array_index, m_cbuf2_data_array.Length - (int)m_cbuf2_data_array_index);
		m_cbuf2_data_array_mutex.ReleaseMutex();
		m_required_byte_count = 0u;
		return true;
	}

	public static bool Retrieve_Raw_Data(ref byte[] p_data_array, uint p_num_bytes) {
		if (p_num_bytes > m_raw_cbuf2_data_array_index) {
			return false;
		}

		for (int i = 0; i < p_num_bytes; i++) {
			p_data_array[i] = m_raw_cbuf2_data_array[i];
		}

		for (int j = 0; j < m_raw_cbuf2_data_array_index - p_num_bytes; j++) {
			m_raw_cbuf2_data_array[j] = m_raw_cbuf2_data_array[p_num_bytes + j];
		}

		m_raw_cbuf2_data_array_index -= p_num_bytes;
		if (m_cb2_array_tag_index < p_num_bytes) {
			m_cb2_array_tag_index = 0u;
		} else {
			m_cb2_array_tag_index -= p_num_bytes;
		}

		Array.Clear(m_raw_cbuf2_data_array, (int)m_raw_cbuf2_data_array_index, m_raw_cbuf2_data_array.Length - (int)m_raw_cbuf2_data_array_index);
		return true;
	}

	public static bool reset_timer_params() {
		m_EVENT_TIME_ROLLOVER = 0.0;
		m_RUNNING_TIME = 0.0;
		if (USBWrite.Send_Event_Timer_Reset_Cmd()) {
			return true;
		}

		return false;
	}
}