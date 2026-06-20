#include "config.h"
#include "display.h"
#include <M5Cardputer.h>

static int getUtf8CharPixelWidth(const char* s, bool useCnFont) {
    unsigned char c = (unsigned char)s[0];
    if (c < 0x80) return ASCII_CHAR_WIDTH;
    return CN_CHAR_WIDTH;
}

static int getUtf8ByteLen(const char* s) {
    unsigned char c = (unsigned char)s[0];
    if (c < 0x80) return 1;
    if ((c & 0xE0) == 0xC0) return 2;
    if ((c & 0xF0) == 0xE0) return 3;
    if ((c & 0xF8) == 0xF0) return 4;
    return 1;
}

static int getStringPixelWidth(const char* s, bool useCnFont) {
    int w = 0;
    const char* p = s;
    while (*p) {
        w += getUtf8CharPixelWidth(p, useCnFont);
        p += getUtf8ByteLen(p);
    }
    return w;
}

void displayInit() {
    M5Cardputer.Display.setRotation(1);
    M5Cardputer.Display.setTextSize(1);
    M5Cardputer.Display.setTextColor(COLOR_TEXT);
    displayClear();
}

void displayClear() {
    M5Cardputer.Display.fillScreen(COLOR_BG);
}

void displaySplash(const char* line1, const char* line2) {
    displayClear();
    M5Cardputer.Display.setFont(&CN_FONT);
    M5Cardputer.Display.setTextColor(COLOR_AI_LABEL);
    int w1 = strlen(line1) * ASCII_CHAR_WIDTH;
    int x1 = (SCREEN_WIDTH - w1) / 2;
    M5Cardputer.Display.setCursor(x1 > 0 ? x1 : 0, 35);
    M5Cardputer.Display.print(line1);
    if (line2) {
        M5Cardputer.Display.setTextColor(COLOR_SYSTEM);
        int w2 = getStringPixelWidth(line2, true);
        int x2 = (SCREEN_WIDTH - w2) / 2;
        M5Cardputer.Display.setCursor(x2 > 0 ? x2 : 0, 55);
        M5Cardputer.Display.print(line2);
    }
    M5Cardputer.Display.setFont(&fonts::Font0);
}

static void drawMenuItem(int y, const char* icon, const char* label, const char* sub, bool selected) {
    uint16_t bgColor = selected ? COLOR_MENU_SEL_BG : COLOR_BG;
    uint16_t textColor = selected ? COLOR_MENU_SEL_TEXT : COLOR_TEXT;
    M5Cardputer.Display.fillRect(0, y, SCREEN_WIDTH, MENU_ITEM_HEIGHT, bgColor);
    M5Cardputer.Display.setFont(&CN_FONT);
    M5Cardputer.Display.setTextColor(textColor);
    int textY = y + (MENU_ITEM_HEIGHT - CN_FONT_HEIGHT) / 2;
    if (textY < y + 1) textY = y + 1;
    M5Cardputer.Display.setCursor(8, textY);
    M5Cardputer.Display.print(icon);
    int iconW = getStringPixelWidth(icon, true);
    M5Cardputer.Display.setCursor(8 + iconW + 6, textY);
    M5Cardputer.Display.print(label);
    if (sub && sub[0]) {
        M5Cardputer.Display.setFont(&fonts::Font0);
        M5Cardputer.Display.setTextColor(selected ? 0xBDF7 : COLOR_SYSTEM);
        M5Cardputer.Display.setCursor(SCREEN_WIDTH - strlen(sub) * ASCII_CHAR_WIDTH - 8, textY + 2);
        M5Cardputer.Display.print(sub);
    }
    M5Cardputer.Display.setFont(&fonts::Font0);
}

