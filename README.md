<p align="center">
  <img src="https://img.shields.io/badge/platform-ESP32--S3-blue?style=flat-square&logo=espressif" alt="Platform">
  <img src="https://img.shields.io/badge/framework-Arduino-00979D?style=flat-square&logo=arduino" alt="Framework">
  <img src="https://img.shields.io/badge/license-GPLv3-green?style=flat-square" alt="License">
  <img src="https://img.shields.io/github/v/release/shi2233/cardputer-deepseek-chat?style=flat-square&color=orange" alt="Release">
</p>

<h1 align="center">🃏 Cardputer DeepSeek Chat</h1>

<p align="center">
  为 <b>M5Stack Cardputer ADV</b> 打造的 DeepSeek AI 聊天客户端，支持拼音输入、菜单导航和流式回复。
</p>

---

## ✨ 功能特色

| 功能 | 说明 |
|------|------|
| 🧠 **DeepSeek API** | 流式输出，回复实时逐字显示 |
| ⌨️ **拼音输入法** | 覆盖 400+ 音节，数字键 1-5 选候选字 |
| 🎮 **菜单导航** | 方向键操作，Enter 确认，Esc 返回 |
| 🌐 **WiFiManager 配网** | 浏览器配置 WiFi 和 API Key，无需写代码 |
| 📜 **历史滚动** | ↑ ↓ 浏览长回复，^v 指示上下内容 |
| 🈶 **中文显示** | 内置 efontCN_12 中文字库，中英混排 |
| 💾 **配置持久化** | API Key 保存在 Flash，重启不丢失 |

## 🚀 快速开始

```
1.  下载 Releases 中的 firmware.bin
2.  按住 G0 键 + 上电，进入下载模式
3.  烧录固件
4.  重启后连接 WiFi 热点「Cardputer-Chat」（密码 12345678）
5.  浏览器打开 192.168.4.1，填写 WiFi 和 DeepSeek API Key
6.  回车进入主菜单，开始聊天
```

## 🎮 按键操作

| 按键 | 菜单 | 聊天 |
|------|------|------|
| ↑ / ↓ | 移动选择 | 滚动历史 |
| Enter | 确认进入 | 发送消息 |
| Esc | — | 返回菜单 |
| Tab | — | 切换中/英文输入 |
| Space | — | 确认首个候选字 |
| 1-5 | — | 选择对应候选字 |
| Backspace | — | 删除字符 |

## 🔧 自行编译

```bash
pio run          # 编译
pio run -t upload  # 烧录
```

需要安装 [PlatformIO](https://platformio.org/)。

## 📁 项目结构

```
cardputer-deepseek-chat/
├── include/
│   ├── config.h        # 全局配置
│   ├── display.h       # 显示接口
│   ├── keyboard.h      # 键盘接口
│   ├── api.h           # API 接口
│   ├── wifi_setup.h    # WiFi 配置接口
│   └── pinyin.h        # 拼音输入法接口
├── src/
│   ├── main.cpp        # 主程序与状态机
│   ├── display.cpp     # LCD 渲染
│   ├── keyboard.cpp    # 按键处理
│   ├── api.cpp         # DeepSeek API 调用
│   ├── wifi_setup.cpp  # WiFi 配网
│   └── pinyin.cpp      # 拼音字典与 IME
└── platformio.ini      # 编译配置
```

## 📜 许可证

[GPL v3](LICENSE) © 2026 shi2233
