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
   2022-03-1     fengbo      v1.00, compatible with firmware version v1.05 or before
   2022-05-1     fengbo      v1.01, compatible with firmware version v1.06 or later

*/
#include <Arduino.h>
#include "HAF.h"
#include "Port.h"

word HAF_Bytes2uint16(const byte* bytes)
{
  return (word)bytes[0] << 8 | (word)bytes[1];
}

long HAF_Bytes2uint32(const byte* bytes)
{
  return (long)bytes[0] << 24 | (long)bytes[1] << 16 |
         (long)bytes[2] << 8 | (long)bytes[3];
}

float HAF_Bytes2float(const byte* bytes)
{
  union {
    long u32_value;
    float float32;
  } tmp;

  tmp.u32_value = HAF_Bytes2uint32(bytes);
  return tmp.float32;
}

word HAF_U16CheckSum(byte *dst, word size)
{
  word sum = 0;
  while ( size-- )
  {
    sum += *dst++;
  }
  return sum;
}
int HAF_GetSn(byte address, byte *sn)
{
  HafCmdCode cmd = HAF_CMD_GET_SN;
  byte res[6];
  Port_I2cWrite(address, (byte *)&cmd, 1);
  Port_I2cRead(address, res, 6);
  if (HAF_U16CheckSum(res, 4) != HAF_Bytes2uint16(&res[4]))return -1;
  memcpy(sn, res, 4);
  return 0;
}

int HAF_SoftReset(byte address)
{
  HafCmdCode cmd = HAF_CMD_SOFT_RESET;
  Port_I2cWrite(address, (byte *)&cmd, 1);
  return 0;
}

int HAF_GetFlow(byte address, word *v)
{
  HafCmdCode cmd = HAF_CMD_GET_FLOW;
  byte res[4];
  Port_I2cWrite(address, (byte *)&cmd, 1);
  Port_I2cRead(address, res, 4);
  if (HAF_U16CheckSum(res, 2) != HAF_Bytes2uint16(&res[2]))return -1;
  *v = (word)HAF_Bytes2uint16(res);
  return 0;
}

int HAF_GetTemp(byte address, short *v)
{
  HafCmdCode cmd = HAF_CMD_GET_TEMP;
  byte res[4];
  Port_I2cWrite(address, (byte *)&cmd, 1);
  Port_I2cRead(address, res, 4);
  if (HAF_U16CheckSum(res, 2) != HAF_Bytes2uint16(&res[2]))return -1;
  *v = (short)HAF_Bytes2uint16(res);
  return 0;
}

int HAF_GetFlowTemp(byte address, word *f, short *t)
{
  HafCmdCode cmd = HAF_CMD_GET_FLOW_TEMP;
  byte res[6];
  Port_I2cWrite(address, (byte *)&cmd, 1);
  Port_I2cRead(address, res, 6);
  if (HAF_U16CheckSum(res, 4) != HAF_Bytes2uint16(&res[4]))return -1;
  *f = (word)HAF_Bytes2uint16(res);
  *t = (short)HAF_Bytes2uint16(&res[2]);
  return 0;
}

int HAF_LowPower(byte address)
{
  HafCmdCode cmd = HAF_CMD_LOW_POWER;
  Port_I2cWrite(address, (byte *)&cmd, 1);
  return 0;
}

int HAF_GetFlowMin(byte address, short *v)
{
  HafCmdCode cmd = HAF_CMD_GET_FLOW_MIN;
  byte res[4];
  Port_I2cWrite(address, (byte *)&cmd, 1);
  Port_I2cRead(address, res, 4);
  if (HAF_U16CheckSum(res, 2) != HAF_Bytes2uint16(&res[2]))return -1;
  *v = (short)HAF_Bytes2uint16(res);
  return 0;
}

