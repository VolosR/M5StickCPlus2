#include "stop.h"
#include "play.h"
#include "M5StickCPlus2.h"
#include "DSEG7_Classic_Regular_16.h"


#define grey 0x65DB
#define color 0xF251

int ss=0; //miliseconds/10
int s=0;   //seconds
int m=0;  //minutes

int lss[30];
int ls[30];
int lm[30];

String minutesL;
String secondsL;
String milisL;
int laps=0;
String lapTime[30];
int pom=0;  //debouncing button
int pom2=0;
int pres3 =0;//deboun

int fase=0;
int bright[4]={8,9,10,12};
int c=0;

void setup() {
    auto cfg = M5.config();
    StickCP2.begin(cfg);
    
   StickCP2.Display.setRotation(0);
   StickCP2.Display.fillScreen(BLACK);
   StickCP2.Display.setSwapBytes(true);

   pinMode(26,INPUT_PULLUP);
   pinMode(39,INPUT_PULLUP);
   pinMode(25,OUTPUT);
   pinMode(0,INPUT_PULLUP);

  StickCP2.Display.setBrightness(40);
   reset();
}

String seconds,minutes,milis;
String current,temp;

int winer=0;

long tt=0;
long tt2=0;

void loop() {

  if(fase<3){
  tt=millis();
  StickCP2.Display.setFreeFont(&DSEG7_Classic_Regular_32);
  if(s<10) seconds="0"+String(s);else seconds=String(s);
  if(m<10) minutes="0"+String(m);else minutes=String(m);
  current=minutes+":"+seconds;

  
  if(current!=temp){
  StickCP2.Display.drawString(current,10,30);
  temp=current;
  }
  
  if(ss<10) milis="0"+String(ss);else milis=String(ss);
  StickCP2.Display.drawString(milis,60,76,7);
  }

  if(tt>tt2+8)
  {  
     tt2=tt;
     if(fase==1)
     ss++;
  }

  if(ss>99)
{
  s++;
  ss=0;
}
  if(s>59)
  {
    s=0;
    m++;
  }
 buttons(); 
}

void buttons()
{
  if(digitalRead(39)==0 )
  {
   if(pom==0)
   {
   if(fase==1){
   winer++;
   laps++;
   if(winer==1){
    StickCP2.Display.fillRect(15,130,120,80,BLACK);
   StickCP2.Display.drawString("LAP:",20,130,2);
   }
   if(winer>3){
      StickCP2.Display.fillRect(15,144,120,70,BLACK);
      winer=1;
   }
   StickCP2.Display.setFreeFont(&DSEG14_Classic_Mini_Regular_15);
     lss[laps]=ss;
     ls[laps]=s;
     lm[laps]=m; 

   if((lm[laps]-lm[laps-1])<10)
   minutesL="0"+String(lm[laps]-lm[laps-1]);
   else
   minutesL=String(lm[laps]-lm[laps-1]);

   if((ls[laps]-ls[laps-1])<10)
   secondsL="0"+String(ls[laps]-ls[laps-1]);
   else
   secondsL=String(ls[laps]-ls[laps-1]);

  int lsss=lss[laps]-lss[laps-1];
  if(lsss<0)
  lsss=lsss*-1;

  if(lsss<10)
  milisL="0"+String(lsss);
  else
  milisL=String(lsss);

   lapTime[laps-1]=minutesL+" : "+secondsL+" : "+milisL;
   StickCP2.Display.drawString(lapTime[laps-1],18,130+winer*22);}
      
   if(fase==0)
   fase=1;
   pom=1; 
     }
  }else{pom=0;}

    if(digitalRead(37)==0 )
  {
   if(pom2==0)
     {
    fase++;
    if(fase==3)
    {
      
      StickCP2.Display.fillScreen(BLACK);
      StickCP2.Display.setFreeFont(&DSEG14_Classic_Mini_Regular_15);
      StickCP2.Display.drawString("TIME:",10,6,2);
      StickCP2.Display.drawString("LAP:",10,50,2);
      StickCP2.Display.drawString(current+":"+milis,10,28);
      for(int i=0;i<laps;i++){
      
      StickCP2.Display.drawString(String(i+1)+".   "+lapTime[i],10,68+(14*i),2);}
      }
    if(fase>3)
       {
         
         reset();
         
       }
   
    pom2=1;
     }
  }else{pom2=0;}



  
  }
  void reset()
  {
    StickCP2.Display.fillScreen(BLACK);
    StickCP2.Display.pushImage(7,75,48,48,stop);
    StickCP2.Display.pushImage(30,136,72,72,play);
    StickCP2.Display.drawString("STOPWATCH",14,7,2);
    StickCP2.Display.drawString("BAT 72%",15,222,2);
    
    StickCP2.Display.fillRect(105,224,4,12,color); 
    StickCP2.Display.setFreeFont(&DSEG7_Classic_Regular_32);
    StickCP2.Display.drawLine(0,220,128,220,grey);
    laps=0;
    lss[0]=0;
    ls[0]=0;
    lm[0]=0;
    ss=0;
    s=0;
    m=0;
    fase=0;
    winer=0;
  }
