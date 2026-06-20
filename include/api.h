#pragma once
#include <Arduino.h>
#include <functional>

typedef std::function<void(const char* chunk)> StreamCallback;

void apiInit(const char* apiKey);
void apiSetKey(const char* apiKey);
String apiSendChat(const String messages[][2], int count, StreamCallback onChunk = nullptr);
bool apiIsBusy();
