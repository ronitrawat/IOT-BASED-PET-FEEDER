/*
 * you need to select the board as NodeMCU 1.0(ESP12E Module). for this syou can watch our other tutorials in our channel. 
 * you can also take the help from this link. and also select the COM port to uplaod the program. 
 */
// Fill-in information from your Blynk Template here

#define BLYNK_TEMPLATE_ID "TMPLGGZhlWEi"
#define BLYNK_DEVICE_NAME "FeedMyPet"

#define BLYNK_FIRMWARE_VERSION        "1.1.0"
#define BLYNK_PRINT Serial
#define USE_NODE_MCU_BOARD

#include "BlynkEdgent.h"
#include<Servo.h>
#include <HX711.h>
#define DOUT  D3               //use esp8266 pin D3 for dout
#define CLK  D2                //use esp8266 pin D2 for clock 

HX711 scale;
float calibration_factor = -172625.00;    //31072;
float weight;

Servo servo;

BLYNK_WRITE(V0)
{
  int s0 = param.asInt(); 
  servo.write(s0);
  Blynk.virtualWrite(V1, s0);
}

void setup()
{
  Serial.begin(9600);
  servo.attach(D1);
  scale.begin(DOUT,CLK);
  scale.set_scale();
  scale.tare(); //Reset the scale to 0
  long zero_factor = scale.read_average();
  BlynkEdgent.begin();

  //delay(2000); 
}

void loop() 
{
  BlynkEdgent.run();
  scale.set_scale(calibration_factor); //Adjust to this calibration factor
  weight = scale.get_units(5);
  Blynk.virtualWrite(V2, weight);
  if(weight > 10 && weight < 20)
  {
    //Blynk.email("ronitrawatcr7@gmail.com", "Alert", "Please Feed Your Pet");
    Blynk.logEvent("Please Feed Your Pet");
  }
  //delay(2000);
  Serial.print("Weight: ");
  Serial.print(weight);
  Serial.println("G");
  Serial.println();
}






//#define BLYNK_TEMPLATE_ID "TMPLG7g3dXdO"
//#define BLYNK_DEVICE_NAME "Pet Feeder Blynk"
//#define BLYNK_AUTH_TOKEN "YsyDdm9wZUcJgn6nDgY_Nx2--SJd6bZG"
