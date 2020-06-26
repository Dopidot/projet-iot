#include <Arduino.h>
#include <Ethernet.h>
#include <PubSubClient.h>
#include <EthernetClient.h>

Ethernet WSA; // WSAStartup
EthernetClient ethClient;

#define LED_PIN 3
#define MOTOR_PIN_1 9
#define MOTOR_PIN_2 10

void runMotor(int runMotorType);

void callback(char *topic, byte *payload, unsigned int length);
PubSubClient mqtt("mqtt.flespi.io", 1883, callback, ethClient);


String payloadToString(byte *payload, unsigned int length)
{
  char buffer[length];
  sprintf(buffer, "%.*s", length, payload);
  return String(buffer);
}

void callback(char *topic, byte *payload, unsigned int length)
{
  String topicStr = String(topic);

  if (topicStr.equals("EssaieIOT/OnOff"))
  {
        String message = payloadToString(payload, length);

        if(message.equals("on"))
        {
            mqtt.publish("EssaieIOT/SendData", "LED On");
            Serial.println("Activation des lumieres");
            digitalWrite(LED_PIN, HIGH);
        }
        else if (message.equals("off")) 
        {
            mqtt.publish("EssaieIOT/SendData", "LED Off");
            Serial.println("Desactivation des lumieres");
            digitalWrite(LED_PIN, LOW);
        }
        else if(message.equals("open"))
        {
            mqtt.publish("EssaieIOT/SendData", "Ouverture du volet");
            Serial.println("Ouverture du volet");
            runMotor(1);
        }
        else if(message.equals("close"))
        {
            mqtt.publish("EssaieIOT/SendData", "Fermeture du volet");
            Serial.println("Fermeture du volet");
            runMotor(2);
        }
  }

}


void setup () 
{
    Serial.begin(115200);
    pinMode(LED_PIN, OUTPUT);

    pinMode(MOTOR_PIN_2, INPUT);
    pinMode(MOTOR_PIN_1, INPUT);

    digitalWrite(MOTOR_PIN_2, LOW); // MOTOR
    digitalWrite(MOTOR_PIN_1, LOW); // MOTOR
}

void reconnect()
{
  while (!mqtt.connected())
  {
    Serial.println("[MQTT] Connecting to MQTT...");
    if (mqtt.connect("MonObjectConnecte", "MQur04j45BOLhvZZ4gfGrTx0HOjkb5dNJx3zrctnC6WJpJwjNBP3kcTQO00QV1ru", "")) // Or mqtt.connect("MonObjectConnecte", "user", "pass") if you have protect the mqtt broker by credentials
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

void runMotor(int runMotorType)
{
    if (runMotorType == 1) 
    {
        digitalWrite(MOTOR_PIN_2, LOW);
  		digitalWrite(MOTOR_PIN_1, HIGH);
        delay(10 * 1000);
    } 
    else if (runMotorType == 2)
    {
        digitalWrite(MOTOR_PIN_2, HIGH);
  		digitalWrite(MOTOR_PIN_1, LOW); 
        delay(10 * 1000);
    }

    digitalWrite(MOTOR_PIN_2, LOW);
    digitalWrite(MOTOR_PIN_1, LOW);
}

void loop() 
{
    if (!mqtt.connected())
    {
        reconnect();
    }

    mqtt.loop();

    delay(2000);
 }
