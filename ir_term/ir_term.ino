#include <nRF24L01.h>
#include <RF24.h>
#include <RF24_config.h>
#include <SPI.h>

#include <IR_Thermometer_Sensor_MLX90614.h>
#include <Wire.h>

RF24 radio(9,10);
const uint64_t pipe = 0xE8E8F0F0E1LL;

IR_Thermometer_Sensor_MLX90614 MLX90614 = IR_Thermometer_Sensor_MLX90614();
float kum_temp;
float ortam_temp;

int t1,t2,t3;

int data[1];

void setup() 
{  
  radio.begin();
  radio.openWritingPipe(pipe);
   
  Serial.begin(9600);
  MLX90614.begin(); 
}

void loop() 
{
  ortam_temp=MLX90614.GetAmbientTemp_Celsius();
  kum_temp=MLX90614.GetObjectTemp_Celsius();

  t1=kum_temp;
  delay(250);
  t2=kum_temp;
  delay(250);
  t3=kum_temp;
  
  data[0]=(t1+t2+t3)/3;
  
  radio.write(data,sizeof(data));       
}
