

#include "Button2.h";
#include <IotWebConf.h>
#include <Wire.h>
#include <Adafruit_MLX90614.h>  
#include <TFT_eSPI.h> // Graphics and font library for ST7735 driver chip
#include <SPI.h>
#include "EspMQTTClient.h"
 
#define TFT_GREY 0x5AEB // New color
#define CONFIG_PIN 35

#define BUTTON_A_PIN  35
#define BUTTON_B_PIN  0

          
const char CUSTOMHTML_BODY_INNER[] PROGMEM = "<div><img src=' data:image/png;base64,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'/></div>\n";

const int powerPin = 34;
const int plugPin = 14;
const int screenBLPin = 4;
bool screenOn = true;
const long interval = 5000; //Data update loop delay
int curX = 0;//display cursor location x
int curY = 50;// display cursor oction y
double objTempF;
double ambTempF;
double objTempC;
double ambTempC; 

Adafruit_MLX90614 mlx = Adafruit_MLX90614(); //Invoke temp sensor
 

TFT_eSPI tft = TFT_eSPI();       // Invoke library, pins defined in User_Setup.h

unsigned long targetTime = 0;    // Used for testing draw times

const char ssid[] = ""; //your WIFI name
const char pass[] = "";   // your WIFI passowrd
const char brokerIP[] = "broker.hivemq.com"; //MQTT broker server IP
const char brokerPort[] = "1883";
const char brokerUser[] = "";   // broker user name (optional)
const char brokerPass[]= "";    // broker password (optional)
const char clientName[] = "Pipe_Sensor1"; // unique name for mqtt device

// -- Initial name of the Thing. Used e.g. as SSID of the own Access Point.
const char thingName[] = "Pipeio";

// -- Initial password to connect to the Thing, when it creates an own Access Point.
const char wifiInitialApPassword[] = "password";

DNSServer dnsServer;
WebServer server(80);

IotWebConf iotWebConf(thingName, &dnsServer, &server, wifiInitialApPassword);


Button2 buttonA = Button2(BUTTON_A_PIN);
Button2 buttonB = Button2(BUTTON_B_PIN);

EspMQTTClient client(
  ssid,
  pass,
  brokerIP,  // MQTT Broker server ip
  //brokerUser,   // Can be omitted if not needed
  //brokerPass,   // Can be omitted if not needed
  clientName,     // Client name that uniquely identify your device
  brokerPort              // The MQTT port, default to 1883. this line can be omitted
);


void setup() 
{
  Serial.begin(115200);
  Serial.println();
  Serial.println("Starting up...");

  buttonA.setClickHandler(click);
  buttonB.setClickHandler(click);
  buttonA.setLongClickHandler(longpress);
  buttonB.setLongClickHandler(longpress);
  
 
  iotWebConf.setConfigPin(CONFIG_PIN);
  iotWebConf.init();// wifimanger Init
  
  // -- Set up required URL handlers on the web server.
  server.on("/", handleRoot);
  server.on("/config", []{ iotWebConf.handleConfig(); });
  server.onNotFound([](){ iotWebConf.handleNotFound(); });

  Wire.begin(21,22); // relocate/set ic2 pins (sda,sca)
  tft.init();// start screen
  tft.fillScreen(TFT_BLACK); // set sceen background to grey
  tft.setTextColor(TFT_GREY, TFT_BLACK);

  tft.setTextFont(2);
  tft.setRotation(1); // set screen horizontal
  mlx.begin();// start temperature sensor


  Serial.println("Ready.");
}


void loop() 
{
  iotWebConf.doLoop();
  buttonA.loop();
  buttonB.loop();
  //analogRead(powerPin);
  
  
  static long currentMillis;
  static byte statCounter = 0;
  int battLev = map(analogRead(powerPin), 0,2840, 0,100);
  

  
    if (millis() - currentMillis >= interval)
   {
      if (statCounter> 10)
        {
         statCounter = 0;
        }
      statCounter++;
      currentMillis = millis();
      tft.setCursor(220,0);
      tft.setTextSize(0);
      tft.print(battLev);
   
    if(iotWebConf.getState() != 4){
      tft.setCursor(curX,curY);
      tft.setTextSize(2);
      tft.print("Statrting...");
      }
  //delay (1000);//wait... dont flood mqtt sever
    if (iotWebConf.getState()== 4){
      client.loop();

     // Subscribe to "mytopic/test" 
    client.subscribe("mytopic/test", [](const String & payload) {
    });

  //assigning doubles to ic2 sensor data
  objTempF = mlx.readObjectTempF(); 
  ambTempF = mlx.readAmbientTempF();

  objTempC = mlx.readObjectTempC();
  ambTempC = mlx.readAmbientTempC();

  //converting doubles to strings for mqtt server 
  String tempString = String(objTempF);


   // Publish sensor data strings to "mytopic/test"
  client.publish("mytopic/test",tempString); // You can activate the retain flag by setting the third parameter to true
    
  //if not freezing dont raise alarm
  if(objTempF >= 33){
    tft.setCursor(curX,curY);
    tft.setTextSize(2);
    tft.println("Object Temp :");
    tft.println(objTempF);

    //Serial.print(objTempF);
    //Serial.print(" ");
    //Serial.println(ambTempF);
  
    client.publish("mytopic/test",tempString); // You can activate the retain flag by setting the third parameter to true
  }

  // if freezing or below raise alarm
    if(objTempF < 33.00){
      client.publish("mytopic/test", "Freezing!!", tempString);
      tft.setCursor(curX,curY);
      tft.println("Object Temp");
      tft.println(objTempF);
      tft.println("Freezing!!!");//alarms here
        } 
        //delay (2000);//wait... dont flood mqtt sever

    }
  }
  //Serial.println(digitalRead(screenBLPin));  

}

void onConnectionEstablished()// wait till connection to MQTT broker to do anyhting else.
{ 

  
}

/**
 * Handle web requests to "/" path.
 */
void handleRoot()
{
  // -- Let IotWebConf test and handle captive portal requests.
  if (iotWebConf.handleCaptivePortal())
  {
    // -- Captive portal request were already served.
    return;
  }
  String s = "<!DOCTYPE html><html lang=\"en\"><head><meta name=\"viewport\"content=\"width=device-width, initial-scale=1, user-scalable=no\"/>";
  s += "<title>PipeioWebConf v1</title>";
  s += "<style>body {background-color: white;text-align: center;color:";
  s += "#3366FF;font-family: Arial, Helvetica, sans-serif;}";
  s += "</style></head><body>";
  s += "<h1>Hello from your Pipeio</h1>";
  s += "<p>So far so good!</p><p>Click <a href='config'>HERE</a>";
  s += "to continue the configuration.</p>";
  // base64 image
  s += FPSTR(CUSTOMHTML_BODY_INNER);
  s += "</body></html>\n";
  //end base64 image (fml btw)
  server.send(200, "text/html", s);
}

void click(Button2& btn) {    if (btn == buttonA) {
      Serial.println("screen on");
      digitalWrite(screenBLPin, HIGH);
    } else if (btn == buttonB) {
      Serial.println("screen on");
      digitalWrite(screenBLPin, HIGH);      
    }
}

void longpress(Button2& btn) {
    unsigned int time = btn.wasPressedFor();
    
    if (time > 1500){
        digitalWrite(screenBLPin, LOW);
        Serial.println("screen off");

    } 
 
 
}
