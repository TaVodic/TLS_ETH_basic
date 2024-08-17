#include <EEPROM.h>
#include <Ethernet.h>
#include <SPI.h>

#include "website.h"

// ARDUINO MEGA - jedna strižna, keepAlive, chceckInputNumber, JavaScript, abillity to save inputs profile, CBM, vylepsenie TCP
// RLS03_basic

// CHANGE MAC ADDRESS!!!

#define DEBUG
#define EEPROMe
#define DHCP

#define VERSION "ETH_basic_v1.2.0"

#define DEFAULT_VALUE 0b001001001001001
#define RECONNECT     1000  // connecting to vMix
#define RESEND        100
#define KEEPALIVE     2000
#define TTK           500  // time to kill - cas do ktoreho ak strizna neodpovie, restartuje sa spojenie
#define ACTIVE        3    // 3
#define PREVIEW       2    // 2
#define NOTHING       1    // 1

#define PIN_HC12_set       22
#define PIN_ETH_PHY_status 9

byte TLS_mac[] = {0x02, 0x54, 0x4C, 0x53, 0x02, 0x00};  // 00:00 - dual, 00:01 - basic, 01:00 - RSG
IPAddress TLS_ip(192, 168, 0, 200);                     // DHCP preferred

EthernetClient client;
EthernetServer server(80);

const char connected[] = "<p id=\"connected\">CONNECTED</p>";
const char disconnected[] = "<p id=\"disconnected\">DISCONNECTED</p>";
const char save[] = "<button name=\"save\" type=\"submit\" id=\"save\">Save</button>";
const char *p_save;
uint8_t status;

unsigned long cMillis;
unsigned long cMillis2;
unsigned long cMillis3;  // keepAlive
uint8_t keepAliveFlag;
uint8_t runned;
uint16_t code = DEFAULT_VALUE;  // 4681
char message[200];
uint8_t IPaddr[4] = {192, 168, 0, 101};  // mixer IP address
uint8_t inputNumber[5] = {1, 2, 3, 4, 5};
uint8_t brightness[5] = {6, 6, 6, 6, 6};
uint8_t old_len;

void setup() {
  Serial.begin(115200);
  Serial1.begin(9600);

  pinMode(PIN_ETH_PHY_status, INPUT);

#ifdef DEBUG
  Serial.print("\n\nversion: ");
  Serial.println(VERSION);
#endif

#ifdef EEPROMe
  for (uint8_t i = 0; i < 4; i++) {
    IPaddr[i] = EEPROM.read(i);
  }
  for (uint8_t i = 0; i < 5; i++) {
    inputNumber[i] = EEPROM.read(i + 4);
  }
#endif

  // Check if PHY link, if not, do nothing till cable is connected
  if (PHY_status() == false) {
    Serial.println("Eth cable disconnected!");
    while (PHY_status() == false);
    Serial.println("Eth cable connected!");
  }

#ifdef DHCP
  if (Ethernet.begin(TLS_mac, 5000) == 0) {
    Serial.println("DHCP timeout, using static IP");
    Ethernet.begin(TLS_mac, TLS_ip);
  } else
    Serial.println("DHCP assigned IP");
#else
  Ethernet.begin(TLS_mac, TLS_ip);
#endif

  server.begin();
#ifdef DEBUG
  Serial.print("server BEGIN: ");
  Serial.println(Ethernet.localIP());
#endif

  connectTovMix();

  // client.setConnectionTimeout(3000);
  // Ethernet.setRetransmissionTimeout(50);
  // Ethernet.setRetransmissionCount(8);
}

void loop() {
  configClientConnect();

  vMixHandle();

  if (millis() > cMillis2 + RESEND) {
    Serial1.write(code);
    Serial1.write(code >> 8);
    Serial1.write(255);
    cMillis2 = millis();
  }
}

