/**
********************************************************************************
*
* @file:    adi_bms_utility.c
*
* @brief:   This file contains utility functions implementation.
*
* @details:
*
*******************************************************************************
Copyright(c) 2020 Analog Devices, Inc. All Rights Reserved. This software is
proprietary & confidential to Analog Devices, Inc. and its licensors. By using
this software you agree to the terms of the associated Analog Devices License
Agreement.
*******************************************************************************
*/
/*! \addtogroup BMS_Driver
*  @{
*/

/*! \addtogroup Driver_Utility
*  @{
*/

/*!<
This documentation provides details about BMS driver APIs and their usage.
Using the BMS Driver Application can:
- Read/Write the configuration registers of the BMS devices stacked in daisy chaining.
- Send commands and Read the Cell Voltages, Aux Voltages and Status registers (Sum of cells, Internal Die temperature etc.)
*/

/*============= I N C L U D E S =============*/
#include "bms.h"
#include "Platform.h"
/*============== D E F I N E S ===============*/
/*============= E X T E R N A L S ============*/
/*============= E N U M E R A T O R S ============*/

#ifdef MBED
extern Serial pc;
#endif /* MBED */

/*!<**************************************** BMS Driver APIs definitions ********************************************/

/*!<
* @brief Precomputed CRC15 Table
*/
const uint16 Crc15Table[256] =
{
  0x0000,0xc599, 0xceab, 0xb32, 0xd8cf, 0x1d56, 0x1664, 0xd3fd, 0xf407, 0x319e, 0x3aac,
  0xff35, 0x2cc8, 0xe951, 0xe263, 0x27fa, 0xad97, 0x680e, 0x633c, 0xa6a5, 0x7558, 0xb0c1,
  0xbbf3, 0x7e6a, 0x5990, 0x9c09, 0x973b, 0x52a2, 0x815f, 0x44c6, 0x4ff4, 0x8a6d, 0x5b2e,
  0x9eb7, 0x9585, 0x501c, 0x83e1, 0x4678, 0x4d4a, 0x88d3, 0xaf29, 0x6ab0, 0x6182, 0xa41b,
  0x77e6, 0xb27f, 0xb94d, 0x7cd4, 0xf6b9, 0x3320, 0x3812, 0xfd8b, 0x2e76, 0xebef, 0xe0dd,
  0x2544, 0x2be, 0xc727, 0xcc15, 0x98c, 0xda71, 0x1fe8, 0x14da, 0xd143, 0xf3c5, 0x365c,
  0x3d6e, 0xf8f7,0x2b0a, 0xee93, 0xe5a1, 0x2038, 0x7c2, 0xc25b, 0xc969, 0xcf0, 0xdf0d,
  0x1a94, 0x11a6, 0xd43f, 0x5e52, 0x9bcb, 0x90f9, 0x5560, 0x869d, 0x4304, 0x4836, 0x8daf,
  0xaa55, 0x6fcc, 0x64fe, 0xa167, 0x729a, 0xb703, 0xbc31, 0x79a8, 0xa8eb, 0x6d72, 0x6640,
  0xa3d9, 0x7024, 0xb5bd, 0xbe8f, 0x7b16, 0x5cec, 0x9975, 0x9247, 0x57de, 0x8423, 0x41ba,
  0x4a88, 0x8f11, 0x57c, 0xc0e5, 0xcbd7, 0xe4e, 0xddb3, 0x182a, 0x1318, 0xd681, 0xf17b,
  0x34e2, 0x3fd0, 0xfa49, 0x29b4, 0xec2d, 0xe71f, 0x2286, 0xa213, 0x678a, 0x6cb8, 0xa921,
  0x7adc, 0xbf45, 0xb477, 0x71ee, 0x5614, 0x938d, 0x98bf, 0x5d26, 0x8edb, 0x4b42, 0x4070,
  0x85e9, 0xf84, 0xca1d, 0xc12f, 0x4b6, 0xd74b, 0x12d2, 0x19e0, 0xdc79, 0xfb83, 0x3e1a, 0x3528,
  0xf0b1, 0x234c, 0xe6d5, 0xede7, 0x287e, 0xf93d, 0x3ca4, 0x3796, 0xf20f, 0x21f2, 0xe46b, 0xef59,
  0x2ac0, 0xd3a, 0xc8a3, 0xc391, 0x608, 0xd5f5, 0x106c, 0x1b5e, 0xdec7, 0x54aa, 0x9133, 0x9a01,
  0x5f98, 0x8c65, 0x49fc, 0x42ce, 0x8757, 0xa0ad, 0x6534, 0x6e06, 0xab9f, 0x7862, 0xbdfb, 0xb6c9,
  0x7350, 0x51d6, 0x944f, 0x9f7d, 0x5ae4, 0x8919, 0x4c80, 0x47b2, 0x822b, 0xa5d1, 0x6048, 0x6b7a,
  0xaee3, 0x7d1e, 0xb887, 0xb3b5, 0x762c, 0xfc41, 0x39d8, 0x32ea, 0xf773, 0x248e, 0xe117, 0xea25,
  0x2fbc, 0x846, 0xcddf, 0xc6ed, 0x374, 0xd089, 0x1510, 0x1e22, 0xdbbb, 0xaf8, 0xcf61, 0xc453,
  0x1ca, 0xd237, 0x17ae, 0x1c9c, 0xd905, 0xfeff, 0x3b66, 0x3054, 0xf5cd, 0x2630, 0xe3a9, 0xe89b,
  0x2d02, 0xa76f, 0x62f6, 0x69c4, 0xac5d, 0x7fa0, 0xba39, 0xb10b, 0x7492, 0x5368, 0x96f1, 0x9dc3,
  0x585a, 0x8ba7, 0x4e3e, 0x450c, 0x8095
};

