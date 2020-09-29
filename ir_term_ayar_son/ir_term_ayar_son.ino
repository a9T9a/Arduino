#include <SoftwareSerial.h>
#include <AvrI2c_Greiman.h>
#include <LiquidCrystal_I2C_AvrI2C.h>
#include <IR_Thermometer_Sensor_MLX90614.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <RF24_config.h>
#include <SPI.h>
#include <Wire.h>

#define potpin A0

int data[1];
int ayar;
int kum;
int pot;
int veri[2];

RF24 radio(9,10);
const uint64_t pipe = 0xE8E8F0F0E1LL;
const uint64_t kpipe= 0xE8E8F0F0E2LL;

LiquidCrystal_I2C_AvrI2C lcd(0x27,16,2);

void setup() 
  {       
    radio.begin();
    radio.openReadingPipe(1, pipe);
    radio.startListening();
    radio.openWritingPipe(kpipe);
    
    Serial.begin(9600);   
    
    lcd.begin();
    lcd.clear(); 
  }

void loop()
{        
    if(radio.available())
      {
        radio.read(data, sizeof(data));
        kum=data[0];
        
        pot=analogRead(potpin);
        ayar=map(pot,0,1023,60,70);
        delay(500);

        radio.stopListening();
        veri[0]=kum;
        veri[1]=ayar;
        radio.write(veri,sizeof(veri));
        radio.startListening();
    
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Kum: ");
        lcd.print(kum);
        lcd.setCursor(0,1);
        lcd.print("Ayar: ");
        lcd.print(ayar);

        delay(500);
      }

    else
      { 
        lcd.clear();
        lcd.setCursor(0,1);
        lcd.print("Baglanti yok");
        delay(1000);
      }
}
