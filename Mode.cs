using System;

namespace PICkitS;

public class Mode {
	private static byte[] m_default_idle_data = new byte[65];

	private static byte[] m_default_i2c_m_data = new byte[65];

	private static byte[] m_default_i2c_s_data = new byte[65];

	private static byte[] m_default_i2c_bbm_data = new byte[65];

	private static byte[] m_default_i2c_sbbm_data = new byte[65];

	private static byte[] m_default_spi_m_data = new byte[65];

	private static byte[] m_default_spi_s_data = new byte[65];

	private static byte[] m_default_usart_a_data = new byte[65];

	private static byte[] m_default_usart_sm_data = new byte[65];

	private static byte[] m_default_usart_ss_data = new byte[65];

	private static byte[] m_default_LIN_data = new byte[65];

	private static byte[] m_default_uwire_m_data = new byte[65];

	private static byte[] m_default_mtouch2_data = new byte[65];

	private static byte[] m_default_LIN_no_autobaud_data = new byte[65];

	private static byte[] m_test_5_volt_src_data = new byte[65];

	private static byte[] m_test_2p5_volt_src_data = new byte[65];

	private static byte[] m_test_0_volt_src_data = new byte[65];

	private static byte[] m_test_5_volt_src_no_pullup_data = new byte[65];

	private static byte[] m_test_i2c_axout_a11_a20_data = new byte[65];

	private static byte[] m_test_i2c_axout_a10_a21_data = new byte[65];

	private static byte[] m_test_i2c_axout_a11_a21_data = new byte[65];

	private static byte[] m_test_i2c_test_sw_enable_data = new byte[65];

