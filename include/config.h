#pragma once

#define WIFI_AP_NAME        "Cardputer-Chat"
#define WIFI_AP_PASSWORD    "12345678"

#define DEEPSEEK_API_URL    "https://api.deepseek.com/chat/completions"
#define DEEPSEEK_MODEL      "deepseek-chat"
#define DEEPSEEK_MAX_TOKENS 512
#define API_TIMEOUT_MS      60000

#define MAX_MESSAGES        20
#define MAX_INPUT_LENGTH    200
#define MAX_MESSAGE_LENGTH  1024

#define SCREEN_WIDTH        240
#define SCREEN_HEIGHT       135

#define STATUS_BAR_HEIGHT   10
#define INPUT_AREA_HEIGHT   18
#define CANDIDATE_HEIGHT    16
#define CHAT_AREA_Y         STATUS_BAR_HEIGHT
#define CHAT_AREA_HEIGHT    (SCREEN_HEIGHT - STATUS_BAR_HEIGHT - INPUT_AREA_HEIGHT - CANDIDATE_HEIGHT)
#define CANDIDATE_AREA_Y    (SCREEN_HEIGHT - INPUT_AREA_HEIGHT - CANDIDATE_HEIGHT)
#define INPUT_AREA_Y        (SCREEN_HEIGHT - INPUT_AREA_HEIGHT)

#define MENU_ITEM_HEIGHT    22
#define MENU_MAX_ITEMS      6

#define CN_FONT             fonts::efontCN_12
#define CN_FONT_HEIGHT      12
#define CN_CHAR_WIDTH       12
#define ASCII_CHAR_WIDTH    6
#define FONT_SIZE           1
#define CHAR_WIDTH          6
#define CHAR_HEIGHT         8
#define LINE_SPACING        1

#define COLOR_BG            0x0841
#define COLOR_TITLE_BG      0x18E3
#define COLOR_TITLE_TEXT     0xFFFF
#define COLOR_STATUS_BG     0x18E3
#define COLOR_STATUS_TEXT    0xFFFF
#define COLOR_USER_LABEL    0x07E0
#define COLOR_AI_LABEL      0x07FF
#define COLOR_TEXT           0xFFFF
#define COLOR_INPUT_BG      0x2104
#define COLOR_INPUT_TEXT     0xFFFF
#define COLOR_ERROR         0xF800
#define COLOR_SYSTEM        0x7BEF
#define COLOR_CANDIDATE_BG  0x3186
#define COLOR_CANDIDATE_NUM  0xFFE0
#define COLOR_CANDIDATE_TEXT 0xFFFF
#define COLOR_PINYIN        0x07FF
#define COLOR_MENU_SEL_BG   0x03FF
#define COLOR_MENU_SEL_TEXT  0x0000
#define COLOR_HINT          0x6B4D

#define PREF_NAMESPACE      "deepseek"
#define PREF_API_KEY        "api_key"

#define MAX_PINYIN_LEN      12
#define MAX_CANDIDATES      5
