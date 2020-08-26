
#include <WiFi.h>
#include <esp_wifi_types.h>
#include <WebServer.h>
const char* www_username = "Admin";
const char* www_password = "Demo123";
const char* s="MAKAUT_WB";
const char* p="Cisco@123";
WebServer server(80);

uint8_t LED1pin = 19;//BUZEER/RELAY
bool LED1status = LOW;
uint8_t OutPUT = 22;//5V POWER SUPPLY
uint8_t LED2pin = 12; //LED
bool LED2status = LOW;
bool disablesensor=false;
int flag=0;
// defines pins numbers       
const int trigPin = 2;
const int echoPin = 4;
const int Buzzer=23;
const int SENSORled=32;
// defines variables
long duration;
int distance;


void setup() {
  // put your setup code here, to run once:
Serial.begin(115200);
pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
pinMode(echoPin, INPUT); // Sets the echoPin as an Input
pinMode(22, OUTPUT);
pinMode(Buzzer,OUTPUT);
pinMode(SENSORled,OUTPUT);//CHECK SENSOR WORKING
digitalWrite(Buzzer,LOW);//INITIALIZE BUZZER
digitalWrite(SENSORled,LOW);//INITIALIZE Sled
digitalWrite(22,LOW);//TO CHECK WIFI CONNECTED STATUS
wifi_connect();
Serial.println(WiFi.macAddress());
digitalWrite(22,HIGH);

  pinMode(LED1pin, OUTPUT);
  pinMode(LED2pin, OUTPUT);
  
  
  delay(100); 

  server.on("/", [](){
    while(!server.authenticate(www_username, www_password))
    {
      return server.requestAuthentication();
    }
     server.send(200, "text/html", SendHTML(LOW,LOW));
  });
   server.begin();
Serial.println("Server Started");
  Serial.print("Open http://");
  Serial.print(WiFi.localIP());
  Serial.println("/ in your browser to see it working");
  server.on("/led1on", handle_led1on);
  server.on("/led1off", handle_led1off);
  server.onNotFound(handle_NotFound);
  server.handleClient();
delay(1000);
  Serial.println("HTTP server started");
}

void loop() {
  // put your main code here, to run repeatedly:
  // Serial.println("Before Client");
   server.handleClient();
  // Serial.println("After client");
  if(LED1status)
  {digitalWrite(LED1pin, HIGH);}
  else
  {digitalWrite(LED1pin, LOW);}
  
  if(LED2status)
  {digitalWrite(LED2pin, HIGH);}
  else
  {digitalWrite(LED2pin, LOW);}
  if(!flag)
  {
    sensor_check();
    digitalWrite(SENSORled,HIGH);
   
  }


  
}

void sensor_check()
{
      //wifi_connect();
     // Serial.print("!flag=");
   // Serial.println(flag);
// Clears the trigPin
digitalWrite(trigPin, LOW);
delayMicroseconds(2);

// Sets the trigPin on HIGH state for 10 micro seconds
digitalWrite(trigPin, HIGH);
delayMicroseconds(10);
digitalWrite(trigPin, LOW);

// Reads the echoPin, returns the sound wave travel time in microseconds
duration = pulseIn(echoPin, HIGH);

// Calculating the distance
distance= duration*0.034/2;

// Prints the distance on the Serial Monitor
Serial.print("Distance: ");
Serial.println(distance);
if(distance> 40 && distance<100)
{
  
  digitalWrite(Buzzer,HIGH);
}
else{
  digitalWrite(Buzzer,LOW);
}
delay(2000);
}

void wifi_connect()
{

  WiFi.begin(s,p);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

}

void handle_OnConnect() {
  LED1status = LOW;
 // LED2status = LOW;
  Serial.println("GPIO4 Status: OFF | GPIO5 Status: OFF");
  server.send(200, "text/html", SendHTML(LED1status,LED2status)); 
}

void handle_led1on() {
  LED1status = HIGH;
  flag=1;
  //LED2status=HIGH;
  Serial.println("GPIO4 Status: ON");
  server.send(200, "text/html", SendHTML(true,LED2status)); 
}

void handle_led1off() {
  flag=0;
  LED1status = LOW;
  LED2status=LOW;
  Serial.println("GPIO4 Status: OFF");
  server.send(200, "text/html", SendHTML(false,LED2status)); 
}


void handle_NotFound(){
  server.send(404, "text/plain", "Not found");
}

String SendHTML(uint8_t led1stat,uint8_t led2stat){
  String ptr = "<!DOCTYPE html> <html>\n";
  ptr +="<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\n";
  ptr +="<title>Switch Control</title>\n";
  ptr +="<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}\n";
  ptr +="body{margin-top: 50px;} h1 {color: #444444;margin: 50px auto 30px;} h3 {color: #444444;margin-bottom: 50px;}\n";
  ptr +=".button {display: block;width: 80px;background-color: #3498db;border: none;color: white;padding: 13px 30px;text-decoration: none;font-size: 25px;margin: 0px auto 35px;cursor: pointer;border-radius: 4px;}\n";
  ptr +=".button-on {background-color: #3498db;}\n";
  ptr +=".button-on:active {background-color: #2980b9;}\n";
  ptr +=".button-off {background-color: #34495e;}\n";
  ptr +=".button-off:active {background-color: #2c3e50;}\n";
  ptr +="p {font-size: 14px;color: #888;margin-bottom: 10px;}\n";
  ptr +="</style>\n";
  ptr +="</head>\n";
  ptr +="<body>\n";
  ptr +="<h1>Web Server</h1>\n";
 
  
   if(led1stat)
  {ptr +="<p>Switch Status: ON</p><a class=\"button button-off\" href=\"/led1off\">OFF</a>\n";}
  else
  {ptr +="<p>Switch Status: OFF</p><a class=\"button button-on\" href=\"/led1on\">ON</a>\n";}


  ptr +="</body>\n";
  ptr +="</html>\n";
  return ptr;
}
