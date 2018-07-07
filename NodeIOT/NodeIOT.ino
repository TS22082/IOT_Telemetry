#include <SoftwareSerial.h>
#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>

SoftwareSerial port1(D2,D3); //RX,TX
SoftwareSerial port2(D6,D7); //RX,TX

int period = 1000;
unsigned long time_now = 0;

void waitFor(int time){
    int period = time;
    unsigned long time_now = millis();
    while(millis() < time_now + period){}
  }

#define FIREBASE_HOST "---fill in your own info here--"
#define FIREBASE_AUTH "--this is the secret key---"
#define WIFI_SSID "--fill in your own info here---"
#define WIFI_PASSWORD "---fill in your own info here--"

void setup(){
  Serial.begin(9600);
  port1.begin(4800);
  port2.begin(4800);

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting");
  while(WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    waitFor(500);  
  }
  Serial.print("connected: ");
  Serial.println(WiFi.localIP());

  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
}

void loop(){ 
  port1.listen();
  delay(200);
  while(port1.available()>0){
    float val = port1.parseFloat();
    if(port1.read()== '\n'){
      Serial.print("CON: ");
      Serial.println(val);
      Firebase.setFloat("CON", val);
      break;
    }
  }
  
  port2.listen();
  delay(200);
  while(port2.available()>0){
    float val = port2.parseFloat();
    if(port2.read()== '\n'){
      Serial.print("PH: ");
      Serial.println(val);
      Firebase.setFloat("PH", val);
      break;
    }
  }
  waitFor(1000);
}
