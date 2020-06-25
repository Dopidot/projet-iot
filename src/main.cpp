#include <Arduino.h>
#include <Ethernet.h>
#include <PubSubClient.h>
#include <EthernetClient.h>

Ethernet WSA; // WSAStartup
EthernetClient ethClient;

#define LED_PIN 3

String payloadToString(byte *payload, unsigned int length)
{
  char buffer[length];
  sprintf(buffer, "%.*s", length, payload);
  return String(buffer);
}

void callback(char *topic, byte *payload, unsigned int length)
{
  String topicStr = String(topic);
  if (topicStr.equals("EssaieIOT/OnOff")) {
    String message = payloadToString(payload, length);
    if(message.equals("on"))
    {
      digitalWrite(LED_PIN, HIGH);
    } else if (message.equals("off")) {
      digitalWrite(LED_PIN, LOW);
    }
  }

  
}

PubSubClient mqtt("mqtt.flespi.io", 1883, callback, ethClient);

void setup () {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);


  pinMode(10, INPUT);

  //Pre
  digitalWrite(10, 80);
  analogWrite(0, 80);
}

void reconnect()
{
  while (!mqtt.connected())
  {
    Serial.println("[MQTT] Connecting to MQTT...");
    if (mqtt.connect("MonObjectConnecte", "90xVpJU7zfbKqcH3lFUvo8oo5U5G7hg67EQmbAhBGg9UiFLu8vNnrZzmJuQFBXBw", "")) // Or mqtt.connect("MonObjectConnecte", "user", "pass") if you have protect the mqtt broker by credentials
    {
      Serial.println("[MQTT] Connected");
      mqtt.subscribe("EssaieIOT/OnOff");
    }
    else
    {
      Serial.print("[ERROR] MQTT Connect: ");
      Serial.println(mqtt.state());
      delay(60 * 1000);
    }
  }
}

void loop() {
  if (!mqtt.connected())
  {
    reconnect();
  }
  mqtt.loop();

  delay(2000);

  if (digitalRead(LED_PIN) == 0 ) {
    mqtt.publish("EssaieIOT/SendData", "Led Off");
  } else {
    mqtt.publish("EssaieIOT/SendData", "Led On");
  }

 }
