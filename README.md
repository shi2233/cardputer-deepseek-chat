<p align="center">
  <img src="https://img.shields.io/badge/platform-ESP32--S3-blue?style=for-the-badge&logo=espressif" alt="Platform">
  <img src="https://img.shields.io/badge/framework-Arduino-00979D?style=for-the-badge&logo=arduino" alt="Framework">
  <img src="https://img.shields.io/badge/license-GPLv3-green?style=for-the-badge" alt="License">
  <img src="https://img.shields.io/github/v/release/shi2233/cardputer-deepseek-chat?style=for-the-badge&color=orange" alt="Release">
</p>

<h1 align="center">🃏 Cardputer DeepSeek Chat</h1>

<p align="center">
  🤖 <b>DeepSeek AI 聊天客户端</b>  —  运行在 <b>M5Stack Cardputer ADV</b> 上<br>
  <i>DeepSeek AI chat client for M5Stack Cardputer ADV</i>
</p>

<p align="center">
  <b>拼音输入 · 菜单导航 · 流式回复 · WiFi配网</b><br>
  <i>Pinyin IME · Menu UI · Streaming · WiFiManager</i>
</p>

---

## 📸 概览 / Overview

| 主菜单 / Main Menu | 聊天 / Chat | 拼音输入 / Pinyin |
|:---:|:---:|:---:|
| 方向键导航 | 流式回复 | 候选字选择 |

---

## ✨ 功能 / Features

| 中文 | English |
|------|---------|
| 🧠 **DeepSeek API 集成**，流式输出，回复实时显示 | DeepSeek API with real-time streaming output |
| ⌨️ **中文拼音输入法**，覆盖 400+ 音节，数字键选字 | Chinese pinyin IME with 400+ syllables, number-key selection |
| 🎮 **方向键菜单导航**，Enter 确认，Esc 返回 | D-pad menu navigation, Enter to confirm, Esc to go back |
| 🌐 **WiFiManager 配网**，浏览器配置 WiFi 和 API Key | WiFiManager web portal for WiFi & API Key config |
| 📜 **聊天历史滚动**，长回复自由浏览 | Scrollable chat history for long responses |
| 🈶 **中文字体渲染**，内置 efontCN_12 点阵字库 | Chinese font rendering with built-in efontCN_12 bitmap font |
| 💾 **持久化配置**，重启不丢失 API Key | Persistent storage — API Key survives reboots |

---

## 🧰 硬件 / Hardware

<p align="center">
  <a href="https://docs.m5stack.com/en/core/Cardputer%20ADV">
    <img src="https://m5stack-doc.oss-cn-shenzhen.aliyuncs.com/1178/Cardputer-Adv_01.webp" width="280" alt="Cardputer ADV">
  </a>
</p>

| 参数 | 规格 |
|------|------|
| SoC | ESP32-S3FN8 @ 240MHz |
| Flash | 8MB |
| 屏幕 / Display | 1.14" ST7789V2, 240×135 px |
| 键盘 / Keyboard | 56 键, TCA8418 I2C 控制器 |
| 音频 / Audio | ES8311 + MEMS mic + 1W 喇叭 |
| 电池 / Battery | 1750mAh |

---

## 📥 下载 / Download

> **预编译固件** 👉 [Releases](https://github.com/shi2233/cardputer-deepseek-chat/releases)
>
> **Precompiled firmware**: Download from the [Releases page](https://github.com/shi2233/cardputer-deepseek-chat/releases).

| 文件 | 说明 |
|------|------|
| `firmware.bin` | 主固件（主应用） |

---

## 🔧 构建 / Build

```bash
# 克隆仓库
git clone https://github.com/shi2233/cardputer-deepseek-chat.git
cd cardputer-deepseek-chat

# 编译
pio run

# 烧录
pio run -t upload
```

> 需要安装 [PlatformIO Core](https://platformio.org/install/cli)。

---

## 🚀 使用说明 / Usage

### 首次开机

1. 🔌 将 `firmware.bin` 烧录到 Cardputer ADV（G0 + 上电进入下载模式）
2. 📡 设备自动开启 WiFi 热点 `Cardputer-Chat`（密码 `12345678`）
3. 📱 手机连接该热点，浏览器打开 `192.168.4.1`
4. ✏️ 填写 WiFi 名称/密码 + DeepSeek API Key，保存
5. ✅ 设备自动联网，进入主菜单

### 日常使用

| 步骤 | 操作 |
|------|------|
| 📋 **主菜单** | `↑` `↓` 选择，`Enter` 进入 |
| 💬 **开始聊天** | 选中「聊天」，进入对话界面 |
| ⌨️ **输入文字** | `Tab` 切换中/英文，拼音直接拼，`1-5` 选字 |
| 📤 **发送** | `Enter` 发送消息，回复实时显示 |
| 🔙 **返回** | `Esc` 回到主菜单 |

---

## 🎮 完整键位表 / Controls

| 按键 | 主菜单 | 聊天模式 |
|------|--------|----------|
| `↑` / `↓` | 移动光标 | 滚动聊天历史 |
| `Enter` | 进入选中页面 | 确认拼音 / 发送消息 |
| `Esc` | — | 返回主菜单 |
| `Tab` | — | 切换中/英文输入法 |
| `Space` | — | 空格 / 确认首个候选字 |
| `1`–`5` | — | 选择对应拼音候选字 |
| `Backspace` | — | 退格删除 |

---

## 📁 项目结构 / Structure

```
cardputer-deepseek-chat/
├── include/
│   ├── config.h          # 全局配置
│   ├── display.h         # 显示驱动
│   ├── keyboard.h        # 键盘驱动
│   ├── api.h             # DeepSeek API 接口
│   ├── wifi_setup.h      # WiFiManager 配置
│   └── pinyin.h          # 拼音输入法
├── src/
│   ├── main.cpp          # 主程序 + 状态机
│   ├── display.cpp       # LCD 渲染
│   ├── keyboard.cpp      # 按键处理
│   ├── api.cpp           # HTTP 请求
│   ├── wifi_setup.cpp    # 配网逻辑
│   └── pinyin.cpp        # 拼音字典 & IME
├── platformio.ini        # PlatformIO 配置
├── firmware.bin          # 📥 预编译固件
└── LICENSE                # GPL v3
```

---

## 🏗️ 技术栈 / Tech Stack

```
ESP32-S3  +  M5Cardputer  +  WiFiManager  +  ArduinoJson  +  DeepSeek API
   ↓            ↓              ↓                ↓               ↓
 双核240M     ST7789V2       配网门户         JSON解析        Chat Completions
 8MB Flash    TCA8418        AP模式           流式支持         OpenAI 兼容
```

---

## 📜 许可证 / License

<p align="center">
  <a href="LICENSE">
    <img src="https://www.gnu.org/graphics/gplv3-with-text-136x68.png" alt="GPL v3">
  </a>
</p>

```
Copyright (C) 2026 shi2233

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
```

---

<p align="center">
  ⭐ 如果这个项目对你有帮助，请点个 Star！<br>
  <i>If you find this project useful, please give it a Star!</i>
</p>
