#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <SoftwareSerial.h>
#include <StringTokenizer.h>


MDNSResponder mdns;

// Replace with your network credentials
const char* ssid = "iAstra";
const char* password = "singhisking";


//const char* ssid = "raneesha";
//const char* password = "raneesha123";

ESP8266WebServer server(80);

String webPage = "";
String sortCompleteList="";
int sortCount=0; 

int gpio0_pin = 0;
int gpio2_pin = 2;
int gpio4_pin = 4;
int gpio5_pin = 5;
int gpio12_pin = 12;
int gpio13_pin = 13;
int gpio14_pin = 14;
int gpio15_pin =15;
int gpio16_pin = 16;

void setup(void){
 webPage += "<h1>ESP8266 Web Server</h1><p>Socket #1 <a href=\"ledArgs?LED5=0&LED4=0&LED2=0&LED0=0&LED12=1&LED13=0&LED14=0&LED16=0&LED15=0\"><button>ON</button></a>&nbsp;<a href=\"ledArgs?LED5=0&LED4=0&LED2=0&LED0=0&LED12=0&LED13=0&LED14=0&LED16=0&LED15=0\"><button>OFF</button></a></p>";
  webPage += "<p>Socket #2 <a href=\"ledArgs?LED5=0&LED4=0&LED2=0&LED0=0&LED12=0&LED13=1&LED14=0&LED16=0&LED15=0\"><button>ON</button></a>&nbsp;<a href=\"ledArgs?LED5=0&LED4=0&LED2=0&LED0=0&LED12=0&LED13=0&LED14=0&LED16=0&LED15=0\"><button>OFF</button></a></p>";
  webPage += "<p>Socket #3 <a href=\"ledArgs?LED5=0&LED4=0&LED2=0&LED0=0&LED12=1&LED13=1&LED14=0&LED16=0&LED15=0\"><button>ON</button></a>&nbsp;<a href=\"ledArgs?LED5=0&LED4=0&LED2=0&LED0=0&LED12=0&LED13=0&LED14=0&LED16=0&LED15=0\"><button>OFF</button></a></p>";
  webPage += "<p>Socket #4 <a href=\"ledArgs?LED5=0&LED4=0&LED2=0&LED0=0&LED12=0&LED13=0&LED14=1&LED16=0&LED15=0\"><button>ON</button></a>&nbsp;<a href=\"ledArgs?LED5=0&LED4=0&LED2=0&LED0=0&LED12=0&LED13=0&LED14=0&LED16=0&LED15=0\"><button>OFF</button></a></p>";
  webPage += "<p>Socket #5 <a href=\"ledArgs?LED5=0&LED4=0&LED2=0&LED0=1&LED12=0&LED13=0&LED14=0&LED16=0&LED15=0\"><button>ON</button></a>&nbsp;<a href=\"ledArgs?LED5=0&LED4=0&LED2=0&LED0=1&LED12=0&LED13=0&LED14=0&LED16=0&LED15=0\"><button>OFF</button></a></p>";
  webPage += "<p>Socket #6 <a href=\"ledArgs?LED5=0&LED4=0&LED2=0&LED0=1&LED12=1&LED13=0&LED14=0&LED16=0&LED15=0\"><button>ON</button></a>&nbsp;<a href=\"ledArgs?LED5=0&LED4=0&LED2=0&LED0=1&LED12=0&LED13=0&LED14=0&LED16=0&LED15=0\"><button>OFF</button></a></p>";
  webPage += "<p>Socket #7 <a href=\"ledArgs?LED5=0&LED4=0&LED2=0&LED0=1&LED12=0&LED13=1&LED14=0&LED16=0&LED15=0\"><button>ON</button></a>&nbsp;<a href=\"ledArgs?LED5=0&LED4=0&LED2=0&LED0=1&LED12=0&LED13=0&LED14=0&LED16=0&LED15=0\"><button>OFF</button></a></p>";
  webPage += "<p>Socket #8 <a href=\"ledArgs?LED5=0&LED4=0&LED2=0&LED0=1&LED12=1&LED13=1&LED14=0&LED16=0&LED15=0\"><button>ON</button></a>&nbsp;<a href=\"ledArgs?LED5=0&LED4=0&LED2=0&LED0=1&LED12=0&LED13=0&LED14=0&LED16=0&LED15=0\"><button>OFF</button></a></p>";
    
  // preparing GPIOs
  pinMode(gpio0_pin, OUTPUT);
  digitalWrite(gpio0_pin, 0);
  pinMode(gpio2_pin, OUTPUT);
  digitalWrite(gpio2_pin, 0);
  pinMode(gpio4_pin, OUTPUT);
  digitalWrite(gpio4_pin, 0);
  pinMode(gpio5_pin, OUTPUT);
  digitalWrite(gpio5_pin, 0);
   pinMode(gpio12_pin, OUTPUT);
  digitalWrite(gpio12_pin, 0);
   pinMode(gpio13_pin, OUTPUT);
  digitalWrite(gpio13_pin, 0);
   pinMode(gpio14_pin, OUTPUT);
  digitalWrite(gpio14_pin, 0);
   pinMode(gpio15_pin, OUTPUT);
  digitalWrite(gpio15_pin, 0);
   pinMode(gpio16_pin, OUTPUT);
  digitalWrite(gpio16_pin, 0);
  
  delay(1000);
  Serial.begin(115200);
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
 
 // WiFi.softAP("Jio_Put_To_Light", "ranveer1992");  //Start HOTspot removing password will disable security

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
  Serial.println("HTTP server started");
}
 