/*server*/
void configClientConnect() {
  char *p_message;
  p_message = message;
  EthernetClient conf = server.available();
  if (conf) {
    // Serial.println("new client");
    memset(message, '\0', sizeof(message));
    boolean currentLineIsBlank = true;
    while (conf.connected()) {
      if (conf.available()) {
        char c = conf.read();
        Serial.write(c);
        *p_message = c;
        if ((message + sizeof(message)) > p_message) {
          p_message++;
        } else {
          Serial.println("\nFATAL ERROR:pointer out of string");
          Serial.print("start:");
          Serial.println((int)&message);
          Serial.print("current:");
          Serial.println((int)p_message);
          break;
        }

        if (c == '\n' && currentLineIsBlank || (strstr(message, "HTTP/1.1") != NULL)) {
#ifdef DEBUG
          Serial.println();
#endif
          processConfData();
          if (status) {
            website(conf, IPaddr, inputNumber, connected, p_save, brightness, VERSION);
          } else {
            website(conf, IPaddr, inputNumber, disconnected, p_save, brightness, VERSION);
          }
          p_message = NULL;
          break;
        }
        if (c == '\n') {
          currentLineIsBlank = true;
        } else if (c != '\r') {
          currentLineIsBlank = false;
        }
      }
    }
    delay(1);
    conf.stop();
    // Serial.println("client(conf) disconnected");
    client.print("SUBSCRIBE TALLY\r\n");
  }
}

void processConfData() {
  char *p_data = strstr(message, "GET /?IPaddr=");
  if (p_data != NULL) {
    p_data += 13;  // move to first data ("IPaddr1=")
    for (uint8_t q = 0; q < 4; q++) {
      uint8_t count = 0;
      while (*p_data != '.' && *p_data != '&') {
        // Serial.print((*p_data) - 48);
        int digit = (*p_data) - 48;
        if (count == 1) {
          IPaddr[q] *= 10;
          IPaddr[q] += digit;
        } else if (count == 2) {
          IPaddr[q] *= 10;
          IPaddr[q] += digit;
        } else {
          IPaddr[q] = digit;
        }
        count++;
        p_data++;
      }
      p_data++;
#ifdef DEBUG
      Serial.print(IPaddr[q]);
      Serial.print(".");
#endif
    }
    p_data += 3;
    for (uint8_t q = 0; q < 5; q++) {
      uint8_t count = 0;
      while (*p_data != '&' && *p_data != ' ') {
        // Serial.print("T");
        // Serial.print((*p_data) - 48);
        uint8_t digit = (*p_data) - 48;
        if (count != 0) {
          inputNumber[q] *= 10;
          inputNumber[q] += digit;
        } else {
          inputNumber[q] = digit;
        }
        count++;
        p_data++;
      }

      p_data += 4;
      count = 0;
      while (*p_data != '&' && *p_data != ' ') {  // brightness
        // Serial.print("B");
        // Serial.print((*p_data) - 48);
        uint8_t digit = (*p_data) - 48;
        if (count != 0) {
          brightness[q] *= 10;
          brightness[q] += digit;
        } else {
          brightness[q] = digit;
        }
        count++;
        p_data++;
      }
      p_data += 4;
    }

    proccessBrightnessFrame();

#ifdef DEBUG
    Serial.println();
    for (int i = 0; i < 5; i++) {
      Serial.print(inputNumber[i]);
      Serial.print("-");
    }
    Serial.println();
    Serial.println();
    for (int i = 0; i < 5; i++) {
      Serial.print(brightness[i]);
      Serial.print("-");
    }
    Serial.println();
#endif

#ifdef EEPROMe
    for (uint8_t i = 0; i < 4; i++) {
      EEPROM.write(i, IPaddr[i]);
    }
    if (strstr(message, "save=")) {
      for (uint8_t i = 0; i < 5; i++) {
        EEPROM.write(i + 4, inputNumber[i]);
      }
      p_save = NULL;
    } else {
      checkInputNumberProfile();
    }

#endif
  }
}

void checkInputNumberProfile() {
  uint8_t flag = 0;
  for (uint8_t i = 0; i < 5; i++) {
    if (EEPROM.read(i + 4) != inputNumber[i]) {
      flag++;
    }
  }
  if (flag == 0) {
    p_save = NULL;
  } else {
    p_save = save;
  }
}

