# ESP32 Wi-Fi + MQTT Starter

This repository is initialized for a Joy-IT NodeMCU ESP32 board using
PlatformIO with the Arduino framework, Wi-Fi, and MQTT.

## Requirements

- [PlatformIO Core](https://platformio.org/install)
  or the PlatformIO IDE extension

## Project layout

- `platformio.ini`: board and build configuration
- `include/secrets.example.h`: committed template for Wi-Fi and MQTT settings
- `include/secrets.h`: tracked wrapper that loads local secrets when present
- `include/secrets.local.h`: local credentials file for your machine
- `src/main.cpp`: firmware with Wi-Fi and MQTT reconnect logic

## Configure credentials

Copy the template and fill in your local values:

```bash
cp include/secrets.example.h include/secrets.local.h
```

The firmware includes `include/secrets.h`, which automatically uses
`include/secrets.local.h` when that file exists and otherwise falls back
to `include/secrets.example.h`. Update these values in
`include/secrets.local.h`:

- `kWifiSsid`
- `kWifiPassword`
- `kMqttHost`
- `kMqttPort`
- `kMqttUsername`
- `kMqttPassword`
- `kClientId`
- `kPublishTopic`
- `kSubscribeTopic`

`include/secrets.local.h` is ignored by Git, so it will not be pushed to
GitHub. Commit `include/secrets.example.h` with placeholder values only.

After upload, the device:

- connects to Wi-Fi
- connects to the MQTT broker
- subscribes to the command topic
- publishes a `booted` message once
- publishes a heartbeat JSON message every 10 seconds

## Common commands

Build the firmware:

```bash
pio run
```

Upload to a connected board:

```bash
pio run --target upload
```

Open the serial monitor:

```bash
pio device monitor
```

## Notes

The project target is `nodemcu-32s`, which is the closest PlatformIO
board definition for the Joy-IT NodeMCU ESP32.
