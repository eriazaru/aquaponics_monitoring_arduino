// Library import for I2C LCD
#include <LiquidCrystal_I2C.h>
#include <Wire.h>




// Library import for DS18B20 Temperature Sensor
#include <OneWire.h>
#include <DallasTemperature.h>

// Library import for TDS Sensor
#include <CQRobotTDS.h>

// Library import for PH Sensor
#include <ph4502c_sensor.h>


// definition for DS18B20 Temperature Sensor
#define ONE_WIRE_TEMP 13

// definition for TDS Sensor
#define ANALOG_TDS A0

// definition for PH Sensor
#define PH4502C_PH_LEVEL_PIN A1
#define placeholder 45
#define PH4502C_CALIBRATION 6.80f



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



float calculateNH3Fraction(float temperatureC, float pH) {
  const float emp_constant1 = 0.0901821;
  const float emp_constant2 = 2729.92;
  const float kelvin_offset = 273.15;
  
  // Convert temperature to Kelvin
  float temperatureK = kelvin_offset + temperatureC;
  
  // Calculate the temperature term
  float temperatureTerm = emp_constant2 / temperatureK;
  
  // Calculate the exponent term
  float exponentTerm = emp_constant1 + temperatureTerm - pH;
  
  // Calculate 10^exponentTerm
  float powerTerm = pow(10, exponentTerm);
  
  // Calculate the fraction of NH3
  float nh3Fraction = 1 / (powerTerm + 1);
  
  return nh3Fraction;
}

void setup() {
  Serial.begin(9600);
  // Temperature Sensor Library Start up
  temp.begin();

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
  float temperatureC = temp.getTempCByIndex(0);
  Serial.println("C Temp: " + String(temperatureC));
  Serial.println("F Temp: " + String(temp.getTempFByIndex(0)));

  // TDS Sensor
  tds_value = tds.update(temperatureC);
  Serial.println("TDS: " + String(tds_value) + " ppm");

  // PH Sensor
  float pH = ph4502.read_ph_level();
  Serial.println("pH: " + String(pH));

  // Calculate NH3 Fraction
  float nh3Fraction = calculateNH3Fraction(temperatureC, pH);
  Serial.println("NH3 Fraction: " + String(nh3Fraction, 6));

  lcd.setCursor(0, 0);
  lcd.print("T  TDS  pH  NH3");

  // Display values on LCD
  lcd.setCursor(0, 1);
  lcd.print(String(int(temperatureC)) + " " + String(tds_value) + "  " + String(pH) + " " + String(nh3Fraction, 6));
  
  delay(5000);
}
