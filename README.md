## Byteduino hardware cosigner

![hardware-cosigner](https://reho.st/preview/self/63427e1cfb0a3e09455ec07a4e874ea4f77757f3.jpg)

This hardware cosigner allows to securize your funds by being a required device for a multisig wallet. Once set up and connected to internet via Wifi, you can pair it with the standard Byteball GUI wallet, add it as cosigner for a multidevice wallet then authorize the signature of a transaction from a webpage accessible from your local network. 
It works on ESP8266 and ESP32 boards but ESP32 is recommended since it has no practical limitation about the size of unit you can cosign.

To get support or discuss about this project, request an [invitation to Byteball Slack](http://slack.byteball.org/) and join **#byteduino** channel.

**Blackbytes are not supported**

### Instructions

#### Install Arduino IDE
- [Windows](https://www.arduino.cc/en/Guide/Windows)
- [Linux](https://www.arduino.cc/en/Guide/Linux)
- [MacOS](https://www.arduino.cc/en/Guide/MacOSX)

#### Install ESP32 addon and test installation
- [Windows](https://randomnerdtutorials.com/installing-the-esp32-board-in-arduino-ide-windows-instructions/)
- [Mac/Linux](https://randomnerdtutorials.com/installing-the-esp32-board-in-arduino-ide-mac-and-linux-instructions/)

#### Install required libraries
Go to Sketch->Include Library->Manage Libraries

Install the following libraries:
- **Byteduino** by Papabyte (last version)
- **ArduinoJson** by Benoit Blanchon version 5.13.2 (not compatible with version 6!)
- **micro-ecc** by Kenneth MacKay version 1.0.0

#### Open Hardware Cosigner sketch
- [Download](https://github.com/Papabyte/Hardware-cosigner/archive/master.zip) this git repository and decompress it on your computer
- Open cosigner.ino file 

#### Generate private keys
- Use the [web based keys generator](http://papabyte.github.io/byteduino-keys-generator)
- Copy and replace into cosigner.ino setup() the 3 functions provided
- Write down seed as backup

#### Set your Wifi credentials
- In your setup(), set your wifi SSID and password at this line: `WiFiMulti.addAP("My_SSID", "My_password");`

#### Upload sketch to ESP32 board then reset it

#### Get webserver IP address
- Watch the serial monitor output, if working properly you should see this output:
```
192.168.43.6
Device address: 
06XOQ6ZVWTV2ZWAEAIPFPJRGGQBMTHC6J
Device name: 
Byteduino
Pairing code: 
ArxPWIrgUvi5YuvugpJEE1aLwu9bRBOxoRFHSX9o6IyJ@byteball.org/bb#0000
Extended Pub Key:
xpub6Chxqf8hRQoLRJFS8mhFCDv5yzUNC7rhhh36qqqt1WtAZcmCNhS5pPndqhx8RmgwFhGPa9FYq3iTXNBkYdkrAKJxa7qnahnAvCzKW5dnfJn
Wss connected to: /bb-test
Authenticated by hub
```
- Copy the IP address (attributed by your router) and paste it on your browser to access to the cosigner control webpage

#### Pair the device
- From your Byteball wallet, add the hardware cosigner to your correspondents list by using the pairing code provided above as invitation code.

#### Create a multidevice wallet
- From your Byteball wallet, create a multidevice wallet with the hardware device as one of the cosigner. Only **single address wallet** is supported.

#### Cosign a transaction
- Select your multidevice wallet and click on send a transaction, the wallet will ask you to approve the transaction on other devices. Go to the cosigner control webpage and click on confirm.
