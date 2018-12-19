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

int celsius;
int far;

void callback(char *topic, byte*  payload, unsigned int lenght)
{
  Serial.print("Message arrived[");
  Serial.print(topic);
  Serial.print("] ");
  for(int i=0;i<lenght;i++){
    Serial.print((char)payload[i]);
  }
  Serial.println();
}

BridgeClient yun;
PubSubClient mqtt(SERVER, PORT, callback, yun);


void setup() {
  Serial.begin(9600);
  Bridge.begin();
  mqtt.connect(MQTT_CLIENTID);
  Serial.println("START LOOP");

  celsius = 0;
  far = 0;
  
}

void loop() {
  mqtt.loop();

  celsius = term.readCelsius();
  far = term.readFahrenheit();


  
  StaticJsonBuffer<300> JSONbuffer;
  JsonObject& JSONencoder = JSONbuffer.createObject();
  JSONencoder["device"]="deviceIOT";
  JSONencoder["sensorType"] = "Temperature";
  JSONencoder["celsius"] = celsius;
  JSONencoder["fahrenheit"] = far;
  JsonArray& values = JSONencoder.createNestedArray("values");
  values.add(20);
  values.add(21);
  values.add(23);
  char JSONmessageBuffer[100];
  JSONencoder.printTo(JSONmessageBuffer, sizeof(JSONmessageBuffer));
  Serial.println("Sending message to MQTT topic..");
  Serial.println(JSONmessageBuffer);
  
  if(mqtt.publish(OUTPUT_TOPIC, JSONmessageBuffer) == true){
    Serial.println("Success sending message");
  }else{
    Serial.println("Error sending message");
  }
  
  Serial.println("-------------");
  
  delay(10000);
}
