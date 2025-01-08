using System.Threading;

namespace PICkitS;

public class mTouch2 {
	internal struct BROADCAST_ENABLE_FLAGS {
		internal bool trip;

		internal bool guardband;

		internal bool raw;

		internal bool avg;

		internal bool detect_flags;

		internal bool aux1;

		internal bool aux2;

		internal bool status;
	}

	internal struct DATA_STATUS {
		internal ushort comm_fw_ver;

		internal ushort touch_fw_ver;

		internal byte hardware_id;

		internal byte max_num_sensors;

		internal byte broadcast_group_id;

		internal BROADCAST_ENABLE_FLAGS broadcast_enable_flags;

		internal ushort time_interval;
	}

	public delegate void Broadcast_All_Data(byte sensor_id, byte num_sensors, ref ushort[] raw, ref ushort[] avg, ref ushort[] trip, ref ushort[] gdbnd, ref byte[] detect);

	public const byte MAX_NUM_SENSORS = 16;

	public const byte NUM_DETECT_BYTES = 5;

	public const ushort AMAD_USB_PRODUCTID = 80;

	internal const byte MT2_END_OF_DATA = 0;

	internal const byte MT2_RESET = 1;

	internal const byte MT2_ARCHIVE = 2;

	internal const byte MT2_RD_STATUS = 17;

	internal const byte MT2_RD_DETECT = 18;

	internal const byte MT2_RD_USERGROUP = 19;

	internal const byte MT2_RD = 20;

	internal const byte MT2_RD_AUTO = 21;

	internal const byte MT2_WR_USERGROUP = 33;

	internal const byte MT2_WR_TRIP = 34;

	internal const byte MT2_WR_GBAND = 35;

	internal const byte MT2_WR_AUX1 = 36;

	internal const byte MT2_WR_AUX2 = 37;

	internal const byte MT2_COMM_TAG_WR_USE_USB = 38;

	internal const byte MT2_DATA_STATUS = 65;

	internal const byte MT2_DATA_DETECT = 66;

	internal const byte MT2_DATA_USERGROUP = 67;

	internal const byte MT2_DATA_TRIP = 68;

	internal const byte MT2_DATA_GBAND = 69;

	internal const byte MT2_DATA_RAW = 70;

	internal const byte MT2_DATA_AVG = 71;

	internal const byte MT2_DATA_AUX1 = 72;

	internal const byte MT2_DATA_AUX2 = 73;

	internal static DATA_STATUS m_data_status = default(DATA_STATUS);

	internal static Mutex m_sensor_data_mutex = new Mutex(initiallyOwned: false);

	internal static Mutex m_sensor_status_mutex = new Mutex(initiallyOwned: false);

	internal static AutoResetEvent m_detect_data_is_ready = new AutoResetEvent(initialState: false);

	internal static AutoResetEvent m_trip_data_is_ready = new AutoResetEvent(initialState: false);

	internal static AutoResetEvent m_gdb_data_is_ready = new AutoResetEvent(initialState: false);

	internal static AutoResetEvent m_status_data_is_ready = new AutoResetEvent(initialState: false);

	internal static AutoResetEvent m_user_sensor_values_are_ready = new AutoResetEvent(initialState: false);

	private static volatile bool m_we_are_broadcasting = false;

	internal static volatile byte m_num_current_sensors = 0;

	internal static volatile byte m_current_sensor_id = 0;

	internal static ushort[] m_raw_values = new ushort[16];

	internal static ushort[] m_avg_values = new ushort[16];

	internal static ushort[] m_trp_values = new ushort[16];

	internal static ushort[] m_gdb_values = new ushort[16];

	internal static ushort[] m_au1_values = new ushort[16];

	internal static ushort[] m_au2_values = new ushort[16];

	internal static byte[] m_user_sensor_values = new byte[17];

	internal static byte[] m_detect_values = new byte[5];

	private static ushort[] m_local_raw_values = new ushort[16];

	private static ushort[] m_local_avg_values = new ushort[16];

	private static ushort[] m_local_trp_values = new ushort[16];

	private static ushort[] m_local_gdb_values = new ushort[16];

	private static byte[] m_local_detect_values = new byte[5];

	public static event Broadcast_All_Data broadcast_all_data;

	public static bool Configure_PICkitSerial_For_MTouch2() {
		return Basic.Configure_PICkitSerial(12, p_reset: true);
	}

