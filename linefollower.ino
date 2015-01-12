
#include<avr/io.h>

#define left 0x07
#define right 0x70
#define fwd 0x77
#define back 0x88

int s[5];
int sum_sensors=0;
int avg_sensors=0;
int sum=0;
int avg=0;
int l_speed,r_speed,mem=1;
int thres=512;
float kp=75.0;
float kd=5.0;
float state=0.0;
float prev_state=0.0;
int P,D;



void sensors(){
  sum_sensors=0;
  avg_sensors=0;
  s[0]=digitalRead(A0);
  s[1]=digitalRead(A1);
  s[2]=digitalRead(A2);
  s[3]=digitalRead(A3);
  s[4]=digitalRead(A4);
  
  for(int i=0;i<5;i++){
  sum_sensors+=s[i];
  }
  avg_sensors=sum_sensors/5;
}

void enable(int lspeed,int rspeed)
{
  analogWrite(10,lspeed);
  analogWrite(9,rspeed);  
}
  
void threshold()
{
  sum_sensors=0;
  for(int i=0;i<5;i++){
    s[i]=(s[i]>thres)?1:0;
    sum_sensors+=s[i];}
  }
  


int autotune(){
  int i=1;
  while(i<101){
  enable(255,255);
  PORTD=left;
  sensors();
  sum+=avg_sensors;
  }
  while(i>101 && i<201){
    enable(255,255);
    PORTD=right;
    sensors();
    sum+=avg_sensors;
   }
  while(i>201 && i<301){
    enable(255,255);
    PORTD=right;
    sensors();
    sum+=avg_sensors;
  }
  while(i>3001 && i<4001){
    enable(255,255);
    PORTD=left;
    sensors();
    sum+=avg_sensors;
   }
  avg=sum/i;
  return avg;
}
  
void setup(){
  Serial.begin(9600);
  PORTC=0x00;
  PORTD=0xFF;
  PORTB=0xFF;
  //thres=autotune();
}

void loop()
{
  sensors();
  //threshold();
  for(int i=0;i<4;i++) 
  Serial.print(s[i]);
  Serial.println(s[4]);
  state=-2*s[0]-1*s[1]+s[3]+2*s[4];
  P=state*kp;
  D=(state-prev_state)*kd;
  l_speed=1023-P-D;
  r_speed=1023+P+D;
  
  if(sum_sensors>3)
      {
        enable(1023,1023);
        PORTD=fwd;
        delay(500);}
  else
  if(s[0]==1){

    enable(1023,1023);
    PORTD=left;
    mem=1;
  }

else if(s[4]==1){
    enable(1023,1023);
    PORTD=right;
    mem=0;}
     else if(sum_sensors==0)
           {
             /*enable(1023,1023);
             PORTD=fwd;
             delay(1000);
             sensors();
             threshold*/
                 //threshold();
                 if(mem==1);
                  {
                    enable(1023,1023);
                    PORTD=right;
                  }
                 if(mem==0);
                  {
                    enable(1023,1023);
                    PORTD=left;
                  }
           }
    else        
    {
           enable(l_speed,r_speed);
           PORTD=fwd;}
     }     
  
