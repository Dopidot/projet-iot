#include <Arduino.h>
#include <Ethernet.h>
#include <PubSubClient.h>
#include <EthernetClient.h>

Ethernet WSA; // WSAStartup
EthernetClient ethClient;

#define LED_PIN 3

long localMinute = 11;
long localSecond = 30;

long openMinute = 11;
long openSecond = 35;

long closeMinute = 11;
long closeSecond = 55;

int runMotorType = 0;

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
  if (topicStr.equals("EssaieIOT/OnOff")) {
    String message = payloadToString(payload, length);

    if(message.equals("on"))
    {
        mqtt.publish("EssaieIOT/SendData", "LED On");
        digitalWrite(LED_PIN, HIGH);
    }
    else if (message.equals("off")) 
    {
        mqtt.publish("EssaieIOT/SendData", "LED Off");
        digitalWrite(LED_PIN, LOW);
    }

    if(message.equals("open"))
    {
        mqtt.publish("EssaieIOT/SendData", "Ouverture du volet");
        runMotor(1);
    }
    else if(message.equals("close"))
    {
        mqtt.publish("EssaieIOT/SendData", "Fermeture du volet");
        runMotor(2);
    }
  }

  
}


void setup () {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);


  pinMode(10, INPUT);
  pinMode(9, INPUT);

  digitalWrite(10, LOW); // MOTOR
  digitalWrite(9, LOW); // MOTOR

  //Pre
  //digitalWrite(10, 80);
  analogWrite(0, 80);
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
        digitalWrite(10, LOW);
  		digitalWrite(9, HIGH);
        delay(10 * 1000);
    } 
    else if (runMotorType == 2)
    {
        digitalWrite(10, HIGH);
  		digitalWrite(9, LOW); 
        delay(10 * 1000);
    }

    digitalWrite(10, LOW);
    digitalWrite(9, LOW);

          Serial.print(millis());

  	/*long elapsedTime = millis();
  	long currentMinute = (elapsedTime / 1000 / 60) + localMinute;
  	long currentSecond = (elapsedTime / 1000) + localSecond;
  
	if (currentMinute >= openMinute &&
        currentSecond >= openSecond && currentSecond - openSecond <= 10)
    {
      	Serial.print(currentMinute);
      	Serial.print(":");
      	Serial.print(currentSecond);
      	Serial.println(" Ouverture du volet");

      	digitalWrite(10, HIGH);
  		digitalWrite(9, LOW);
    }
  	else if (currentMinute >= closeMinute && 
        currentSecond >= closeSecond && currentSecond - closeSecond < 10 )
  	{
		Serial.print(currentMinute);
      	Serial.print(":");
      	Serial.print(currentSecond);
      	Serial.println(" Fermeture du volet");
      
      	digitalWrite(10, LOW);
  		digitalWrite(9, HIGH);
  	}
  	else
  	{
      	digitalWrite(10, LOW);
  		digitalWrite(9, LOW);
  	}
  
    delay(5000);
    mySerial.print("Coucou");
    delay(5000);
    mySerial.print("Goodbye");*/
}

void loop() {
    if (!mqtt.connected())
    {
        reconnect();
    }
    mqtt.loop();

    delay(2000);

    /*if (digitalRead(LED_PIN) == 0 ) {
        mqtt.publish("EssaieIOT/SendData", "Led Off");
    } else {
        mqtt.publish("EssaieIOT/SendData", "Led On");
    }*/

 }
