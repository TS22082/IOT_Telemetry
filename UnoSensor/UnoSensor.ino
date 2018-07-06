#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include <SoftwareSerial.h>

#define coAddress 100
#define phAddress 99

byte code=0;                
char data[20];            
byte in_char=0;                  
byte i=0;                      
int time_=2000;                 
float RTD_float;   

LiquidCrystal_I2C lcd(0x27,20,4);  // set the LCD address to 0x27 for a 16 chars and 2 line display
SoftwareSerial port1(11,10); //RX,TX
SoftwareSerial port2(13,12);

int period = 1000;
unsigned long time_now = 0;

void setup() {  
  Serial.begin(9600);
  port1.begin(4800);
  port2.begin(4800);
  Wire.begin();
  lcd.init();
  lcd.backlight();
}

void loop() {
 //for conductivity
 Wire.beginTransmission(coAddress);
 Wire.write('r');
 Wire.endTransmission();
 delay(time_);
 Wire.requestFrom(coAddress, 20, 1);
 
 code = Wire.read();
 while(Wire.available()){        
  in_char = Wire.read();        
  data[i]= in_char;             
  i+=1;                            
  if(in_char==0){                 
    i=0;                        
    Wire.endTransmission();     
    break;                      
    }
  }
  lcd.setCursor(0,0);
  lcd.print("CN: ");
  lcd.setCursor(6,0);
  lcd.print(data);
  port1.print(data);
  port1.println("\n");
  delay(30);
  
  //for PH
  Wire.beginTransmission(phAddress);
  Wire.write('r');
  Wire.endTransmission();
  delay(time_);
  Wire.requestFrom(phAddress, 20, 1);
  code = Wire.read();
  while(Wire.available()){        
    in_char = Wire.read();        
    data[i]= in_char;             
    i+=1;                            
    if(in_char==0){                 
      i=0;                        
      Wire.endTransmission();     
      break;                      
    }
  }
  lcd.setCursor(0,1);
  lcd.print("PH: ");
  lcd.setCursor(6,1);
  lcd.print(data);
  port2.print(data);
  port2.println("\n");
  delay(30);

  port1.listen();
  delay(200);
  while(port1.available()>0){
    float val = port1.parseFloat();
    if(port1.read()== '\n'){
      Serial.print("from port 1: ");
      Serial.println(val);
      break;
    }
  }
 }
