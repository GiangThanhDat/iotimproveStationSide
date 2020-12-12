#include <WiFi.h>
#include <HTTPClient.h>
#include <OpenMQ135.h>
#include <OpenMQ2.h>
#include <DHT.h>

#define DHTPIN 22
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

#define MQ2 35
#define MQ135 34

OpenMQ2 mq2(MQ2);  //OpenMQ2(analogpin): declare analog pin of sensor.
OpenMQ135 mq135(MQ135);  //OpenMQ135(analogpin): declare analog pin of sensor.


const char* ssid = "wifi minh";
const char* pass = "phuminh07";

const String host = "192.168.43.114"; // My local IP ADRESS send to


void ConnectToWiFi()
{
 
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, pass);  
  Serial.print("Connecting to "); 
  Serial.println(ssid);
  uint8_t i = 0;
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print('.');
    delay(500);
 
    if ((++i % 16) == 0)
    {
      Serial.println(F(" still trying to connect"));
    }
  }
 
  Serial.print(F("Connected. My IP address is: "));
  Serial.println(WiFi.localIP());
}

void setup() {
  Serial.begin(115200);
  
  pinMode(MQ2, INPUT);
  pinMode(MQ135, INPUT);
  pinMode(DHTPIN, INPUT);

  ConnectToWiFi();
  
  dht.begin();
  mq2.setup(5, 10);   //begin(Vcc, resolution): declare Vcc and resolution value of sensor.
  mq135.setup(5, 10);   //begin(Vcc, resolution): declare Vcc and resolution value of sensor.
  
  // Getting info about sensor.
  Serial.println("*********************< OpenMQ2 >************************");
  Serial.print("Name: ");
  Serial.println(mq2.getSensor().name);
  Serial.print("Version: ");
  Serial.println(mq2.getSensor().version);
  Serial.print("Type: ");
  Serial.println(mq2.getSensor().type);
  Serial.print("Min value: ");
  Serial.println(mq2.getSensor().min_value);
  Serial.print("Max value: ");
  Serial.println(mq2.getSensor().max_value);
  Serial.print("Vcc: ");
  Serial.print(mq2.getSensor().Vcc);
  Serial.println("V");
  Serial.print("Resolution: ");
  Serial.print(mq2.getSensor().resolution);
  Serial.println("bit");
  Serial.println("******************************************************");

//// Getting info about sensor.
  Serial.println("*********************< OpenMQ135 >************************");
  Serial.print("Name: ");
  Serial.println(mq135.getSensor().name);
  Serial.print("Version: ");
  Serial.println(mq135.getSensor().version);
  Serial.print("Type: ");
  Serial.println(mq135.getSensor().type);
  Serial.print("Min value: ");
  Serial.println(mq135.getSensor().min_value);
  Serial.print("Max value: ");
  Serial.println(mq135.getSensor().max_value);
  Serial.print("Vcc: ");
  Serial.print(mq135.getSensor().Vcc);
  Serial.println("V");
  Serial.print("Resolution: ");
  Serial.print(mq135.getSensor().resolution);
  Serial.println("bit");
  Serial.println("******************************************************");
 
}

void loop() {
  Serial.println();
  if ((WiFi.status() == WL_CONNECTED)) { // Check the current connection status  
    String httpGetSendingRequest = String("http://") + host + "/iotimprove/giatri/thuthap";
    float doAm = dht.readHumidity();
    float nhietDo = dht.readTemperature();
//    if (isnan(doAm) || isnan(nhietDo)) {
//      Serial.println("Failed to read from DHT sensor!");
////      return;
//    }else{
      Serial.print("Temperature: ");
      Serial.print(nhietDo);// nhiệt độ tại trạm 1 có mã là 1
      httpGetSendingRequest = httpGetSendingRequest + "&1=" + nhietDo;
      Serial.println(" *C ");  
      
      Serial.print("Humidity: ");
      Serial.print(doAm); // độ ẩm tại trạm 1 có mã là 2
      httpGetSendingRequest = httpGetSendingRequest + "&2=" + doAm;
      Serial.print(" %\t");
//    }    
    float LPG = mq2.readLPG(); // LPG ở trạm 1 có mã là 5
    if (isnan(LPG)) {
      Serial.println("Failed to read from OpenMQ2!");      
    }else{
      Serial.println(String("MQ2-LPG: ") + LPG + "ppm");
      httpGetSendingRequest = httpGetSendingRequest + "&5=" + LPG;
    }
    float CO2 = mq135.readCO2(); // CO2 ở trạm 1 có mã là 6
    float CO = mq135.readCO();// CO2 ở trạm 1 có mã là 7
//    if (isnan(CO2) || isnan(CO)) {
//      Serial.println("Failed to read from OpenMQ135!");
//    }else{
      Serial.print(String("MQ135-CO: ") + CO + "ppm\t");
      Serial.print(String("MQ135-CO2: ") + CO2 + "ppm");
      httpGetSendingRequest = httpGetSendingRequest + "&6=" + CO2;
      httpGetSendingRequest = httpGetSendingRequest + "&7=" + CO;
//    }
    
    Serial.println(httpGetSendingRequest);
    HTTPClient http;
    http.begin(httpGetSendingRequest); // Specify the URL
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
  delay(2000);
}