void displayDrawMainMenu(int selectedIndex, bool wifiConnected, bool hasApiKey) {
    displayClear();

    M5Cardputer.Display.fillRect(0, 0, SCREEN_WIDTH, 22, COLOR_TITLE_BG);
    M5Cardputer.Display.setFont(&CN_FONT);
    M5Cardputer.Display.setTextColor(COLOR_TITLE_TEXT);
    M5Cardputer.Display.setCursor(6, 5);
    M5Cardputer.Display.print("DeepSeek Chat");

    M5Cardputer.Display.setFont(&fonts::Font0);
    M5Cardputer.Display.setTextColor(wifiConnected ? COLOR_USER_LABEL : COLOR_ERROR);
    M5Cardputer.Display.setCursor(SCREEN_WIDTH - 50, 8);
    M5Cardputer.Display.print(wifiConnected ? "ONLINE" : "OFFLINE");

    int menuY = 26;
    const char* icons[] = {
        "\xe8\x81\x8a\xe5\xa4\xa9",
        "\xe8\xae\xbe\xe7\xbd\xae",
        "\xe5\x85\xb3\xe4\xba\x8e"
    };
    const char* labels[] = { "", "", "" };
    const char* subs[3] = { "", "", "" };

    if (wifiConnected && hasApiKey) {
        subs[0] = "Ready";
    } else if (!wifiConnected) {
        subs[0] = "No WiFi";
    } else {
        subs[0] = "No Key";
    }
    subs[1] = wifiConnected ? "OK" : "Config";
    subs[2] = "v1.0";

    for (int i = 0; i < 3; i++) {
        drawMenuItem(menuY + i * MENU_ITEM_HEIGHT, icons[i], labels[i], subs[i], i == selectedIndex);
    }

    M5Cardputer.Display.setFont(&fonts::Font0);
    M5Cardputer.Display.setTextColor(COLOR_HINT);
    M5Cardputer.Display.setCursor(4, SCREEN_HEIGHT - 10);
    M5Cardputer.Display.print("Up/Down  Enter=OK  Esc=Back");
}

void displayDrawSettings(bool wifiConnected, const char* wifiSSID, const char* apiKeyPreview, int selectedIndex) {
    displayClear();

    M5Cardputer.Display.fillRect(0, 0, SCREEN_WIDTH, 22, COLOR_TITLE_BG);
    M5Cardputer.Display.setFont(&CN_FONT);
    M5Cardputer.Display.setTextColor(COLOR_TITLE_TEXT);
    M5Cardputer.Display.setCursor(6, 5);
    M5Cardputer.Display.print("\xe8\xae\xbe\xe7\xbd\xae");
    M5Cardputer.Display.setFont(&fonts::Font0);

    int menuY = 26;

    char wifiStatus[32];
    if (wifiConnected) {
        snprintf(wifiStatus, sizeof(wifiStatus), "OK");
    } else {
        snprintf(wifiStatus, sizeof(wifiStatus), "Config");
    }
    drawMenuItem(menuY, "WiFi", "", wifiStatus, selectedIndex == 0);

    char keyStatus[32];
    if (apiKeyPreview && apiKeyPreview[0]) {
        snprintf(keyStatus, sizeof(keyStatus), "%s...%s",
            apiKeyPreview, apiKeyPreview + strlen(apiKeyPreview) - 4);
    } else {
        snprintf(keyStatus, sizeof(keyStatus), "None");
    }
    drawMenuItem(menuY + MENU_ITEM_HEIGHT, "API Key", "", keyStatus, selectedIndex == 1);

    drawMenuItem(menuY + MENU_ITEM_HEIGHT * 2,
        "\xe9\x87\x8d\xe6\x96\xb0\xe9\x85\x8d\xe7\xbd\xae", "", "", selectedIndex == 2);

    drawMenuItem(menuY + MENU_ITEM_HEIGHT * 3,
        "\xe8\xbf\x94\xe5\x9b\x9e\xe4\xb8\xbb\xe8\x8f\x9c\xe5\x8d\x95", "", "", selectedIndex == 3);

    M5Cardputer.Display.setFont(&fonts::Font0);
    M5Cardputer.Display.setTextColor(COLOR_HINT);
    M5Cardputer.Display.setCursor(4, SCREEN_HEIGHT - 10);
    M5Cardputer.Display.print("Up/Down  Enter=OK  Esc=Back");
}

void displayDrawChatHeader() {
    M5Cardputer.Display.fillRect(0, 0, SCREEN_WIDTH, STATUS_BAR_HEIGHT, COLOR_STATUS_BG);
    M5Cardputer.Display.setFont(&fonts::Font0);
    M5Cardputer.Display.setTextSize(FONT_SIZE);
    M5Cardputer.Display.setTextColor(COLOR_TITLE_TEXT);
    M5Cardputer.Display.setCursor(2, 2);
    M5Cardputer.Display.print("Chat | Esc=Menu");
}

