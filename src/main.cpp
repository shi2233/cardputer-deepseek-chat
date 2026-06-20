#include <Arduino.h>
#include <WiFi.h>
#include <M5Cardputer.h>
#include "config.h"
#include "display.h"
#include "keyboard.h"
#include "api.h"
#include "wifi_setup.h"
#include "pinyin.h"

enum Page {
    PAGE_MENU,
    PAGE_SETTINGS,
    PAGE_CHAT,
    PAGE_ABOUT
};

struct ChatMessage {
    String role;
    String content;
};

static Page currentPage = PAGE_MENU;
static int menuIndex = 0;
static int settingsIndex = 0;

static ChatMessage messages[MAX_MESSAGES];
static int messageCount = 0;
static String inputBuffer = "";
static bool isWaiting = false;
static bool cursorVisible = true;
static unsigned long lastCursorBlink = 0;
static String streamingBuffer = "";
static int chatScrollOffset = 0;

enum InputMode { MODE_ENGLISH, MODE_PINYIN };
static InputMode currentMode = MODE_PINYIN;

static void addMessage(const String &role, const String &content) {
    if (messageCount >= MAX_MESSAGES) {
        for (int i = 0; i < MAX_MESSAGES - 1; i++) messages[i] = messages[i + 1];
        messageCount = MAX_MESSAGES - 1;
    }
    messages[messageCount].role = role;
    messages[messageCount].content = content;
    messageCount++;
}

static void renderChat() {
    String msgTexts[MAX_MESSAGES];
    uint16_t msgColors[MAX_MESSAGES];
    for (int i = 0; i < messageCount; i++) {
        msgTexts[i] = messages[i].content;
        msgColors[i] = (messages[i].role == "user") ? COLOR_USER_LABEL : COLOR_AI_LABEL;
    }
    displayChatArea(msgTexts, msgColors, messageCount, chatScrollOffset);
}

static void renderChatAll() {
    displayStatusBar(wifiIsConnected(), DEEPSEEK_MODEL);
    renderChat();
    bool imeMode = (currentMode == MODE_PINYIN);
    displayInputArea(inputBuffer, cursorVisible, imeMode);
    if (currentMode == MODE_PINYIN && pinyinIsActive()) {
        const char* cands[MAX_CANDIDATES];
        int count = pinyinGetCandidates(cands, MAX_CANDIDATES);
        displayCandidates(cands, count, pinyinGetText());
    } else {
        displayCandidates(nullptr, 0, "");
    }
}

static void commitPinyinChar(int index) {
    char utf8[5] = {0};
    if (pinyinSelectCandidate(index, utf8, sizeof(utf8))) {
        inputBuffer += utf8;
        pinyinClear();
    }
}

static void sendChat() {
    if (inputBuffer.isEmpty() || isWaiting) return;
    String userMsg = inputBuffer;
    inputBuffer = "";
    pinyinClear();
    chatScrollOffset = 0;
    addMessage("user", userMsg);
    renderChatAll();
    displayThinking();
    isWaiting = true;

    String apiMessages[MAX_MESSAGES][2];
    apiMessages[0][0] = "system";
    apiMessages[0][1] = "You are a helpful assistant. Reply concisely in Chinese.";
    int totalApiMsgs = 1;
    for (int i = 0; i < messageCount; i++) {
        apiMessages[totalApiMsgs][0] = messages[i].role;
        apiMessages[totalApiMsgs][1] = messages[i].content;
        totalApiMsgs++;
    }

    streamingBuffer = "";
    String response = apiSendChat(apiMessages, totalApiMsgs, [](const char* chunk) {
        streamingBuffer += chunk;
        if (messageCount > 0 && messages[messageCount - 1].role == "assistant") {
            messages[messageCount - 1].content = streamingBuffer;
        } else {
            addMessage("assistant", streamingBuffer);
        }
        renderChat();
        displayInputArea("", false, false);
    });

    if (streamingBuffer.isEmpty()) {
        addMessage("assistant", response);
    } else if (messages[messageCount - 1].content != streamingBuffer) {
        messages[messageCount - 1].content = streamingBuffer;
    }
    isWaiting = false;
    streamingBuffer = "";
    renderChatAll();
}

