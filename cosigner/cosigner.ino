// Byteduino Cosigner Device - papabyte.com
// MIT License

#include <Arduino.h>
#include <ESP8266WiFiMulti.h>
#include <byteduino.h>

#include <ESP8266WebServer.h>

ESP8266WiFiMulti WiFiMulti;

WiFiServer server(80);
void sendWebpage(WiFiClient &client);

void setup() {

  Serial.begin(115200);

  while (!Serial)
    continue;

  setDeviceName("Byteduino");
  setHub("byteball.org/bb");

  //don't forget to change the keys below, you will get troubles if more than 1 device is connected using the same keys
  setPrivateKeyM1("lgVGw/OfKKK4NqtK9fmJjbLCkLv7BGLetrdvsKAngWY=");
  setExtPubKey("xpub6Chxqf8hRQoLRJFS8mhFCDv5yzUNC7rhhh36qqqt1WtAZcmCNhS5pPndqhx8RmgwFhGPa9FYq3iTXNBkYdkrAKJxa7qnahnAvCzKW5dnfJn");
  setPrivateKeyM4400("qA1CxKyzvpKX9+IRhLH8OjLYY06H3RLl+zqc3lT86aI=");

  WiFi.softAPdisconnect(true);
  WiFiMulti.addAP("my_SSID", "my_password");

  while (WiFiMulti.run() != WL_CONNECTED) {
    delay(100);
    Serial.println(F("Not connected to wifi yet"));
  }
  Serial.println(WiFi.localIP());


  server.begin();
}

void sendWalletsJson(WiFiClient &client) {
  client.flush();
  client.print(getWalletsJsonString());

}

void sendPairedDevicesJson(WiFiClient &client) {
  client.flush();
  client.print(getDevicesJsonString());
}

void sendDeviceInfosJson(WiFiClient &client) {

  client.flush();
  client.print(getDeviceInfosJsonString());
}

void sendOnGoingSignatureJson(WiFiClient &client) {
  client.flush();
  client.print(getOnGoingSignatureJsonString());
}

void loop() {
  // put your main code here, to run repeatedly:
  byteduino_loop();

  WiFiClient client = server.available();

  if (client) {
    // an http request ends with a blank line
    boolean currentLineIsBlank = true;

    int i = 0;
    char bufferClient[300];
    char textToSign[45];
    while (client.connected()) {
      while (client.available()) {
        char c = client.read();
        if (i < 300) {
          bufferClient[i] = c;
          i++;
        }
        // if you've gotten to the end of the line (received a newline
        // character) and the line is blank, the http request has ended,
        // so you can send a reply
        if (c == '\n' && currentLineIsBlank) {

          // Here is where the POST data is.
          int j = 0;
          while (client.available())
          {
            textToSign[j] = client.read();
            j++;
          }
          textToSign[44] = 0x00;
          Serial.println(textToSign);
          // Serial.println("Sending response");
          // send a standard http response header

          if (strstr(bufferClient, "ongoing_signature.json") != nullptr) {
            sendOnGoingSignatureJson(client);
          } else if (strstr(bufferClient, "paired_devices.json") != nullptr) {
            sendPairedDevicesJson(client);
          } else if (strstr(bufferClient, "device_infos.json") != nullptr) {
            sendDeviceInfosJson(client);
          } else if (strstr(bufferClient, "wallets.json") != nullptr) {
            sendWalletsJson(client);
          } else if (strstr(bufferClient, "refuse_signature") != nullptr) {
            refuseTosign(textToSign);
            Serial.println("deny signature");
          } else if (strstr(bufferClient, "confirm_signature") != nullptr) {
            acceptToSign(textToSign);
            Serial.println("confirm signature");
          } else if (strstr(bufferClient, "favico") == nullptr ) {
            sendWebpage(client);
          }


          client.stop();
        }
        else if (c == '\n') {
          // you're starting a new line
          currentLineIsBlank = true;
        }
        else if (c != '\r') {
          // you've gotten a character on the current line
          currentLineIsBlank = false;
        }
      }
    }
    Serial.println("Disconnected");
  }

}
