#include <WiFi.h>
#include "secrets.h"
#include "ThingSpeak.h" // always include thingspeak header file after other header files and custom macros
#include <Adafruit_AHTX0.h>
#include <Wire.h>

#define analogPin 34 // Define analog pin for ESP32
#define ADC_RESOLUTION 4095.0 // ESP32 has 12-bit ADC resolution
#define REF_VOLTAGE 3.3
#define data 25

Adafruit_AHTX0 aht;
//DHT22 dht22(data); 


const int CO2Pin = 34; // pin connected to the sensor's output
//const int C2H4Pin=32;
//const float range = 600; // Sensor's range in ppm
sensors_event_t humidity, temp;

char ssid[] = SECRET_SSID;   // your network SSID (name) 
char pass[] = SECRET_PASS;   // your network password
int keyIndex = 0;            // your network key Index number (needed only for WEP)
WiFiClient  client;

unsigned long myChannelNumber = SECRET_CH_ID;
const char * myWriteAPIKey = SECRET_WRITE_APIKEY;

// Initialize our values
//int number1 = 0;
int number2 = 0;
int number3 = 0;
int number4 = 0;
// int number5 = 0;
// int number6 = 0;



String myStatus = "";

void setup() {
  Serial.begin(115200);  //Initialize serial
  pinMode(analogPin, INPUT);
  pinMode(CO2Pin, INPUT);
  aht.begin();
  Wire.begin();
 
  while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo native USB port only
  }
  
  WiFi.mode(WIFI_STA);   
  ThingSpeak.begin(client);  // Initialize ThingSpeak
  
  Serial.println("Wait 90s......");
  delay(90000);
}

void loop() {

  // Connect or reconnect to WiFi
  if(WiFi.status() != WL_CONNECTED){
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(SECRET_SSID);
    while(WiFi.status() != WL_CONNECTED){
      WiFi.begin(ssid, pass);  // Connect to WPA/WPA2 network. Change this line if using open or WEP network
      Serial.print(".");
      delay(5000);     
    } 
    Serial.println("\nConnected.");
  }

  //number1 =C2H4PPm();
  number2 =CO2PPM();
  number3 =InTemperature();
  number4 =InHumidity();
  // number5 =OutTemperature();
  // number6 =OutHumidity();
  
  
  
  // set the fields with the values
  //ThingSpeak.setField(1, number1);
  ThingSpeak.setField(2, number2);
  ThingSpeak.setField(3, number3);
  ThingSpeak.setField(4, number4);
  // ThingSpeak.setField(5, number5);
  // ThingSpeak.setField(6, number6);


  

  
  // write to the ThingSpeak channel
  int x = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
  if(x == 200){
    Serial.println("Channel update successful.");
  }
  else{
    Serial.println("Problem updating channel. HTTP error code " + String(x));
  }
delay(15000);
}

// For Outside Himidity
float InHumidity()
{
  
  aht.getEvent(&humidity, &temp);
  Serial.print("Humidity: "); 
  Serial.print(humidity.relative_humidity);
  Serial.println(" %");

  return(humidity.relative_humidity);
}

//For Outside Temperature
float InTemperature()
{
  Serial.print("Temperature: ");
  aht.getEvent(&humidity, &temp);
  Serial.print("Temperature: "); 
  Serial.print(temp.temperature); 
  Serial.println(" C");
    return(temp.temperature);
}


//CO2 in PPM
float CO2PPM()
{
  int analogValue = analogRead(analogPin); // Read the analog value from the pin
  float voltage = analogValue * (REF_VOLTAGE / ADC_RESOLUTION); 
  //Serial.print("Analog Voltage: ");
  //Serial.println(voltage, 3); 

  float sensorVoltage = voltage * (5.0 / REF_VOLTAGE); // Adjust voltage to match the sensor's 5V power supply
  float co2Concentration = (sensorVoltage / 3.0) * 3000; // Adjust this formula based on datasheet specifics
  Serial.print("CO2 Concentration: ");
  Serial.print(co2Concentration);
  Serial.println(" ppm");
  return(co2Concentration);
}
