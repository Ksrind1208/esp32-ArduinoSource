#include <WiFi.h>
#include <PubSubClient.h>
#include <WiFiClientSecure.h>
#include <DHT.h>
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include <SPI.h>
#include <Ethernet.h>
#include "SSLClient.h"

//define ethernet
#define ETHERNET_MAC            "BA:E5:E3:B1:44:DD" // Ethernet MAC address (have to be unique between devices in the same network)
#define ETHERNET_IP             "192.168.1.15"       // IP address of RoomHub when on Ethernet connection

#define ETHERNET_RESET_PIN      -1      // ESP32 pin where reset pin from W5500 is connected
#define ETHERNET_CS_PIN         5       // ESP32 pin where CS pin from W5500 is connected

//define ble
#define SERVICE_UUID        "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"

BLEServer *pServer;
BLEService *pService;
BLECharacteristic *pCharacteristic;
BLEAdvertising *pAdvertising;
bool deviceConnected = false;

class MyCallbacks: public BLECharacteristicCallbacks {
    void onWrite(BLECharacteristic *pCharacteristic) {
      String value = pCharacteristic->getValue();
      
      if (value.length() > 0) {
        if(value=="ON"){
          digitalWrite(2,1);
        }
        if(value=="OFF"){
          digitalWrite(2,0);
        }
      }
    }
};

class ServerCallbacks: public BLEServerCallbacks {
    void onConnect(BLEServer* pServer) {
      deviceConnected = true;
    };

    void onDisconnect(BLEServer* pServer) {
      deviceConnected = false;
      pAdvertising->start(); // Start advertising again
    }
};

//---- WiFi settings
const char* ssid = "AIoT_JSC";
const char* password = "aiot1234@";
// ---- MQTT Broker settings
// const char* mqtt_server = "f41b6c6a5f49462c8c57817532ae5e39.s1.eu.hivemq.cloud"; 
// const char* mqtt_username = "duc123123tc"; 
// const char* mqtt_password = "Duc1282003"; 
// const int mqtt_port =8883;
const char* mqtt_server = "mqtt.eclipseprojects.io"; 
const char* mqtt_username = ""; 
const char* mqtt_password = ""; 
const int mqtt_port =1883;
const int max_retry=10;
static int curRetry=0;
const int max_retry_mqtt=10;
static int curRetryMqtt=0;
unsigned long lastMsg=0;

// WiFiClientSecure wifiClient;
WiFiClient wifiClient;
EthernetClient ethClient;
SSLClient secureClientEth(&ethClient);
PubSubClient client;

const char* led_Topic="/topic/qos0";
const char* temp_humid_Topic="home/sensor/TemperatureandHumid";

#define MSG_BUFFER_SIZE (50)
char msg[MSG_BUFFER_SIZE];