int HAF_GetFlowMax(byte address, short *v)
{
  HafCmdCode cmd = HAF_CMD_GET_FLOW_MAX;
  byte res[4];
  Port_I2cWrite(address, (byte *)&cmd, 1);
  Port_I2cRead(address, res, 4);
  if (HAF_U16CheckSum(res, 2) != HAF_Bytes2uint16(&res[2]))return -1;
  *v = (short)HAF_Bytes2uint16(res);
  return 0;
}

int HAF_GetUnit(byte address, HafFlowUnit *v)
{
  HafCmdCode cmd = HAF_CMD_GET_UNIT;
  byte res[4];
  Port_I2cWrite(address, (byte *)&cmd, 1);
  Port_I2cRead(address, res, 4);
  if (HAF_U16CheckSum(res, 2) != HAF_Bytes2uint16(&res[2]))return -1;
  *v = (HafFlowUnit)HAF_Bytes2uint16(res);
  return 0;
}

int HAF_GetGasType(byte address, HafGasType *v)
{
  HafCmdCode cmd = HAF_CMD_GET_GAS_TYPE;
  byte res[4];
  Port_I2cWrite(address, (byte *)&cmd, 1);
  Port_I2cRead(address, res, 4);
  if (HAF_U16CheckSum(res, 2) != HAF_Bytes2uint16(&res[2]))return -1;
  *v = (HafGasType)HAF_Bytes2uint16(res);
  return 0;
}

int HAF_GetGasProportion(byte address, word *v)
{
  HafCmdCode cmd = HAF_CMD_GET_GAS_PROPORTION;
  byte res[4];
  Port_I2cWrite(address, (byte *)&cmd, 1);
  Port_I2cRead(address, res, 4);
  if (HAF_U16CheckSum(res, 2) != HAF_Bytes2uint16(&res[2]))return -1;
  *v = HAF_Bytes2uint16(res);
  return 0;
}

int HAF_SetGasType(byte address, HafGasType v)
{
  byte cmd[5];
  byte res[2];
  word sum;
  cmd[0] = (byte)HAF_CMD_SET_GAS_TYPE;
  cmd[1] = 0;
  switch (v)
  {
    case HAF_GAS_TYPE_AIR:
      cmd[2] = (byte)HAF_GAS_TYPE_AIR;
      break;
    case HAF_GAS_TYPE_O2:
      cmd[2] = (byte)HAF_GAS_TYPE_O2;
      break;
    case HAF_GAS_TYPE_N2O:
      cmd[2] = (byte)HAF_GAS_TYPE_N2O;
      break;
    case HAF_GAS_TYPE_O2_AIR:
      cmd[2] = (byte)HAF_GAS_TYPE_O2_AIR;
      break;
    case HAF_GAS_TYPE_N2O_AIR:
      cmd[2] = (byte)HAF_GAS_TYPE_N2O_AIR;
      break;
    case HAF_GAS_TYPE_O2_N2O:
      cmd[2] = (byte)HAF_GAS_TYPE_O2_N2O;
      break;
    default:
      return -1;
  }
  sum = cmd[1] + cmd[2];
  cmd[3] = (byte)((sum >> 8) & 0xFF);
  cmd[4] = (byte)(sum & 0xFF);
  Port_I2cWrite(address, (byte *)&cmd, 5);
  Port_I2cRead(address, res, 2);
  if (HAF_Bytes2uint16(res) != (word)HAF_EC_OK)return -1;
  return 0;
}

int HAF_SetGasProportion(byte address, word v)
{
  byte cmd[5];
  byte res[2];
  word sum;
  cmd[0] = (byte)HAF_CMD_SET_GAS_PROPORTION;
  cmd[1] = (byte)((v >> 8) & 0xFF);
  cmd[2] = (byte)(v & 0xFF);
  sum = cmd[1] + cmd[2];
  cmd[3] = (byte)((sum >> 8) & 0xFF);
  cmd[4] = (byte)(sum & 0xFF);
  Port_I2cWrite(address, (byte *)&cmd, 5);
  Port_I2cRead(address, res, 2);
  if (HAF_Bytes2uint16(res) != (word)HAF_EC_OK)return -1;
  return 0;
}
