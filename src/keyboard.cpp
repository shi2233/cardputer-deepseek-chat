#include "keyboard.h"
#include <M5Cardputer.h>

static char _lastChar = 0;
static bool _enterPressed = false;
static bool _backspacePressed = false;
static bool _tabPressed = false;
static bool _spacePressed = false;
static bool _escPressed = false;
static bool _upPressed = false;
static bool _downPressed = false;
static bool _leftPressed = false;
static bool _rightPressed = false;
static bool _numberPressed = false;
static int _numberValue = -1;
static bool _hasNewChar = false;
static bool _anyKeyPressed = false;

void keyboardInit() {
    M5Cardputer.Keyboard.begin();
}

void keyboardUpdate() {
    _lastChar = 0;
    _enterPressed = false;
    _backspacePressed = false;
    _tabPressed = false;
    _spacePressed = false;
    _escPressed = false;
    _upPressed = false;
    _downPressed = false;
    _leftPressed = false;
    _rightPressed = false;
    _numberPressed = false;
    _numberValue = -1;
    _hasNewChar = false;
    _anyKeyPressed = false;

    M5Cardputer.Keyboard.updateKeysState();
    if (M5Cardputer.Keyboard.isChange()) {
        if (M5Cardputer.Keyboard.isPressed()) {
            auto &state = M5Cardputer.Keyboard.keysState();
            for (auto key : state.word) {
                _lastChar = key;
                _hasNewChar = true;
                _anyKeyPressed = true;
            }
            if (state.enter) { _enterPressed = true; _anyKeyPressed = true; }
            if (state.del) { _backspacePressed = true; _anyKeyPressed = true; }
            if (state.tab) { _tabPressed = true; _anyKeyPressed = true; }
            if (state.space) { _spacePressed = true; _anyKeyPressed = true; }
            if (state.esc) { _escPressed = true; _anyKeyPressed = true; }
            if (state.up) { _upPressed = true; _anyKeyPressed = true; }
            if (state.down) { _downPressed = true; _anyKeyPressed = true; }
            if (state.left) { _leftPressed = true; _anyKeyPressed = true; }
            if (state.right) { _rightPressed = true; _anyKeyPressed = true; }
            if (_hasNewChar && _lastChar >= '0' && _lastChar <= '9') {
                _numberPressed = true;
                _numberValue = _lastChar - '0';
            }
        }
    }
}

bool keyboardHasNewChar() { return _hasNewChar; }
char keyboardGetChar() { return _lastChar; }
bool keyboardEnterPressed() { return _enterPressed; }
bool keyboardBackspacePressed() { return _backspacePressed; }
bool keyboardTabPressed() { return _tabPressed; }
bool keyboardSpacePressed() { return _spacePressed; }
bool keyboardEscPressed() { return _escPressed; }
bool keyboardUpPressed() { return _upPressed; }
bool keyboardDownPressed() { return _downPressed; }
bool keyboardLeftPressed() { return _leftPressed; }
bool keyboardRightPressed() { return _rightPressed; }
bool keyboardNumberPressed() { return _numberPressed; }
int keyboardGetNumber() { return _numberValue; }
bool keyboardKeyPressed() { return _anyKeyPressed; }
