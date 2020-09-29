#include <nRF24L01.h>
#include <RF24.h>
#include <RF24_config.h>
#include <SPI.h>
#include <Wire.h>
#include <AvrI2c_Greiman.h>
#include <LiquidCrystal_I2C_AvrI2C.h>

#define r_ac 2
#define r_kapat 3
#define r_bunker 4
#define resetP 7

RF24 radio(9,10);
const uint64_t kpipe = 0xE8E8F0F0E2LL;

LiquidCrystal_I2C_AvrI2C lcd(0x27,16,2);

int kum;
int ayar;
int fark;
int veri[2];
int san;
int last_temp=0;

void setup() 
  {
    digitalWrite(resetP,HIGH);
    delay(200);
    pinMode(resetP,OUTPUT);
    
    pinMode(r_ac,OUTPUT);
    pinMode(r_kapat,OUTPUT);
    pinMode(r_bunker,OUTPUT);
    
    digitalWrite(r_ac,HIGH);
    digitalWrite(r_kapat,HIGH);
    digitalWrite(r_bunker,HIGH);

    radio.begin();
    radio.openReadingPipe(1, kpipe);
    radio.startListening();
    
    Serial.begin(9600);
    
    lcd.begin();
    lcd.clear();
  }

void(* resetFunc) (void) = 0;

void loop()
{
  if(radio.available())
    {   
      radio.read(veri,sizeof(veri));
      kum=veri[0];
      ayar=veri[1];
      fark=ayar-kum;
      delay(250);

      if (kum!=last_temp)
        {                 
          lcd.clear();
          lcd.setCursor(0,0);
          lcd.print("Kum: ");
          lcd.print(kum);
          lcd.setCursor(0,1);
          lcd.print("Ayar: ");
          lcd.print(ayar);
          
          Serial.println(kum);
          Serial.println(ayar);
          delay(2000);
    
          if(kum>50)
            {        
              if(kum<60)
                {
                  digitalWrite(r_bunker,LOW);

                  for(san=60;san>=0;san--)
                    {
                      lcd.clear();
                      lcd.setCursor(0,0);
                      lcd.print("Kum: ");
                      lcd.print(kum);
                      lcd.setCursor(0,1);
                      lcd.print("00");
                      lcd.setCursor(2,1);
                      lcd.print(":");
                      lcd.setCursor(3,1);
                      lcd.print(san);
                      lcd.setCursor(9,1);
                      lcd.print("Bunker");
                      delay(1000);
                    }

                  digitalWrite(r_kapat,LOW);
                  delay(5*1000);
                  digitalWrite(r_kapat,HIGH);
                  
                  digitalWrite(r_bunker,HIGH);
                }
              
              else if(fark<0)
                {            
                  if(abs(fark)>5)
                    {
                      digitalWrite(r_ac,LOW);
                      lcd.clear();
                      lcd.setCursor(0,0);
                      lcd.print("Kapak");
                      lcd.setCursor(6,1);
                      lcd.print("Yukari");
                      delay(5*200);
                      digitalWrite(r_ac,HIGH); 
                    }
      
                  else if(abs(fark)>3 && abs(fark)<=5)
                    {
                      digitalWrite(r_ac,LOW);
                      lcd.clear();
                      lcd.setCursor(0,0);
                      lcd.print("Kapak");
                      lcd.setCursor(6,1);
                      lcd.print("Yukari");
                      delay(abs(fark)*150);
                      digitalWrite(r_ac,HIGH);
                    }
    
                  else if(abs(fark)>1 && abs(fark)<=3)
                    {
                      digitalWrite(r_ac,LOW);
                      lcd.clear();
                      lcd.setCursor(0,0);
                      lcd.print("Kapak");
                      lcd.setCursor(6,1);
                      lcd.print("Yukari");
                      delay(abs(fark)*100);
                      digitalWrite(r_ac,HIGH);
                    }
                  
                  else
                    {
                      lcd.clear();
                      lcd.setCursor(0,0);
                      lcd.print("Beklemede");
                      delay(1000);
                    }
                }
                
              else if(fark>0)
                {
                  if(fark>5)
                    {
                      digitalWrite(r_bunker,LOW);

                      for(san=50;san>=0;san--)
                        {
                          lcd.clear();
                          lcd.setCursor(0,0);
                          lcd.print("Kum: ");
                          lcd.print(kum);
                          lcd.setCursor(0,1);
                          lcd.print("00");
                          lcd.setCursor(2,1);
                          lcd.print(":");
                          lcd.setCursor(3,1);
                          lcd.print(san);
                          lcd.setCursor(9,1);
                          lcd.print("Bunker");
                          delay(1000);
                        }

                      digitalWrite(r_kapat,LOW);
                      delay(5*1000);
                      digitalWrite(r_kapat,HIGH);
                  
                      digitalWrite(r_bunker,HIGH);
                    }
      
                  else if(fark>3 && fark<=5)
                    {
                      digitalWrite(r_kapat,LOW);
                      lcd.clear();
                      lcd.setCursor(0,0);
                      lcd.print("Kapak");
                      lcd.setCursor(6,1);
                      lcd.print("Asagi");  
                      delay(fark*200);
                      digitalWrite(r_kapat,HIGH);
                    }
    
                  else if(fark>1 && fark<=3)
                    {
                      digitalWrite(r_kapat,LOW);
                      lcd.clear();
                      lcd.setCursor(0,0);
                      lcd.print("Kapak");
                      lcd.setCursor(6,1);
                      lcd.print("Asagi");  
                      delay(fark*150);
                      digitalWrite(r_kapat,HIGH);
                    }
      
                  else
                    {  
                      lcd.clear();
                      lcd.setCursor(0,0);
                      lcd.print("Beklemede");
                      delay(1000);

                    }  
                }
                
              else{Serial.println("---");}           
            }
      
          else 
            {
              lcd.clear();
              lcd.setCursor(0,0);
              lcd.print("Kum bekleniyor");
              delay(3000);
            }
          
          last_temp=kum;

          for(san=70;san>=0;san--)
            {
              lcd.clear();
              lcd.setCursor(0,0);
              lcd.print("Kum: ");
              lcd.print(kum);
              lcd.setCursor(0,1);
              lcd.print("00");
              lcd.setCursor(2,1);
              lcd.print(":");
              lcd.setCursor(3,1);
              lcd.print(san);
              lcd.setCursor(7,1);
              lcd.print("Bekliyor");
              delay(1000);
            }
          digitalWrite(resetP,LOW);
        }

      else
        {
          lcd.clear();
          lcd.setCursor(0,1);
          lcd.print("Veri Bekleniyor");
          delay(1000);
        }
    }
        
  else
    { 
      lcd.clear();
      lcd.setCursor(0,1);
      lcd.print("Baglanti yok");
      delay(500);
    }
    
}