static void handleMenuPage() {
    if (keyboardUpPressed()) {
        menuIndex--;
        if (menuIndex < 0) menuIndex = 2;
        displayDrawMainMenu(menuIndex, wifiIsConnected(), !wifiGetApiKey().isEmpty());
    }
    if (keyboardDownPressed()) {
        menuIndex++;
        if (menuIndex > 2) menuIndex = 0;
        displayDrawMainMenu(menuIndex, wifiIsConnected(), !wifiGetApiKey().isEmpty());
    }
    if (keyboardEnterPressed()) {
        if (menuIndex == 0) {
            if (!wifiIsConnected()) {
                displaySplash("No WiFi", "\xe8\xaf\xb7\xe5\x85\x88\xe9\x85\x8d\xe7\xbd\xaeWiFi");
                delay(1500);
                displayDrawMainMenu(menuIndex, wifiIsConnected(), !wifiGetApiKey().isEmpty());
                return;
            }
            if (wifiGetApiKey().isEmpty()) {
                displaySplash("No API Key", "\xe8\xaf\xb7\xe5\x85\x88\xe8\xae\xbe\xe7\xbd\xae");
                delay(1500);
                displayDrawMainMenu(menuIndex, wifiIsConnected(), !wifiGetApiKey().isEmpty());
                return;
            }
            currentPage = PAGE_CHAT;
            displayDrawChatHeader();
            if (messageCount == 0) {
                addMessage("system", "\xe5\xb0\xb1\xe7\xbb\xaa\xef\xbc\x81Tab\xe5\x88\x87\xe6\x8d\xa2\xe4\xb8\xad/\xe8\x8b\xb1 Esc\xe8\xbf\x94\xe5\x9b\x9e");
            }
            renderChatAll();
        } else if (menuIndex == 1) {
            currentPage = PAGE_SETTINGS;
            settingsIndex = 0;
            String ssid = WiFi.SSID();
            String key = wifiGetApiKey();
            char keyPreview[16] = "";
            if (key.length() > 8) {
                strncpy(keyPreview, key.c_str(), 6);
                keyPreview[6] = 0;
            } else if (key.length() > 0) {
                strncpy(keyPreview, key.c_str(), sizeof(keyPreview) - 1);
            }
            displayDrawSettings(wifiIsConnected(), ssid.c_str(), keyPreview, settingsIndex);
        } else if (menuIndex == 2) {
            currentPage = PAGE_ABOUT;
            displayClear();
            M5Cardputer.Display.setFont(&CN_FONT);
            M5Cardputer.Display.setTextColor(COLOR_AI_LABEL);
            M5Cardputer.Display.setCursor(10, 15);
            M5Cardputer.Display.print("DeepSeek Chat");
            M5Cardputer.Display.setTextColor(COLOR_TEXT);
            M5Cardputer.Display.setCursor(10, 35);
            M5Cardputer.Display.print("Cardputer ADV");
            M5Cardputer.Display.setCursor(10, 55);
            M5Cardputer.Display.print("v1.0");
            M5Cardputer.Display.setFont(&fonts::Font0);
            M5Cardputer.Display.setTextColor(COLOR_HINT);
            M5Cardputer.Display.setCursor(4, SCREEN_HEIGHT - 10);
            M5Cardputer.Display.print("Esc=Back");
        }
    }
}

