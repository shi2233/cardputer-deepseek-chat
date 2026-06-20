#include "config.h"
#include "api.h"
#include <WiFiClientSecure.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

static String _apiKey = "";
static bool _busy = false;

void apiInit(const char* apiKey) {
    _apiKey = apiKey;
}

void apiSetKey(const char* apiKey) {
    _apiKey = apiKey;
}

bool apiIsBusy() {
    return _busy;
}

String apiSendChat(const String messages[][2], int count, StreamCallback onChunk) {
    if (_apiKey.isEmpty()) return "Error: No API Key";
    if (_busy) return "Error: Busy";
    _busy = true;

    WiFiClientSecure client;
    client.setInsecure();
    HTTPClient http;
    http.begin(client, DEEPSEEK_API_URL);
    http.addHeader("Content-Type", "application/json");
    http.addHeader("Authorization", "Bearer " + _apiKey);
    http.setTimeout(API_TIMEOUT_MS);

    JsonDocument requestDoc;
    requestDoc["model"] = DEEPSEEK_MODEL;
    requestDoc["stream"] = (onChunk != nullptr);
    requestDoc["max_tokens"] = DEEPSEEK_MAX_TOKENS;
    JsonArray msgsArr = requestDoc["messages"].to<JsonArray>();
    for (int i = 0; i < count; i++) {
        JsonObject msg = msgsArr.add<JsonObject>();
        msg["role"] = messages[i][0];
        msg["content"] = messages[i][1];
    }

    String requestBody;
    serializeJson(requestDoc, requestBody);

    int httpCode = http.POST(requestBody);
    if (httpCode != 200) {
        String err = "HTTP " + String(httpCode);
        if (httpCode > 0) {
            String body = http.getString();
            JsonDocument errDoc;
            deserializeJson(errDoc, body);
            if (errDoc["error"]["message"].is<const char*>()) {
                err = errDoc["error"]["message"].as<const char*>();
            }
        }
        http.end();
        _busy = false;
        return "Error: " + err;
    }

    String fullResponse = "";

    if (onChunk) {
        WiFiClient* stream = http.getStreamPtr();
        String lineBuffer = "";
        unsigned long lastData = millis();
        while (stream->connected() || stream->available()) {
            if (stream->available()) {
                lastData = millis();
                char c = stream->read();
                lineBuffer += c;
                if (c == '\n') {
                    lineBuffer.trim();
                    if (lineBuffer.startsWith("data: ")) {
                        String data = lineBuffer.substring(6);
                        if (data == "[DONE]") {
                            break;
                        }
                        JsonDocument chunkDoc;
                        DeserializationError err = deserializeJson(chunkDoc, data);
                        if (!err) {
                            const char* delta = chunkDoc["choices"][0]["delta"]["content"];
                            if (delta) {
                                fullResponse += delta;
                                onChunk(delta);
                            }
                        }
                    }
                    lineBuffer = "";
                }
            } else {
                if (millis() - lastData > API_TIMEOUT_MS) break;
                delay(1);
            }
        }
    } else {
        String responseBody = http.getString();
        JsonDocument responseDoc;
        DeserializationError err = deserializeJson(responseDoc, responseBody);
        if (err) {
            http.end();
            _busy = false;
            return "Error: JSON parse failed";
        }
        const char* content = responseDoc["choices"][0]["message"]["content"];
        if (content) {
            fullResponse = content;
        } else {
            fullResponse = "Error: Empty response";
        }
    }

    http.end();
    _busy = false;
    return fullResponse;
}
