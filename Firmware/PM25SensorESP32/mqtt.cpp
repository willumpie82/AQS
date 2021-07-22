#include "mqtt.h"

// Wifi
#define WIFI_CONNECT_INTERVAL 500    // Wait 500ms intervals for wifi connection
#define WIFI_CONNECT_MAX_ATTEMPTS 10 // Number of attempts/intervals to wait

// MQTT
uint32_t g_device_id; // Unique ID from ESP chip ID

WiFiClient esp_client;
PubSubClient client(esp_client);

uint32_t getDeviceID(void)
{
  return g_device_id;
}

/*
  This callback is invoked when an MQTT message is received. It's not important
  right now for this project because we don't receive commands via MQTT. You
  can modify this function to make the device act on commands that you send it.
*/
void callback(char *topic, byte *message, unsigned int length)
{
  //Serial.print("Message arrived [");
  //Serial.print(topic);
  //Serial.print("] ");
  //for (int i = 0; i < length; i++) {
  //  Serial.print((char)payload[i]);
  //}
  //Serial.println();
}

/**
  Reconnect to MQTT broker, and publish a notification to the status topic
*/
void reconnectMqtt()
{
  char g_mqtt_message_buffer[150]; // General purpose buffer for MQTT messages
  char mqtt_client_id[20];
  sprintf(mqtt_client_id, "esp32-%x", g_device_id);

  // Loop until we're reconnected
  while (!client.connected())
  {
    Serial.print("Attempting MQTT connection to ");
    Serial.print(mqtt_broker);
    Serial.print(" as ");
    Serial.print(mqtt_client_id);
    Serial.print("... ");
    // Attempt to connect
    if (client.connect(mqtt_client_id))
    {
      //Serial.println("connected");
      // Once connected, publish an announcement
      sprintf(g_mqtt_message_buffer, "Device %s starting up", mqtt_client_id);
      client.publish(status_topic, g_mqtt_message_buffer);
      // Resubscribe
      //client.subscribe(g_command_topic);
    }
    else
    {
      //Serial.print("failed, rc=");
      //Serial.print(client.state());
      //Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

bool initWifi()
{
  Serial.print("InitWifi:");

  // Clean up any old auto-connections
  if (WiFi.status() == WL_CONNECTED)
  {
    WiFi.disconnect();
  }
  WiFi.setAutoConnect(false);

  // RETURN: No SSID, so no wifi!
  if (sizeof(WIFI_SSID) == 1)
  {
    return false;
  }

  // Connect to wifi
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  // Wait for connection set amount of intervals
  int num_attempts = 0;
  while (WiFi.status() != WL_CONNECTED && num_attempts <= WIFI_CONNECT_MAX_ATTEMPTS)
  {
    delay(WIFI_CONNECT_INTERVAL);
    num_attempts++;
  }

  if (WiFi.status() != WL_CONNECTED)
  {
    return false;
    Serial.println("FAILED");
  }
  else
  {
    return true;
    Serial.println(" OK");
  }

}

void initMQTT(void)
{
  Serial.print("InitMQTT:");

  /* Set up the MQTT client */
  // We need a unique device ID for our MQTT client connection

  uint64_t macAddress = ESP.getEfuseMac();
  uint64_t macAddressTrunc = macAddress << 40;
  g_device_id = macAddressTrunc >> 40;
  Serial.print("Device ID: ");
  Serial.print(g_device_id, HEX);

  client.setServer(mqtt_broker, 1883);
  client.setCallback(callback);

  Serial.println(" OK");
}


void handleMQTT(void)
{
  if (WiFi.status() == WL_CONNECTED)
  {
    if (!client.connected())
    {
      reconnectMqtt();
    }
  }
  client.loop(); // Process any outstanding MQTT messages
}

/**
  Report the most recent values to MQTT if enough time has passed
*/

void publishSingle(uint32_t value, char *topic)
{
  char g_mqtt_message_buffer[150]; // General purpose buffer for MQTT messages
  String message_string;
  message_string = String(value);
  message_string.toCharArray(g_mqtt_message_buffer, message_string.length() + 1);
  client.publish(topic, g_mqtt_message_buffer);
}