void displayStatusBar(bool wifiConnected, const char* model) {
    M5Cardputer.Display.fillRect(0, 0, SCREEN_WIDTH, STATUS_BAR_HEIGHT, COLOR_STATUS_BG);
    M5Cardputer.Display.setFont(&fonts::Font0);
    M5Cardputer.Display.setTextSize(FONT_SIZE);
    M5Cardputer.Display.setCursor(2, 2);
    M5Cardputer.Display.setTextColor(wifiConnected ? COLOR_USER_LABEL : COLOR_ERROR);
    M5Cardputer.Display.print(wifiConnected ? "OK" : "--");
    M5Cardputer.Display.setTextColor(COLOR_STATUS_TEXT);
    M5Cardputer.Display.print(" Esc=Back");
    int freeHeap = ESP.getFreeHeap() / 1024;
    char heapStr[16];
    snprintf(heapStr, sizeof(heapStr), "%dK", freeHeap);
    M5Cardputer.Display.setTextColor(COLOR_SYSTEM);
    int hx = SCREEN_WIDTH - strlen(heapStr) * CHAR_WIDTH - 2;
    M5Cardputer.Display.setCursor(hx, 2);
    M5Cardputer.Display.print(heapStr);
}

static int drawWrappedText(const char* text, int startX, int startY, int maxWidth, uint16_t color, bool useCnFont, int areaBottom) {
    if (useCnFont) {
        M5Cardputer.Display.setFont(&CN_FONT);
    } else {
        M5Cardputer.Display.setFont(&fonts::Font0);
    }
    M5Cardputer.Display.setTextColor(color);
    int curX = startX;
    int curY = startY;
    int lineH = useCnFont ? (CN_FONT_HEIGHT + LINE_SPACING) : (CHAR_HEIGHT + LINE_SPACING);
    const char* p = text;
    while (*p) {
        int byteLen = getUtf8ByteLen(p);
        int charW = getUtf8CharPixelWidth(p, useCnFont);
        if (*p == '\n') {
            curX = startX;
            curY += lineH;
            p++;
            continue;
        }
        if (curX + charW > startX + maxWidth) {
            curX = startX;
            curY += lineH;
        }
        if (curY + lineH > areaBottom) break;
        M5Cardputer.Display.setCursor(curX, curY);
        char tmp[5] = {0};
        memcpy(tmp, p, byteLen);
        M5Cardputer.Display.print(tmp);
        curX += charW;
        p += byteLen;
    }
    M5Cardputer.Display.setFont(&fonts::Font0);
    return curY + lineH;
}

static int countTextLines(const char* text, int startX, int maxWidth, bool useCnFont) {
    int curX = startX;
    int lines = 1;
    const char* p = text;
    while (*p) {
        int charW = getUtf8CharPixelWidth(p, useCnFont);
        if (*p == '\n') {
            lines++;
            curX = startX;
            p++;
            continue;
        }
        if (curX + charW > startX + maxWidth) {
            lines++;
            curX = startX;
        }
        curX += charW;
        p += getUtf8ByteLen(p);
    }
    return lines;
}

static int drawTextWithSkip(const char* text, int startX, int startY, int maxWidth, uint16_t color, bool useCnFont, int areaBottom, int skipLines) {
    if (useCnFont) {
        M5Cardputer.Display.setFont(&CN_FONT);
    } else {
        M5Cardputer.Display.setFont(&fonts::Font0);
    }
    M5Cardputer.Display.setTextColor(color);
    int curX = startX;
    int curY = startY;
    int lineH = useCnFont ? (CN_FONT_HEIGHT + LINE_SPACING) : (CHAR_HEIGHT + LINE_SPACING);
    int lineNum = 0;
    const char* p = text;
    while (*p) {
        int byteLen = getUtf8ByteLen(p);
        int charW = getUtf8CharPixelWidth(p, useCnFont);
        if (*p == '\n') {
            lineNum++;
            curX = startX;
            if (lineNum > skipLines) curY += lineH;
            p++;
            continue;
        }
        if (curX + charW > startX + maxWidth) {
            lineNum++;
            curX = startX;
            if (lineNum > skipLines) curY += lineH;
        }
        if (lineNum >= skipLines) {
            if (curY + lineH > areaBottom) break;
            M5Cardputer.Display.setCursor(curX, curY);
            char tmp[5] = {0};
            memcpy(tmp, p, byteLen);
            M5Cardputer.Display.print(tmp);
        }
        curX += charW;
        p += byteLen;
    }
    M5Cardputer.Display.setFont(&fonts::Font0);
    return curY + lineH;
}

