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
  setHub("byteball.org/bb-test"); //hub for testnet

  //don't forget to change the keys below, you will get troubles if more than 1 device is connected using the same keys
  setPrivateKeyM1("lgVGw/OfKKK4NqtK9fmJjbLCkLv7BGLetrdvsKAngWY=");
  setExtPubKey("xpub6Chxqf8hRQoLRJFS8mhFCDv5yzUNC7rhhh36qqqt1WtAZcmCNhS5pPndqhx8RmgwFhGPa9FYq3iTXNBkYdkrAKJxa7qnahnAvCzKW5dnfJn");
  setPrivateKeyM4400("qA1CxKyzvpKX9+IRhLH8OjLYY06H3RLl+zqc3lT86aI=");

  WiFi.softAPdisconnect(true);
  WiFiMulti.addAP("ben", "grosgros5");

  while (WiFiMulti.run() != WL_CONNECTED) {
    delay(100);
    Serial.println(F("Not connected to wifi yet"));
  }
  Serial.println(WiFi.localIP());


  server.begin();
}

void sendWalletsJson(WiFiClient &client) {
  char json[PAIRED_DEVICES];
  readWalletsJson(json);
  Serial.println(json);
  client.flush();
  client.print(json);
}

void sendPairedDevicesJson(WiFiClient &client) {
  char json[WALLETS_CREATED_FLASH_SIZE];
  readPairedDevicesJson(json);
  Serial.println(json);
  client.flush();
  client.print(json);
}

void sendDeviceInfosJson(WiFiClient &client) {
  char json[300];
  getDeviceInfosJson(json);
  Serial.println(json);
  client.flush();
  client.print(json);
}

void sendOnGoingSignatureJson(WiFiClient &client) {
  char json[300];
  getOnGoingSignatureJson(json);
  Serial.println(json);
  client.flush();
  client.print(json);
}

void loop() {
  // put your main code here, to run repeatedly:
  byteduino_loop();

  WiFiClient client = server.available();

  if (client) {
    Serial.println("Client");
    // an http request ends with a blank line
    boolean currentLineIsBlank = true;
    char bufferClient[250];
    int i = 0;
    while (client.connected()) {
      while (client.available()) {
        char c = client.read();
        if (i < 250) {
          bufferClient[i] = c;
          i++;
        }
        // if you've gotten to the end of the line (received a newline
        // character) and the line is blank, the http request has ended,
        // so you can send a reply
        if (c == '\n' && currentLineIsBlank) {

          // Here is where the POST data is.
          while (client.available())
          {
            Serial.write(client.read());
          }


          Serial.println("Sending response");
          // send a standard http response header

          if (strstr(bufferClient, "ongoing_signature.json") != nullptr) {
            sendOnGoingSignatureJson(client);
          } else if (strstr(bufferClient, "paired_devices.json") != nullptr) {
            sendPairedDevicesJson(client);
          } else if (strstr(bufferClient, "device_infos.json") != nullptr) {
            sendDeviceInfosJson(client);
          } else if (strstr(bufferClient, "wallets.json") != nullptr) {
            sendWalletsJson(client);
          } else {
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


  /*
    if (client) {
    String req = client.readString();
    Serial.println("client");


    Serial.println(req);

    if (req.indexOf("wallets.json") != -1) {

      sendWalletsJson(client);
    } else if (req.indexOf("paired_devices.json") != -1) {
      sendPairedDevicesJson(client);
    }  else if (req.indexOf("device_infos.json") != -1) {
      sendDeviceInfosJson(client);
    } else{
     sendWebpage(client);
    }
    }
  */
}