void loop(void){

  if(sortCount==0)
  {
  digitalWrite(gpio12_pin,0);
  digitalWrite(gpio14_pin,0); 
  digitalWrite(gpio16_pin,0);
  }
  StringTokenizer tokens(sortCompleteList, ";");
  Serial.println("List: "+sortCompleteList);
  while(tokens.hasNext()){
        String temp= tokens.nextToken();
        delay(100);
    StringTokenizer token(temp, ":"); 
     int count=0;
      while(token.hasNext()){
      if(count==0)
      {
      digitalWrite(gpio12_pin,token.nextToken().toInt());
      }
      if(count==1)
      {
         digitalWrite(gpio14_pin,token.nextToken().toInt()); 
      }
      if(count==2)
      {
         digitalWrite(gpio16_pin,token.nextToken().toInt());
      }
      count++;
     // prints the next token in the string
  }
  }
 
 server.handleClient();
} 

void handleSubmit() {
  // Actualise le GPIO / Update GPIO 
  int C1LED5,C1LED4,C1LED0,C2LED2,C2LED15,C2LED13,C3LED12,C3LED14,C3LED16;
  String sC1LED5,sC1LED4,sC1LED0,sC2LED2,sC2LED15,sC2LED13,sC3LED12,sC3LED14,sC3LED16,sSortCompleteFlag,sPackFlag,sOldPack;
  
  sC1LED5 = server.arg("LED5");  
  sC1LED4 = server.arg("LED4");
  sC1LED0 = server.arg("LED0");
  sC2LED15 = server.arg("LED15");
  sC2LED2 = server.arg("LED2");
  sC2LED13 = server.arg("LED13");
  sC3LED12 = server.arg("LED12");
  sC3LED14 = server.arg("LED14");
  sC3LED16 = server.arg("LED16");
  sSortCompleteFlag = server.arg("CFLAG");
  sPackFlag = server.arg("PFLAG");
  sOldPack = server.arg("OPACK");
  
  if(sC1LED5!="")
  {
     C1LED5 = sC1LED5.toInt();
    digitalWrite(gpio5_pin, C1LED5);
  }
  
  if(sC1LED4!="")
  {
     C1LED4 = sC1LED4.toInt();
   digitalWrite(gpio4_pin, C1LED4);
   
  }
  
  if(sC1LED0!="")
  {
     C1LED0 = sC1LED0.toInt();
   digitalWrite(gpio0_pin, C1LED0);   
  }

  
  if(sC2LED2!="")
  {
     C2LED2 = sC2LED2.toInt();
   digitalWrite(gpio2_pin, C2LED2);
  }
  if(sC2LED15!="")
  {
     C2LED15 = sC2LED15.toInt();
   digitalWrite(gpio15_pin, C2LED15);
   }
  
if(sC2LED13!="")
  {
     C2LED13 = sC2LED13.toInt();
   digitalWrite(gpio13_pin,C2LED13);
   }
   
  if(sC3LED12!="")
  {
     C3LED12 = sC3LED12.toInt();
   digitalWrite(gpio12_pin,C3LED12);
  }
  if(sC3LED14!="")
  {
   C3LED14 = sC3LED14.toInt();
   digitalWrite(gpio14_pin, C3LED14);   
  }
  if(sC3LED16!="")
  {
   C3LED16 = sC3LED16.toInt();
   digitalWrite(gpio16_pin, C3LED16);   
  }
  if(sSortCompleteFlag=="Y")
  {
    Serial.println("BEFORE ADD: "+sortCompleteList); 
    sortCompleteList=sortCompleteList+";"+C3LED12+":"+C3LED14+":"+C3LED16;
    Serial.println("AFTER ADD: "+sortCompleteList);
    sortCount++;
  }
  if(sPackFlag=="Y")
  {
      Serial.println("OPACK :"+sOldPack);
      Serial.println("BEFORE REMOVE: "+sortCompleteList);
      String tempData=sortCompleteList;
      sortCompleteList.replace(sOldPack,"");
      if(tempData!=sortCompleteList)
        sortCount--;
      sortCompleteList.replace(";;",";");
      Serial.println("AFTER REMOVE: "+sortCompleteList);
      
  }
  server.send(200, "text/html", webPage);
    
  }

