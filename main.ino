/* OOOOK FUNZIONA
 *  Bisogna usare il circuito integrato L293D per pilotare i segnali RGB come se fossero sei servomotori:
 *   
 * IoT ESP8266 Based Mood Lamp (RGB LED) Controller Program
 * https://circuits4you.com
 * https://circuits4you.com/2019/03/28/esp8266-iot-rgb-led-strip-mood-lamp-controller/
 * 
 *  L293D      ESP8266         SMDd050
 *             Wemos D1 mini
 *  1 EN1      -
 *  2  Input1  D5
 *  3  Output1 -                Blue
 *  4  Ground  G
 *  5  Ground  G
 *  6  Output2 -                Green
 *  7  Input2  D6
 *  8  12Vcc   -                12Vcc
 *  9  EN2     -
 *  10 Input3  D7
 *  11 Output3 -                Red
 *  12 Ground  G
 *  13 Ground  G
 *  14 Output4 -
 *  15 Input4  -
 *  16 5Vcc    5V               
 */
 
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include "index.h"
 
//SSID and Password of your WiFi router
const char* ssid = "Vodafone-34162499";
const char* password = "exvwmfckpc5bkav";
 
ESP8266WebServer server(80);
 
//LED Connections
//const int BlueLED = 16; // D0
//const int RedLED = 5;    // D1 
//const int GreenLED = 4; // D2

// per ESP8266 Wemos D1 mini è meglio d5, d6, d7
const int BlueLED = 14; // D5
const int RedLED = 12;    // D6 
const int GreenLED = 13; // D7
 
//=======================================================================
//                    handles main page
//=======================================================================
void handleRoot() {
  Serial.println("Root Page Requested");
  server.send(200, "text/html", MAIN_page);
}
 
//=======================================================================
//                    Handle Set Color
//=======================================================================
void handleForm() {
  //Saperate Colors are sent through javascript
  String red = server.arg("r");
  String green = server.arg("g");
  String blue = server.arg("b");
  int r = red.toInt();
  int g = green.toInt();
  int b = blue.toInt();
 
  Serial.print("Red:");Serial.println(r);
  Serial.print("Green:");Serial.println(g);
  Serial.print("Blue:");Serial.println(b);
  
  //PWM Correction 8-bit to 10-bit
  r = r * 4; 
  g = g * 4;
  b = b * 4;
 
  //for ULN2003 or Common Cathode RGB Led not needed
  /*
  r = 1024 - r;
  g = 1024 - g;
  b = 1024 - b;
  */
  //ESP supports analogWrite All IOs are PWM
  analogWrite(RedLED,r);
  analogWrite(GreenLED,g);
  analogWrite(BlueLED,b);
 
  server.sendHeader("Location", "/");
  server.send(302, "text/plain", "Updated-- Press Back Button");
 
  delay(500);  
}
//=======================================================================
//                    SETUP
//=======================================================================
void setup(){
  Serial.begin(115200);   //Start serial connection  
  
  pinMode(RedLED,OUTPUT);
  pinMode(GreenLED,OUTPUT);
  pinMode(BlueLED,OUTPUT);
  
  WiFi.mode(WIFI_STA);
  
  WiFi.begin(ssid, password);     //Connect to your WiFi router
  Serial.println("");
  
  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
 
  //If connection successful show IP address in serial monitor
  Serial.println("");
  Serial.print("Connected to ");  
  Serial.println("WiFi");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());  //IP address assigned to your ESP
  
  server.on("/", handleRoot);  //Associate handler function to path
  server.on("/setRGB",handleForm);
    
  server.begin();                           //Start server
  Serial.println("HTTP server started");
}
//=======================================================================
//                    LOOP
//=======================================================================
void loop(){
  server.handleClient();
}