	internal static void broadcast_latest_data() {
		if (m_we_are_broadcasting) {
			m_sensor_data_mutex.WaitOne();
			for (int i = 0; i < m_num_current_sensors; i++) {
				m_local_raw_values[i] = m_raw_values[i];
				m_local_avg_values[i] = m_avg_values[i];
				m_local_trp_values[i] = m_trp_values[i];
				m_local_gdb_values[i] = m_gdb_values[i];
			}

			for (int j = 0; j < 5; j++) {
				m_local_detect_values[j] = m_detect_values[j];
			}

			m_sensor_data_mutex.ReleaseMutex();
			mTouch2.broadcast_all_data(m_current_sensor_id, m_num_current_sensors, ref m_local_raw_values, ref m_local_avg_values, ref m_local_trp_values, ref m_local_gdb_values, ref m_local_detect_values);
		}
	}

	public static bool Get_Sensor_Data(byte p_sensor_id, byte p_num_sensors, ref ushort[] p_raw, ref ushort[] p_avg, ref ushort[] p_trip, ref ushort[] p_gdbnd, ref byte[] p_detect) {
		bool result = false;
		m_detect_data_is_ready.Reset();
		if (Send_MT2_RD_Command(p_sensor_id, p_trip: true, p_gdbnd: true, p_raw: true, p_avg: true, p_detect: true, p_aux1: false, p_aux2: false, p_status: false) && m_detect_data_is_ready.WaitOne(500, exitContext: false) && p_sensor_id == m_current_sensor_id && p_num_sensors == m_num_current_sensors) {
			m_sensor_data_mutex.WaitOne();
			for (int i = 0; i < m_num_current_sensors; i++) {
				p_raw[i] = m_raw_values[i];
				p_avg[i] = m_avg_values[i];
				p_trip[i] = m_trp_values[i];
				p_gdbnd[i] = m_gdb_values[i];
			}

			for (int j = 0; j < 5; j++) {
				p_detect[j] = m_detect_values[j];
			}

			m_sensor_data_mutex.ReleaseMutex();
			result = true;
		}

		return result;
	}

	private static bool Send_MT2_RD_Command(byte p_sensor_id, bool p_trip, bool p_gdbnd, bool p_raw, bool p_avg, bool p_detect, bool p_aux1, bool p_aux2, bool p_status) {
		byte[] p_data = new byte[65];
		byte b = 0;
		if (p_trip) {
			b = (byte)(b | 1u);
		}

		if (p_gdbnd) {
			b = (byte)(b | 2u);
		}

		if (p_raw) {
			b = (byte)(b | 4u);
		}

		if (p_avg) {
			b = (byte)(b | 8u);
		}

		if (p_detect) {
			b = (byte)(b | 0x10u);
		}

		if (p_aux1) {
			b = (byte)(b | 0x20u);
		}

		if (p_aux2) {
			b = (byte)(b | 0x40u);
		}

		if (p_status) {
			b = (byte)(b | 0x80u);
		}

		p_data[0] = 0;
		p_data[1] = 20;
		p_data[2] = p_sensor_id;
		p_data[3] = b;
		p_data[4] = 0;
		bool result = USBWrite.Send_Data_Packet_To_PICkitS(ref p_data);
		m_we_are_broadcasting = false;
		return result;
	}

	private static bool Send_MT2_RD_AUTO_Command(byte p_sensor_id, bool p_trip, bool p_gdbnd, bool p_raw, bool p_avg, bool p_detect, bool p_aux1, bool p_aux2, bool p_status, ushort p_interval) {
		byte[] p_data = new byte[65];
		byte b = 0;
		if (p_trip) {
			b = (byte)(b | 1u);
		}

		if (p_gdbnd) {
			b = (byte)(b | 2u);
		}

		if (p_raw) {
			b = (byte)(b | 4u);
		}

		if (p_avg) {
			b = (byte)(b | 8u);
		}

		if (p_detect) {
			b = (byte)(b | 0x10u);
		}

		if (p_aux1) {
			b = (byte)(b | 0x20u);
		}

		if (p_aux2) {
			b = (byte)(b | 0x40u);
		}

		if (p_status) {
			b = (byte)(b | 0x80u);
		}

		p_data[0] = 0;
		p_data[1] = 21;
		p_data[2] = p_sensor_id;
		p_data[3] = b;
		p_data[4] = (byte)p_interval;
		p_data[5] = (byte)(p_interval >> 8);
		p_data[6] = 0;
		bool result = USBWrite.Send_Data_Packet_To_PICkitS(ref p_data);
		if (p_interval == 0) {
			m_we_are_broadcasting = false;
		} else {
			m_we_are_broadcasting = true;
		}

		return result;
	}

	public static bool Send_MT2_RESET_Command() {
		byte[] p_data = new byte[65];
		p_data[0] = 0;
		p_data[1] = 1;
		p_data[2] = 0;
		bool result = USBWrite.Send_Data_Packet_To_PICkitS(ref p_data);
		m_we_are_broadcasting = false;
		return result;
	}

