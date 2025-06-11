/**
 * @file TelegramBot.h
 * @brief Clase para interactuar con la API de Telegram mediante un bot.
 * 
 * Esta clase permite enviar mensajes y recibir actualizaciones (mensajes nuevos) desde un bot de Telegram.
 */
#ifndef TELEGRAM_BOT_H
#define TELEGRAM_BOT_H

// Librerías
#include <WiFiClientSecure.h>
#include <HTTPClient.h>

/**
 * @class TelegramBot
 * @brief Clase para manejar la comunicación con la API de Telegram.
 * 
 * Proporciona métodos para enviar mensajes y obtener mensajes nuevos utilizando un bot de Telegram.
 */
class TelegramBot {
private:
    String botToken; ///< Token del bot de Telegram.
    String chatId; ///< ID del chat al que se enviarán los mensajes.
    String apiHost = "https://api.telegram.org"; ///< URL base de la API de Telegram.


public:
    /**
     * @brief Constructor de la clase TelegramBot.
     * @param token Token del bot de Telegram.
     * @param chat_id ID del chat al que se enviarán los mensajes.
     */
    TelegramBot(const String& token, const String& chat_id)
        : botToken(token), chatId(chat_id) {}

    /**
     * @brief Envía un mensaje al chat configurado.
     * 
     * Este método utiliza la API de Telegram para enviar un mensaje al chat especificado.
     * 
     * @param message Mensaje a enviar.
     * @return true si el mensaje se envió correctamente, false en caso de error.
     */
    bool sendMessage(const String& message) {
        if (WiFi.status() != WL_CONNECTED) return false;

        WiFiClientSecure client;
        client.setInsecure();

        HTTPClient https;
        String url = apiHost + "/bot" + botToken + "/sendMessage";

        String payload = "chat_id=" + chatId + "&text=" + urlencode(message);

        https.begin(client, url);
        https.addHeader("Content-Type", "application/x-www-form-urlencoded");

        int httpCode = https.POST(payload);
        https.end();

        return httpCode == 200;
    }

    /**
     * @brief Obtiene el siguiente mensaje nuevo del chat.
     * 
     * Este método utiliza la API de Telegram para obtener mensajes nuevos desde el bot.
     * 
     * @param lastUpdateId ID de la última actualización procesada. Se actualiza con el ID del último mensaje recibido.
     * @return Texto del mensaje recibido o una cadena vacía si no hay mensajes nuevos.
     */
    String getNextMessage(int &lastUpdateId) {
        if (WiFi.status() != WL_CONNECTED) return "";

        WiFiClientSecure client;
        client.setInsecure();

        HTTPClient https;
        String url = apiHost + "/bot" + botToken + "/getUpdates?offset=" + String(lastUpdateId + 1);

        https.begin(client, url);
        int httpCode = https.GET();

        if (httpCode != 200) {
            https.end();
            return "";
        }

        String response = https.getString();
        https.end();

        int updateIdIndex = response.indexOf("\"update_id\":");
        if (updateIdIndex == -1) return "";

        int idStart = updateIdIndex + 12;
        int idEnd = response.indexOf(",", idStart);
        int updateId = response.substring(idStart, idEnd).toInt();

        if (updateId <= lastUpdateId) return "";

        lastUpdateId = updateId;

        int textIndex = response.indexOf("\"text\":\"");
        if (textIndex == -1) return "";

        int start = textIndex + 8;
        int end = response.indexOf("\"", start);
        String text = response.substring(start, end);

        return text;
    }

private:
    /**
     * @brief Codifica una cadena en formato URL.
     * 
     * Este método convierte caracteres especiales en su representación codificada para ser utilizada en URLs.
     * 
     * @param str Cadena a codificar.
     * @return Cadena codificada en formato URL.
     */
    String urlencode(const String& str) {
        String encoded = "";
        char c;
        char code0;
        char code1;
        for (int i = 0; i < str.length(); i++) {
            c = str.charAt(i);
            if (isalnum(c)) {
                encoded += c;
            } else {
                code0 = (c >> 4) & 0xF;
                code1 = c & 0xF;
                encoded += '%';
                encoded += char(code0 > 9 ? code0 - 10 + 'A' : code0 + '0');
                encoded += char(code1 > 9 ? code1 - 10 + 'A' : code1 + '0');
            }
        }
        return encoded;
    }
};

#endif // TELEGRAM_BOT_H
