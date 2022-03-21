/*
   Copyright (c) 2021-2022, Honeywell Air Flow Team

   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions are met:

 * * Redistributions of source code must retain the above copyright notice, this
     list of conditions and the following disclaimer.

 * * Redistributions in binary form must reproduce the above copyright notice,
     this list of conditions and the following disclaimer in the documentation
     and/or other materials provided with the distribution.

 * * Neither the name of Honeywell Air Flow Team nor the names of its
     contributors may be used to endorse or promote products derived from
     this software without specific prior written permission.

   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
   AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
   IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
   ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
   LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
   CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
   SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
   INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
   CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
   ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
   POSSIBILITY OF SUCH DAMAGE.

   Change Logs:
   Date           Author       Notes
   2022-03-20     fengbo      v1.00, compatible with firmware version v1.05 or before

*/
#include <string.h>
#include "stm32f1xx_hal.h"
#include "HAF.h"
#include "Port.h"

uint16_t HAF_Bytes2uint16(const uint8_t* bytes)
{
  return (uint16_t)bytes[0] << 8 | (uint16_t)bytes[1];
}

long HAF_Bytes2uint32(const uint8_t* bytes)
{
  return (long)bytes[0] << 24 | (long)bytes[1] << 16 |
         (long)bytes[2] << 8 | (long)bytes[3];
}

float HAF_Bytes2float(const uint8_t* bytes)
{
  union {
    uint32_t u32_value;
    float float32;
  } tmp;

  tmp.u32_value = HAF_Bytes2uint32(bytes);
  return tmp.float32;
}

uint16_t HAF_U16CheckSum(uint8_t *dst, uint16_t size)
{
  uint16_t sum = 0;
  while ( size-- )
  {
    sum += *dst++;
  }
  return sum;
}
int HAF_GetSn(uint8_t address, uint8_t *sn)
{
  HafCmdCode cmd = HAF_CMD_GET_SN;
  uint8_t res[6];
  Port_I2cWrite(address, (uint8_t *)&cmd, 1);
  Port_I2cRead(address, res, 6);
  if (HAF_U16CheckSum(res, 4) != HAF_Bytes2uint16(&res[4]))return -1;
  memcpy(sn, res, 4);
  return 0;
}

int HAF_SoftReset(uint8_t address)
{
  HafCmdCode cmd = HAF_CMD_SOFT_RESET;
  Port_I2cWrite(address, (uint8_t *)&cmd, 1);
  return 0;
}

int HAF_GetTemp(uint8_t address, int16_t *v)
{
  HafCmdCode cmd = HAF_CMD_GET_TEMP;
  uint8_t res[4];
  Port_I2cWrite(address, (uint8_t *)&cmd, 1);
  Port_I2cRead(address, res, 4);
  if (HAF_U16CheckSum(res, 2) != HAF_Bytes2uint16(&res[2]))return -1;
  *v = (int16_t)HAF_Bytes2uint16(res);
  return 0;
}

int HAF_LowPower(uint8_t address)
{
  HafCmdCode cmd = HAF_CMD_LOW_POWER;
  Port_I2cWrite(address, (uint8_t *)&cmd, 1);
  return 0;
}

int HAF_GetFactor(uint8_t address, uint16_t *v)
{
  HafCmdCode cmd = HAF_CMD_GET_FACTOR;
  uint8_t res[4];
  Port_I2cWrite(address, (uint8_t *)&cmd, 1);
  Port_I2cRead(address, res, 4);
  if (HAF_U16CheckSum(res, 2) != HAF_Bytes2uint16(&res[2]))return -1;
  *v = HAF_Bytes2uint16(res);
  return 0;
}

int HAF_GetUnit(uint8_t address, HafFlowUnit *v)
{
  HafCmdCode cmd = HAF_CMD_GET_UNIT;
  uint8_t res[4];
  Port_I2cWrite(address, (uint8_t *)&cmd, 1);
  Port_I2cRead(address, res, 4);
  if (HAF_U16CheckSum(res, 2) != HAF_Bytes2uint16(&res[2]))return -1;
  *v = (HafFlowUnit)HAF_Bytes2uint16(res);
  return 0;
}

