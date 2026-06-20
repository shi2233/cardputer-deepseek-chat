#include "config.h"
#include "wifi_setup.h"
#include "display.h"
#include <WiFi.h>
#include <WiFiManager.h>
#include <Preferences.h>

static String _apiKey = "";
static Preferences _prefs;

static void saveApiKeyCallback() {}

bool wifiSetup(bool forceConfigPortal) {
    WiFiManager wm;
    wm.setConfigPortalTimeout(300);
    wm.setConnectTimeout(15);

    _prefs.begin(PREF_NAMESPACE, false);
    _apiKey = _prefs.getString(PREF_API_KEY, "");

    char apiKeyBuf[128] = "";
    if (_apiKey.length() > 0) {
        _apiKey.toCharArray(apiKeyBuf, sizeof(apiKeyBuf));
    }

    WiFiManagerParameter customApiKey("api_key", "DeepSeek API Key", apiKeyBuf, 128);
    wm.addParameter(&customApiKey);

    bool connected = false;
    if (forceConfigPortal) {
        displaySplash("Config Portal", WIFI_AP_NAME);
        connected = wm.startConfigPortal(WIFI_AP_NAME, WIFI_AP_PASSWORD);
    } else {
        displaySplash("Connecting WiFi...", WiFi.SSID().c_str());
        connected = wm.autoConnect(WIFI_AP_NAME, WIFI_AP_PASSWORD);
    }

    String newKey = customApiKey.getValue();
    if (newKey.length() > 0 && newKey != _apiKey) {
        _apiKey = newKey;
        _prefs.putString(PREF_API_KEY, _apiKey);
    }
    _prefs.end();

    return connected;
}

bool wifiIsConnected() {
    return WiFi.status() == WL_CONNECTED;
}

String wifiGetIP() {
    return WiFi.localIP().toString();
}

String wifiGetApiKey() {
    return _apiKey;
}

void wifiSaveApiKey(const String& key) {
    _apiKey = key;
    _prefs.begin(PREF_NAMESPACE, false);
    _prefs.putString(PREF_API_KEY, _apiKey);
    _prefs.end();
}

String wifiLoadApiKey() {
    _prefs.begin(PREF_NAMESPACE, true);
    String key = _prefs.getString(PREF_API_KEY, "");
    _prefs.end();
    return key;
}