void displayChatArea(String messages[], const uint16_t colors[], int count, int scrollOffset) {
    M5Cardputer.Display.fillRect(0, CHAT_AREA_Y, SCREEN_WIDTH, CHAT_AREA_HEIGHT, COLOR_BG);
    if (count == 0) return;
    int lineH = CN_FONT_HEIGHT + LINE_SPACING;
    int maxVisible = CHAT_AREA_HEIGHT / lineH;
    int startX = 2;
    int maxWidth = SCREEN_WIDTH - 4;
    int areaBottom = CHAT_AREA_Y + CHAT_AREA_HEIGHT;
    int msgLines[MAX_MESSAGES];
    int totalLines = 0;
    for (int m = 0; m < count; m++) {
        String prefix = (colors[m] == COLOR_USER_LABEL) ? "> " : "< ";
        String full = prefix + messages[m];
        msgLines[m] = countTextLines(full.c_str(), startX, maxWidth, true);
        totalLines += msgLines[m];
    }
    int maxScroll = totalLines - maxVisible;
    if (maxScroll < 0) maxScroll = 0;
    if (scrollOffset < 0) scrollOffset = 0;
    if (scrollOffset > maxScroll) scrollOffset = maxScroll;
    int skipLines = maxScroll - scrollOffset;
    int linesSkipped = 0;
    int curY = CHAT_AREA_Y + 1;
    for (int m = 0; m < count; m++) {
        String prefix = (colors[m] == COLOR_USER_LABEL) ? "> " : "< ";
        String full = prefix + messages[m];
        if (linesSkipped + msgLines[m] <= skipLines) {
            linesSkipped += msgLines[m];
            continue;
        }
        int skipInMsg = 0;
        if (linesSkipped < skipLines) {
            skipInMsg = skipLines - linesSkipped;
        }
        curY = drawTextWithSkip(full.c_str(), startX, curY, maxWidth, colors[m], true, areaBottom, skipInMsg);
        linesSkipped += msgLines[m];
        if (curY > areaBottom - lineH) break;
    }
    if (skipLines > 0) {
        M5Cardputer.Display.setFont(&fonts::Font0);
        M5Cardputer.Display.setTextColor(COLOR_SYSTEM);
        M5Cardputer.Display.setCursor(SCREEN_WIDTH - 10, CHAT_AREA_Y + 1);
        M5Cardputer.Display.print("^");
    }
    if (scrollOffset > 0) {
        M5Cardputer.Display.setFont(&fonts::Font0);
        M5Cardputer.Display.setTextColor(COLOR_SYSTEM);
        M5Cardputer.Display.setCursor(SCREEN_WIDTH - 10, CHAT_AREA_Y + CHAT_AREA_HEIGHT - CHAR_HEIGHT);
        M5Cardputer.Display.print("v");
    }
}

void displayInputArea(const String &text, bool cursor, bool imeMode) {
    M5Cardputer.Display.fillRect(0, INPUT_AREA_Y, SCREEN_WIDTH, INPUT_AREA_HEIGHT, COLOR_INPUT_BG);
    M5Cardputer.Display.setFont(&CN_FONT);
    M5Cardputer.Display.setTextSize(1);
    M5Cardputer.Display.setTextColor(imeMode ? COLOR_PINYIN : COLOR_INPUT_TEXT);
    int curX = 2;
    int textY = INPUT_AREA_Y + (INPUT_AREA_HEIGHT - CN_FONT_HEIGHT) / 2;
    if (textY < INPUT_AREA_Y + 1) textY = INPUT_AREA_Y + 1;
    if (imeMode) {
        M5Cardputer.Display.setFont(&fonts::Font0);
        M5Cardputer.Display.setCursor(curX, textY + 2);
        M5Cardputer.Display.print("[PY]");
        curX += 4 * ASCII_CHAR_WIDTH;
        M5Cardputer.Display.setFont(&CN_FONT);
    }
    const char* p = text.c_str();
    int textEndX = curX;
    while (*p) {
        int byteLen = getUtf8ByteLen(p);
        int charW = getUtf8CharPixelWidth(p, true);
        if (curX + charW > SCREEN_WIDTH - 2) break;
        M5Cardputer.Display.setCursor(curX, textY);
        char tmp[5] = {0};
        memcpy(tmp, p, byteLen);
        M5Cardputer.Display.print(tmp);
        curX += charW;
        textEndX = curX;
        p += byteLen;
    }
    if (cursor) {
        M5Cardputer.Display.fillRect(textEndX, textY, 2, CN_FONT_HEIGHT, COLOR_INPUT_TEXT);
    }
    M5Cardputer.Display.setFont(&fonts::Font0);
}

