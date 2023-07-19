

#include <ArduinoJson.h>

// Wifi
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>

ESP8266WiFiMulti WiFiMulti;
#define USE_SERIAL Serial
WiFiClient client;
HTTPClient http;

String urlGetData = "http://172.20.10.12/praktek6-arduino-json/index.php";
String respon;

// lcd
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);

// SDA ---------------> D2
// SCL ---------------> D1
// VCC ---------------> VIN
// GND ---------------> GND

void setup() {
  WiFi.mode(WIFI_STA);
  Serial.begin(115200);
  
  WiFiMulti.addAP("Minulya", "123456789"); // Sesuaikan SSID dan password ini

  lcd.init();
  lcd.backlight();
  
  while ((WiFiMulti.run() != WL_CONNECTED))
  {
    Serial.println("Internet Not Connected");

    lcd.clear();
    lcd.setCursor(6, 0);
    lcd.print("WiFi");
    lcd.setCursor(2, 1);
    lcd.print("NOT CONNECTED");
      
    delay(1000);
  }

  Serial.println("Internet Connected");
  
  lcd.clear();
  lcd.setCursor(6, 0);
  lcd.print("WiFi");
  lcd.setCursor(2, 1);
  lcd.print("CONNECTED!!!");

  delay(1000);
}

void loop() {
  getData();

  delay(1000);
}

void getData() {
  if ((WiFiMulti.run() == WL_CONNECTED))
  {
    USE_SERIAL.print("[HTTP] Memulai...\n");
    
    http.begin( client, urlGetData );
    
    USE_SERIAL.print("[HTTP] Ambil data di database ...\n");
    int httpCode = http.GET();

    if(httpCode > 0)
    {
      USE_SERIAL.printf("[HTTP] kode response GET : %d\n", httpCode);

      if (httpCode == HTTP_CODE_OK) // code 200
      {
        respon = http.getString();

        Serial.println();
        Serial.println("Respon : " + respon);
        Serial.println();

        int str_len = respon.length() + 1; 
        char json[str_len];
        
        respon.toCharArray(json, str_len);

        DynamicJsonDocument doc(1024);
        deserializeJson(doc, json);
      
        String nama = doc["nama"];
        String alamat = doc["alamat"];
        String nim = doc["nim"];

        Serial.println("Hallo, nama saya " + nama);
        Serial.println("Saya berasal dari " + alamat);
        Serial.println("Nim saya " + nim);

        Serial.println();

        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("NAMA : " + nama) ;
        lcd.setCursor(0, 1);
        lcd.print("ALAMAT : " + alamat);
        delay(5000);
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("NIM : " + nim);
      }
    }
    else
    {
      USE_SERIAL.printf("[HTTP] GET data gagal, error: %s\n", http.errorToString(httpCode).c_str());
    }
    http.end();
  }
}
