using Microsoft.VisualBasic;
using System;
using System.Runtime.InteropServices;
using System.Threading;

namespace PICkitS;

public class Utilities {
	public struct FLAGS {
		public ushort orbl;

		public ushort irbl;

		public IntPtr HID_write_handle;

		public IntPtr HID_read_handle;

		public byte[] write_buffer;

		public byte[] read_buffer;

		public byte[] bl_buffer;

		public Mutex g_status_packet_mutex;

		public AutoResetEvent g_status_packet_data_update_event;

		public AutoResetEvent g_data_arrived_event;

		public AutoResetEvent g_bl_data_arrived_event;

		public AutoResetEvent g_special_status_request_event;

		internal AutoResetEvent g_PKSA_has_completed_script;

		public volatile bool g_need_to_copy_bl_data;
	}

	public enum COMM_MODE {
		IDLE,
		I2C_M,
		SPI_M,
		SPI_S,
		USART_A,
		USART_SM,
		USART_SS,
		I2C_S,
		I2C_BBM,
		I2C_SBBM,
		LIN,
		UWIRE,
		MTOUCH2,
		CM_ERROR
	}

	internal enum I2CS_MODE {
		DEFAULT,
		INTERACTIVE,
		AUTO
	}

	public struct OVERLAPPED {
		public int Internal;

		public int InternalHigh;

		public int Offset;

		public int OffsetHigh;

		public int hEvent;
	}

	public struct SECURITY_ATTRIBUTES {
		public int nLength;

		public int lpSecurityDescriptor;

		public int bInheritHandle;
	}

	public enum ThreadAccess {
		TERMINATE = 1,
		SUSPEND_RESUME = 2,
		GET_CONTEXT = 8,
		SET_CONTEXT = 0x10,
		SET_INFORMATION = 0x20,
		QUERY_INFORMATION = 0x40,
		SET_THREAD_TOKEN = 0x80,
		IMPERSONATE = 0x100,
		DIRECT_IMPERSONATION = 0x200
	}

	public static FLAGS m_flags;

	public static COMM_MODE g_comm_mode;

	internal static I2CS_MODE g_i2cs_mode;

	public static int AddTwo(int p_int1, int p_int2) {
		return p_int1 + p_int2;
	}

	[DllImport("User32.dll")]
	public static extern int MessageBox(int h, string m, string c, int type);

	[DllImport("kernel32.dll", CharSet = CharSet.Auto)]
	public static extern int CreateEvent(ref SECURITY_ATTRIBUTES SecurityAttributes, int bManualReset, int bInitialState, string lpName);

	[DllImport("kernel32.dll", SetLastError = true)]
	public static extern bool WriteFile(IntPtr hFile, byte[] Buffer, int numBytesToWrite, ref int numBytesWritten, int Overlapped);

	[DllImport("kernel32", SetLastError = true)]
	public static extern bool ReadFile(IntPtr hFile, byte[] Buffer, int NumberOfBytesToRead, ref int pNumberOfBytesRead, int Overlapped);

	[DllImport("kernel32.dll", SetLastError = true)]
	public static extern int ReadFileEx(IntPtr hFile, byte[] lpBuffer, int nNumberOfBytesToRead, ref OVERLAPPED lpOverlapped, int lpCompletionRoutine);

	[DllImport("kernel32.dll")]
	public static extern int CloseHandle(IntPtr hObject);

	[DllImport("kernel32.dll")]
	public static extern bool TerminateThread(IntPtr hThread, uint dwExitCode);

	[DllImport("kernel32.dll")]
	public static extern IntPtr OpenThread(ThreadAccess dwDesiredAccess, bool bInheritHandle, uint dwThreadId);

	[DllImport("kernel32.dll")]
	public static extern int WaitForSingleObject(int hHandle, int dwMilliseconds);

	public static void InitializeParams() {
		m_flags.HID_write_handle = IntPtr.Zero;
		m_flags.HID_read_handle = IntPtr.Zero;
		m_flags.write_buffer = new byte[65];
		m_flags.read_buffer = new byte[65];
		m_flags.bl_buffer = new byte[65];
		m_flags.orbl = 65;
		m_flags.irbl = 65;
		m_flags.g_status_packet_mutex = new Mutex(initiallyOwned: false);
		g_comm_mode = COMM_MODE.IDLE;
		g_i2cs_mode = I2CS_MODE.DEFAULT;
		Constants.STATUS_PACKET_DATA = new byte[65];
		Mode.configure_run_mode_arrays();
		m_flags.g_status_packet_data_update_event = new AutoResetEvent(initialState: false);
		m_flags.g_data_arrived_event = new AutoResetEvent(initialState: false);
		m_flags.g_bl_data_arrived_event = new AutoResetEvent(initialState: false);
		m_flags.g_PKSA_has_completed_script = new AutoResetEvent(initialState: false);
		m_flags.g_special_status_request_event = new AutoResetEvent(initialState: false);
		USBWrite.Initialize_Write_Objects();
		USBRead.Initialize_Read_Objects();
	}