void displayCandidates(const char* candidates[], int count, const char* pinyin) {
    M5Cardputer.Display.fillRect(0, CANDIDATE_AREA_Y, SCREEN_WIDTH, CANDIDATE_HEIGHT, COLOR_CANDIDATE_BG);
    int curY = CANDIDATE_AREA_Y + (CANDIDATE_HEIGHT - CN_FONT_HEIGHT) / 2;
    if (curY < CANDIDATE_AREA_Y + 1) curY = CANDIDATE_AREA_Y + 1;
    if (pinyin != nullptr && pinyin[0] != 0) {
        M5Cardputer.Display.setFont(&fonts::Font0);
        M5Cardputer.Display.setTextColor(COLOR_PINYIN);
        M5Cardputer.Display.setCursor(2, curY + 2);
        M5Cardputer.Display.print(pinyin);
        int pinyinW = strlen(pinyin) * ASCII_CHAR_WIDTH;
        M5Cardputer.Display.setTextColor(COLOR_CANDIDATE_TEXT);
        M5Cardputer.Display.print(" > ");
        int curX = 2 + pinyinW + 3 * ASCII_CHAR_WIDTH;
        if (count > 0) {
            for (int i = 0; i < count; i++) {
                if (curX + CN_CHAR_WIDTH + 2 * ASCII_CHAR_WIDTH + 6 > SCREEN_WIDTH) break;
                M5Cardputer.Display.setFont(&fonts::Font0);
                M5Cardputer.Display.setTextColor(COLOR_CANDIDATE_NUM);
                M5Cardputer.Display.setCursor(curX, curY + 2);
                char numStr[4];
                snprintf(numStr, sizeof(numStr), "%d:", i + 1);
                M5Cardputer.Display.print(numStr);
                curX += 2 * ASCII_CHAR_WIDTH;
                M5Cardputer.Display.setFont(&CN_FONT);
                M5Cardputer.Display.setTextColor(COLOR_CANDIDATE_TEXT);
                M5Cardputer.Display.setCursor(curX, curY);
                int charLen = getUtf8ByteLen(candidates[i]);
                char tmp[5] = {0};
                memcpy(tmp, candidates[i], charLen);
                M5Cardputer.Display.print(tmp);
                curX += CN_CHAR_WIDTH + 6;
            }
        }
    }
    M5Cardputer.Display.setFont(&fonts::Font0);
}

void displayThinking() {
    M5Cardputer.Display.fillRect(0, INPUT_AREA_Y, SCREEN_WIDTH, INPUT_AREA_HEIGHT, COLOR_INPUT_BG);
    M5Cardputer.Display.fillRect(0, CANDIDATE_AREA_Y, SCREEN_WIDTH, CANDIDATE_HEIGHT, COLOR_BG);
    M5Cardputer.Display.setFont(&CN_FONT);
    M5Cardputer.Display.setTextColor(COLOR_AI_LABEL);
    M5Cardputer.Display.setCursor(2, INPUT_AREA_Y + (INPUT_AREA_HEIGHT - CN_FONT_HEIGHT) / 2);
    M5Cardputer.Display.print("\xe6\x80\x9d\xe8\x80\x83\xe4\xb8\xad...");
    M5Cardputer.Display.setFont(&fonts::Font0);
}

void displayError(const char* msg) {
    M5Cardputer.Display.setFont(&CN_FONT);
    M5Cardputer.Display.setTextColor(COLOR_ERROR);
    M5Cardputer.Display.setCursor(4, CHAT_AREA_Y + CHAT_AREA_HEIGHT / 2 - 6);
    M5Cardputer.Display.print(msg);
    M5Cardputer.Display.setFont(&fonts::Font0);
}
