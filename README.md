# Cardputer DeepSeek Chat

运行在 M5Stack Cardputer ADV 上的 DeepSeek 聊天客户端。  
DeepSeek AI chat client for M5Stack Cardputer ADV.

## Features

- DeepSeek API 流式输出 / Streaming output
- 中文拼音输入法（400+ 音节）/ Chinese pinyin IME
- 方向键菜单导航 / D-pad menu navigation
- WiFiManager 配网 / WiFi and API Key config via web portal
- 聊天历史滚动 / Scrollable chat history
- 中文字体渲染 / Chinese font rendering (efontCN_12)

## Quick Start

1. 从 [Releases](https://github.com/shi2233/cardputer-deepseek-chat/releases) 下载 `firmware.bin`
2. 按住 G0 上电进入下载模式，烧录固件
3. 重启后连接 WiFi 热点 `Cardputer-Chat`（密码 `12345678`）
4. 浏览器打开 `192.168.4.1`，填写 WiFi 和 DeepSeek API Key
5. 方向键导航，Enter 确认，Esc 返回

## Controls

| Key | Menu | Chat |
|-----|------|------|
| ↑ / ↓ | Navigate | Scroll history |
| Enter | Select | Confirm / Send |
| Esc | — | Back to menu |
| Tab | — | Toggle CN/EN IME |
| Space | — | Confirm candidate |
| 1-5 | — | Select candidate |
| Backspace | — | Delete |

## Build

```
pio run
pio run -t upload
```

## License

GPL v3
