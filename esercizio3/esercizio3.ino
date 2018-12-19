#include <SoftwareSerial.h>
#include <Bridge.h>
#include <HttpClient.h>
#include <BridgeClient.h>
#include <BridgeServer.h>
#include <TinkerKit.h>
#include <ArduinoJson.h>
#include <PubSubClient.h>

#define SERVER "test.mosquitto.org"
#define PORT 1883
#define OUTPUT_TOPIC "outputTopic"
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
  mqtt.connect(MQTT_CLIENTID);
  Serial.println("START LOOP");

  celsius = 0;

}

void loop() {
  mqtt.loop();

  JsonArray& values = JSONencoder.createNestedArray("values");

  JSONencoder["device"]="deviceIOT";
  JSONencoder["sensorType"] = "Temperature";
  JSONencoder["celsius"] = celsius;

  
  for(int i=0;i<3;i++){
    celsius = term.readCelsius();
    values.add(celsius);
    delay(5000);

    char JSONmessageBuffer[500];
    JSONencoder.printTo(JSONmessageBuffer, sizeof(JSONmessageBuffer));
    Serial.println("Sending message to MQTT topic..");
    Serial.println(JSONmessageBuffer);
    
    if(mqtt.publish(OUTPUT_TOPIC, JSONmessageBuffer) == true){
      Serial.println("Success sending message");
    }else{
      Serial.println("Error sending message");
    }
    
    Serial.println("-------------");
  }
 
  delay(10000);
}
