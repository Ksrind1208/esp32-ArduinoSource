#include <WiFi.h>
#include <PubSubClient.h>
#include <WiFiClientSecure.h>
#include <DHT.h>
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>

//define ble
#define SERVICE_UUID        "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"

BLEServer *pServer;
BLEService *pService;
BLECharacteristic *pCharacteristic;
class MyCallbacks: public BLECharacteristicCallbacks {
    void onWrite(BLECharacteristic *pCharacteristic) {
      pinMode(2,OUTPUT);
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
//---- WiFi settings
const char* ssid = "Minh Tam 2.4 G";
const char* password = "21072018";
//---- MQTT Broker settings
const char* mqtt_server = "f41b6c6a5f49462c8c57817532ae5e39.s1.eu.hivemq.cloud"; // replace with your broker url
const char* mqtt_username = "duc123123tc"; // replace with your username
const char* mqtt_password = "Duc1282003"; // replace with your password
const int mqtt_port =8883;
const int max_retry=10;
static int curRetry=0;
unsigned long lastMsg=0;

WiFiClientSecure espClient;
PubSubClient client(espClient);

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
    espClient.setCACert(root_ca);
    client.setServer(mqtt_server, mqtt_port);
    client.setCallback(callback);
    connectMQTT();
  }

}

void setup() {

Serial.begin(115200);
dht.begin();
pinMode(led_pin,OUTPUT);
delay(2000);

Serial.println("Starting BLE work!");

BLEDevice::init("Controll over BLE");
BLEServer *pServer= BLEDevice::createServer();
pService = pServer->createService(SERVICE_UUID);
pCharacteristic =pService->createCharacteristic(CHARACTERISTIC_UUID, BLECharacteristic::PROPERTY_READ|BLECharacteristic::PROPERTY_WRITE);
pCharacteristic->setValue("Hello world from BLE");
pCharacteristic->setCallbacks(new MyCallbacks());

pService->start();
// BLEAdvertising *pAdvertising = pServer->getAdvertising();  // this still is working for backward compatibility
BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
pAdvertising->addServiceUUID(SERVICE_UUID);
pAdvertising->setScanResponse(true);
pAdvertising->setMinPreferred(0x06);  // functions that help with iPhone connections issue
pAdvertising->setMinPreferred(0x12);
BLEDevice::startAdvertising();
Serial.println("Characteristic defined! Now you can read it in your phone!");

Serial.print("\nConnecting to ");
Serial.println(ssid);

connectWifi(ssid,password);

}

void loop() {
  if(WiFi.status() == WL_CONNECTED){
    client.loop();
  }
  unsigned long now=millis();
  if(now-lastMsg>2000){
    lastMsg=now;
    float temp=dht.readTemperature();
    float humid=dht.readHumidity();

    String data="";
    data+=(String(temp, 2)+"-"+String(humid, 2));
    pCharacteristic->setValue(data);
    client.publish(temp_humid_Topic,data.c_str());
  }
}


void connectMQTT() {
  // Loop until we’re reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection…");
    String clientId = "ESP32Client-"; 
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
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
