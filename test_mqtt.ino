#include <WiFi.h>
#include <PubSubClient.h>

const char* ssid = "Main Server";        // Wi-Fi network name
const char* password = "admin@123";// Wi-Fi password
const char* mqtt_server = "192.168.1.100"; // MQTT broker IP

WiFiClient gigaClient;
PubSubClient client(gigaClient);

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message received on topic: ");
  Serial.println(topic);

  String message;
  for (int i = 0; i < length; i++) {
    message += (char)payload[i];
  }
  Serial.println("Message: " + message);
}

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("WiFi connected");

  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
  
  while (!client.connected()) {
    Serial.println("Connecting to MQTT...");
    if (client.connect("ArduinoGIGAClient")) {
      Serial.println("Connected to MQTT");
      client.subscribe("sensor/data"); // Subscribe to the same topic
    } else {
      delay(2000);
      Serial.print(".");
    }
  }
}

void loop() {
  if (!client.connected()) {
    while (!client.connected()) {
      Serial.println("Reconnecting to MQTT...");
      if (client.connect("ArduinoGIGAClient")) {
        Serial.println("Reconnected");
        client.subscribe("sensor/data");
      } else {
        delay(2000);
      }
    }
  }
  client.loop();
}
