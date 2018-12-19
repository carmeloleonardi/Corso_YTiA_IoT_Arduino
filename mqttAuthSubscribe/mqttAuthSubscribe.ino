#include <SoftwareSerial.h>
#include <Bridge.h>
#include <HttpClient.h>
#include <BridgeClient.h>
#include <BridgeServer.h>
#include <TinkerKit.h>
#include <ArduinoJson.h>
#include <PubSubClient.h>

#define SERVER "156.54.99.219"
#define PORT 8081
#define OUTPUT_TOPIC "outputTopic"
#define INPUT_TOPIC "/api/plugins/telemetry/devices/A6LKwg9KkCwR9r2wo8IE/values/timeseries"
#define MQTT_CLIENTID "testDemo"

TKThermistor term(I1);
TKLed ld1(O1);



int celsius;
int far;

void callback(char *topic, byte*  payload, unsigned int lenght)
{
  StaticJsonBuffer<250> jsonBufferInput;
  char inData[300];
  Serial.print("payload: ");

  for(int i=0;i<lenght;i++){
    inData[i] = (char)payload[i];
  }
  Serial.println();

  JsonObject& root = jsonBufferInput.parseObject(inData);

  int val = root["led"];

  Serial.print("LED: ");
  Serial.print(val);

  ld1.write(val);
  
  
}

BridgeClient yun;
PubSubClient mqtt(SERVER, PORT, callback, yun);


void setup() {
  Serial.begin(9600);
  Bridge.begin();
  mqtt.connect(MQTT_CLIENTID);
  Serial.println("START LOOP");
  mqtt.subscribe(OUTPUT_TOPIC);

  celsius = 0;
  far = 0;
  
}

void loop() {
  mqtt.loop();
}
