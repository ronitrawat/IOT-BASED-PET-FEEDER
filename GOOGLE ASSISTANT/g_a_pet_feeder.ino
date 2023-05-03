#include <ESP8266WiFi.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"
#include<Servo.h>
#include <NTPClient.h>
#include <WiFiUdp.h>

#include <Wire.h> 

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", 19800,60000);

Servo servo;


/*#define WIFI_SSID "Redmi 9A"
#define WIFI_PASS "12345678"*/

#define WIFI_SSID "Galaxy M12C1D0"
#define WIFI_PASS "ronaldoki"

#define MQTT_SERV "io.adafruit.com"
#define MQTT_PORT 1883
#define MQTT_NAME "Shubham_1"
#define MQTT_PASS "aio_oXhW21EC5e5cddYvyEWJoVbRx0wl"

int SERVO_PIN = D1;    // The pin which the servo is attached to
int CLOSE_ANGLE = 0;  // The closing angle of the servo motor arm
int OPEN_ANGLE = 170;  // The opening angle of the servo motor arm
int  hh, mm, ss;
int feed_hour = 0;
int feed_minute = 0;

//Set up MQTT and WiFi clients
WiFiClient client;
Adafruit_MQTT_Client mqtt(&client, MQTT_SERV, MQTT_PORT, MQTT_NAME, MQTT_PASS);

//Set up the feed you're subscribing to
Adafruit_MQTT_Subscribe feedpet = Adafruit_MQTT_Subscribe(&mqtt, MQTT_NAME "/f/feedpet");
boolean feed = true; // condition for alarm

void setup()
{
  Serial.begin(9600); 
  timeClient.begin();
  //Wire.begin(D2, D1);
  //lcd.begin();
  
  //Connect to WiFi
  Serial.print("\n\nConnecting Wifi... ");
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
  }

  Serial.println("OK!");

  //Subscribe to the feedpet feed
  mqtt.subscribe(&feedpet);
  servo.attach(SERVO_PIN);
  servo.write(CLOSE_ANGLE);
  
  
}

void loop()
{
   MQTT_connect();
   timeClient.update();
   
   //lcd.setCursor(0,0);
   //lcd.print("Time:");
    
  Adafruit_MQTT_Subscribe * subscription;
  while ((subscription = mqtt.readSubscription(5000)))
  {
    
    if (subscription == &feedpet)
    {
      //Print the new value to the serial monitor
      Serial.println((char*) feedpet.lastread);
     
    if (!strcmp((char*) feedpet.lastread, "yes"))
      {
        
        open_door();
        delay(1000);
        close_door();
      }
    }
   }
}

void MQTT_connect() 
/{
  int8_t ret;

  // Stop if already connected.
  if (mqtt.connected()) 
  {
    return;
  }

  uint8_t retries = 3;
  while ((ret = mqtt.connect()) != 0) // connect will return 0 for connected
  { 
       
       mqtt.disconnect();
       delay(2000);  // wait 5 seconds
       retries--;
       if (retries == 0) 
       {
         // basically die and wait for WDT to reset me
         while (1);
       }
  }
  
}

void open_door(){
  
  servo.write(OPEN_ANGLE);   // Send the command to the servo motor to open the trap door
}

void close_door(){
  
  servo.write(CLOSE_ANGLE);   // Send te command to the servo motor to close the trap door
}
 
