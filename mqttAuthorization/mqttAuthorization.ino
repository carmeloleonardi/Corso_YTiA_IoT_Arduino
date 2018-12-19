#include <SoftwareSerial.h>
#include <Bridge.h>
#include <HttpClient.h>
#include <BridgeClient.h>
#include <BridgeServer.h>
#include <TinkerKit.h>
#include <ArduinoJson.h>
#include <PubSubClient.h>

#define SERVER "156.54.99.219"
#define PORT 8081 //mqtt thingsboard
#define OUTPUT_TOPIC "v1/devices/me/telemetry"
#define INPUT_TOPIC "inputTopic"
#define MQTT_CLIENTID "testDemo"

TKThermistor term(I1);
TKLed ld1(O1);

float celsius;


void callback(char *topic, byte*  payload, unsigned int lenght)
{
  Serial.println();
}

BridgeClient yun;
PubSubClient mqtt(SERVER, PORT, callback, yun);

StaticJsonBuffer<300> JSONbuffer;
JsonObject& JSONencoder = JSONbuffer.createObject();

void setup() {
  Serial.begin(9600);
  Bridge.begin();
  mqtt.connect(MQTT_CLIENTID,"A6LKwg9KkCwR9r2wo8IE","");
  Serial.println("START LOOP");

  celsius = 0;

}

void loop() {
  mqtt.loop();

  celsius = term.readCelsius();
  JSONencoder["mbare"] = celsius;

  char JSONmessageBuffer[100];
  JSONencoder.printTo(JSONmessageBuffer, sizeof(JSONmessageBuffer));
  Serial.println("Sending message to MQTT topic..");
  Serial.println(JSONmessageBuffer);
  
  if(mqtt.publish(OUTPUT_TOPIC, JSONmessageBuffer) == true){
      Serial.println("Success sending message");
    }else{
      Serial.println("Error sending message");
   }
 
  delay(10000);
}