	public static void configure_run_mode_arrays() {
		Array.Clear(m_default_idle_data, 0, m_default_idle_data.Length);
		Array.Clear(m_default_i2c_m_data, 0, m_default_i2c_m_data.Length);
		Array.Clear(m_default_i2c_s_data, 0, m_default_i2c_s_data.Length);
		Array.Clear(m_default_i2c_bbm_data, 0, m_default_i2c_bbm_data.Length);
		Array.Clear(m_default_i2c_sbbm_data, 0, m_default_i2c_sbbm_data.Length);
		Array.Clear(m_default_spi_m_data, 0, m_default_spi_m_data.Length);
		Array.Clear(m_default_spi_s_data, 0, m_default_spi_s_data.Length);
		Array.Clear(m_default_usart_a_data, 0, m_default_usart_a_data.Length);
		Array.Clear(m_default_usart_sm_data, 0, m_default_usart_sm_data.Length);
		Array.Clear(m_default_usart_ss_data, 0, m_default_usart_ss_data.Length);
		Array.Clear(m_default_LIN_data, 0, m_default_LIN_data.Length);
		Array.Clear(m_default_LIN_no_autobaud_data, 0, m_default_LIN_no_autobaud_data.Length);
		Array.Clear(m_default_uwire_m_data, 0, m_default_uwire_m_data.Length);
		Array.Clear(m_default_mtouch2_data, 0, m_default_mtouch2_data.Length);
		Array.Clear(m_test_5_volt_src_data, 0, m_test_5_volt_src_data.Length);
		Array.Clear(m_test_2p5_volt_src_data, 0, m_test_2p5_volt_src_data.Length);
		Array.Clear(m_test_0_volt_src_data, 0, m_test_0_volt_src_data.Length);
		Array.Clear(m_test_5_volt_src_no_pullup_data, 0, m_test_5_volt_src_no_pullup_data.Length);
		Array.Clear(m_test_i2c_axout_a11_a20_data, 0, m_test_i2c_axout_a11_a20_data.Length);
		Array.Clear(m_test_i2c_axout_a10_a21_data, 0, m_test_i2c_axout_a10_a21_data.Length);
		Array.Clear(m_test_i2c_axout_a11_a21_data, 0, m_test_i2c_axout_a11_a21_data.Length);
		Array.Clear(m_test_i2c_test_sw_enable_data, 0, m_test_i2c_test_sw_enable_data.Length);
		m_default_idle_data[7] = 192;
		m_default_idle_data[10] = 10;
		m_default_idle_data[11] = byte.MaxValue;
		m_default_idle_data[16] = 1;
		m_default_idle_data[17] = 32;
		m_default_idle_data[23] = 1;
		m_default_idle_data[24] = 7;
		m_default_i2c_m_data[7] = 192;
		m_default_i2c_m_data[10] = 10;
		m_default_i2c_m_data[11] = byte.MaxValue;
		m_default_i2c_m_data[15] = 1;
		m_default_i2c_m_data[16] = 49;
		m_default_i2c_m_data[17] = 32;
		m_default_i2c_m_data[23] = 3;
		m_default_i2c_m_data[24] = 6;
		m_default_i2c_m_data[30] = 49;
		m_default_spi_m_data[7] = 192;
		m_default_spi_m_data[10] = 10;
		m_default_spi_m_data[11] = byte.MaxValue;
		m_default_spi_m_data[15] = 2;
		m_default_spi_m_data[16] = 33;
		m_default_spi_m_data[17] = 32;
		m_default_spi_m_data[19] = 176;
		m_default_spi_m_data[20] = 44;
		m_default_spi_m_data[23] = 0;
		m_default_spi_m_data[24] = 3;
		m_default_spi_m_data[29] = 0;
		m_default_spi_m_data[30] = byte.MaxValue;
		m_default_spi_s_data[7] = 192;
		m_default_spi_s_data[10] = 10;
		m_default_spi_s_data[11] = byte.MaxValue;
		m_default_spi_s_data[15] = 3;
		m_default_spi_s_data[16] = 33;
		m_default_spi_s_data[17] = 32;
		m_default_spi_s_data[19] = 176;
		m_default_spi_s_data[20] = 44;
		m_default_spi_s_data[23] = 0;
		m_default_spi_s_data[24] = 3;
		m_default_spi_s_data[29] = 0;
		m_default_spi_s_data[30] = byte.MaxValue;
		m_default_uwire_m_data[7] = 192;
		m_default_uwire_m_data[10] = 10;
		m_default_uwire_m_data[11] = byte.MaxValue;
		m_default_uwire_m_data[15] = 11;
		m_default_uwire_m_data[16] = 33;
		m_default_uwire_m_data[17] = 32;
		m_default_uwire_m_data[19] = 176;
		m_default_uwire_m_data[20] = 44;
		m_default_uwire_m_data[23] = 0;
		m_default_uwire_m_data[24] = 131;
		m_default_uwire_m_data[29] = 0;
		m_default_uwire_m_data[30] = byte.MaxValue;
		m_default_usart_a_data[7] = 192;
		m_default_usart_a_data[10] = 10;
		m_default_usart_a_data[11] = byte.MaxValue;
		m_default_usart_a_data[15] = 4;
		m_default_usart_a_data[16] = 33;
		m_default_usart_a_data[17] = 32;
		m_default_usart_a_data[23] = 1;
		m_default_usart_a_data[24] = 0;
		m_default_usart_a_data[25] = 15;
		m_default_usart_a_data[30] = 16;
		m_default_usart_sm_data[7] = 192;
		m_default_usart_sm_data[10] = 10;
		m_default_usart_sm_data[11] = byte.MaxValue;
		m_default_usart_sm_data[15] = 5;
		m_default_usart_sm_data[16] = 33;
		m_default_usart_sm_data[17] = 32;
		m_default_usart_sm_data[23] = 1;
		m_default_usart_sm_data[24] = 0;
		m_default_usart_sm_data[25] = 15;
		m_default_usart_sm_data[30] = 16;
		m_default_usart_ss_data[7] = 192;
		m_default_usart_ss_data[10] = 10;
		m_default_usart_ss_data[11] = byte.MaxValue;
		m_default_usart_ss_data[15] = 6;
		m_default_usart_ss_data[16] = 33;
		m_default_usart_ss_data[17] = 32;
		m_default_usart_ss_data[23] = 1;
		m_default_usart_ss_data[24] = 0;
		m_default_usart_ss_data[25] = 15;
		m_default_usart_ss_data[30] = 16;
		m_default_i2c_s_data[7] = 192;
		m_default_i2c_s_data[10] = 10;
		m_default_i2c_s_data[11] = byte.MaxValue;
		m_default_i2c_s_data[15] = 7;
		m_default_i2c_s_data[16] = 33;
		m_default_i2c_s_data[17] = 32;
		m_default_i2c_s_data[23] = 1;
		m_default_i2c_s_data[24] = byte.MaxValue;
		m_default_i2c_s_data[25] = 6;
		m_default_i2c_s_data[27] = 0;
		m_default_i2c_s_data[28] = 0;
		m_default_i2c_s_data[29] = 0;
		m_default_i2c_s_data[30] = 0;
		m_default_i2c_bbm_data[7] = 192;
		m_default_i2c_bbm_data[10] = 10;
		m_default_i2c_bbm_data[11] = byte.MaxValue;
		m_default_i2c_bbm_data[15] = 8;
		m_default_i2c_bbm_data[16] = 33;
		m_default_i2c_bbm_data[17] = 32;
		m_default_i2c_bbm_data[23] = 3;
		m_default_i2c_bbm_data[24] = 6;
		m_default_i2c_bbm_data[30] = 127;
		m_default_i2c_sbbm_data[7] = 192;
		m_default_i2c_sbbm_data[10] = 10;
		m_default_i2c_sbbm_data[11] = byte.MaxValue;
		m_default_i2c_sbbm_data[15] = 9;
		m_default_i2c_sbbm_data[16] = 33;
		m_default_i2c_sbbm_data[17] = 32;
		m_default_i2c_sbbm_data[23] = 3;
		m_default_i2c_sbbm_data[24] = 6;
		m_default_i2c_sbbm_data[30] = 127;
		m_default_LIN_data[7] = 192;
		m_default_LIN_data[10] = 53;
		m_default_LIN_data[11] = 124;
		m_default_LIN_data[15] = 10;
		m_default_LIN_data[16] = 51;
		m_default_LIN_data[17] = 32;
		m_default_LIN_data[23] = 200;
		m_default_LIN_data[24] = 152;
		m_default_LIN_data[25] = 15;
		m_default_LIN_data[29] = 243;
		m_default_LIN_data[30] = 1;
		m_default_LIN_no_autobaud_data[7] = 192;
		m_default_LIN_no_autobaud_data[10] = 53;
		m_default_LIN_no_autobaud_data[11] = 124;
		m_default_LIN_no_autobaud_data[15] = 10;
		m_default_LIN_no_autobaud_data[16] = 51;
		m_default_LIN_no_autobaud_data[17] = 32;
		m_default_LIN_no_autobaud_data[23] = 72;
		m_default_LIN_no_autobaud_data[24] = 152;
		m_default_LIN_no_autobaud_data[25] = 15;
		m_default_LIN_no_autobaud_data[29] = 243;
		m_default_LIN_no_autobaud_data[30] = 1;
		m_default_mtouch2_data[7] = 192;
		m_default_mtouch2_data[10] = 10;
		m_default_mtouch2_data[11] = byte.MaxValue;
		m_default_mtouch2_data[15] = 12;
		m_default_mtouch2_data[16] = 49;
		m_default_mtouch2_data[17] = 32;
		m_default_mtouch2_data[23] = 3;
		m_default_mtouch2_data[24] = 6;
		m_default_mtouch2_data[30] = 49;
		m_test_5_volt_src_data[7] = 192;
		m_test_5_volt_src_data[10] = 10;
		m_test_5_volt_src_data[11] = byte.MaxValue;
		m_test_5_volt_src_data[15] = 1;
		m_test_5_volt_src_data[16] = 113;
		m_test_5_volt_src_data[17] = 32;
		m_test_5_volt_src_data[19] = byte.MaxValue;
		m_test_5_volt_src_data[20] = 63;
		m_test_5_volt_src_data[23] = 3;
		m_test_5_volt_src_data[24] = 6;
		m_test_5_volt_src_data[30] = 127;
		m_test_2p5_volt_src_data[7] = 192;
		m_test_2p5_volt_src_data[10] = 10;
		m_test_2p5_volt_src_data[11] = byte.MaxValue;
		m_test_2p5_volt_src_data[15] = 1;
		m_test_2p5_volt_src_data[16] = 113;
		m_test_2p5_volt_src_data[17] = 32;
		m_test_2p5_volt_src_data[19] = 120;
		m_test_2p5_volt_src_data[20] = 30;
		m_test_2p5_volt_src_data[23] = 3;
		m_test_2p5_volt_src_data[24] = 6;
		m_test_2p5_volt_src_data[30] = 127;
		m_test_0_volt_src_data[7] = 192;
		m_test_0_volt_src_data[10] = 10;
		m_test_0_volt_src_data[11] = byte.MaxValue;
		m_test_0_volt_src_data[15] = 1;
		m_test_0_volt_src_data[16] = 113;
		m_test_0_volt_src_data[17] = 32;
		m_test_0_volt_src_data[23] = 3;
		m_test_0_volt_src_data[24] = 6;
		m_test_0_volt_src_data[30] = 127;
		m_test_5_volt_src_no_pullup_data[7] = 192;
		m_test_5_volt_src_no_pullup_data[10] = 10;
		m_test_5_volt_src_no_pullup_data[11] = byte.MaxValue;
		m_test_5_volt_src_no_pullup_data[15] = 1;
		m_test_5_volt_src_no_pullup_data[16] = 97;
		m_test_5_volt_src_no_pullup_data[17] = 32;
		m_test_5_volt_src_no_pullup_data[19] = byte.MaxValue;
		m_test_5_volt_src_no_pullup_data[20] = 63;
		m_test_5_volt_src_no_pullup_data[23] = 3;
		m_test_5_volt_src_no_pullup_data[24] = 6;
		m_test_5_volt_src_no_pullup_data[30] = 127;
		m_test_i2c_axout_a11_a20_data[7] = 192;
		m_test_i2c_axout_a11_a20_data[10] = 10;
		m_test_i2c_axout_a11_a20_data[11] = byte.MaxValue;
		m_test_i2c_axout_a11_a20_data[15] = 1;
		m_test_i2c_axout_a11_a20_data[16] = 49;
		m_test_i2c_axout_a11_a20_data[17] = 32;
		m_test_i2c_axout_a11_a20_data[23] = 3;
		m_test_i2c_axout_a11_a20_data[24] = 6;
		m_test_i2c_axout_a11_a20_data[28] = 1;
		m_test_i2c_axout_a11_a20_data[30] = 127;
		m_test_i2c_axout_a10_a21_data[7] = 192;
		m_test_i2c_axout_a10_a21_data[10] = 160;
		m_test_i2c_axout_a10_a21_data[11] = byte.MaxValue;
		m_test_i2c_axout_a10_a21_data[15] = 1;
		m_test_i2c_axout_a10_a21_data[16] = 49;
		m_test_i2c_axout_a10_a21_data[17] = 32;
		m_test_i2c_axout_a10_a21_data[23] = 3;
		m_test_i2c_axout_a10_a21_data[24] = 6;
		m_test_i2c_axout_a10_a21_data[28] = 2;
		m_test_i2c_axout_a10_a21_data[30] = 127;
		m_test_i2c_axout_a11_a21_data[7] = 192;
		m_test_i2c_axout_a11_a21_data[10] = 160;
		m_test_i2c_axout_a11_a21_data[11] = byte.MaxValue;
		m_test_i2c_axout_a11_a21_data[15] = 1;
		m_test_i2c_axout_a11_a21_data[16] = 49;
		m_test_i2c_axout_a11_a21_data[17] = 32;
		m_test_i2c_axout_a11_a21_data[23] = 3;
		m_test_i2c_axout_a11_a21_data[24] = 6;
		m_test_i2c_axout_a11_a21_data[28] = 3;
		m_test_i2c_axout_a11_a21_data[30] = 127;
		m_test_i2c_test_sw_enable_data[7] = 192;
		m_test_i2c_test_sw_enable_data[10] = 160;
		m_test_i2c_test_sw_enable_data[8] = 1;
		m_test_i2c_test_sw_enable_data[10] = 0;
		m_test_i2c_test_sw_enable_data[11] = byte.MaxValue;
		m_test_i2c_test_sw_enable_data[15] = 1;
		m_test_i2c_test_sw_enable_data[16] = 49;
		m_test_i2c_test_sw_enable_data[17] = 32;
		m_test_i2c_test_sw_enable_data[23] = 3;
		m_test_i2c_test_sw_enable_data[24] = 6;
		m_test_i2c_test_sw_enable_data[30] = 127;
	}