static const char *root_ca PROGMEM = R"EOF(
-----BEGIN CERTIFICATE-----
MIIFazCCA1OgAwIBAgIRAIIQz7DSQONZRGPgu2OCiwAwDQYJKoZIhvcNAQELBQAw
TzELMAkGA1UEBhMCVVMxKTAnBgNVBAoTIEludGVybmV0IFNlY3VyaXR5IFJlc2Vh
cmNoIEdyb3VwMRUwEwYDVQQDEwxJU1JHIFJvb3QgWDEwHhcNMTUwNjA0MTEwNDM4
WhcNMzUwNjA0MTEwNDM4WjBPMQswCQYDVQQGEwJVUzEpMCcGA1UEChMgSW50ZXJu
ZXQgU2VjdXJpdHkgUmVzZWFyY2ggR3JvdXAxFTATBgNVBAMTDElTUkcgUm9vdCBY
MTCCAiIwDQYJKoZIhvcNAQEBBQADggIPADCCAgoCggIBAK3oJHP0FDfzm54rVygc
h77ct984kIxuPOZXoHj3dcKi/vVqbvYATyjb3miGbESTtrFj/RQSa78f0uoxmyF+
0TM8ukj13Xnfs7j/EvEhmkvBioZxaUpmZmyPfjxwv60pIgbz5MDmgK7iS4+3mX6U
A5/TR5d8mUgjU+g4rk8Kb4Mu0UlXjIB0ttov0DiNewNwIRt18jA8+o+u3dpjq+sW
T8KOEUt+zwvo/7V3LvSye0rgTBIlDHCNAymg4VMk7BPZ7hm/ELNKjD+Jo2FR3qyH
B5T0Y3HsLuJvW5iB4YlcNHlsdu87kGJ55tukmi8mxdAQ4Q7e2RCOFvu396j3x+UC
B5iPNgiV5+I3lg02dZ77DnKxHZu8A/lJBdiB3QW0KtZB6awBdpUKD9jf1b0SHzUv
KBds0pjBqAlkd25HN7rOrFleaJ1/ctaJxQZBKT5ZPt0m9STJEadao0xAH0ahmbWn
OlFuhjuefXKnEgV4We0+UXgVCwOPjdAvBbI+e0ocS3MFEvzG6uBQE3xDk3SzynTn
jh8BCNAw1FtxNrQHusEwMFxIt4I7mKZ9YIqioymCzLq9gwQbooMDQaHWBfEbwrbw
qHyGO0aoSCqI3Haadr8faqU9GY/rOPNk3sgrDQoo//fb4hVC1CLQJ13hef4Y53CI
rU7m2Ys6xt0nUW7/vGT1M0NPAgMBAAGjQjBAMA4GA1UdDwEB/wQEAwIBBjAPBgNV
HRMBAf8EBTADAQH/MB0GA1UdDgQWBBR5tFnme7bl5AFzgAiIyBpY9umbbjANBgkq
hkiG9w0BAQsFAAOCAgEAVR9YqbyyqFDQDLHYGmkgJykIrGF1XIpu+ILlaS/V9lZL
ubhzEFnTIZd+50xx+7LSYK05qAvqFyFWhfFQDlnrzuBZ6brJFe+GnY+EgPbk6ZGQ
3BebYhtF8GaV0nxvwuo77x/Py9auJ/GpsMiu/X1+mvoiBOv/2X/qkSsisRcOj/KK
NFtY2PwByVS5uCbMiogziUwthDyC3+6WVwW6LLv3xLfHTjuCvjHIInNzktHCgKQ5
ORAzI4JMPJ+GslWYHb4phowim57iaztXOoJwTdwJx4nLCgdNbOhdjsnvzqvHu7Ur
TkXWStAmzOVyyghqpZXjFaH3pO3JLF+l+/+sKAIuvtd7u+Nxe5AW0wdeRlN8NwdC
jNPElpzVmbUq4JUagEiuTDkHzsxHpFKVK7q4+63SM1N95R1NbdWhscdCb+ZAJzVc
oyi3B43njTOQ5yOf+1CceWxG1bQVs5ZufpsMljq4Ui0/1lvh+wjChP4kqKOJ2qxq
4RgqsahDYVvTH9w7jXbyLeiNdd8XM2w9U/t7y0Ff/9yi0GE44Za4rF2LN9d11TPA
mRGunUHBcnWEvgJBQl9nJEiU0Zsnvgc/ubhPgXRR4Xq37Z0j4r7g1SgEEzwxA57d
emyPxgcYxn/eR44/KJ4EBs+lVDR3veyJm+kXQ99b21/+jh5Xos1AnX5iItreGCc=
-----END CERTIFICATE-----
)EOF";

DHT dht(4,DHT11);
const int led_pin=2;

void macCharArrayToBytes(const char* str, byte* bytes) {
    for (int i = 0; i < 6; i++) {
        bytes[i] = strtoul(str, NULL, 16);
        str = strchr(str, ':');
        if (str == NULL || *str == '\0') {
            break;
        }
        str++;
    }
}

void connectEthernet() {
    // delay(500);
    // byte* mac = new byte[6];
    // macCharArrayToBytes(ETHERNET_MAC, mac);
    // ipAddress.fromString(ETHERNET_IP);
    byte* mac = new byte[6];
    macCharArrayToBytes(ETHERNET_MAC, mac);
    Ethernet.init(ETHERNET_CS_PIN);
    // IPAddress ip;
    // ip.fromString(ETHERNET_IP);
    // Serial.println("Starting ETHERNET connection...");
    Ethernet.begin(mac);
    // delay(200);

    Serial.print("Ethernet IP is: ");
    Serial.println(Ethernet.localIP());
    client.setClient(ethClient);
    // secureClientEth.setCACert(root_ca);
    // secureClientEth.setCACert(root_ca);
    client.setServer(mqtt_server, mqtt_port);
    client.setCallback(callback);
    connectMQTT();
}

