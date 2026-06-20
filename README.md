# Cardputer DeepSeek Chat

**Cardputer DeepSeek Chat** 是运行在 [M5Stack Cardputer ADV](https://docs.m5stack.com/en/core/Cardputer%20ADV) 上的 DeepSeek AI 聊天客户端，支持拼音输入、菜单导航和流式回复显示。

A DeepSeek AI chat client for M5Stack Cardputer ADV, featuring Chinese pinyin input, menu navigation, and streaming response display.

---

## 功能 / Features

- DeepSeek API 集成，流式输出 / DeepSeek API integration with streaming output
- 中文拼音输入法（400+ 音节）/ Chinese pinyin input method (400+ syllables)
- 方向键菜单导航 / Menu-based UI with arrow key navigation
- WiFiManager 配网 / WiFi and API Key configuration via WiFiManager
- 聊天历史滚动浏览 / Scrollable chat history
- UTF-8 中文字体渲染（efontCN_12）/ UTF-8 Chinese text rendering

## 硬件 / Hardware

- [M5Stack Cardputer ADV](https://docs.m5stack.com/en/core/Cardputer%20ADV) (ESP32-S3, 1.14" LCD, 56-key keyboard)

## 下载 / Download

预编译固件在 [Releases](https://github.com/shi2233/cardputer-deepseek-chat/releases) 页面下载。

Precompiled firmware is available in the [Releases](https://github.com/shi2233/cardputer-deepseek-chat/releases) page.

## 构建 / Build

需要 [PlatformIO](https://platformio.org/)。

```bash
pio run
pio run -t upload
```

## 使用说明 / Usage

1. 刷入固件到 Cardputer ADV / Flash the firmware to your Cardputer ADV
2. 首次开机连接 WiFi 热点 `Cardputer-Chat`（密码 `12345678`）/ On first boot, connect to WiFi hotspot `Cardputer-Chat`
3. 浏览器打开 `192.168.4.1`，填写 WiFi 和 DeepSeek API Key / Open `192.168.4.1` in browser
4. 方向键选菜单，Enter 确认，Esc 返回 / Navigate with arrow keys
5. 聊天模式：Tab 切换中/英输入，Space/1-5 选拼音候选字 / In chat: Tab to switch IME, Space/1-5 to select candidates

## 操作键位 / Controls

| 按键 | 功能 |
|------|------|
| `↑` `↓` | 菜单导航 / 聊天滚动 |
| `Enter` | 确认 / 发送消息 |
| `Esc` | 返回上级菜单 |
| `Tab` | 切换中/英文输入模式 |
| `Space` | 空格 / 确认候选字 |
| `1-5` | 选择拼音候选字 |
| `Backspace` | 退格删除 |

## 许可证 / License

GPL v3
