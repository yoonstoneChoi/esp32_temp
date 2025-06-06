
#include <WiFi.h>
#include <PubSubClient.h>
#include <DHT.h>
#include <TM1638plus.h>


const char* ssid = "wifi-name";
const char* password = "wifi-password";
const char* mqtt_server = "broker.emqx.io";

WiFiClient espClient;
PubSubClient client(espClient);
unsigned long lastMsg = 0;
#define MSG_BUFFER_SIZE	(50)
#define BUILTIN_LED 2


#define  LED_BUILTIN 2
#define  DHTPIN 15 
#define  DHTTYPE DHT11   // DHT 11
DHT dht(DHTPIN, DHTTYPE);

#define  STROBE_TM 18 
#define  CLOCK_TM 19
#define  DIO_TM 21 
bool high_freq = false; 

TM1638plus tm(STROBE_TM, CLOCK_TM , DIO_TM, high_freq);

#define myTestDelay  5000
#define myTestDelay1 1000
#define myTestDelay3 3000



char msg[MSG_BUFFER_SIZE];
int value = 0;

void setup_wifi() {

  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  randomSeed(micros());

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();

  if ((char)payload[0] == '1') {
    digitalWrite(BUILTIN_LED, LOW); 
  } else {
    digitalWrite(BUILTIN_LED, HIGH); 
  }

}

void reconnect() {

  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
 
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);

    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
  
      client.publish("outTopic", "hello world");
    
      client.subscribe("/ESP32/LAAS/INTOPIC/YSH");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}


void Serialinit()
{
  Serial.begin(115200);
  delay(100);
  Serial.println("--Comms UP--TM1638plus_TEST_Model1.ino--");
}

float GetTemp() {

  float h = dht.readHumidity();
  float t = dht.readTemperature();

  float voltage = t;
  uint16_t temp = 0;
  char workStr[11];
  uint8_t  digit1, digit2, digit3 , digit4;
  voltage =  voltage * 100; 
  temp = (uint16_t)voltage;
  digit1 = (temp / 1000) % 10;
  digit2 = (temp / 100) % 10;
  digit3 = (temp / 10) % 10;
  digit4 =  temp % 10;

  sprintf(workStr, "ONDO.%d%d.%d%d", digit1, digit2, digit3, digit4);
  Serial.print(t);
  Serial.print(F("°C "));
  Serial.println(workStr);
  tm.displayText(workStr);
  delay(5000);
  tm.reset();
  return t;
}

void setup() {
  pinMode(BUILTIN_LED, OUTPUT);  
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
  Serialinit();
  dht.begin();
  tm.displayBegin();
}

void loop() {

  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  unsigned long now = millis();
  if (now - lastMsg > 2000) {
    lastMsg = now;
    ++value;
    float temp = GetTemp();
    snprintf (msg, MSG_BUFFER_SIZE, "%.2f", temp);
    Serial.print("Publish message: ");
    Serial.println(msg);
    client.publish("ESP32/LAAS/TEMP/YSH", msg);
  }
}