/* Pre-computed CRC10 Table */
static const uint16 crc10Table[256] =
    {
        0x000, 0x08f, 0x11e, 0x191, 0x23c, 0x2b3, 0x322, 0x3ad, 0x0f7, 0x078, 0x1e9, 0x166, 0x2cb, 0x244, 0x3d5, 0x35a,
        0x1ee, 0x161, 0x0f0, 0x07f, 0x3d2, 0x35d, 0x2cc, 0x243, 0x119, 0x196, 0x007, 0x088, 0x325, 0x3aa, 0x23b, 0x2b4,
        0x3dc, 0x353, 0x2c2, 0x24d, 0x1e0, 0x16f, 0x0fe, 0x071, 0x32b, 0x3a4, 0x235, 0x2ba, 0x117, 0x198, 0x009, 0x086,
        0x232, 0x2bd, 0x32c, 0x3a3, 0x00e, 0x081, 0x110, 0x19f, 0x2c5, 0x24a, 0x3db, 0x354, 0x0f9, 0x076, 0x1e7, 0x168,
        0x337, 0x3b8, 0x229, 0x2a6, 0x10b, 0x184, 0x015, 0x09a, 0x3c0, 0x34f, 0x2de, 0x251, 0x1fc, 0x173, 0x0e2, 0x06d,
        0x2d9, 0x256, 0x3c7, 0x348, 0x0e5, 0x06a, 0x1fb, 0x174, 0x22e, 0x2a1, 0x330, 0x3bf, 0x012, 0x09d, 0x10c, 0x183,
        0x0eb, 0x064, 0x1f5, 0x17a, 0x2d7, 0x258, 0x3c9, 0x346, 0x01c, 0x093, 0x102, 0x18d, 0x220, 0x2af, 0x33e, 0x3b1,
        0x105, 0x18a, 0x01b, 0x094, 0x339, 0x3b6, 0x227, 0x2a8, 0x1f2, 0x17d, 0x0ec, 0x063, 0x3ce, 0x341, 0x2d0, 0x25f,
        0x2e1, 0x26e, 0x3ff, 0x370, 0x0dd, 0x052, 0x1c3, 0x14c, 0x216, 0x299, 0x308, 0x387, 0x02a, 0x0a5, 0x134, 0x1bb,
        0x30f, 0x380, 0x211, 0x29e, 0x133, 0x1bc, 0x02d, 0x0a2, 0x3f8, 0x377, 0x2e6, 0x269, 0x1c4, 0x14b, 0x0da, 0x055,
        0x13d, 0x1b2, 0x023, 0x0ac, 0x301, 0x38e, 0x21f, 0x290, 0x1ca, 0x145, 0x0d4, 0x05b, 0x3f6, 0x379, 0x2e8, 0x267,
        0x0d3, 0x05c, 0x1cd, 0x142, 0x2ef, 0x260, 0x3f1, 0x37e, 0x024, 0x0ab, 0x13a, 0x1b5, 0x218, 0x297, 0x306, 0x389,
        0x1d6, 0x159, 0x0c8, 0x047, 0x3ea, 0x365, 0x2f4, 0x27b, 0x121, 0x1ae, 0x03f, 0x0b0, 0x31d, 0x392, 0x203, 0x28c,
        0x038, 0x0b7, 0x126, 0x1a9, 0x204, 0x28b, 0x31a, 0x395, 0x0cf, 0x040, 0x1d1, 0x15e, 0x2f3, 0x27c, 0x3ed, 0x362,
        0x20a, 0x285, 0x314, 0x39b, 0x036, 0x0b9, 0x128, 0x1a7, 0x2fd, 0x272, 0x3e3, 0x36c, 0x0c1, 0x04e, 0x1df, 0x150,
        0x3e4, 0x36b, 0x2fa, 0x275, 0x1d8, 0x157, 0x0c6, 0x049, 0x313, 0x39c, 0x20d, 0x282, 0x12f, 0x1a0, 0x031, 0x0be};
/* Const 16 section end */


/**
*******************************************************************************
* Function: Pec15_Calc
* @brief CRC15 Pec Calculation Function
*
* @details This function calculates and return the CRC15 value
*
* Parameters:
* @param [in]	Len	Data length
*
* @param [in] *data    Data pointer
*
* @return CRC15_Value
*
*******************************************************************************
*/
uint16 Pec15_Calc
(
	uint8 len, /*!< Number of bytes that will be used to calculate a PEC */
	uint8 *data /*!< Array of data that will be used to calculate  a PEC */
)
{
  uint16 remainder,addr;
  remainder = 16u; /*!< initialize the PEC */
  for (uint8 i = 0; i<len; i++) /*!< loops for each byte in data array */
  {
    addr = (((remainder>>7u)^data[i])&0xff);/*!< calculate PEC table address */
    remainder = ((remainder<<8u)^Crc15Table[addr]);
  }
  return(remainder*2u);/*!< The CRC15 has a 0 in the LSB so the remainder must be multiplied by 2 */
}