	public static void update_status_packet_data(int p_index, ref byte[] p_status_packet_data) {
		Device.Set_Script_Timeout_Option(p_use_timeout: true);
		switch (p_index) {
			case 0: {
				for (int num4 = 7; num4 < m_default_idle_data.Length; num4++) {
					p_status_packet_data[num4] = m_default_idle_data[num4];
				}

				break;
			}
			case 1: {
				for (int num12 = 7; num12 < m_default_idle_data.Length; num12++) {
					p_status_packet_data[num12] = m_default_i2c_m_data[num12];
				}

				break;
			}
			case 2: {
				for (int j = 7; j < m_default_idle_data.Length; j++) {
					p_status_packet_data[j] = m_default_spi_m_data[j];
				}

				break;
			}
			case 3: {
				for (int num8 = 7; num8 < m_default_idle_data.Length; num8++) {
					p_status_packet_data[num8] = m_default_spi_s_data[num8];
				}

				break;
			}
			case 4: {
				for (int n = 7; n < m_default_idle_data.Length; n++) {
					p_status_packet_data[n] = m_default_usart_a_data[n];
				}

				break;
			}
			case 5: {
				for (int num14 = 7; num14 < m_default_idle_data.Length; num14++) {
					p_status_packet_data[num14] = m_default_usart_sm_data[num14];
				}

				break;
			}
			case 6: {
				for (int num10 = 7; num10 < m_default_idle_data.Length; num10++) {
					p_status_packet_data[num10] = m_default_usart_ss_data[num10];
				}

				break;
			}
			case 7: {
				for (int num6 = 7; num6 < m_default_idle_data.Length; num6++) {
					p_status_packet_data[num6] = m_default_i2c_s_data[num6];
				}

				break;
			}
			case 8: {
				for (int num2 = 7; num2 < m_default_idle_data.Length; num2++) {
					p_status_packet_data[num2] = m_default_i2c_bbm_data[num2];
				}

				break;
			}
			case 9: {
				for (int l = 7; l < m_default_idle_data.Length; l++) {
					p_status_packet_data[l] = m_default_i2c_sbbm_data[l];
				}

				break;
			}
			case 10: {
				Device.Set_Script_Timeout_Option(p_use_timeout: false);
				for (int num15 = 7; num15 < m_default_idle_data.Length; num15++) {
					p_status_packet_data[num15] = m_default_LIN_data[num15];
				}

				break;
			}
			case 11: {
				for (int num13 = 7; num13 < m_default_idle_data.Length; num13++) {
					p_status_packet_data[num13] = m_default_uwire_m_data[num13];
				}

				break;
			}
			case 19: {
				Device.Set_Script_Timeout_Option(p_use_timeout: false);
				for (int num11 = 7; num11 < m_default_idle_data.Length; num11++) {
					p_status_packet_data[num11] = m_default_LIN_no_autobaud_data[num11];
				}

				break;
			}
			case 20: {
				for (int num9 = 7; num9 < m_default_idle_data.Length; num9++) {
					p_status_packet_data[num9] = m_test_5_volt_src_data[num9];
				}

				break;
			}
			case 21: {
				for (int num7 = 7; num7 < m_default_idle_data.Length; num7++) {
					p_status_packet_data[num7] = m_test_2p5_volt_src_data[num7];
				}

				break;
			}
			case 22: {
				for (int num5 = 7; num5 < m_default_idle_data.Length; num5++) {
					p_status_packet_data[num5] = m_test_0_volt_src_data[num5];
				}

				break;
			}
			case 23: {
				for (int num3 = 7; num3 < m_default_idle_data.Length; num3++) {
					p_status_packet_data[num3] = m_test_5_volt_src_no_pullup_data[num3];
				}

				break;
			}
			case 24: {
				for (int num = 7; num < m_default_idle_data.Length; num++) {
					p_status_packet_data[num] = m_test_i2c_axout_a11_a20_data[num];
				}

				break;
			}
			case 25: {
				for (int m = 7; m < m_default_idle_data.Length; m++) {
					p_status_packet_data[m] = m_test_i2c_axout_a10_a21_data[m];
				}

				break;
			}
			case 26: {
				for (int k = 7; k < m_default_idle_data.Length; k++) {
					p_status_packet_data[k] = m_test_i2c_axout_a11_a21_data[k];
				}

				break;
			}
			case 27: {
				for (int i = 7; i < m_default_idle_data.Length; i++) {
					p_status_packet_data[i] = m_test_i2c_test_sw_enable_data[i];
				}

				break;
			}
			case 12:
			case 13:
			case 14:
			case 15:
			case 16:
			case 17:
			case 18:
			break;
		}
	}
}