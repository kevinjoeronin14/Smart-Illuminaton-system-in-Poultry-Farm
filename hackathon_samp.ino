 #include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <Wire.h>
#include <Time.h>
#include <TimeLib.h>
#include <Servo.h>
#include <BlynkSimpleEsp8266.h>
#define BLYNK_PRINT Serial
#include <SimpleTimer.h>
SimpleTimer timmer;
#include "DHT.h"
#define ldrPin A0
#define ledPin D3
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>
#define SSD1306_LCDHEIGHT 64
#define OLED_ADDR   0x3C
Adafruit_SSD1306 display(-1);
#if (SSD1306_LCDHEIGHT != 64)
#error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif
int timer;
Servo servo1;
Servo servo2;//defining the name usage as servo itself
int lopper;
int fanled=D8;
int bal;
#define  DHTPIN D0
#define DHTTYPE DHT11   
DHT dht(DHTPIN, DHTTYPE); 
//String command;



char auth[] = "d02da247a04b4f1eb9aae9366e3d7412";
const char* ssid = "kevin";
const char* password = "12345678i";
char pass[] = "12345678i";



#define ORG "gawt0d"
#define DEVICE_TYPE "praneetha"
#define DEVICE_ID "1843"
#define TOKEN "123456789"
String command;


char server[] = ORG ".messaging.internetofthings.ibmcloud.com";
char pubtopic[] = "iot-2/evt/poultry/fmt/json";
char topic[] = "iot-2/cmd/poultry/fmt/String";
char authMethod[] = "use-token-auth";
char token[] = TOKEN;
char clientId[] = "d:" ORG ":" DEVICE_TYPE ":" DEVICE_ID;


WiFiClient wifiClient;
void callback(char* topic, byte* payload, unsigned int payloadLength);
PubSubClient client(server, 1883, callback, wifiClient);

float h;




void setup() 
{

 wifiConnect();
  mqttConnect();

 Blynk.begin(auth, ssid, pass);
  
    dht.begin();
  timer=0;
Serial.begin(9600); //sets serial port for communication
pinMode(ldrPin, INPUT);
pinMode(ledPin, OUTPUT);
servo1.attach(D5);
servo2.attach(D6);

int templdr = analogRead(ldrPin);      
lopper=1;



display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR);
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(10,10);
  display.print("Smart Illumination");
  Serial.println("Smart Illumination");
  display.display();
   delay(3000);

}
void loop() 
{


   Blynk.run(); 
  timmer.run();

  
int gassensor = digitalRead(D7);
  //float h = gassensor;
  //Serial.print("Pin D7: ");
  //Serial.println(gassensor);
int ldrStatus = analogRead(ldrPin);
int i,k;
float t = dht.readTemperature();
//if(lopper==1)
//{
if(t>35)
  {
   display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR);
   display.clearDisplay();
   display.setTextSize(1);
   display.setTextColor(WHITE);
   display.setCursor(10,10);
   display.print("Temperature is HIGH");
   display.display();
      char q[]=("Temperature is High");
 Blynk.virtualWrite(V7, q);
    delay(100);
   Serial.println("Temperature is HIGH");
   //digitalWrite(temp_led, HIGH); //Red
   delay(200);
   //   char i[]=("Temperature is High");
 //Blynk.virtualWrite(V5, i);
      delay(100);
  }
else if(t<25)
  {
   display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR);
   display.clearDisplay();
   display.setTextSize(1);
   display.setTextColor(WHITE);
   display.setCursor(10,10);
   display.print("Temperature is LOW");
   display.display();
   char q[]=("Temperature is LOW");
 Blynk.virtualWrite(V7, q);
    delay(100);
   Serial.println("Temperature is LOW");
     // char h[]=("Temperature is Low");
// Blynk.virtualWrite(V5, h);
      delay(100);
  }
else
{
//  digitalWrite(temp_led,  LOW);
    // char j[]=("Temperature is Normal");
    display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR);
   display.clearDisplay();
   display.setTextSize(1);
   display.setTextColor(WHITE);
   display.setCursor(10,10);
   display.print("Temperature is Normal");
   display.display();
   char q[]=("Temperature is Normal");
 Blynk.virtualWrite(V7, q);
     delay(100);
     Serial.println("Temperature is Normal");
 //Blynk.virtualWrite(V5, j);
      delay(1000);
}




if (ldrStatus <= 1000)
{                              //its bright

digitalWrite(ledPin, LOW);

display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR);
   display.clearDisplay();
   display.setTextSize(1);
   display.setTextColor(WHITE);
   display.setCursor(10,10);
   display.print("Its Bright,LED is OFF : ");
   display.display();
   char w[]=("Its Bright,LED is OFF");
 Blynk.virtualWrite(V5, w);
   delay(1000);
   display.clearDisplay();
   display.setTextSize(1);
   display.setTextColor(WHITE);
     display.setCursor(10,10);
     display.print("Door is Opening");
      char e[]=("Door is Opening");
 Blynk.virtualWrite(V4, e);


Serial.println("Its Bright,Turn off the LED : ");
delay(100);
Serial.println("Door is Opening");
Serial.println(ldrStatus);
delay(100);
servo1.write(180);
servo2.write(0);
if (gassensor==0)
  {
   display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR);
   display.clearDisplay();
   digitalWrite(fanled,HIGH);
   display.setTextSize(1);
   display.setTextColor(WHITE);
   display.setCursor(10,10);
   display.print("Fan is ON");
   display.display();
   Serial.println("Fan is ON");
         char r[]=("Fan is ON");
 Blynk.virtualWrite(V6, r);