/**
*******************************************************************************
* Function: Pec10_Calc
* @brief CRC10 Pec Calculation Function
*
* @details This function calculates and return the CRC15 value
*
* Parameters:
* @param [in]	Len	Data length
*
* @param [in] *data    Data pointer
*
* @return CRC10_Value
*
*******************************************************************************
*/
uint16 pec10_calc(bool rx_cmd, int len, uint8 *data)
{
  uint16 remainder = 16u; /*!< PEC_SEED;   0000010000 */
  uint16 polynom = 0x8F; /*!< x10 + x7 + x3 + x2 + x + 1 <- the CRC15 polynomial         100 1000 1111   48F */

  /*!< Perform modulo-2 division, a byte at a time. */
  for (uint8 pbyte = 0; pbyte < len; ++pbyte)
  {
    /*!< Bring the next byte into the remainder. */
    remainder ^= (uint16)((uint16)data[pbyte] << 2u);
    /*!< Perform modulo-2 division, a bit at a time.*/
    for (uint8 bit_ = 8u; bit_ > 0u; --bit_)
    {
      /*!< Try to divide the current data bit. */
      if ((remainder & 0x200) > 0u)/*!<equivalent to remainder & 2^14 simply check for MSB */
      {
        remainder = (uint16)((remainder << 1u));
        remainder = (uint16)(remainder ^ polynom);
      }
      else
      {
        remainder = (uint16)(remainder << 1u);
      }
    }
  }
  if (rx_cmd == true)
  {
    remainder ^= (uint16)(((uint16)data[len] & (uint8)0xFC) << 2u);
  }
  /*!< Perform modulo-2 division, a bit at a time */
  for (uint8 bit_ = 6u; bit_ > 0u; --bit_)
  {
    /*!< Try to divide the current data bit */
    if ((remainder & 0x200) > 0u)/*!<equivalent to remainder & 2^14 simply check for MSB*/
    {
      remainder = (uint16)((remainder << 1u));
      remainder = (uint16)(remainder ^ polynom);
    }
    else
    {
      remainder = (uint16)((remainder << 1u));
    }
  }
  return ((uint16)(remainder & 0x3FFu));
}

/**
*******************************************************************************
* Function: pec10_calc_modular
* @brief calculate data pec
*
* @details This function is used to calculate Data PEC.
*
* @return Pec
*
*******************************************************************************
*/
#if 0
uint16 pec10_calc_modular(
    uint8 * data, // Array of data that will be used to calculate a PEC
    uint8 PEC_Format // use any of PEC10_...
)
{
    uint16 remainder = 16u; // PEC_SEED;
    uint16 len;
    switch (PEC_Format)
    {
    case PEC10_WRITE:
        data[6] = 0; // for write commands the command counter is all zero
        // step through
    case PEC10_READ:
        len = 6;
        break;
    case PEC10_READ256:
        len = 256;
        break;
    case PEC10_READ512:
        len = 512;
        break;
    case PEC10_WRITE2:
        data[2] = 0;
        // step through
    case PEC10_READ2:
        len = 2;
        break;
    default:
        return 0xFFFF;
        break;
    }
    //Perform modulo-2 division, a byte at a time.
    for (uint8 pbyte = 0; pbyte < len; ++pbyte)
    {
        // Bring the next byte into the remainder.
        remainder ^= (uint16)(data[pbyte]) << 2u;
        remainder = pec10_calc_int(remainder, 8u);
    }
    // the last byte is different as it holds the 6-bit command counter
    // Note: for write commands, those bits are zero!
    remainder ^= (uint16)((data[len] & 0xFC) << 2u);
    remainder = pec10_calc_int(remainder, 6u);
    return ((uint16)(remainder & 0x3FF));
}
#endif
/**
*******************************************************************************
* Function: pec10_calc_int
* @brief calculate data pec
*
* @details This function is used to calculate Data PEC.
*
* @return Pec
*
*******************************************************************************
*/
#if 0
uint16 pec10_calc_int(uint16 remainder, uint8 bit)
{
  uint8 PEC10_POLY = 0x8F;
    //Perform modulo-2 division, a bit at a time.
    //Perform modulo-2 division, a bit at a time.
    for (; bit > 0; --bit)
    {
        //Try to divide the current data bit.
        if ((remainder & 0x200u) > 0u)//equivalent to remainder & 2^14 simply check for MSB
        {
            remainder = (uint16)((remainder << 1u));
            remainder = (uint16)(remainder ^ PEC10_POLY);
        }
        else
        {
            remainder = (uint16)(remainder << 1u);
        }
    }
    return ((uint16)(remainder & 0x3FFu));
}
#endif
/**
*******************************************************************************
* Function: spiSendCmd
* @brief Send command over SPI
*
* @details This function send bms command in spi line
*
* Parameters:
* @param [in]	tIC      Total IC
*
* @param [in]  *ic      cell_asic structure pointer
*
* @param [in]  tx_cmd   Tx command byte array
*
* @return None
*
*******************************************************************************
*/
#if 0
void spiSendCmd(uint8 tIC, cell_asic *ic, uint8 tx_cmd[2])
{
  uint8 cmd[4];
  uint16 cmd_pec;
  cmd[0] = tx_cmd[0];
  cmd[1] =  tx_cmd[1];
  cmd_pec = Pec15_Calc(2, cmd);
  cmd[2] = (uint8)(cmd_pec >> 8);
  cmd[3] = (uint8)(cmd_pec);
  // SRST & RSTCC cmd not increment command counter
  if((cmd[1] == 0x2E)|| (cmd[1] == 0x27)) { }
  else { adBms2950_IncCalCmdCounter(tIC, ic); }
#ifdef ISOSPI_WAKEUP
  adBmsWakeupIc(tIC);
#endif
  adBmsCsLow();
  spiWriteBytes(&cmd[0], 4);
  adBmsCsHigh();

  if(tx_cmd == SRST)
  {
    Delay_us(RegulatorStartupTime_us);
  }
}
#endif