	public static bool Send_MT2_ARCHIVE_Command() {
		byte[] p_data = new byte[65];
		p_data[0] = 0;
		p_data[1] = 2;
		p_data[2] = 0;
		return USBWrite.Send_Data_Packet_To_PICkitS(ref p_data);
	}

	public static bool Send_MT2_COMM_TAG_WR_USE_USB_Command(bool p_enable) {
		byte[] p_data = new byte[65];
		byte b = 0;
		if (p_enable) {
			b = 1;
		}

		p_data[0] = 0;
		p_data[1] = 38;
		p_data[2] = b;
		p_data[3] = 0;
		return USBWrite.Send_Data_Packet_To_PICkitS(ref p_data);
	}

	public static bool Create_User_Defined_Sensor_Group(byte p_sensor_count, ref byte[] p_sensor_array) {
		byte[] p_data = new byte[65];
		p_data[0] = 0;
		p_data[1] = 33;
		p_data[2] = p_sensor_count;
		for (int i = 3; i < p_sensor_count + 3; i++) {
			p_data[i] = p_sensor_array[i - 3];
		}

		p_data[p_sensor_count + 3] = 0;
		bool flag = USBWrite.Send_Data_Packet_To_PICkitS(ref p_data);
		byte p_sensor_count2 = 0;
		byte[] p_sensor_array2 = new byte[17];
		flag = Read_User_Defined_Sensor_Group(ref p_sensor_count2, ref p_sensor_array2);
		if (flag && p_sensor_count2 == p_sensor_count) {
			for (int j = 0; j < p_sensor_count; j++) {
				if (p_sensor_array2[j] != p_sensor_array[j]) {
					flag = false;
					break;
				}
			}
		}

		return flag;
	}

	public static bool Read_User_Defined_Sensor_Group(ref byte p_sensor_count, ref byte[] p_sensor_array) {
		byte[] p_data = new byte[65];
		p_data[0] = 0;
		p_data[1] = 19;
		p_data[2] = 0;
		m_user_sensor_values_are_ready.Reset();
		bool flag = USBWrite.Send_Data_Packet_To_PICkitS(ref p_data);
		if (flag) {
			flag = m_user_sensor_values_are_ready.WaitOne(500, exitContext: false);
			if (flag) {
				p_sensor_count = m_user_sensor_values[0];
				for (int i = 0; i < p_sensor_count; i++) {
					p_sensor_array[i] = m_user_sensor_values[1 + i];
				}
			}
		}

		return flag;
	}

	internal static bool Send_MT2_RD_STATUS_Command() {
		byte[] p_data = new byte[65];
		bool result = false;
		p_data[0] = 0;
		p_data[1] = 17;
		p_data[2] = 0;
		m_status_data_is_ready.Reset();
		if (USBWrite.Send_Data_Packet_To_PICkitS(ref p_data)) {
			result = m_status_data_is_ready.WaitOne(500, exitContext: false);
		}

		return result;
	}

	public static bool Get_MT2_DATA_STATUS(ref ushort p_comm_fw_ver, ref ushort p_touch_fw_ver, ref byte p_hardware_id, ref byte p_max_num_sensors, ref byte p_broadcast_group_id, ref bool p_trip, ref bool p_gdbnd, ref bool p_raw, ref bool p_avg, ref bool p_detect, ref bool p_aux1, ref bool p_aux2, ref bool p_status, ref ushort p_interval) {
		bool result = false;
		if (Send_MT2_RD_STATUS_Command()) {
			m_sensor_status_mutex.WaitOne();
			p_comm_fw_ver = m_data_status.comm_fw_ver;
			p_touch_fw_ver = m_data_status.touch_fw_ver;
			p_hardware_id = m_data_status.hardware_id;
			p_max_num_sensors = m_data_status.max_num_sensors;
			p_broadcast_group_id = m_data_status.broadcast_group_id;
			p_trip = m_data_status.broadcast_enable_flags.trip;
			p_gdbnd = m_data_status.broadcast_enable_flags.guardband;
			p_raw = m_data_status.broadcast_enable_flags.raw;
			p_avg = m_data_status.broadcast_enable_flags.avg;
			p_detect = m_data_status.broadcast_enable_flags.detect_flags;
			p_aux1 = m_data_status.broadcast_enable_flags.aux1;
			p_aux2 = m_data_status.broadcast_enable_flags.aux2;
			p_status = m_data_status.broadcast_enable_flags.status;
			p_interval = m_data_status.time_interval;
			m_sensor_status_mutex.ReleaseMutex();
			result = true;
		}

		return result;
	}

