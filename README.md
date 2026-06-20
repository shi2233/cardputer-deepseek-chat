# Cardputer DeepSeek Chat

A DeepSeek AI chat client for M5Stack Cardputer ADV, featuring Chinese pinyin input, menu navigation, and streaming response display.

## Features

- DeepSeek API integration with streaming output
- Chinese pinyin input method (400+ syllables)
- Menu-based UI with directional key navigation
- WiFi and API Key configuration via WiFiManager
- Scrollable chat history
- UTF-8 Chinese text rendering with efontCN_12 font

## Hardware

- [M5Stack Cardputer ADV](https://docs.m5stack.com/en/core/Cardputer%20ADV) (ESP32-S3, 1.14" LCD, 56-key keyboard)

## Build

Requires [PlatformIO](https://platformio.org/).

```bash
pio run
pio run -t upload
```

## Usage

1. Flash the firmware to your Cardputer ADV
2. On first boot, connect to WiFi hotspot `Cardputer-Chat` (password: `12345678`)
3. Open `192.168.4.1` in browser, enter WiFi credentials and DeepSeek API Key
4. Navigate menus with arrow keys, Enter to select, Esc to go back
5. In chat mode: Tab to switch Chinese/English, Space/1-5 to select candidates

## License

GPL v3
