#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <SoftwareSerial.h>
#include <StringTokenizer.h>
#include <Adafruit_NeoPixel.h>


#define PIN            0
#define NUMPIXELS      10
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);


int delayval = 100;
MDNSResponder mdns;

// Your wifi name
const char* ssid = "";
//Your wifi password
const char* password = "";

ESP8266WebServer server(80);

String webPage = "";

int gpio0_pin = 0;

void setup(void){
	// Demo test page
 webPage += "<h1>ESP8266 Web Server</h1><p>User v028 #1 <a href=\"ledArgs?LED0=0&RED=0&GREEN=0&BLUE=128\"><button>ON</button></a>&nbsp;<a href=\"ledArgs?LED0=0&RED=0&GREEN=0&BLUE=0\"><button>OFF</button></a>&nbsp;";
    
  // preparing GPIOs
  pinMode(gpio0_pin, OUTPUT);
  delay(1000);
  Serial.begin(115200);
  // Make changes for Esp8266 devices behaviour
  //WiFi.mode(WIFI_STA); 
  //WiFi.mode(WIFI_AP); 
  //WiFi.mode(WIFI_AP_STA); 
  WiFi.begin(ssid, password);
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
 
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.print("Configuring access point...");

  if (mdns.begin("esp8266", WiFi.localIP())) {
    Serial.println("MDNS responder started");
  }
  server.on("/", [](){
    server.send(200, "text/html", webPage);
  });
  server.on ( "/ledArgs", handleSubmit );
  server.begin();
  pixels.begin(); // This initializes the NeoPixel library.
  for(int i=0;i<NUMPIXELS;i++){
  pixels.setPixelColor(i, pixels.Color(0,0,0)); // Moderately bright red color.
  }
  pixels.show(); // This sends the updated pixel color to the hardware.
  
  Serial.println("HTTP server started");
}
 
void loop(void){ 
 server.handleClient();
} 

void handleSubmit() {
  // Actualise le GPIO / Update GPIO 
  int C1LED0;
  String sC1LED0,sRed,sGreen,sBlue;;  
  sC1LED0 = server.arg("LED0");  
  sRed = server.arg("RED");
  sGreen = server.arg("GREEN");
  sBlue = server.arg("BLUE");
 
  if(sC1LED0!="")
  {
    C1LED0 = sC1LED0.toInt();
    pixels.setPixelColor(C1LED0, pixels.Color(sRed.toInt(),sGreen.toInt(),sBlue.toInt())); 
    pixels.show(); // This sends the updated pixel color to the hardware.
  }
  delay(delayval);
  server.send(200, "text/html", webPage);
  }