/**
*******************************************************************************
* Function: spiReadData
* @brief Spi Read Data, Lower Level function for adBmsReadData
*
* @details This function send command to DUT and read command corrospond data byte.
*
* Parameters:
* @param [in]	tIC     Total IC
*
* @param [in]  tx_cmd   Tx command bytes
*
* @param [in]  *rx_data Rx data pointer
*
* @param [in]  *pec_error Pec error pointer
*
* @param [in]  *cmd_cntr command counter pointer
*
* @return None
*
*******************************************************************************
*/
#if 0
void spiReadData
(
uint8 tIC,
uint8 tx_cmd[2],
uint8 *rx_data,
uint8 *pec_error,
uint8 *cmd_cntr,
uint8 regData_size
)
{
  uint8 *data, *copyArray, src_address = 0;
  uint16 cmd_pec, received_pec, calculated_pec;
  uint8 BYTES_IN_REG = regData_size;
  uint8 RX_BUFFER = (regData_size * tIC);

  data = (uint8 *)calloc(RX_BUFFER, sizeof(uint8));
  copyArray = (uint8 *)calloc(BYTES_IN_REG, sizeof(uint8));
  if((data == NULL) || (copyArray == NULL))
  {
    printMsg("Failed to allocate spi read data memory");
    exit(0);
  }
  else
  {
    uint8 cmd[4];
    cmd[0] = tx_cmd[0];
    cmd[1] = tx_cmd[1];
    cmd_pec = Pec15_Calc(2, cmd);
    cmd[2] = (uint8)(cmd_pec >> 8);
    cmd[3] = (uint8)(cmd_pec);

#ifdef ISOSPI_WAKEUP
    adBmsWakeupIc(tIC);
#endif
    adBmsCsLow();
    spiWriteReadBytes(&cmd[0], &data[0], RX_BUFFER);                 //!< Read the configuration data of all ICs on the daisy chain into readdata array
    adBmsCsHigh();
    for (uint8 current_ic = 0; current_ic < tIC; current_ic++)     //!< executes for each ic in the daisy chain and packs the data
    {																																      //!< Into the r_comm array as well as check the received data for any bit errors
      // for (uint8 current_byte = 0; current_byte < (BYTES_IN_REG-2); current_byte++)
      for (uint8 current_byte = 0; current_byte < (BYTES_IN_REG); current_byte++)
      {
        rx_data[(current_ic*BYTES_IN_REG)+current_byte] = data[current_byte + (current_ic*BYTES_IN_REG)];
      }
      //!< Get command counter value
      cmd_cntr[current_ic] = (data[(current_ic * BYTES_IN_REG) + (BYTES_IN_REG - 2)] >> 2);
      //!< Get received pec value from ic
      received_pec = (uint16)(((data[(current_ic * BYTES_IN_REG) + (BYTES_IN_REG - 2)] & 0x03) << 8) | data[(current_ic * BYTES_IN_REG) + (BYTES_IN_REG - 1)]);
      //!< Copy each ic correspond data + pec value for calculate data pec
      memcpy(&copyArray[0], &data[src_address], BYTES_IN_REG);
      src_address = ((current_ic+1) * (regData_size));
      //!< Calculate data pec
      calculated_pec = (uint16)pec10_calc(true, (BYTES_IN_REG-2), &copyArray[0]);
      // calculated_pec = (uint16)pec10_calc(false, (BYTES_IN_REG), &copyArray[0]);
      // printf("spiReadData() received_pec=0x%x, calculated_pec=0x%x\n", received_pec, calculated_pec);
      //!< Match received pec with calculated pec
      if (received_pec == calculated_pec){ pec_error[current_ic] = 0; }//!< If no error is there value set to 0
      else{ pec_error[current_ic] = 1; }                               //!< If error is there value set to 1
    }
  }
  free(data);
  free(copyArray);
}
#endif

