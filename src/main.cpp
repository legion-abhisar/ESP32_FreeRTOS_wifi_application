#include <Arduino.h>
#include <WiFi.h>

// put function declarations here:
#define WIFI_NETWORK "ACT102685105293_Asit"
#define WIFI_PASSWORD "Shadowblack123"
#define WIFI_TIMEOUT_MS 20000

void keepWiFiAlive(void * parameters);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  xTaskCreatePinnedToCore(
    keepWiFiAlive,
    "Keep WiFi Alive",
    5000,
    NULL,
    1,
    NULL,
    CONFIG_ARDUINO_RUNNING_CORE
  );
}

void loop() {
  // put your main code here, to run repeatedly:
  
}

// put function definitions here:
void keepWiFiAlive(void * parameters) {
  vTaskDelay(20000 / portTICK_PERIOD_MS);
  
  for(;;) {
    if(WiFi.status() == WL_CONNECTED) {
      Serial.println("WiFi still connected");
      vTaskDelay(1000 / portTICK_PERIOD_MS);
      continue;
    }

    // Try to connect to WiFi
    Serial.println("WiFi Connecting");
    WiFi.mode(WIFI_STA);
    WiFi.begin(WIFI_NETWORK, WIFI_PASSWORD);

    unsigned long startAttemptTime = millis();

    // Keep looping while we're not connected and haven't reached the timeout
    while (WiFi.status() != WL_CONNECTED &&
      millis() - startAttemptTime < WIFI_TIMEOUT_MS)
    {}

    // When we couldn't make a WiFi connection
    if(WiFi.status() != WL_CONNECTED) {
      Serial.println("WIFI Failed");
      vTaskDelay(2000 / portTICK_PERIOD_MS);
    }

    Serial.println("WiFi Connected : ");
    Serial.println(WiFi.localIP());
  }
}