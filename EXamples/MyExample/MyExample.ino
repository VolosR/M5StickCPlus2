#include "M5StickCPlus2.h"
#include "sFont.h"
#include "ssFont.h"
#include "mFont.h"
#include "hFont.h"
M5Canvas img(&StickCP2.Display);

#include <IRremote.hpp>
#include <TFT_eSPI.h>
TFT_eSPI tft = TFT_eSPI();
TFT_eSprite sprite = TFT_eSprite(&tft);

#define DISABLE_CODE_FOR_RECEIVER  
#define SEND_PWM_BY_TIMER
#define IR_TX_PIN 19

String days[7]={"SU","MO","TO","WE","TH","FF","SA"};

unsigned short grays[13];
unsigned short blue=0x1BB7;
unsigned short blue2=0x469F;
unsigned short darkOrange=0xC460 ;

int s,h,m=0; //sec,min,hour
int ts=0; //temporerly s
String ho,mi,se; // hour, minute,seconds but strings
int da, mo, ye,wd; //day, month, year

int n=0;
int vol=0;
int volE; // voltage segments
int x,y=0;
int xx,yy;

int brightness[5]={30,50,80,110,150};
int selBri=1;
int sx=40;
int sy=120;
int r=26;
double rad=0.01745;
float xpos[61];
float ypos[61];
float xpos2[61];
float ypos2[61];
bool rot=false;

bool btnState[3];
String btnStr[3]={"BA","BB","BC"};
unsigned short btnCol[2]={TFT_GREEN,TFT_RED};

//menu
int chosen=0;
String lbls[4]={"SOUND","LED","SND IR","BRIGHT"};
bool led=false;
bool sound=true;


void setup() {
    auto cfg = M5.config();
    StickCP2.begin(cfg);
    //StickCP2.Rtc.setDateTime( { { 2023, 12, 30 }, { 22, 43, 0 } } );
   
    StickCP2.Display.setBrightness(50);
    sprite.createSprite(135,240);  

    pinMode(35,INPUT_PULLUP);
     pinMode(37,INPUT_PULLUP);
      pinMode(39,INPUT_PULLUP);
       pinMode(19,OUTPUT);

    IrSender.begin(DISABLE_LED_FEEDBACK);  // Start with IR_SEND_PIN as send pin
    IrSender.setSendPin(IR_TX_PIN);

     int co=220;
     for(int i=0;i<13;i++)
     {grays[i]=StickCP2.Display.color565(co, co, co);
     co=co-20;}

    int st=270;
      for(int i=0;i<60;i++)
    {
       xpos[i]=(r*cos(rad*(st)))+sx;
       ypos[i]=(r*sin(rad*(st)))+sy;
       xpos2[i]=((r-10)*cos(rad*(st)))+sx;
       ypos2[i]=((r-10)*sin(rad*(st)))+sy;
       st=st+6;
       if(st>=360) st=0;
    }
}