	public static int Convert_Value_To_Int(string p_value) {
		if (p_value == "") {
			return 0;
		}

		uint[] array = new uint[34]
		{
			0u, 0u, 2147483648u, 1073741824u, 536870912u, 268435456u, 134217728u, 67108864u, 33554432u, 16777216u,
			8388608u, 4194304u, 2097152u, 1048576u, 524288u, 262144u, 131072u, 65536u, 32768u, 16384u,
			8192u, 4096u, 2048u, 1024u, 512u, 256u, 128u, 64u, 32u, 16u,
			8u, 4u, 2u, 1u
		};
		uint[] array2 = new uint[10] { 0u, 0u, 268435456u, 16777216u, 1048576u, 65536u, 4096u, 256u, 16u, 1u };
		int result = 0;
		if (p_value[0] == '\0') {
			result = 0;
		} else if (p_value[0] == 'Y' || p_value[0] == 'y') {
			result = 1;
		} else if (p_value[0] == 'N' || p_value[0] == 'n') {
			result = 0;
		} else if (p_value.Length > 1) {
			if ((p_value[0] == '0' && (p_value[1] == 'b' || p_value[1] == 'B')) || p_value[0] == 'b' || p_value[0] == 'B') {
				if (p_value.Length > 36) {
					result = 0;
				} else {
					int num = p_value.Length - 1;
					int num2 = ((p_value[0] != '0') ? 1 : 2);
					for (int i = num2; i <= num; i++) {
						int num3 = ((p_value[i] == '1') ? 1 : 0);
						result += (int)array[i + 34 - p_value.Length] * num3;
					}
				}
			} else if (p_value[0] == '0' && (p_value[1] == 'x' || p_value[1] == 'X')) {
				if (p_value.Length > 12) {
					result = 0;
				} else {
					int num = p_value.Length - 1;
					for (int i = 2; i <= num; i++) {
						int num3;
						switch (p_value[i]) {
							case 'A':
							case 'a':
							num3 = 10;
							break;
							case 'B':
							case 'b':
							num3 = 11;
							break;
							case 'C':
							case 'c':
							num3 = 12;
							break;
							case 'D':
							case 'd':
							num3 = 13;
							break;
							case 'E':
							case 'e':
							num3 = 14;
							break;
							case 'F':
							case 'f':
							num3 = 15;
							break;
							default: {
								string s = p_value[i].ToString();
								if (!int.TryParse(s, out num3)) {
									num3 = 0;
								}

								break;
							}
						}

						result += (int)array2[i + 10 - p_value.Length] * num3;
					}
				}
			} else if (!int.TryParse(p_value, out result)) {
				result = 0;
			}
		} else if (!int.TryParse(p_value, out result)) {
			result = 0;
		}

		return result;
	}

	public static bool This_Is_A_Valid_Number(string p_text) {
		bool result = false;
		if (p_text.Length > 0) {
			char[] anyOf = "0123456789aAbBcCdDeEfFxX".ToCharArray();
			result = true;
			for (int i = 0; i < p_text.Length; i++) {
				if (p_text.LastIndexOfAny(anyOf, i, 1) < 0) {
					result = false;
					break;
				}
			}
		}

		return result;
	}

	public static void Set_Comm_Mode(byte p_comm_mode, byte p_i2cs_mode) {
		switch (p_comm_mode) {
			case 0:
			g_comm_mode = COMM_MODE.IDLE;
			break;
			case 1:
			g_comm_mode = COMM_MODE.I2C_M;
			break;
			case 2:
			g_comm_mode = COMM_MODE.SPI_M;
			break;
			case 3:
			g_comm_mode = COMM_MODE.SPI_S;
			break;
			case 4:
			g_comm_mode = COMM_MODE.USART_A;
			break;
			case 5:
			g_comm_mode = COMM_MODE.USART_SM;
			break;
			case 6:
			g_comm_mode = COMM_MODE.USART_SS;
			break;
			case 7:
			g_comm_mode = COMM_MODE.I2C_S;
			g_i2cs_mode = (I2CS_MODE)p_i2cs_mode;
			break;
			case 8:
			g_comm_mode = COMM_MODE.I2C_BBM;
			break;
			case 9:
			g_comm_mode = COMM_MODE.I2C_SBBM;
			break;
			case 10:
			g_comm_mode = COMM_MODE.LIN;
			break;
			case 11:
			g_comm_mode = COMM_MODE.UWIRE;
			break;
			case 12:
			g_comm_mode = COMM_MODE.MTOUCH2;
			break;
			default:
			g_comm_mode = COMM_MODE.CM_ERROR;
			break;
		}
	}

	public static byte calculate_crc8(byte p_data, byte p_start_crc) {
		uint num = (byte)((uint)(p_data ^ p_start_crc) & 0xFFu);
		for (int i = 0; i < 8; i++) {
			num = (((num & 0x80) != 128) ? (num * 2) : ((num * 2) ^ 0x107u));
		}

		return (byte)(num & 0xFFu);
	}
}