static void handleSettingsPage() {
    String ssid = WiFi.SSID();
    String key = wifiGetApiKey();
    char keyPreview[16] = "";
    if (key.length() > 8) {
        strncpy(keyPreview, key.c_str(), 6);
        keyPreview[6] = 0;
    } else if (key.length() > 0) {
        strncpy(keyPreview, key.c_str(), sizeof(keyPreview) - 1);
    }

    if (keyboardUpPressed()) {
        settingsIndex--;
        if (settingsIndex < 0) settingsIndex = 3;
        displayDrawSettings(wifiIsConnected(), ssid.c_str(), keyPreview, settingsIndex);
    }
    if (keyboardDownPressed()) {
        settingsIndex++;
        if (settingsIndex > 3) settingsIndex = 0;
        displayDrawSettings(wifiIsConnected(), ssid.c_str(), keyPreview, settingsIndex);
    }
    if (keyboardEscPressed()) {
        currentPage = PAGE_MENU;
        displayDrawMainMenu(menuIndex, wifiIsConnected(), !wifiGetApiKey().isEmpty());
    }
    if (keyboardEnterPressed()) {
        if (settingsIndex == 0 || settingsIndex == 2) {
            displaySplash("WiFi Config", "\xe5\x90\xaf\xe5\x8a\xa8\xe9\x85\x8d\xe7\xbd\xae\xe9\xa1\xb5\xe9\x9d\xa2...");
            delay(500);
            wifiSetup(true);
            String newKey = wifiGetApiKey();
            if (!newKey.isEmpty()) {
                apiSetKey(newKey.c_str());
            }
            displayDrawSettings(wifiIsConnected(), WiFi.SSID().c_str(), keyPreview, settingsIndex);
        } else if (settingsIndex == 1) {
            displaySplash("API Key Config", "\xe5\x90\xaf\xe5\x8a\xa8\xe9\x85\x8d\xe7\xbd\xae\xe9\xa1\xb5\xe9\x9d\xa2...");
            delay(500);
            wifiSetup(true);
            String newKey = wifiGetApiKey();
            if (!newKey.isEmpty()) {
                apiSetKey(newKey.c_str());
            }
            key = wifiGetApiKey();
            memset(keyPreview, 0, sizeof(keyPreview));
            if (key.length() > 8) {
                strncpy(keyPreview, key.c_str(), 6);
            } else if (key.length() > 0) {
                strncpy(keyPreview, key.c_str(), sizeof(keyPreview) - 1);
            }
            displayDrawSettings(wifiIsConnected(), WiFi.SSID().c_str(), keyPreview, settingsIndex);
        } else if (settingsIndex == 3) {
            currentPage = PAGE_MENU;
            displayDrawMainMenu(menuIndex, wifiIsConnected(), !wifiGetApiKey().isEmpty());
        }
    }
}

