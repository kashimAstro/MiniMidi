/*

***************************************

  ESP-03:
        vcc      wifi_ant
        14       ch_pd (vcc)
        12       18
        13       rx
  (gnd) 15       tx
        2        nc
  (gnd) 0        gnd

***************************************

  ESP-07:
        rst      tx
        adc      rx
  (vcc) ch_pd    5
        16       4
        14       0   (gnd)
        12       2
        13       15  (gnd)
        vcc      gnd

***************************************

*/

#include <ESP8266WiFi.h>

const char *ssid     = "*******";
const char *password = "*******";
const char *host     = "192.168.1.2";

String ip            = "";
#define NUM 6

int pin  [NUM] = { 14, 12, 13, 2, 18 };
int note [NUM] = { 65, 66, 67, 68,69 };
int state[NUM] = { 0,  0,  0,  0, 0  };
int tmp  [NUM] = { 1,  1,  1,  1, 1  };

void setup() {
  for (int i = 0; i < NUM; i++)
  {
    pinMode(pin[i], INPUT);
  }
  Serial.begin(115200);
  delay(10);
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  ip = WiFi.localIP().toString();
}

void loop() {
  for (int i = 0; i < NUM; i++)
  {
    state[i] = digitalRead(pin[i]);
    if (state[i] != tmp[i])
    {

      WiFiClient client;
      const int port = 9055;
      if (!client.connect(host, port)) 
      {
        Serial.println("connection failed");
        return;
      }
      
      Serial.print(ip);
      Serial.print(":");
      Serial.print(note[i]);
      Serial.print(":");
      Serial.println(state[i]?"0":"100");
      
      client.print(ip);
      client.print(":");
      client.print(note[i]);
      client.print(":");
      client.println(state[i]?"0":"100");
      client.stop();
      
    }
    tmp[i] = state[i];
  }
  delay(1);
}

