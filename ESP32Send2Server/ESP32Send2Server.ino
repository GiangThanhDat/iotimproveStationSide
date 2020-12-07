#include <WiFi.h>
#include <HTTPClient.h>
//#include <OpenLM35.h>
//#include <OpenMQ2.h>
#include <DHT.h>
//
//#define DHTPIN 27
//#define DHTTYPE DHT11
//DHT dht(DHTPIN, DHTTYPE);

const char* ssid = "Thanh Dat";
const char* pass = "4849398892";

const String host = "192.168.1.2"; // My local IP ADRESS send to

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, pass);
  Serial.println("Connecting");
  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
//  dht.begin();
}

void loop() {
  Serial.println();
  if ((WiFi.status() == WL_CONNECTED)) { // Check the current connection status
//    float h = dht.readHumidity();
//    float t = dht.readTemperature();
//    if (isnan(h) || isnan(t)) {
//      Serial.println("Failed to read from DHT sensor!");
//      return;
//    }
//    Serial.print("Humidity: ");
//    Serial.print(h);
//    Serial.print(" %\t");
//    Serial.print("Temperature: ");
//    Serial.print(t);
//    Serial.print(" *C ");
    HTTPClient http;
    http.begin(String("http://") + host + "/iotimprove/giatri/thuthap&1=" +random(0,100) + "&2=" + random(0,100) + "&3=" + random(0,100) + "&4=" + random(random(0,100) - 50,random(0,100) + 1)); // Specify the URL
    int httpCode = http.GET(); // Make the request
    if (httpCode > 0) { //Check for the returning code
        String payload = http.getString();
        Serial.println(httpCode);
        Serial.println(payload);
      }
    else {
      Serial.println("Error on HTTP request");
      Serial.println(httpCode);
    }
    http.end(); //Free the resources
  }
  delay(1000);
}