void proccessBrightnessFrame() {
  uint16_t frame;
  for (uint8_t q = 0; q < 5; q++) {
    frame = (~(0b111 << (q * 3)) & frame) | brightness[q] << (q * 3);
  }
  // frame = 0b0110110110110110;
  Serial1.write(0);
  Serial1.write(frame);
  Serial1.write(frame >> 8);
  cMillis2 = millis();
}

/*client vMix*/
void vMixHandle() {
  size_t len = client.available();
  if (len > 0) {
    keepAliveFlag = 0;  // vynuluj keepAlive flag - prisli data, niet dovod na paniku
    cMillis3 = millis();
    char buffer[len + 10];
    client.read((uint8_t *)buffer, len);
    // Serial.println(len);
    processSwitcherData(buffer);
#ifdef DEBUG
    Serial.write(buffer, len);  // show data from vmix
#endif
    runned = 1;
  }

  if (!client.connected()) {
    if (millis() > cMillis + RECONNECT) {
      cMillis = millis();
      if (connectTovMix()) {
        return;
      }
    }
    if (runned) {
#ifdef DEBUG
      Serial.println("client DISCONNECTED, stopping.");
#endif
      client.stop();
      runned = 0;
      code = DEFAULT_VALUE;
    }
    status = 0;
  } else {
    keepAlive();
    status = 1;
  }
}

uint8_t connectTovMix() {
  IPAddress mixer(IPaddr[0], IPaddr[1], IPaddr[2], IPaddr[3]);
#ifdef DEBUG
  Serial.print("connecting to ");
  Serial.print(mixer);
  Serial.print("...");
#endif
  uint8_t result = client.connect(mixer, 8099);
#ifdef DEBUG
  Serial.printf("result:%d...on socket:%d...", result, client.getSocketNumber());
#endif
  if (result == 23) {
#ifdef DEBUG
    Serial.println("successful");
    cMillis3 = millis();
#endif
    client.print("SUBSCRIBE TALLY\r\n");
    return 1;
  } else {
#ifdef DEBUG
    Serial.println("connection failed");
    return 0;
#endif
  }
}

void keepAlive() {
  if (millis() > cMillis3 + KEEPALIVE) {
    client.print("TALLY\r\n");
    Serial.println("->TALLY");
    cMillis3 = millis();
    keepAliveFlag = 1;
  }
  if ((millis() > cMillis3 + TTK) && keepAliveFlag) {
#ifdef DEBUG
    Serial.printf("Request timeout, restarting client. Socket status: %d", client.status());
#endif
    client.stop();
    runned = 0;
    code = DEFAULT_VALUE;
    connectTovMix();
    keepAliveFlag = 0;
  }
}

void processSwitcherData(char *gdata) {
  char *p_data = strstr(gdata, "TALLY OK ");
  if (p_data != NULL) {
    p_data = strrchr(gdata, ' ');  // find last space from end -  TALLY OK' '01200
    // checkInputNumber(p_data);
    p_data += 1;
    char *default_p = p_data;
    uint8_t len = strlen(p_data) - 3;  // number of current vMix inputs
    // Serial.println(len);

    for (int q = 0; q < 5; q++) {
      if (inputNumber[q] == 0 || inputNumber[q] > len) {
        code = (~(0b111 << (q * 3)) & code) | NOTHING << (q * 3);
      } else {
        p_data = default_p + (inputNumber[q] - 1);
        switch (*p_data) {
          case '0':
            code = (~(0b111 << (q * 3)) & code) | NOTHING << (q * 3);
            break;
          case '2':
            code = (~(0b111 << (q * 3)) & code) | PREVIEW << (q * 3);
            break;
          case '1':
            code = (~(0b111 << (q * 3)) & code) | ACTIVE << (q * 3);
            break;
        }
      }
    }
#ifdef DEBUG
    Serial.print("code:");
    Serial.println(code, BIN);
#endif
    Serial1.write(code);
    Serial1.write(code >> 8);
    Serial1.write(255);
  }
}

bool PHY_status() {
  if (digitalRead(PIN_ETH_PHY_status))  // active low
    return 0;
  else
    return 1;
}