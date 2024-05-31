// Library import for I2C LCD
#include <LiquidCrystal_I2C.h>
#include <Wire.h>

// Library import for Bluetooth Serial
#include <SoftwareSerial.h>

// Library import for DS18B20 Temperature Sensor
#include <OneWire.h>
#include <DallasTemperature.h>

// Library import for TDS Sensor
#include <CQRobotTDS.h>

// Library import for PH Sensor
#include <ph4502c_sensor.h>

// Library import for MQ137
#include <MQ137.h>

// definition for Bluetooth Sensor Module
#define BT_TX 6
#define BT_RX 7

// definition for DS18B20 Temperature Sensor
#define ONE_WIRE_TEMP 13

// definition for TDS Sensor
#define ANALOG_TDS A0

// definition for PH Sensor
#define PH4502C_PH_LEVEL_PIN A1
#define placeholder 45
#define PH4502C_CALIBRATION 14.8f

// definition for MQ137 Ammonia Sensor
#define MQ137_ANALOG_PIN A2
// calibration variable for MQ137 Ammonia Sensor
#define MQ137_R0 26.21f

// Instance and Declaration for I2C LCD
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Instance and Declaration for Temperature Sensor
OneWire Tsensor(ONE_WIRE_TEMP);
DallasTemperature temp(&Tsensor);

// Instance and Declaration for TDS Sensor
int tds_value = 0;
CQRobotTDS tds(ANALOG_TDS);

// Instance and Declaration for PH Sensor
PH4502C_Sensor ph4502(PH4502C_PH_LEVEL_PIN, placeholder);

// Instance and Declaration for MQ137 Ammonia Sensor
MQ137 mq137(MQ137_ANALOG_PIN, MQ137_R0, true);

void setup() {
  Serial.begin(9600);
  // Temperature Sensor Library Start up
  temp.begin();
  // Ammonia Sensor Library Start up
  mq137.begin();

  // PH Sensor Initialization
  ph4502.init();
  ph4502.recalibrate(PH4502C_CALIBRATION);

  // LCD i2C Initialization
  lcd.init();
  lcd.backlight();
  

}

void loop() {
  // Temperature Sensor
  temp.requestTemperatures();
  Serial.println("C Temp: " + String(temp.getTempCByIndex(0)));
  Serial.println("F Temp: " + String(temp.getTempFByIndex(0)));

  // TDS Sensor Serial Print
  tds_value = tds.update(temp.getTempCByIndex(0));
  Serial.println("TDS: " + String(tds_value) + " ppm");

  // PH Sensor Serial Print
  Serial.println("pH: " + String(ph4502.read_ph_level()));

  lcd.setCursor(0,0);
  lcd.print("T  TDS  pH  NH3");

  lcd.setCursor(0, 1);
  lcd.print(String(int(temp.getTempCByIndex(0))) + " " + String(tds_value) + "  " + String(ph4502.read_ph_level()) + " " + mq137.getPPM());
  delay(5000);

}