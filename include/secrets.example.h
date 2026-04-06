#pragma once

namespace secrets {

constexpr char kWifiSsid[] = "YOUR_WIFI_SSID";
constexpr char kWifiPassword[] = "YOUR_WIFI_PASSWORD";

constexpr char kMqttHost[] = "192.168.1.10";
constexpr uint16_t kMqttPort = 1883;
constexpr char kMqttUsername[] = "mqtt-user";
constexpr char kMqttPassword[] = "mqtt-password";

constexpr char kClientId[] = "joyit-esp32";
constexpr char kPublishTopic[] = "joyit/esp32/status";
constexpr char kSubscribeTopic[] = "joyit/esp32/command";

}  // namespace secrets