//Shows the value of LDR sensor

}
else
  {
     display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR);
   display.clearDisplay();
   digitalWrite(fanled,LOW);
   display.setTextSize(1);
   display.setTextColor(WHITE);
   display.setCursor(10,10);
   display.print("Fan is OFF");
   display.display();
   Serial.println("Fan is OFF");
   Serial.println(gassensor);
          char r[]=("Fan is OFF");
 Blynk.virtualWrite(V6, r);
   lopper=1;
    
    }
delay(10000);
timer=timer+1;

h=timer;
//Serial.println(ldrStatus);    //Shows the value of LDR sensor

} 
else if(ldrStatus > 1000)
{                              //its dark
  Serial.println("Finished");
//   elapsed=finished-start;
  // Serial.print(elapsed);
//digitalWrite(ledPin, HIGH); //led light illumination  


display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR);
   display.clearDisplay();
   display.setTextSize(1);
   display.setTextColor(WHITE);
   display.setCursor(10,10);
   display.print("Its Dark,LED is on : ");
   display.display();
     char w[]=("Its Bright,LED is Off");
 Blynk.virtualWrite(V5, w);
   delay(1000);
   display.clearDisplay();
   display.setTextSize(1);
   display.setTextColor(WHITE);
     display.setCursor(10,10);
     display.print("Door is closing");
           char e[]=("Door is closing");
 Blynk.virtualWrite(V4, e);


     
Serial.println("Its Dark,LED is ON: ");
delay(100);
Serial.println("Door is Closing");
delay(100);
servo1.write(0);
servo2.write(180);
Serial.print("Total Daylight Time : ");
Serial.print(timer);
         int z=timer;
 Blynk.virtualWrite(V2, z);
Serial.println("Hours");

Serial.print("ldrStatus:       ");

Serial.println(ldrStatus);
if(lopper==1)
{
   display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR);
   display.clearDisplay();
   digitalWrite(fanled,HIGH);
   display.setTextSize(1);
   display.setTextColor(WHITE);
   display.setCursor(10,10);
   display.print("Exhaust fan is on");
   display.display();
   Serial.println("Exhaust fan is on");
   
  
if(timer<16)
{
  k=timer;
  bal=(16-k);
           int p=bal;
 Blynk.virtualWrite(V3,p);
  
  for(i=0;i<bal;i++)
  {
    digitalWrite(ledPin, HIGH);
    delay(1000);
  }
   digitalWrite(ledPin,LOW);
}
}


lopper=0;
 if (gassensor==0)
  {
   display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR);
   display.clearDisplay();
   digitalWrite(fanled,HIGH);
   display.setTextSize(1);
   display.setTextColor(WHITE);
   display.setCursor(10,10);
   display.print("Fan is ON");
   display.display();
   Serial.println("Fan is ON");
         char r[]=("Fan is ON");
 Blynk.virtualWrite(V6, r);
//Shows the value of LDR sensor

}
else
  {
     display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR);
   display.clearDisplay();
   digitalWrite(fanled,LOW);
   display.setTextSize(1);
   display.setTextColor(WHITE);
   display.setCursor(10,10);
   display.print("Fan is OFF");
   display.display();
   Serial.println("Fan is OFF");
   Serial.println(gassensor);
         char r[]=("Fan is OFF");
 Blynk.virtualWrite(V6, r);
    
    }
}


 if(!client.loop()) {
    mqttConnect();
  }
  PublishData(t,h);
delay(100);
}



void wifiConnect() {
  Serial.print("Connecting to "); Serial.print(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.print("nWiFi connected, IP address: "); Serial.println(WiFi.localIP());
}

void mqttConnect() {
  if (!client.connected()) {
    Serial.print("Reconnecting MQTT client to "); Serial.println(server);
    while (!client.connect(clientId, authMethod, token)) {
      Serial.print(".");
      delay(500);
    }
    initManagedDevice();
    Serial.println();
  }
}
void initManagedDevice() {
  if (client.subscribe(topic)) {
    Serial.println("subscribe to cmd OK");
  } else {
    Serial.println("subscribe to cmd FAILED");
  }
}

void callback(char* topic, byte* payload, unsigned int payloadLength) {
  Serial.print("callback invoked for topic: "); Serial.println(topic);

  for (int i = 0; i < payloadLength; i++) {
    //Serial.println((char)payload[i]);
    command += (char)payload[i];
  }

command ="";
}
void PublishData(float t,int h){
 if (!!!client.connected()) {
 Serial.print("Reconnecting client to ");
 Serial.println(server);
 while (!!!client.connect(clientId, authMethod, token)) {
 Serial.print(".");
 delay(500);
 }
 Serial.println();
 }
  String payload = "{\"d\":{\"temperature\":";
  payload += t;
  payload+="," "\"daylight\":";
  payload += h;
  payload += "}}";
 Serial.print("Sending payload: ");
 Serial.println(payload);
  
 if (client.publish(pubtopic, (char*) payload.c_str())) {
 Serial.println("Publish ok");
 } else {
 Serial.println("Publish failed");
 }
}


