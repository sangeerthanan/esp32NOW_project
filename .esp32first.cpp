#include <esp_now.h>
#include <WiFi.h>
#include <DHT.h>


#define DHTPIN 4 
#define LDRPIN 34        // Pin where the DHT sensor is connected
#define DHTTYPE DHT11   // DHT 11 or DHT 22
DHT dht(DHTPIN, DHTTYPE);

// MAC address of the receiver ESP32
uint8_t receiverAddress[] = {0x10, 0x06, 0x1C, 0x68, 0x3A, 0x64};
int LDRDATA = analogRead(LDRPIN);
// Structure example to send data
typedef struct struct_message {
   float humidity;
  
  float temperature;
 float LDRDATA;

} struct_message;

// Create a structure
struct_message myData;

void onSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
    Serial.print("\nLast Packet Send Status:\t");
    Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}

void setup() {
    Serial.begin(115200);
    WiFi.mode(WIFI_STA);

    // Initialize DHT sensor
    dht.begin();

    // Initialize ESP-NOW
    if (esp_now_init() != ESP_OK) {
        Serial.println("Error initializing ESP-NOW");
        return;
    }

    // Register send callback
    esp_now_register_send_cb(onSent);

    // Add peer (receiver)
    esp_now_peer_info_t peerInfo;
    memcpy(peerInfo.peer_addr, receiverAddress, 6);
    peerInfo.channel = 0;
    peerInfo.encrypt = false;
    if (esp_now_add_peer(&peerInfo) != ESP_OK) {
        Serial.println("Failed to add peer");
        return;
    }
}

void loop() {
    // Read temperature from the sensor
    myData.temperature = dht.readTemperature();
     myData.humidity = dht.readHumidity();
myData.LDRDATA = analogRead(LDRPIN);
    // Send the data
    esp_err_t result = esp_now_send(receiverAddress, (uint8_t *) &myData, sizeof(myData));

    if (result == ESP_OK) {
        Serial.println("Sent with success");
    } else {
        Serial.println("Error sending the data");
    }

    delay(2000); // Send every 2 seconds
}
