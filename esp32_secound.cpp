#include <esp_now.h>
#include <WiFi.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128  // OLED display width
#define SCREEN_HEIGHT 64  // OLED display height
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// Structure example to receive data
typedef struct struct_message {
   float humidity ;
  // read temperature in Celsius
  float temperature ;
 float LDRDATA;
  
} struct_message;

// Create a structure to hold the incoming data
struct_message incomingData;

void onReceive(const esp_now_recv_info_t *info, const uint8_t *data, int len) {
  memcpy(&incomingData, data, sizeof(incomingData));  // Correctly copy incoming data into struct

  Serial.print("Temperature received: ");
  Serial.println(incomingData.temperature);
  Serial.print("humidity received: ");
 Serial.println(incomingData.humidity);
 Serial.print("lightintensity data received: ");
 Serial.println(incomingData.LDRDATA);
  // Display temperature on OLED
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 10);
  display.print("TEMPERATURE: ");
  display.print(incomingData.temperature);
  display.print(" C");
  display.display();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 30);
  display.print("HUMIDITY: ");
  display.print(incomingData.humidity);
  display.display();
  display.display();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 50);
  display.print("lightintensity: ");
  display.print(incomingData.LDRDATA);
  display.display();
}

void setup() {
  Serial.begin(115200);

  // Initialize display
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;)
      ;
  }
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.print("Waiting for temp...");
  display.display();

  // Set device as a Wi-Fi station
  WiFi.mode(WIFI_STA);

  // Initialize ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // Register receive callback
  esp_now_register_recv_cb(onReceive);
}

void loop() {
  // No need to add anything in loop for receiver
}
