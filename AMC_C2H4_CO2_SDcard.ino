#include <SPI.h> 
#include <SD.h> 
#include <Adafruit_AHTX0.h>
#include <Wire.h>
#define analogPin A0

Adafruit_AHTX0 aht;
//const int pwmPin = 2; 
unsigned long highDuration;
//float concentration;
const int chipSelect = 10; 
sensors_event_t humidity, temp;
int analogValue = analogRead(analogPin); 
float voltage = analogValue * (5.0 / 1023.0);      //  analog value to voltage (assuming 5V reference)
float co2Concentration = (voltage / 3.0) * 3000;

void setup()
{ 
  Serial.begin(115200);
  Wire.begin();
  pinMode(analogPin, INPUT);
  //pinMode(pwmPin, INPUT);
  aht.begin();
  Initialize_SDcard();
}

uint16_t line = 1;

void loop()
{
  Write_SDcard();
  Read_data();
  //highDuration = pulseIn(pwmPin, HIGH);
  delay(15000);      
}
void Read_data()
{
  int analogValue = analogRead(analogPin);
  float co2Concentration = (voltage / 3.0) * 3000; // Adjust this formula based on datasheet specifics
    
  //concentration = pulseWidthToConcentration(highDuration); 

  Serial.print("Temperature: ");
  aht.getEvent(&humidity, &temp);
  Serial.print("Temperature: "); 
  Serial.print(temp.temperature); 
  Serial.println(" C");
  Serial.print("Humidity: "); 
  Serial.print(humidity.relative_humidity);
   Serial.println(" %");
  Serial.print("CO2 Concentration: ");
  Serial.print(co2Concentration);
  Serial.println(" ppm");
  // Serial.print("Ethylene Concentration: ");
  // Serial.print(concentration);
  // Serial.println(" ppm");
  delay(500);
}

// float pulseWidthToConcentration(unsigned long highDuration)
//  {
//   // Conversion formula from the datasheet:
//   // Measurement (ppm) = ((tH - 2000µs) / 300000µs) * 10ppm
//   float rangePPM = 10.0;
//   return ((highDuration - 2000) / 300000.0) * rangePPM;
// }

void Initialize_SDcard()
{
  // see if the card is present and can be initialized:
  if (!SD.begin(chipSelect))
  {
    Serial.println("Card failed, or not present");
    // don't do anything more:
    return;
   }
   // open the file. note that only one file can be open at a time,
   // so you have to close this one before opening another.
   File dataFile = SD.open("Logger.txt", FILE_WRITE);
   // if the file is available, write to it:
   if (dataFile)
   {
    dataFile.println("    Temp,    Hum,   CO2,  C2H4"); //Write the first row of the excel file
    dataFile.close();
   }
}
void Write_SDcard()
{
    // open the file. note that only one file can be open at a time,
   // so you have to close this one before opening another.
   
   File dataFile = SD.open("Logger.txt", FILE_WRITE);
   // if the file is available, write to it:
   if (dataFile)
   {
    //Serial.print(line);
    dataFile.print(line++);
    //dataFile.print(line);
    dataFile.print(". ");
    dataFile.print(temp.temperature); 
    dataFile.print("  "); 
    dataFile.print(humidity.relative_humidity); 
    dataFile.print("  "); 
    dataFile.print(co2Concentration);
    dataFile.print("  ");
    // dataFile.print(concentration);
    // dataFile.println();
    // dataFile.close(); 
   }
  else
  Serial.println("SD card writing failed");
}

