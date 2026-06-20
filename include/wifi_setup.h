#pragma once
#include <Arduino.h>

bool wifiSetup(bool forceConfigPortal);
bool wifiIsConnected();
String wifiGetIP();
String wifiGetApiKey();
void wifiSaveApiKey(const String& key);
String wifiLoadApiKey();