void connectWifi(const char*SSID,const char* PASSWORD)
{
  WiFi.mode(WIFI_STA);
  WiFi.begin(SSID, PASSWORD);
  
  // Tentativa de estabelecer conexão
  while(WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    curRetry++;
    if(curRetry>max_retry){
      Serial.println("Connect to wifi fail");
      curRetry=0;   
      break;
    }
    delay(1000);
  }
  if(WiFi.status() == WL_CONNECTED){
    Serial.println("\nWiFi connected\nIP address: ");
    Serial.println(WiFi.localIP());
    client.setClient(wifiClient);

    // wifiClient.setCACert(root_ca);
    // BearSSL::X509List *serverTrustedCA = new BearSSL::X509List(root_ca);
    // wifiClient.setTrustAnchors(serverTrustedCA);
    client.setServer(mqtt_server, mqtt_port);
    client.setCallback(callback);
    connectMQTT();
  }
}
void connectBLE(){
    Serial.println("Starting BLE work!");

  BLEDevice::init("Controll over BLE");
  pServer = BLEDevice::createServer();
  pServer->setCallbacks(new ServerCallbacks());
  pService = pServer->createService(SERVICE_UUID);
  pCharacteristic = pService->createCharacteristic(CHARACTERISTIC_UUID, BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE);
  pCharacteristic->setValue("Hello world from BLE");
  pCharacteristic->setCallbacks(new MyCallbacks());

  pService->start();

  pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);
  pAdvertising->setScanResponse(true);
  pAdvertising->setMinPreferred(0x06);  // functions that help with iPhone connections issue
  pAdvertising->setMinPreferred(0x12);
  pAdvertising->start();
  Serial.println("Characteristic defined! Now you can read it in your phone!");
}
void vTaskWifi(void * pvParameters) {
  while (true) {
    if(WiFi.status()==WL_CONNECTED){
      client.setClient(wifiClient);
      // wifiClient.setCACert(root_ca);
      // BearSSL::X509List *serverTrustedCA = new BearSSL::X509List(root_ca);
      // wifiClient.setTrustAnchors(serverTrustedCA);
      client.setServer(mqtt_server, mqtt_port);
      client.setCallback(callback);
      connectMQTT();
      break;
    }
    delay(20);
  }
  while(true){
  }
}
void setup() {

  Serial.begin(115200);
  dht.begin();
  pinMode(led_pin,OUTPUT);
  delay(2000);

  connectBLE();


  Serial.print("\nConnecting to ");
  Serial.println(ssid);

  // xTaskCreatePinnedToCore(vTaskWifi, "WifiTask", 2048*3, NULL, 5, NULL, 0);
  connectWifi(ssid,password);
  // connectEthernet();
}

void loop() {
  if(WiFi.status() == WL_CONNECTED){
    client.loop();
  }
  // if(Ethernet.linkStatus() == LinkON){
  //   client.loop();
  // }
  unsigned long now=millis();
  if(now-lastMsg>2000){
    lastMsg=now;
    float temp=dht.readTemperature();
    float humid=dht.readHumidity();

    String data="";
    data+=(String(temp, 2)+"-"+String(humid, 2));
    pCharacteristic->setValue(data);
    client.publish(temp_humid_Topic,data.c_str());
    client.publish("dhtTemp",String(dht.readTemperature()).c_str());
    client.publish("dhtHum",String(dht.readHumidity()).c_str());
  }
}

void connectMQTT() {
  // Loop until we’re reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection…");
    String clientId = "ESP32Client-"; 
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    curRetryMqtt++;
    if(curRetryMqtt>max_retry_mqtt){
      Serial.print("Mat ket noi mqtt");
      break;
    }
    if (client.connect(clientId.c_str(), mqtt_username, mqtt_password)) {
      Serial.println("connected");

      client.subscribe(led_Topic);
      client.subscribe(temp_humid_Topic);

    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 3 seconds");   
      delay(3000);
    }
  }
}

void callback(char* topic, byte* payload, unsigned int length) {
  String incommingMessage = "";
  for (int i = 0; i < length; i++) incommingMessage+=(char)payload[i];
  Serial.println("Message arrived ["+String(topic)+"] "+incommingMessage);

  if(strcmp(topic,led_Topic)==0){
    if(incommingMessage.equals("ON")){
      digitalWrite(led_pin,1);
    }
    if(incommingMessage.equals("OFF")){
      digitalWrite(led_pin,0);
    }
  }
  if(strcmp(topic,temp_humid_Topic)==0){
    if(incommingMessage.equals("Closethedoor")){
      Serial.println("Closethedoor");
    }
  }
}