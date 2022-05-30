#include "HAF.h"
#include "Port.h"

HafSensorConfig senssor = {0};
void setup()
{
  // put your setup code here, to run once:
  senssor.i2cAddress = 0x48;
  Port_I2cInit();
  Serial.begin(9600);
  Serial.print("The driver version is V");
  Serial.print(MAJOR_VERSION, DEC);
  Serial.print(".");
  Serial.print(MINOR_VERSION < 10 ? "0" : "");
  Serial.println(MINOR_VERSION, DEC);
  Serial.println("I2c init OK!");
  //suggest warm up time
  delay(50);

  /*********************get serial number**********************/
  while (HAF_GetSn(senssor.i2cAddress, senssor.sn) != 0)
  {
    Serial.println("Get sn error!");
    delay(1000);
  }
  Serial.print("SN: ");
  Serial.print(senssor.sn[0] < 16 ? "0" : "");
  Serial.print(senssor.sn[0], HEX);
  Serial.print(senssor.sn[1] < 16 ? "0" : "");
  Serial.print(senssor.sn[1], HEX);
  Serial.print(senssor.sn[2] < 16 ? "0" : "");
  Serial.print(senssor.sn[2], HEX);
  Serial.print(senssor.sn[3] < 16 ? "0" : "");
  Serial.println(senssor.sn[3], HEX);

  /********************get flow min and max********************/
  while (HAF_GetFlowMin(senssor.i2cAddress, &senssor.flowMin) != 0)
  {
    Serial.println("Get flowMin error!");
    delay(1000);
  }
  while (HAF_GetFlowMax(senssor.i2cAddress, &senssor.flowMax) != 0)
  {
    Serial.println("Get flowMax error!");
    delay(1000);
  }
  Serial.print("Flow min: ");
  Serial.println(senssor.flowMin, DEC);
  Serial.print("Flow max: ");
  Serial.println(senssor.flowMax, DEC);

  /**************************get unit**************************/
  if (HAF_GetUnit(senssor.i2cAddress, &senssor.flowUnit) != 0)
  {
    Serial.println("Get unit error!");
  }
  else
  {
    Serial.print("Unit: ");
    if (senssor.flowUnit == HAF_UNIT_SCCM)
      Serial.println("sccm");
    else if (senssor.flowUnit == HAF_UNIT_SLM)
      Serial.println("slm");
  }

  /**************************set gas type**********************/
  if (HAF_SetGasType(senssor.i2cAddress, HAF_GAS_TYPE_AIR) != 0)
  {
    Serial.println("Set gas type error!");
  }
  else
  {
    Serial.println("Set gas type successfully!");
  }

  /*********************set gas proportion**********************/
  if (HAF_SetGasProportion(senssor.i2cAddress, 100) != 0)
  {
    Serial.println("Set gas proportion error!");
  }
  else
  {
    Serial.println("Set gas proportion successfully!");
  }

  /*********************get gas type***************************/
  if (HAF_GetGasType(senssor.i2cAddress, &senssor.gasType) != 0)
  {
    Serial.println("Get gas type error!");
  }
  else
  {
    Serial.print("Gas type: ");
    if (senssor.gasType == HAF_GAS_TYPE_AIR)
      Serial.println("Air");
    else if (senssor.gasType == HAF_GAS_TYPE_O2)
      Serial.println("O2");
    else if (senssor.gasType == HAF_GAS_TYPE_N2O)
      Serial.println("N2O");
    else if (senssor.gasType == HAF_GAS_TYPE_O2_AIR)
      Serial.println("O2&Air");
    else if (senssor.gasType == HAF_GAS_TYPE_N2O_AIR)
      Serial.println("N2O&Air");
    else if (senssor.gasType == HAF_GAS_TYPE_O2_N2O)
      Serial.println("O2&N2O");
  }

  /*********************get gas proportion**********************/
  if (HAF_GetGasProportion(senssor.i2cAddress, &senssor.gasProportion) != 0)
  {
    Serial.println("Get gas proportion error!");
  }
  else
  {
    Serial.print("Gas proportion: ");
    Serial.print(senssor.gasProportion, DEC);
    Serial.println("%");
  }
}

void loop()
{
  // put your main code here, to run repeatedly:
  /*********************get flow and temp**********************/
  if (HAF_GetFlowTemp(senssor.i2cAddress, &senssor.flow, &senssor.temp) != 0)
  {
    Serial.println("Get flow and temp error!");
  }
  else
  {
    Serial.print("Flow: ");
    float flow = (senssor.flowMax - senssor.flowMin) * ((float)senssor.flow / 16384 - 0.1) / 0.8 + senssor.flowMin;
    Serial.print(flow, 3);
    if (senssor.flowUnit == HAF_UNIT_SCCM)
      Serial.print("sccm");
    else if (senssor.flowUnit == HAF_UNIT_SLM)
      Serial.print("slm");
    Serial.print("   Temp: ");
    Serial.print((float)senssor.temp / 100, 1);
    Serial.println("℃");
  }
  delay(1000);
}
