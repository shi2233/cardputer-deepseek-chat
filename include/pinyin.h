#pragma once
#include <Arduino.h>

void pinyinInit();
bool pinyinAddChar(char c);
bool pinyinBackspace();
const char* pinyinGetText();
int pinyinGetCandidates(const char** outCandidates, int maxCount);
bool pinyinSelectCandidate(int index, char* outUtf8, int outLen);
void pinyinClear();
bool pinyinIsActive();