void draw()
{
  sprite.fillSprite(TFT_BLACK);
  sprite.fillSmoothRoundRect(10,86,60,64,3,grays[10]);
  
  sprite.drawRect(118,13,12,30,grays[3]);
  sprite.fillRect(121,9,6,4,grays[3]);

  for(int i=0;i<volE;i++)
  sprite.fillRect(120,37-(i*5),8,3,TFT_GREEN);

  sprite.setTextDatum(0);
  sprite.loadFont(hFont);
  sprite.setTextColor(grays[0],TFT_BLACK);
  sprite.drawString(ho+":"+mi,10,10);
  sprite.unloadFont();
  
  //sprite.drawString(String((int)fps),100,160,4);

  sprite.loadFont(mFont);
  sprite.setTextColor(grays[2],TFT_BLACK);
  sprite.drawString(String(da)+"/"+String(mo)+"/"+String(ye),12,60);
  sprite.unloadFont();

   sprite.fillSmoothRoundRect(102,58,26,23,2,blue);
   sprite.loadFont(sFont);
   sprite.setTextColor(TFT_WHITE,blue);
   sprite.drawString(days[wd],108,62);
   sprite.setTextDatum(4);
   sprite.setTextColor(grays[0],grays[9]);
   sprite.drawString(String(s),sx,sy);
   sprite.setTextDatum(0);
   sprite.unloadFont();


   //menu....................
  sprite.loadFont(ssFont);
  for(int i=0;i<4;i++)
  {
    if(i==chosen)sprite.setTextColor(grays[1],TFT_BLACK); 
    else sprite.setTextColor(grays[4],TFT_BLACK);
    sprite.drawString(lbls[i],15,160+(i*18));
  }
  sprite.fillRect(10, 160+(chosen*18), 3, 11, grays[0]);
  

  //switches
  sprite.fillRect(74,160,22,11,blue);
  sprite.fillRect(74,178,22,11,blue);
   sprite.fillRect(76+(sound*14),162,4,7,TFT_WHITE);
   sprite.fillRect(76+(led*14),180,4,7,TFT_WHITE);

  //brightness
  for(int i=0;i<5;i++)
  if(selBri>=i)
  sprite.fillRect(72+(i*6),214,3,11,TFT_GREEN);
  else
  sprite.fillRect(72+(i*6),214,3,11,grays[7]);
  sprite.unloadFont();

     for(int i=0;i<3;i++)
     {
       sprite.fillSmoothRoundRect(104,160+(i*18),24,12,2,btnCol[btnState[i]]);
       sprite.setTextColor(grays[0],btnCol[btnState[i]]);
       sprite.drawString(btnStr[i],110,162+(i*18));
     }
    
  
  //accelometer
  sprite.setTextColor(grays[2],TFT_BLACK);
  sprite.fillSmoothRoundRect(76,98,52,52,3,grays[10]);
  sprite.fillSmoothCircle(102,124,22,grays[4],grays[10]);
  sprite.fillSmoothCircle(102,124,21,grays[10],grays[4]);
  sprite.fillSmoothCircle(102,124,15,grays[4],grays[10]);
  sprite.fillSmoothCircle(102,124,14,grays[10],grays[4]);
  sprite.fillSmoothCircle(102,124,8,grays[4],grays[10]);
  sprite.fillSmoothCircle(102,124,7,grays[10],grays[4]);
  sprite.fillSmoothCircle(102,124,2,grays[4],grays[10]);
  sprite.drawString("ACCELE:", 76,88);
  sprite.drawString(String(vol/1000.00), 104,218);
  sprite.fillSmoothCircle(xx,yy,3,TFT_RED,grays[10]);
  
  //seconds
  for(int i=0;i<12;i++)
  sprite.fillSmoothCircle(xpos[i*5],ypos[i*5],1,grays[2],grays[10]);
  sprite.drawWedgeLine(xpos2[n],ypos2[n],xpos[n],ypos[n],2,2,grays[1],grays[10]);


  StickCP2.Display.pushImage(0,0,135,240,(uint16_t*)sprite.getPointer());
}

void loop() {

    StickCP2.update();

    btnState[0]=digitalRead(37);
    btnState[1]=digitalRead(39);
    btnState[2]=digitalRead(35);

    if (StickCP2.BtnB.wasPressed()) {
      chosen++; if(chosen==4) chosen=0;
      if(sound)
      StickCP2.Speaker.tone(6000, 100);
    }
    if (StickCP2.BtnA.wasPressed()) {
      if(sound)
      StickCP2.Speaker.tone(8000, 100);
      if(chosen==0) {sound=!sound;}
      if(chosen==1) {led=!led; digitalWrite(19,led);}
      if(chosen==2) {IrSender.sendSAMSUNG(0xE0E040BF, 32); digitalWrite(19,0);}
      if(chosen==3) {selBri++; if(selBri==5) selBri=0; StickCP2.Display.setBrightness(brightness[selBri]);}
    }
   
    auto dt = StickCP2.Rtc.getDateTime();
    auto imu_update = StickCP2.Imu.update();
    auto data = StickCP2.Imu.getImuData();  


//sprite.fillSmoothRoundRect(76,98,52,52,3,grays[9]);
        x=data.accel.x*1000; 
        y=data.accel.y*1000; 
        xx=map(x,-800,800,80,124);
        yy=map(y,-800,800,102,146);
    
    h=dt.time.hours;
    m=dt.time.minutes;
    s=dt.time.seconds;
    da=dt.date.date;
    mo=dt.date.month;
    ye=dt.date.year;
    wd=dt.date.weekDay;
    
    if(ts!=s){rot=true; ts=s;}
    
    if(rot)
    n=n+4;
    if(n==60) {n=0; rot=false;}

    if(h<10) ho="0"+String(h); else ho=String(h);
    if(m<10) mi="0"+String(m); else mi=String(m);
    if(s<10) se="0"+String(s); else se=String(s);

    vol = StickCP2.Power.getBatteryVoltage();
    volE=map(vol,3000,4180,0,5);
    draw();
       
}
