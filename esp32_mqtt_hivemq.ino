
#include "DHT.h"
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include "ArduinoJson.h"
#include <Wire.h>
// Thông tin về wifi

#define ssid "*"
#define password "12345678"
//#define mqtt_server "broker.mqtt-dashboard.com"

#define mqtt_server "broker.emqx.io"
const uint16_t mqtt_port = 1883; //Port của MQTT

//#define mqtt_server "efcd8b499d544cb291cef21f5fa6f04a.s1.eu.hivemq.cloud"
//const uint16_t mqtt_port = 8884; //Port của MQTT

#define topic1 "temperature-humidity"

#define DHTPIN D7
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);

WiFiClient espClient;
//WiFiClientSecure espClient;
PubSubClient client(espClient);

void setup()
{
  Serial.begin(115200);

  Wire.begin();
  pinMode(A0, INPUT); //gan A0 cho LDR


  setup_wifi();                             //thực hiện kết nối Wifi
  client.setServer(mqtt_server, mqtt_port); // cài đặt server và lắng nghe client ở port 1883
  client.setCallback(callback);             // gọi hàm callback để thực hiện các chức năng publish/subcribe

  //    if (!client.connected())
  //    { // Kiểm tra kết nối
  //      reconnect();
  //    }
  client.subscribe("livingroomLight206");
  client.subscribe("livingroomAirConditioner206");
  client.subscribe("television206");
  client.subscribe("bedroomLight");
  client.subscribe("bedroomAirConditioner");
  client.subscribe("airVent");

  pinMode(D1, OUTPUT); // livingroomLight
  pinMode(D2, OUTPUT); // livingroomAirConditioner
  pinMode(D3, OUTPUT); // television
  pinMode(D4, OUTPUT); // bedroomLight
  pinMode(D5, OUTPUT); // bedroomAirConditioner
  pinMode(D6, OUTPUT); // airVent
  dht.begin();
}

// Hàm kết nối wifi
void setup_wifi()
{
  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  // in ra thông báo đã kết nối và địa chỉ IP của ESP8266
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

// Hàm call back để nhận dữ liệu
void callback(char *topic, byte *payload, unsigned int length)
{
  //-----------------------------------------------------------------
  //in ra tên của topic và nội dung nhận được
  Serial.print("Co tin nhan moi tu topic: ");
  Serial.println(topic);
  char p[length + 1];
  memcpy(p, payload, length);
  p[length] = NULL;
  String message(p);

  if (String(topic) == "livingroomLight206")
  {
    if (message == "livingroomLightOn")
    {
      digitalWrite(D1, HIGH);
    }
    if (message == "livingroomLightOff")
    {
      digitalWrite(D1, LOW);
    }
  }

  if (String(topic) == "livingroomAirConditioner206")
  {
    if (message == "livingroomAirConditionerOn")
    {
      digitalWrite(D2, HIGH);
    }
    if (message == "livingroomAirConditionerOff")
    {
      digitalWrite(D2, LOW);
    }
  }

  if (String(topic) == "television206")
  {
    if (message == "televisionOn")
    {
      digitalWrite(LED_BUILTIN, HIGH);
    }
    if (message == "televisionOff")
    {
      digitalWrite(LED_BUILTIN, LOW);
    }
  }

  if (String(topic) == "bedroomLight")
  {
    if (message == "bedroomLightOn")
    {
      digitalWrite(D4, HIGH);
    }
    if (message == "bedroomLightOff")
    {
      digitalWrite(D4, LOW);
    }
  }

  if (String(topic) == "bedroomAirConditioner")
  {
    if (message == "bedroomAirConditionerOn")
    {
      digitalWrite(D5, HIGH);
    }
    if (message == "bedroomAirConditionerOff")
    {
      digitalWrite(D5, LOW);
    }
  }

  if (String(topic) == "airVent")
  {
    if (message == "airVentOn")
    {
      digitalWrite(D6, HIGH);
    }
    if (message == "airVentOff")
    {
      digitalWrite(D6, LOW);
    }
  }

  Serial.println(message);
  Serial.write(payload, length);
  Serial.println();
  //-------------------------------------------------------------------------
}

//Hàm reconnect thực hiện kết nối lại khi mất kết nối với MQTT Broker
void reconnect()
{
  while (!client.connected()) // Chờ tới khi kết nối
  {
    String clientId = "clientId-mI6qnOJ5KU";
    if (client.connect(clientId.c_str())) //kết nối vào broker
  // if (client.connect(clientId.c_str(),"shaypc731@gmail.com", "Templar1")) //kết nối vào broker
    {
        Serial.println("Đã kết nối:");
        //đăng kí nhận dữ liệu từ topic
        client.subscribe("livingroomLight206");
        client.subscribe("livingroomAirConditioner206");
        client.subscribe("television206");
        client.subscribe("bedroomLight");
        client.subscribe("bedroomAirConditioner");
        client.subscribe("airVent");
      }
      else
      {
        // in ra trạng thái của client khi không kết nối được với broker
        Serial.print("Lỗi:, rc=");
        Serial.print(client.state());
        Serial.println(" try again in 5 seconds");
        // Đợi 5s
        delay(5000);
      }
    }
  }

  unsigned long lastMsg = 0;
  void loop()
  {
    if (!client.connected()){// Kiểm tra kết nối
            reconnect();
    }
    client.loop();

    long now = millis();
    if (now - lastMsg > 2000)
    {
      lastMsg = now;
      int h = dht.readHumidity();
      // Read temperature as Celsius (the default)
      int t = dht.readTemperature();
      // Read temperature as Fahrenheit (isFahrenheit = true)
      float luminance = 0.00, ADC_value = 0.0048828125, LDR_value;
      LDR_value = analogRead(A0);
      Serial.print(LDR_value);
      luminance = (250 / (ADC_value * LDR_value)) - 50;
      luminance = 50;
      // kiem tra DHT duoc noi dung chan hay k
      if (isnan(h) || isnan(t))
      {
        Serial.println(F("Failed to read from DHT sensor!"));
        return;
      }
      char tempString[10];
      sprintf(tempString, "%d", t);
      char humiString[10];
      sprintf(humiString, "%d", h);
      char lightString[10];
      sprintf(lightString, "%f", luminance);

      StaticJsonDocument<100> doc;
      doc["Temperature"] = t;
      doc["Humidity"] = h;
      doc["Light"] = luminance;

      char buffer[256];
      serializeJson(doc, buffer);
      client.publish("temperature-humidity206", buffer);
      Serial.println("buffer");
      Serial.println(buffer);
    }
  }
