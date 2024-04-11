#include <HTTPClient.h>
#include "settings.h"
#include <M5StickC.h>

// ネットワーク設定
const char *ssid = SSID;
const char *password = PASSWORD;
const char *host = HOSTNAME;
const int port = PORT;
const int RESPONSE_TIMEOUT_MILLIS = 5000;

const int HOURS = TARGET_HOURS;

void updateLatest();
int getDiffHours(int baseHours);

/*
  初回処理
*/
void setup()
{
  // 初期化
  M5.begin();

  // LCD スクリーン初期化
  M5.Lcd.fillScreen(BLACK);
  M5.Lcd.setTextColor(WHITE);
  M5.Axp.ScreenBreath(10);
  M5.Lcd.setRotation(1);
  M5.Lcd.setTextSize(1);
  M5.Lcd.println("Time Indicator");

  // Wi-Fi 接続
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    // 接続待ち
    delay(500);
    Serial.print(".");
  }

  // 接続成功後
  Serial.println();
  Serial.println("Wi-Fi connected.");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  M5.Lcd.print("IP address: ");
  M5.Lcd.println(WiFi.localIP());
  delay(3000);
}

/*
  メインループ
*/
void loop()
{
  M5.update();

  // 最新の状態を取得
  updateLatest();

  delay(5 * 60 * 1000);
}

/**
 * サーバーから自機の最新の状態を取得します。
 */
void updateLatest()
{
  M5.Lcd.fillScreen(BLACK);

  // サーバーから最新情報を取得
  int hours = getDiffHours(TARGET_HOURS);

  Serial.print(hours);
  Serial.println(" Hours");

  M5.Lcd.setTextSize(10);
  M5.Lcd.drawCentreString(String(hours), 160 / 2, 15, 1);

  Serial.println("Update Completed.");
}

/**
 * 現在時刻から指定した時間の差を返します。
 */
int getDiffHours(int baseHours)
{
  HTTPClient client;
  client.setTimeout(RESPONSE_TIMEOUT_MILLIS);
  String url = "https://" + String(host);

  if (!client.begin(url))
  {
    Serial.println("Connection failed.");
    return -1;
  }

  int httpCode = client.GET();
  String now = client.getString();
  client.end();
  Serial.println(now);

  // 時間の差を計算
  long hours = now.substring(11, 13).toInt();
  long minutes = now.substring(14, 16).toInt();
  long diffHours = (hours > baseHours) ? (24 - (hours - baseHours)) : (baseHours - hours);

  if (minutes > 0)
  {
    diffHours--;
  }

  return (int)diffHours;
}
