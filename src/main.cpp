#include <Arduino.h>
#include <PubSubClient.h>
#include <WiFi.h>

#include "secrets.h"

namespace {
constexpr uint32_t kBaudRate = 115200;
constexpr uint32_t kWifiRetryDelayMs = 500;
constexpr uint32_t kMqttRetryDelayMs = 2000;
constexpr uint32_t kPublishIntervalMs = 10000;
constexpr int kLedPin = LED_BUILTIN;

WiFiClient wifiClient;
PubSubClient mqttClient(wifiClient);
uint32_t lastPublishMs = 0;

void connectToWifi() {
  if (WiFi.status() == WL_CONNECTED) {
    return;
  }

  Serial.printf("Connecting to Wi-Fi SSID \"%s\"", secrets::kWifiSsid);
  WiFi.begin(secrets::kWifiSsid, secrets::kWifiPassword);

  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(kWifiRetryDelayMs);
  }

  Serial.println();
  Serial.print("Wi-Fi connected. IP address: ");
  Serial.println(WiFi.localIP());
}

void handleMqttMessage(char* topic, byte* payload, unsigned int length) {
  Serial.printf("Message arrived on topic %s: ", topic);
  for (unsigned int i = 0; i < length; ++i) {
    Serial.print(static_cast<char>(payload[i]));
  }
  Serial.println();
}

bool connectToMqtt() {
  if (mqttClient.connected()) {
    return true;
  }

  Serial.printf("Connecting to MQTT broker %s:%u\n",
                secrets::kMqttHost,
                secrets::kMqttPort);

  const bool connected = mqttClient.connect(secrets::kClientId,
                                            secrets::kMqttUsername,
                                            secrets::kMqttPassword);
  if (!connected) {
    Serial.printf("MQTT connect failed, rc=%d. Retrying in %lu ms.\n",
                  mqttClient.state(),
                  static_cast<unsigned long>(kMqttRetryDelayMs));
    delay(kMqttRetryDelayMs);
    return false;
  }

  Serial.println("MQTT connected.");
  mqttClient.subscribe(secrets::kSubscribeTopic);
  mqttClient.publish(secrets::kPublishTopic, "booted");
  return true;
}

void publishHeartbeat() {
  const uint32_t now = millis();
  if (now - lastPublishMs < kPublishIntervalMs) {
    return;
  }

  lastPublishMs = now;

  char payload[96];
  snprintf(payload,
           sizeof(payload),
           "{\"uptime_ms\":%lu,\"wifi_rssi\":%ld}",
           static_cast<unsigned long>(now),
           static_cast<long>(WiFi.RSSI()));

  mqttClient.publish(secrets::kPublishTopic, payload);
  Serial.printf("Published heartbeat to %s\n", secrets::kPublishTopic);
}

}  // namespace

void setup() {
  pinMode(kLedPin, OUTPUT);
  digitalWrite(kLedPin, LOW);

  Serial.begin(kBaudRate);
  delay(1000);

  Serial.println();
  Serial.println("Joy-IT NodeMCU ESP32 Wi-Fi/MQTT starter");

  WiFi.mode(WIFI_STA);
  mqttClient.setServer(secrets::kMqttHost, secrets::kMqttPort);
  mqttClient.setCallback(handleMqttMessage);

  connectToWifi();
}

void loop() {
  connectToWifi();

  if (!connectToMqtt()) {
    digitalWrite(kLedPin, LOW);
    return;
  }

  mqttClient.loop();
  publishHeartbeat();
  digitalWrite(kLedPin, HIGH);
}
