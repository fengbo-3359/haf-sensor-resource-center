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

*/
#include <Arduino.h>
#include <Wire.h>

#ifdef __cplusplus
extern "C" {
#endif

void Port_I2cInit(void)
{
  Wire.begin();
}

int Port_I2cRead(byte address, byte *data, word count)
{
  byte readData[count];
  word rxByteCount = 0;

  Wire.requestFrom(address, count);
  while (Wire.available())
  { // wait till all arrive
    readData[rxByteCount++] = Wire.read();
    if (rxByteCount >= count)
      break;
  }
  memcpy(data, readData, count);
  return 0;
}

int Port_I2cWrite(byte address, const byte *data, word count)
{
  Wire.beginTransmission(address);
  Wire.write(data, count);
  Wire.endTransmission();
  return 0;
}
#ifdef __cplusplus
}  // extern "C"
#endif