/**
*******************************************************************************
* Function: spiWriteData
* @brief write data over SPI, Lower Level function for adBmsWriteData
*
* @details This function write the data into bms ic
*
* Parameters:
* @param [in]	tIC      Total IC
*
* @param [in]  tx_cmd   Tx command bytes
*
* @param [in]  *data   Data pointer
*
* @return None
*
*******************************************************************************
*/
#if 0
void spiWriteData
(
uint8 tIC,
uint8 tx_cmd[2],
uint8 *data
)
{
  uint8 BYTES_IN_REG = TX_DATA;
  uint8 CMD_LEN = 4 + (RX_DATA * tIC);
  uint16 data_pec, cmd_pec;
  uint8 *cmd, copyArray[TX_DATA], src_address = 0;
  uint8 cmd_index;
  cmd = (uint8 *)calloc(CMD_LEN, sizeof(uint8));
  if(cmd == NULL)
  {
#ifdef MBED
    pc.printf(" Failed to allocate cmd array memory \n");
#else
    printf(" Failed to allocate cmd array memory \n");
#endif
    exit(0);
  }
  else
  {
    cmd[0] = tx_cmd[0];
    cmd[1] = tx_cmd[1];
    cmd_pec = Pec15_Calc(2, cmd);
    cmd[2] = (uint8)(cmd_pec >> 8);
    cmd[3] = (uint8)(cmd_pec);
    cmd_index = 4;
    /*!< executes for each LTC68xx, this loops starts with the last IC on the stack */
    for (uint8 current_ic = tIC; current_ic > 0; current_ic--)
    {
      src_address = ((current_ic-1) * TX_DATA);
      /*!< The first configuration written is received by the last IC in the daisy chain */
      for (uint8 current_byte = 0; current_byte < BYTES_IN_REG; current_byte++)
      {
        cmd[cmd_index] = data[((current_ic-1)*6)+current_byte];
        cmd_index = cmd_index + 1;
      }
      /*!< Copy each ic correspond data + pec value for calculate data pec */
      memcpy(&copyArray[0], &data[src_address], TX_DATA); /*!< dst, src, size */
      /*!< calculating the PEC for each Ics configuration register data */
      data_pec = (uint16)pec10_calc_modular(&copyArray[0], PEC10_WRITE);
      // data_pec = (uint16)pec10_calc(true,BYTES_IN_REG, &copyArray[0]);
      cmd[cmd_index] = (uint8)(data_pec >> 8);
      cmd_index = cmd_index + 1;
      cmd[cmd_index] = (uint8)data_pec;
      cmd_index = cmd_index + 1;
    }
    adBmsCsLow();
    spiWriteBytes(&cmd[0], CMD_LEN);
    adBmsCsHigh();
  }
  free(cmd);
}
#endif
/**
*******************************************************************************
* Function: adBmsReadData
* @brief Adbms Read Data From Bms ic.
*
* @details This function sends the command read payload, parse data put it in structure and check pec error.(Ex: RDCFGA)
*
* Parameters:
* @param [in]	tIC      Total IC
*
* @param [in]  *ic      cell_asic stucture pointer
*
* @param [in]  cmd_arg   command bytes
*
* @param [in]  TYPE   Enum type of resistor
*
* @param [in]  GRP   Enum type of resistor group
*
* @return None
*
*******************************************************************************
*/
#if 0
void adBmsReadData(uint8 tIC, cell_asic *ic, uint8 cmd_arg[2], TYPE type, GRP group)
{
  uint16 rBuff_size;
  uint8 regData_size;
  if(group == ALL_GRP)
  {
    if(type == Rdalli){rBuff_size = RDALLI_SIZE; regData_size = RDALLI_SIZE;}
    else if(type == Rdalla){rBuff_size = RDALLA_SIZE; regData_size = RDALLA_SIZE;}
    else if(type == Rdallc){rBuff_size = RDALLC_SIZE; regData_size = RDALLC_SIZE;}
    else if(type == Rdallv){rBuff_size = RDALLV_SIZE; regData_size = RDALLV_SIZE;}
    else if(type == Rdallr){rBuff_size = RDALLR_SIZE; regData_size = RDALLR_SIZE;}
    else if(type == Rdallx){rBuff_size = RDALLX_SIZE; regData_size = RDALLX_SIZE;}
    else
    {
      printMsg("Read All cmd wrong type select");
    }
  }
  else{rBuff_size = (tIC * RX_DATA); regData_size = RX_DATA;}
  uint8 *read_buffer, *pec_error, *cmd_count;
  read_buffer = (uint8 *)calloc(rBuff_size, sizeof(uint8));
  pec_error = (uint8 *)calloc(tIC, sizeof(uint8));
  cmd_count = (uint8 *)calloc(tIC, sizeof(uint8));
  if((pec_error == NULL) || (cmd_count == NULL) || (read_buffer == NULL))
  {
    printMsg("Failed to allocate memory");
    exit(0);
  }
  else
  {
    spiReadData(tIC, &cmd_arg[0], &read_buffer[0], &pec_error[0], &cmd_count[0], regData_size);
    switch (type)
    {
    case Config:
      adBms2950ParseConfig(tIC, ic, group, &read_buffer[0]);
      for (uint8 cic = 0; cic < tIC; cic++)
      {
        ic[cic].cccrc.cfgr_pec = pec_error[cic];
        ic[cic].cccrc.cmd_cntr = cmd_count[cic];
        ic[cic].cccrc.cc_error  = adBms2950_CCErrorCheck(ic[cic].cccrc.cmd_cntr, ic[cic].cccrc.cal_cmd_cntr);
      }
      break;

    case GPV1:
      adBms2950V1ParseData(tIC, ic, group, &read_buffer[0]);
      for (uint8 cic = 0; cic < tIC; cic++)
      {
        ic[cic].cccrc.vr_pec = pec_error[cic];
        ic[cic].cccrc.cmd_cntr = cmd_count[cic];
        ic[cic].cccrc.cc_error  = adBms2950_CCErrorCheck(ic[cic].cccrc.cmd_cntr, ic[cic].cccrc.cal_cmd_cntr);
      }
      break;

    case GPV2:
      adBms2950V2ParseData(tIC, ic, group, &read_buffer[0]);
      for (uint8 cic = 0; cic < tIC; cic++)
      {
        ic[cic].cccrc.rvr_pec = pec_error[cic];
        ic[cic].cccrc.cmd_cntr = cmd_count[cic];
        ic[cic].cccrc.cc_error  = adBms2950_CCErrorCheck(ic[cic].cccrc.cmd_cntr, ic[cic].cccrc.cal_cmd_cntr);
      }
      break;

    case Cr:
      adBms2950ParseCurrentRegData(tIC, ic, &read_buffer[0]);
      for (uint8 cic = 0; cic < tIC; cic++)
      {
        ic[cic].cccrc.cr_pec = pec_error[cic];
        ic[cic].cccrc.cmd_cntr = cmd_count[cic];
        ic[cic].cccrc.cc_error  = adBms2950_CCErrorCheck(ic[cic].cccrc.cmd_cntr, ic[cic].cccrc.cal_cmd_cntr);
      }
      break;

    case Vbat:
      adBms2950ParseVbatRegData(tIC, ic, &read_buffer[0]);
      for (uint8 cic = 0; cic < tIC; cic++)
      {
        ic[cic].cccrc.vbat_pec = pec_error[cic];
        ic[cic].cccrc.cmd_cntr = cmd_count[cic];
        ic[cic].cccrc.cc_error  = adBms2950_CCErrorCheck(ic[cic].cccrc.cmd_cntr, ic[cic].cccrc.cal_cmd_cntr);
      }
      break;

    case Ivbat:
      adBms2950ParseIVbatRegData(tIC, ic, &read_buffer[0]);
      for (uint8 cic = 0; cic < tIC; cic++)
      {
        ic[cic].cccrc.ivbat_pec = pec_error[cic];
        ic[cic].cccrc.cmd_cntr = cmd_count[cic];
        ic[cic].cccrc.cc_error  = adBms2950_CCErrorCheck(ic[cic].cccrc.cmd_cntr, ic[cic].cccrc.cal_cmd_cntr);
      }
      break;

    case Oc:
      adBms2950ParseOcRegData(tIC, ic, &read_buffer[0]);
      for (uint8 cic = 0; cic < tIC; cic++)
      {
        ic[cic].cccrc.oc_pec = pec_error[cic];
        ic[cic].cccrc.cmd_cntr = cmd_count[cic];
        ic[cic].cccrc.cc_error  = adBms2950_CCErrorCheck(ic[cic].cccrc.cmd_cntr, ic[cic].cccrc.cal_cmd_cntr);
      }
      break;

    case AccCr:
      adBms2950ParseAccCurrentRegData(tIC, ic, &read_buffer[0]);
      for (uint8 cic = 0; cic < tIC; cic++)
      {
        ic[cic].cccrc.avgcr_pec = pec_error[cic];
        ic[cic].cccrc.cmd_cntr = cmd_count[cic];
        ic[cic].cccrc.cc_error  = adBms2950_CCErrorCheck(ic[cic].cccrc.cmd_cntr, ic[cic].cccrc.cal_cmd_cntr);
      }
      break;

    case AccVbat:
      adBms2950ParseAccVbatRegData(tIC, ic, &read_buffer[0]);
      for (uint8 cic = 0; cic < tIC; cic++)
      {
        ic[cic].cccrc.avgvbat_pec = pec_error[cic];
        ic[cic].cccrc.cmd_cntr = cmd_count[cic];
        ic[cic].cccrc.cc_error  = adBms2950_CCErrorCheck(ic[cic].cccrc.cmd_cntr, ic[cic].cccrc.cal_cmd_cntr);
      }
      break;

    case AccIvbat:
      adBms2950ParseAccIVbatRegData(tIC, ic, &read_buffer[0]);
      for (uint8 cic = 0; cic < tIC; cic++)
      {
        ic[cic].cccrc.avgivbat_pec = pec_error[cic];
        ic[cic].cccrc.cmd_cntr = cmd_count[cic];
        ic[cic].cccrc.cc_error  = adBms2950_CCErrorCheck(ic[cic].cccrc.cmd_cntr, ic[cic].cccrc.cal_cmd_cntr);
      }
      break;

    case Aux:
      adBms2950ParseAux(tIC, ic, group, &read_buffer[0]);
      for (uint8 cic = 0; cic < tIC; cic++)
      {
        ic[cic].cccrc.aux_pec = pec_error[cic];
        ic[cic].cccrc.cmd_cntr = cmd_count[cic];
        ic[cic].cccrc.cc_error  = adBms2950_CCErrorCheck(ic[cic].cccrc.cmd_cntr, ic[cic].cccrc.cal_cmd_cntr);
      }
      break;

    case Flag:
      adBms2950ParseFlag(tIC, ic, &read_buffer[0]);
      for (uint8 cic = 0; cic < tIC; cic++)
      {
        ic[cic].cccrc.flag_pec = pec_error[cic];
        ic[cic].cccrc.cmd_cntr = cmd_count[cic];
        ic[cic].cccrc.cc_error  = adBms2950_CCErrorCheck(ic[cic].cccrc.cmd_cntr, ic[cic].cccrc.cal_cmd_cntr);
      }
      break;

    case Status:
      adBms2950ParseStatus(tIC, ic, group, &read_buffer[0]);
      for (uint8 cic = 0; cic < tIC; cic++)
      {
        ic[cic].cccrc.stat_pec = pec_error[cic];
        ic[cic].cccrc.cmd_cntr = cmd_count[cic];
        ic[cic].cccrc.cc_error  = adBms2950_CCErrorCheck(ic[cic].cccrc.cmd_cntr, ic[cic].cccrc.cal_cmd_cntr);
      }
      break;

    case Comm:
      adBms2950ParseComm(tIC, ic, &read_buffer[0]);
      for (uint8 cic = 0; cic < tIC; cic++)
      {
        ic[cic].cccrc.comm_pec = pec_error[cic];
        ic[cic].cccrc.cmd_cntr = cmd_count[cic];
        ic[cic].cccrc.cc_error  = adBms2950_CCErrorCheck(ic[cic].cccrc.cmd_cntr, ic[cic].cccrc.cal_cmd_cntr);
      }
      break;

    case SID:
      adBms2950ParseSID(tIC, ic, &read_buffer[0]);
      for (uint8 cic = 0; cic < tIC; cic++)
      {
        ic[cic].cccrc.sid_pec = pec_error[cic];
        ic[cic].cccrc.cmd_cntr = cmd_count[cic];
        ic[cic].cccrc.cc_error  = adBms2950_CCErrorCheck(ic[cic].cccrc.cmd_cntr, ic[cic].cccrc.cal_cmd_cntr);
      }
      break;

    case Rdalli:
      adBms2950ParseRdalli(tIC, ic, &read_buffer[0]);
      for (uint8 cic = 0; cic < tIC; cic++)
      {
        ic[cic].cccrc.rdalli_pec = pec_error[cic];
        ic[cic].cccrc.cmd_cntr = cmd_count[cic];
        ic[cic].cccrc.cc_error  = adBms2950_CCErrorCheck(ic[cic].cccrc.cmd_cntr, ic[cic].cccrc.cal_cmd_cntr);
      }
      break;

    case Rdalla:
      adBms2950ParseRdalla(tIC, ic, &read_buffer[0]);
      for (uint8 cic = 0; cic < tIC; cic++)
      {
        ic[cic].cccrc.rdalla_pec = pec_error[cic];
        ic[cic].cccrc.cmd_cntr = cmd_count[cic];
        ic[cic].cccrc.cc_error  = adBms2950_CCErrorCheck(ic[cic].cccrc.cmd_cntr, ic[cic].cccrc.cal_cmd_cntr);
      }
      break;

    case Rdallc:
      adBms2950ParseRdallc(tIC, ic, &read_buffer[0]);
      for (uint8 cic = 0; cic < tIC; cic++)
      {
        ic[cic].cccrc.rdallc_pec = pec_error[cic];
        ic[cic].cccrc.cmd_cntr = cmd_count[cic];
        ic[cic].cccrc.cc_error  = adBms2950_CCErrorCheck(ic[cic].cccrc.cmd_cntr, ic[cic].cccrc.cal_cmd_cntr);
      }
      break;

    case Rdallv:
      adBms2950ParseRdallv(tIC, ic, &read_buffer[0]);
      for (uint8 cic = 0; cic < tIC; cic++)
      {
        ic[cic].cccrc.rdallv_pec = pec_error[cic];
        ic[cic].cccrc.cmd_cntr = cmd_count[cic];
        ic[cic].cccrc.cc_error  = adBms2950_CCErrorCheck(ic[cic].cccrc.cmd_cntr, ic[cic].cccrc.cal_cmd_cntr);
      }
      break;

  case Rdallr:
      adBms2950ParseRdallr(tIC, ic, &read_buffer[0]);
      for (uint8 cic = 0; cic < tIC; cic++)
      {
        ic[cic].cccrc.rdallr_pec = pec_error[cic];
        ic[cic].cccrc.cmd_cntr = cmd_count[cic];
        ic[cic].cccrc.cc_error  = adBms2950_CCErrorCheck(ic[cic].cccrc.cmd_cntr, ic[cic].cccrc.cal_cmd_cntr);
      }
      break;

    case Rdallx:
      adBms2950ParseRdallx(tIC, ic, &read_buffer[0]);
      for (uint8 cic = 0; cic < tIC; cic++)
      {
        ic[cic].cccrc.rdallr_pec = pec_error[cic];
        ic[cic].cccrc.cmd_cntr = cmd_count[cic];
        ic[cic].cccrc.cc_error  = adBms2950_CCErrorCheck(ic[cic].cccrc.cmd_cntr, ic[cic].cccrc.cal_cmd_cntr);
      }
      break;

    default:
      break;
    }
  }
  free(read_buffer);
  free(pec_error);
  free(cmd_count);
}
#endif
/**
*******************************************************************************
* Function: adBmsWriteData
* @brief Adbms Write Data into Bms ic.
*
* @details This function write the data to DUT. (EX: WRCFGA)
*
* Parameters:
* @param [in]	tIC      Total IC
*
* @param [in]  *ic      cell_asic stucture pointer
*
* @param [in]  cmd_arg   command bytes
*
* @param [in]  TYPE   Enum type of resistor
*
* @param [in]  GRP   Enum type of resistor group
*
* @return None
*
*******************************************************************************
*/
#if 0
void adBmsWriteData(uint8 tIC, cell_asic *ic, uint8 cmd_arg[2], TYPE type, GRP group)
{
  uint8 data_len = TX_DATA, write_size = (TX_DATA * tIC);
  uint8 *write_buffer = (uint8 *)calloc(write_size, sizeof(uint8));
  if(write_buffer == NULL)
  {
    printMsg("Failed to allocate write_buffer array memory");
    exit(0);
  }
  else
  {
    switch (type)
    {
    case Config:
      switch (group)
      {
      case A:
        adBms2950CreateConfiga(tIC, &ic[0]);
        for (uint8 cic = 0; cic < tIC; cic++)
        {
          if ( ic[cic].cccrc.cal_cmd_cntr  == 63)
          {
            ic[cic].cccrc.cal_cmd_cntr  = 0;
          }
          ++ ic[cic].cccrc.cal_cmd_cntr ;
          for (uint8 data = 0; data < data_len; data++)
          {
            write_buffer[(cic * data_len) + data] = ic[cic].configa.tx_data[data];
          }
        }
        break;
      case B:
        adBms2950CreateConfigb(tIC, &ic[0]);
        for (uint8 cic = 0; cic < tIC; cic++)
        {
          if ( ic[cic].cccrc.cal_cmd_cntr  == 63)
          {
            ic[cic].cccrc.cal_cmd_cntr  = 0;
          }
          ++ ic[cic].cccrc.cal_cmd_cntr ;
          for (uint8 data = 0; data < data_len; data++)
          {
            write_buffer[(cic * data_len) + data] = ic[cic].configb.tx_data[data];
          }
        }
        break;
      }
      break;

    case Comm:
      adBms2950CreateComm(tIC, &ic[0]);
      for (uint8 cic = 0; cic < tIC; cic++)
      {
        if ( ic[cic].cccrc.cal_cmd_cntr  == 63)
        {
          ic[cic].cccrc.cal_cmd_cntr  = 0;
        }
        ++ ic[cic].cccrc.cal_cmd_cntr ;
        for (uint8 data = 0; data < data_len; data++)
        {
          write_buffer[(cic * data_len) + data] = ic[cic].com.tx_data[data];
        }
      }
      break;

    case Clrflag:
      //adBms2950CreateClrflagData(tIC, &ic[0]);
      ic[0].clrflag.tx_data[0] = 0xFF;
      ic[0].clrflag.tx_data[1] = 0xFF;
      ic[0].clrflag.tx_data[2] = 0xFF;
      ic[0].clrflag.tx_data[3] = 0xFF;
      ic[0].clrflag.tx_data[4] = 0xFF;
      ic[0].clrflag.tx_data[5] = 0xFF;
      for (uint8 cic = 0; cic < tIC; cic++)
      {
        if ( ic[cic].cccrc.cal_cmd_cntr  == 63)
        {
          ic[cic].cccrc.cal_cmd_cntr  = 0;
        }
        ++ ic[cic].cccrc.cal_cmd_cntr ;
        for (uint8 data = 0; data < data_len; data++)
        {
          write_buffer[(cic * data_len) + data] = ic[cic].clrflag.tx_data[data];
        }
      }
      break;
    case Clrflag_OC:
      //adBms2950CreateClrflagData(tIC, &ic[0]);
      ic[0].clrflag.tx_data[0] = 0x0B;
      ic[0].clrflag.tx_data[1] = 0x0B;
      ic[0].clrflag.tx_data[2] = 0x00;
      ic[0].clrflag.tx_data[3] = 0x00;
      ic[0].clrflag.tx_data[4] = 0x00;
      ic[0].clrflag.tx_data[5] = 0x00;
      for (uint8 cic = 0; cic < tIC; cic++)
      {
        if ( ic[cic].cccrc.cal_cmd_cntr  == 63)
        {
          ic[cic].cccrc.cal_cmd_cntr  = 0;
        }
        ++ ic[cic].cccrc.cal_cmd_cntr ;
        for (uint8 data = 0; data < data_len; data++)
        {
          write_buffer[(cic * data_len) + data] = ic[cic].clrflag.tx_data[data];
        }
      }
      break;

    default:
      printf("I DON'T KNOW THIS TYPE");
      break;
    }
  }
#ifdef ISOSPI_WAKEUP
  adBmsWakeupIc(tIC);
#endif
  spiWriteData(tIC, cmd_arg, &write_buffer[0]);
  free(write_buffer);
}
#endif
/**
*******************************************************************************
* Function: adBmsPollAdc
* @brief PLADC Command.
*
* @details Send poll adc command and retun adc conversion count.
*
* Parameters:
*
* @param [in]  tIC      Total IC
*
* @param [in]  *ic      cell_asic structure pointer
*
* @param [in]  tx_cmd   Tx command byte
*
* @return Pladc_count(uint32_t)
*
*******************************************************************************
*/
#if 0
uint32_t adBmsPollAdc(uint8 tIC, cell_asic *ic, uint8 tx_cmd[2])
{
  volatile uint32_t pladc_count = 0;
  uint8 cmd[4];
  uint16 cmd_pec;
  uint8 read_data = 0x00;
  uint8 SDO_Line = 0xFF;
  cmd[0] = tx_cmd[0];
  cmd[1] = tx_cmd[1];
  cmd_pec = Pec15_Calc(2, cmd);
  cmd[2] = (uint8)(cmd_pec >> 8);
  cmd[3] = (uint8)(cmd_pec);
  adBms2950_IncCalCmdCounter(tIC, ic);
  adBmsCsLow();
  spiWriteBytes(&cmd[0], 4);
  startTimer();
  do{
    spiReadBytes(&read_data, 1);
  }while(!(read_data == SDO_Line));
  pladc_count = getTimCount();
  adBmsCsHigh();
  stopTimer();
  return(pladc_count);
}
#endif
/**
*******************************************************************************
* Function: adBms2950_CCErrorCheck
* @brief Command Counter Error Check
*
* @details Command counter error check.
*
* Parameters:
* @param [in]	ic_cc      Received command counter value
*
* @param [in]  cal_cc     Calculated command counter value
*
* @return cc_error(flag)
*
*******************************************************************************
*/
uint8 adBms2950_CCErrorCheck(uint8 ic_cc, uint8 cal_cc)
{
  uint8 cc_flag = 1;
   if(ic_cc  == cal_cc)
   {
     cc_flag = 0;
   }
   return cc_flag;
}

/** @}*/
/** @}*/
