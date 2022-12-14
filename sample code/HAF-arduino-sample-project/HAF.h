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

#ifndef __HAF_H
#define __HAF_H

#ifdef __cplusplus
extern "C" {
#endif

#define MAJOR_VERSION   1
#define MINOR_VERSION   1

typedef enum {
  HAF_CMD_GET_SN = 0x01,
  HAF_CMD_SOFT_RESET = 0x02,
  HAF_CMD_GET_FLOW = 0x04,
  HAF_CMD_GET_TEMP = 0x05,
  HAF_CMD_GET_FLOW_TEMP = 0x06,
  HAF_CMD_LOW_POWER = 0x08,
  HAF_CMD_GET_FLOW_MIN = 0xA0,
  HAF_CMD_GET_FLOW_MAX = 0xA1,
  HAF_CMD_GET_UNIT = 0xA2,
  HAF_CMD_GET_GAS_TYPE = 0xA7,
  HAF_CMD_GET_GAS_PROPORTION = 0xA8,
  HAF_CMD_SET_GAS_TYPE = 0xA9,
  HAF_CMD_SET_GAS_PROPORTION = 0xAA
} HafCmdCode;

typedef enum
{
  HAF_EC_OK = 0xFFFE,
  HAF_EC_BAD_CMD = 0xFFFD,
  HAF_EC_BAD_PARAM = 0xFFFC,
  HAF_EC_BAD_CHECKSUM = 0xFFFA
} HafErrorCode;

typedef enum {
  HAF_GAS_TYPE_AIR = 0x00,
  HAF_GAS_TYPE_O2 = 0x01,
  HAF_GAS_TYPE_N2O = 0x02,
  HAF_GAS_TYPE_O2_AIR = 0x0A,
  HAF_GAS_TYPE_N2O_AIR = 0x0B,
  HAF_GAS_TYPE_O2_N2O = 0x0C
} HafGasType;

typedef enum {
  HAF_UNIT_SCCM = 0x10,
  HAF_UNIT_SLM = 0x11
} HafFlowUnit;

typedef struct {
  byte i2cAddress;
  byte sn[4];
  word flow;
  short temp;
  short flowMin;
  short flowMax;
  HafFlowUnit flowUnit;
  HafGasType gasType;
  word gasProportion;
} HafSensorConfig;

int HAF_GetSn(byte address, byte *sn);
int HAF_SoftReset(byte address);
int HAF_GetFlow(byte address, word *v);
int HAF_GetTemp(byte address, short *v);
int HAF_GetFlowTemp(byte address, word *f, short *t);
int HAF_LowPower(byte address);
int HAF_GetFlowMin(byte address, short *v);
int HAF_GetFlowMax(byte address, short *v);
int HAF_GetUnit(byte address, HafFlowUnit *v);
int HAF_GetGasType(byte address, HafGasType *v);
int HAF_GetGasProportion(byte address, word *v);
int HAF_SetGasType(byte address, HafGasType v);
int HAF_SetGasProportion(byte address, word v);
#ifdef __cplusplus
}  // extern "C"
#endif

#endif /* __PORT_H */