int HAF_GetFlow(uint8_t address, int16_t *v)
{
  HafCmdCode cmd = HAF_CMD_GET_FLOW;
  uint8_t res[4];
  Port_I2cWrite(address, (uint8_t *)&cmd, 1);
  Port_I2cRead(address, res, 4);
  if (HAF_U16CheckSum(res, 2) != HAF_Bytes2uint16(&res[2]))return -1;
  *v = (int16_t)HAF_Bytes2uint16(res);
  return 0;
}

int HAF_GetFlowTemp(uint8_t address, int16_t *f, int16_t *t)
{
  HafCmdCode cmd = HAF_CMD_GET_FLOW_TEMP;
  uint8_t res[6];
  Port_I2cWrite(address, (uint8_t *)&cmd, 1);
  Port_I2cRead(address, res, 6);
  if (HAF_U16CheckSum(res, 4) != HAF_Bytes2uint16(&res[4]))return -1;
  *f = (int16_t)HAF_Bytes2uint16(res);
  *t = (int16_t)HAF_Bytes2uint16(&res[2]);
  return 0;
}

int HAF_GetGasType(uint8_t address, HafGasType *v)
{
  HafCmdCode cmd = HAF_CMD_GET_GAS_TYPE;
  uint8_t res[4];
  Port_I2cWrite(address, (uint8_t *)&cmd, 1);
  Port_I2cRead(address, res, 4);
  if (HAF_U16CheckSum(res, 2) != HAF_Bytes2uint16(&res[2]))return -1;
  *v = (HafGasType)HAF_Bytes2uint16(res);
  return 0;
}

int HAF_GetGasProportion(uint8_t address, uint16_t *v)
{
  HafCmdCode cmd = HAF_CMD_GET_GAS_PROPORTION;
  uint8_t res[4];
  Port_I2cWrite(address, (uint8_t *)&cmd, 1);
  Port_I2cRead(address, res, 4);
  if (HAF_U16CheckSum(res, 2) != HAF_Bytes2uint16(&res[2]))return -1;
  *v = HAF_Bytes2uint16(res);
  return 0;
}

int HAF_SetGasType(uint8_t address, HafGasType v)
{
  uint8_t cmd[5];
  uint8_t res[2];
  uint16_t sum;
  cmd[0] = (uint8_t)HAF_CMD_SET_GAS_TYPE;
  cmd[1] = 0;
  switch (v)
  {
    case HAF_GAS_TYPE_AIR:
      cmd[2] = (uint8_t)HAF_GAS_TYPE_AIR;
      break;
    case HAF_GAS_TYPE_O2:
      cmd[2] = (uint8_t)HAF_GAS_TYPE_O2;
      break;
    case HAF_GAS_TYPE_N2O:
      cmd[2] = (uint8_t)HAF_GAS_TYPE_N2O;
      break;
    case HAF_GAS_TYPE_O2_AIR:
      cmd[2] = (uint8_t)HAF_GAS_TYPE_O2_AIR;
      break;
    case HAF_GAS_TYPE_N2O_AIR:
      cmd[2] = (uint8_t)HAF_GAS_TYPE_N2O_AIR;
      break;
    case HAF_GAS_TYPE_O2_N2O:
      cmd[2] = (uint8_t)HAF_GAS_TYPE_O2_N2O;
      break;
    default:
      return -1;
  }
  sum = cmd[1] + cmd[2];
  cmd[3] = (uint8_t)((sum >> 8) & 0xFF);
  cmd[4] = (uint8_t)(sum & 0xFF);
  Port_I2cWrite(address, (uint8_t *)&cmd, 5);
  Port_I2cRead(address, res, 2);
  if (HAF_Bytes2uint16(res) != (uint16_t)HAF_EC_OK)return -1;
  return 0;
}

int HAF_SetGasProportion(uint8_t address, uint16_t v)
{
  uint8_t cmd[5];
  uint8_t res[2];
  uint16_t sum;
  cmd[0] = (uint8_t)HAF_CMD_SET_GAS_PROPORTION;
  cmd[1] = (uint8_t)((v >> 8) & 0xFF);
  cmd[2] = (uint8_t)(v & 0xFF);
  sum = cmd[1] + cmd[2];
  cmd[3] = (uint8_t)((sum >> 8) & 0xFF);
  cmd[4] = (uint8_t)(sum & 0xFF);
  Port_I2cWrite(address, (uint8_t *)&cmd, 5);
  Port_I2cRead(address, res, 2);
  if (HAF_Bytes2uint16(res) != (uint16_t)HAF_EC_OK)return -1;
  return 0;
}
