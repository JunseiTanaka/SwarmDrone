#include <Adafruit_NeoPixel.h>
#include <WiFi.h>

#define PIN 48
#define NUMPIXELS 1

const char *ssid = "Junsei_iPhone";
const char *password = "junsei1234";
WiFiServer server(80);
//TODO: 固定IPアドレス
/*
#include <WiFi.h>

const char *ssid = "YourNetworkSSID";
const char *password = "YourNetworkPassword";
IPAddress staticIP(192, 168, 1, 123); // 任意の固定IPアドレスを指定します
IPAddress gateway(192, 168, 1, 1);     // ゲートウェイのIPアドレスを指定します
IPAddress subnet(255, 255, 255, 0);    // サブネットマスクを指定します

void setup() {
  Serial.begin(115200);
  
  // WiFi接続の設定
  WiFi.begin(ssid, password);

  // 静的IPアドレスの設定
  if (WiFi.config(staticIP, gateway, subnet)) {
    Serial.println("Static IP configured successfully");
  } else {
    Serial.println("Static IP configuration failed");
  }

  // WiFi接続待機
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }

  Serial.println("Connected to WiFi");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  // ここにループ処理を追加
}
*/
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

void setup() {
  Serial.begin(9600);
  delay(500);
  Serial.println("WiFi Connection Start");

  pixels.begin(); // LEDの初期化

  // 初めはLEDを赤く点灯
  pixels.setPixelColor(0, pixels.Color(255, 0, 0));
  pixels.show();

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  Serial.println();
  Serial.println("WiFi Connected.");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  // WiFi接続が確立されたらLEDをダークブルーに点灯
  pixels.setPixelColor(0, pixels.Color(0, 0, 30));
  pixels.show();

  server.begin();
}

void loop() {
  WiFiClient client = server.available();

  if (client) {
    Serial.println("New Client.");

    while (client.connected()) {
      if (client.available()) {
        String line = client.readStringUntil('\r'); // リクエストの1行を読み込む
        if (line == "\n") { // 空行が来たら、次はボディ部分
          String request = client.readString(); // ボディ部分の全てを読み込む
          Serial.println("Received Request:");
          Serial.println(request);

          // もしもリクエストが "wifi" を含む場合、LEDを黄色に点灯
          if (request.indexOf("wifi") != -1) {
            pixels.setPixelColor(0, pixels.Color(30, 30, 0)); // 黄色
            pixels.show();
          }
          else {
            pixels.setPixelColor(0, pixels.Color(0, 30, 30)); // 
            pixels.show();
          }

          // HTTPレスポンスを送信
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/plain");
          client.println("Connection: close");
          client.println();
          client.println("Message received by ESP32.");

          client.flush();
          delay(10); // 応答を完全に送信するための遅延

          Serial.println("Response Sent.");

          break; // 1つのリクエストに対して1回のみレスポンスを送信するため、ループを抜ける
        }
      }
    }

    client.stop();
    Serial.println("Client Disconnected.");
  }
}