	public static bool Write_Trip_Value(byte p_sensor_id, ushort p_trip) {
		Send_MT2_RESET_Command();
		byte[] p_data = new byte[65];
		ushort time_interval = 0;
		if (m_we_are_broadcasting && Send_MT2_RD_STATUS_Command()) {
			time_interval = m_data_status.time_interval;
		}

		p_data[0] = 0;
		p_data[1] = 34;
		p_data[2] = p_sensor_id;
		p_data[3] = 2;
		p_data[4] = (byte)p_trip;
		p_data[5] = (byte)(p_trip >> 8);
		p_data[6] = 0;
		bool flag = USBWrite.Send_Data_Packet_To_PICkitS(ref p_data);
		if (flag) {
			m_trip_data_is_ready.Reset();
			if (Send_MT2_RD_Command(p_sensor_id, p_trip: true, p_gdbnd: false, p_raw: false, p_avg: false, p_detect: false, p_aux1: false, p_aux2: false, p_status: false)) {
				flag = m_trip_data_is_ready.WaitOne(500, exitContext: false);
				if (flag) {
					m_sensor_data_mutex.WaitOne();
					if (p_trip != m_trp_values[0]) {
						flag = false;
					}

					m_sensor_data_mutex.ReleaseMutex();
				}
			}
		}

		if (m_we_are_broadcasting && Send_MT2_RD_STATUS_Command()) {
			m_data_status.time_interval = time_interval;
			Send_MT2_RD_AUTO_Command(m_data_status.broadcast_group_id, m_data_status.broadcast_enable_flags.trip, m_data_status.broadcast_enable_flags.guardband, m_data_status.broadcast_enable_flags.raw, m_data_status.broadcast_enable_flags.avg, m_data_status.broadcast_enable_flags.detect_flags, m_data_status.broadcast_enable_flags.aux1, m_data_status.broadcast_enable_flags.aux2, m_data_status.broadcast_enable_flags.status, m_data_status.time_interval);
		}

		return flag;
	}

	public static bool Write_Gdbnd_Value(byte p_sensor_id, ushort p_gdbnd) {
		Send_MT2_RESET_Command();
		byte[] p_data = new byte[65];
		ushort time_interval = 0;
		if (m_we_are_broadcasting && Send_MT2_RD_STATUS_Command()) {
			time_interval = m_data_status.time_interval;
		}

		p_data[0] = 0;
		p_data[1] = 35;
		p_data[2] = p_sensor_id;
		p_data[3] = 2;
		p_data[4] = (byte)p_gdbnd;
		p_data[5] = (byte)(p_gdbnd >> 8);
		p_data[6] = 0;
		bool flag = USBWrite.Send_Data_Packet_To_PICkitS(ref p_data);
		if (flag) {
			m_gdb_data_is_ready.Reset();
			if (Send_MT2_RD_Command(p_sensor_id, p_trip: false, p_gdbnd: true, p_raw: false, p_avg: false, p_detect: false, p_aux1: false, p_aux2: false, p_status: false)) {
				flag = m_gdb_data_is_ready.WaitOne(500, exitContext: false);
				if (flag) {
					m_sensor_data_mutex.WaitOne();
					if (p_gdbnd != m_gdb_values[0]) {
						flag = false;
					}

					m_sensor_data_mutex.ReleaseMutex();
				}
			}
		}

		if (m_we_are_broadcasting && Send_MT2_RD_STATUS_Command()) {
			m_data_status.time_interval = time_interval;
			Send_MT2_RD_AUTO_Command(m_data_status.broadcast_group_id, m_data_status.broadcast_enable_flags.trip, m_data_status.broadcast_enable_flags.guardband, m_data_status.broadcast_enable_flags.raw, m_data_status.broadcast_enable_flags.avg, m_data_status.broadcast_enable_flags.detect_flags, m_data_status.broadcast_enable_flags.aux1, m_data_status.broadcast_enable_flags.aux2, m_data_status.broadcast_enable_flags.status, m_data_status.time_interval);
		}

		return flag;
	}

	public static bool Get_Trip_and_Gdbnd_Data(byte p_sensor_id, int p_num_sensors, ref ushort[] p_trip, ref ushort[] p_gdbnd) {
		bool result = false;
		m_gdb_data_is_ready.Reset();
		if (Send_MT2_RD_Command(p_sensor_id, p_trip: true, p_gdbnd: true, p_raw: false, p_avg: false, p_detect: false, p_aux1: false, p_aux2: false, p_status: false) && m_gdb_data_is_ready.WaitOne(500, exitContext: false)) {
			m_sensor_data_mutex.WaitOne();
			for (int i = 0; i < p_num_sensors; i++) {
				p_trip[i] = m_trp_values[i];
				p_gdbnd[i] = m_gdb_values[i];
			}

			m_sensor_data_mutex.ReleaseMutex();
			result = true;
		}

		return result;
	}
}