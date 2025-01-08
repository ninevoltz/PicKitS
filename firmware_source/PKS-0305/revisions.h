//----------------------------------------------------------
//    0305-08  07-14-09 ME
//----------------------------------------------------------
//
//--- comm_spi.c
//    - re-arranged comm_spi_init() to the paradigm of
//      _init() = comm_spi_reset(), _config(), _clear()
//    - corrected comm_spi_clear() to properly restore
//      status block data - static and dynamic
//      (issue was evident in the reported bit rate)
//
//--- comm_usart.c
//    - re-arranged comm_usart_init() to the paradigm of
//      _init() = comm_usart_reset(), _config(), _clear()
//    - corrected comm_usart_clear() to properly restore
//      status block data - static and dynamic
//      (issue was evident in the reported baud rate)
//
//----------------------------------------------------------
//    0305-03  04-23-09 ME
//    0305-02  04-23-09 ME
//----------------------------------------------------------
//
//--- comm_gen.h
//    - added/defined tag 0x22 - wait on button 0
//    - added/defined tag 0x23 - wait on button 1
//    - added/defined tag 0x24 - special data generation
//
//--- comm_gen.c
//    - added action to debug tag 0x22 - wait on button 0
//    - added action to debug tag 0x23 - wait on button 1
//    - added action to debug tag 0x24 - special data generation
//    - set COMM_FLUSH_FLAG on COMM_TAG_GEN_MARKER_HOST = 0x77
//      used by PKSA GUI as end of script
//    - set COMM_FLUSH_FLAG processing COMM_TAG_GEN_END_SCRIPT
//
//--- exec.c
//    - added reset of COMM_FLUSH_FLAG if set
//
//----------------------------------------------------------
//    0305-01  04-21-09 ME
//----------------------------------------------------------
//
//--- project.h
//    - modified firmware version from 0x0304 to 0x0305
//
//--- comm_gen.h
//    - added/defined SCMND tag COMM_TAG_GEN_FLUSH_CBUF2 0x21
//    - defined flag COMM_FLUSH_FLAG to force flush of CBUF2
//
//--- comm_exec.c
//    - added support for COMM_FLUSH_FLAG as another condition
//      to force flush of CBUF2
//
//--- comm_spi.h
//    - added definition of AUX1 to control block
//    - added definition of AUX1 to status block
//    - added SCMND tags to define AUX1 functions
//
//--- comm_spi.c
//    - added support for AUX1 SCMND tags
//
//----------------------------------------------------------
//    0304-02  03-11-09 ME
//----------------------------------------------------------
//
//--- project.h
//    - modified firmware version from 0x0303 to 0x0304
//
//----------------------------------------------------------
//    0304-01  11-12-08 ME
//----------------------------------------------------------
//
//--- project
//
//    --- MISC
//    - added file "revisions.h" to document changes
//
//--- comm_lin.h
//
//    --- TO CORRECT CHECKSUM CALCULATION
//    - moved COMM_LIN_COUNT to comm_varbs_B[9] (allows checksum to be 2 bytes)
//    - made room for COMM_LIN_SP_CHECKSUM to be 2 bytes comm_varbs_B[7],[8] 
//
//--- comm_lin.c
//
//    --- TO CORRECT CHECKSUM CALCULATION
//    - defined pointer to LSB of checksum: COMM_LIN_SP_CHECKSUM_BP
//    - defined pointer to 16-bit checksum: COMM_LIN_SP_CHECKSUM_WP
//    - modified all references to COMM_LIN_SP_CHECKSUM to use pointer(s)
//    - modified checksum calculation(s) to include carry bit of LSB into LSB
//