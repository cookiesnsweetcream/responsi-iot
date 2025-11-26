#include <WiFi.h>
#include <DHT.h>
#include <HTTPClient.h>

#define DHTPIN 27
#define DHTTYPE DHT22
#define LDR_PIN 34

DHT dht(DHTPIN, DHTTYPE);

const char* ssid = "GEDUNG B - WPA";
const char* password = "123123123";

unsigned long lastSend = 0;


String cekCahaya(int adcValue) {
  if (adcValue < 1500) return "Terang";       // Cahaya kena → nilai kecil
  else if (adcValue < 3000) return "Agak Gelap";
  else return "Gelap";                        // Tidak kena cahaya → nilai besar
}


void konekWiFiCepat() {
  Serial.println("Menghubungkan ke WiFi...");
  WiFi.begin(ssid, password);

  unsigned long start = millis();
  while (WiFi.status() != WL_CONNECTED && millis() - start < 5000) {
    Serial.print(".");
    delay(300);
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\nWiFi Connected!");
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());
    Serial.print("MAC Address: ");
    Serial.println(WiFi.macAddress());
  } else {
    Serial.println("\nWiFi gagal terhubung (timeout 5 detik)");
  }
}

void setup() {
  Serial.begin(115200);
  dht.begin();
  pinMode(LDR_PIN, INPUT);

  konekWiFiCepat();
}

void loop() {
  float suhu = dht.readTemperature();
  float kelembapan = dht.readHumidity();
  int cahayaRaw = analogRead(LDR_PIN);
  String kondisiCahaya = cekCahaya(cahayaRaw);

  Serial.println("====================================");

  // TAMPILKAN STATUS WIFI DI OUTPUT
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("Koneksi Wi-Fi: Terhubung");
    Serial.println("IP Address   : " + WiFi.localIP().toString());
    Serial.println("MAC Address  : " + WiFi.macAddress());
  } else {
    Serial.println("Koneksi Wi-Fi: Tidak Terhubung");
  }

  Serial.println("Suhu          : " + String(suhu, 2) + " °C");
  Serial.println("Kelembapan    : " + String(kelembapan, 2) + " %");
  Serial.println("Cahaya (ADC)  : " + String(cahayaRaw));
  Serial.println("Kondisi       : " + kondisiCahaya);
  Serial.println("====================================\n");

  delay(1000);   
}