static void handleChatPage() {
    if (isWaiting) return;

    if (keyboardEscPressed()) {
        currentPage = PAGE_MENU;
        displayDrawMainMenu(menuIndex, wifiIsConnected(), !wifiGetApiKey().isEmpty());
        return;
    }

    if (keyboardUpPressed()) {
        chatScrollOffset += 2;
        renderChat();
        return;
    }
    if (keyboardDownPressed()) {
        chatScrollOffset -= 2;
        if (chatScrollOffset < 0) chatScrollOffset = 0;
        renderChat();
        return;
    }

    if (keyboardTabPressed()) {
        if (currentMode == MODE_PINYIN) {
            if (pinyinIsActive()) {
                inputBuffer += pinyinGetText();
                pinyinClear();
            }
            currentMode = MODE_ENGLISH;
        } else {
            currentMode = MODE_PINYIN;
        }
        renderChatAll();
    }

    if (keyboardEnterPressed()) {
        if (currentMode == MODE_PINYIN && pinyinIsActive()) {
            const char* cands[MAX_CANDIDATES];
            int count = pinyinGetCandidates(cands, MAX_CANDIDATES);
            if (count > 0) {
                commitPinyinChar(0);
            } else {
                inputBuffer += pinyinGetText();
                pinyinClear();
            }
        }
        sendChat();
    } else if (keyboardSpacePressed()) {
        if (currentMode == MODE_PINYIN && pinyinIsActive()) {
            const char* cands[MAX_CANDIDATES];
            int count = pinyinGetCandidates(cands, MAX_CANDIDATES);
            if (count > 0) {
                commitPinyinChar(0);
            } else {
                inputBuffer += pinyinGetText();
                pinyinClear();
            }
            renderChatAll();
        } else {
            if (inputBuffer.length() < MAX_INPUT_LENGTH) inputBuffer += ' ';
        }
    } else if (keyboardNumberPressed()) {
        int num = keyboardGetNumber();
        if (currentMode == MODE_PINYIN && pinyinIsActive() && num >= 1 && num <= MAX_CANDIDATES) {
            const char* cands[MAX_CANDIDATES];
            int count = pinyinGetCandidates(cands, MAX_CANDIDATES);
            if (num <= count) {
                commitPinyinChar(num - 1);
                renderChatAll();
            }
        } else if (currentMode == MODE_ENGLISH || !pinyinIsActive()) {
            if (inputBuffer.length() < MAX_INPUT_LENGTH) inputBuffer += (char)('0' + num);
        }
    } else if (keyboardBackspacePressed()) {
        if (currentMode == MODE_PINYIN && pinyinIsActive()) {
            if (!pinyinBackspace()) {
                if (inputBuffer.length() > 0) {
                    int removeLen = 1;
                    if (inputBuffer.length() >= 3) {
                        unsigned char lastByte = inputBuffer[inputBuffer.length() - 1];
                        if (lastByte >= 0x80) {
                            int pos = inputBuffer.length() - 1;
                            while (pos > 0 && (inputBuffer[pos] & 0xC0) == 0x80) pos--;
                            removeLen = inputBuffer.length() - pos;
                        }
                    }
                    inputBuffer.remove(inputBuffer.length() - removeLen);
                }
            }
        } else {
            if (inputBuffer.length() > 0) {
                int removeLen = 1;
                if (inputBuffer.length() >= 3) {
                    unsigned char lastByte = inputBuffer[inputBuffer.length() - 1];
                    if (lastByte >= 0x80) {
                        int pos = inputBuffer.length() - 1;
                        while (pos > 0 && (inputBuffer[pos] & 0xC0) == 0x80) pos--;
                        removeLen = inputBuffer.length() - pos;
                    }
                }
                inputBuffer.remove(inputBuffer.length() - removeLen);
            }
        }
        renderChatAll();
    } else if (keyboardHasNewChar()) {
        char c = keyboardGetChar();
        if (c == ' ' || c == '\t' || (c >= '0' && c <= '9')) {
        } else if (currentMode == MODE_PINYIN) {
            if (c >= 'a' && c <= 'z') { pinyinAddChar(c); renderChatAll(); }
            else if (c >= 'A' && c <= 'Z') { pinyinAddChar(c + 32); renderChatAll(); }
            else if (c >= 33 && c <= 126 && inputBuffer.length() < MAX_INPUT_LENGTH) {
                if (pinyinIsActive()) { inputBuffer += pinyinGetText(); pinyinClear(); }
                inputBuffer += c;
                renderChatAll();
            }
        } else {
            if (c >= 32 && c <= 126 && inputBuffer.length() < MAX_INPUT_LENGTH) {
                inputBuffer += c;
                displayInputArea(inputBuffer, cursorVisible, false);
            }
        }
    }

    if (millis() - lastCursorBlink > 500) {
        cursorVisible = !cursorVisible;
        lastCursorBlink = millis();
        bool imeMode = (currentMode == MODE_PINYIN);
        displayInputArea(inputBuffer, cursorVisible, imeMode);
    }
}

static void handleAboutPage() {
    if (keyboardEscPressed() || keyboardEnterPressed()) {
        currentPage = PAGE_MENU;
        displayDrawMainMenu(menuIndex, wifiIsConnected(), !wifiGetApiKey().isEmpty());
    }
}

void setup() {
    auto cfg = M5.config();
    M5Cardputer.begin(cfg);
    displayInit();
    displaySplash("DeepSeek Chat", "\xe5\x8d\xa1\xe7\x89\x87\xe7\x94\xb5\xe8\x84\x91 ADV");
    delay(1500);

    displaySplash("\xe8\xbf\x9e\xe6\x8e\xa5WiFi...", "");
    if (!wifiSetup(false)) {
        displaySplash("WiFi Failed", "\xe8\xbf\x9b\xe5\x85\xa5\xe8\x8f\x9c\xe5\x8d\x95\xe9\x85\x8d\xe7\xbd\xae");
        delay(2000);
    }

    String apiKey = wifiGetApiKey();
    if (!apiKey.isEmpty()) {
        apiInit(apiKey.c_str());
    }

    keyboardInit();
    pinyinInit();
    displayDrawMainMenu(menuIndex, wifiIsConnected(), !wifiGetApiKey().isEmpty());
}

void loop() {
    M5Cardputer.update();
    keyboardUpdate();

    switch (currentPage) {
        case PAGE_MENU:     handleMenuPage(); break;
        case PAGE_SETTINGS: handleSettingsPage(); break;
        case PAGE_CHAT:     handleChatPage(); break;
        case PAGE_ABOUT:    handleAboutPage(); break;
    }

    delay(10);
}
