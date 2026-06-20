#pragma once
#include <Arduino.h>

void displayInit();
void displayClear();
void displaySplash(const char* line1, const char* line2 = nullptr);

void displayDrawMainMenu(int selectedIndex, bool wifiConnected, bool hasApiKey);
void displayDrawSettings(bool wifiConnected, const char* wifiSSID, const char* apiKeyPreview, int selectedIndex);
void displayDrawChatHeader();
void displayChatArea(String messages[], const uint16_t colors[], int count, int scrollOffset);
void displayInputArea(const String &text, bool cursor, bool imeMode);
void displayCandidates(const char* candidates[], int count, const char* pinyin);
void displayThinking();
void displayStatusBar(bool wifiConnected, const char* model);
void displayError(const char* msg);
