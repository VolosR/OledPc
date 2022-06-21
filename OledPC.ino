#include <SPI.h>
#include <Wire.h>
#include "calculator.h"
#include "stopwatch.h"
#include "game.h"
#include "calendar.h"
#include "menu.h"


#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h> 



#define OLED_RESET     4 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(OLED_RESET);

byte fase=0;

void setup()
  {
    pinMode(up,INPUT_PULLUP);
    pinMode(presS,INPUT_PULLUP);
    pinMode(down,INPUT_PULLUP);
    pinMode(3,OUTPUT);
   
      display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS);
  display.display();

  display.clearDisplay();
    display.setRotation(3); 
    playerX=random(10,50);
    display.clearDisplay();
    display.setFont(0);
    display.setTextColor(WHITE);
    display.display();
  }

void loop() {

   if(fase==0){
  checkButtonsMenu();  
  drawMenu();}

  if(fase==1){
  checkButtonsCalc();  
  drawCalc();}

  if(fase==2){
  checkButtonsStop();  
  drawStop();}

  if(fase==3){
  checkColision();
  checkButtonsGame();  
  drawGame();}

  if(fase==4){
  calendarDraw();
  checkButtonsCalendar();
  }

   if(fase==5){
  phoneDraw();
  
  }



  if(digitalRead(down)==0 && digitalRead(presS)==0 ){
    GameReset();
    display.setRotation(3);
    resetAll();
    fase=0;
    delay(500);
  }

}

void drawCalc()
{
  display.clearDisplay();
  display.drawRoundRect(0,0,64,128,3,1);
  display.fillRoundRect(6,16,52,16,2,1);

        display.setCursor(6,4);
        display.print("CALC");

       

  for(int i=0;i<n;i++){
    posY[i]=fromTop+(boxH*i)+(space*i);
    for(int j=0;j<m;j++)
    {   
      posX[j]=fromLeft+(boxW*j)+(space*j);
      display.fillRoundRect(posX[j],posY[i],boxW,boxH,2,1);
      display.setCursor(posX[j]+(boxW/2)-3,posY[i]+(boxH/2)-3);
      display.setTextColor(0);
      display.print(buttons[j][i]);  
    }}
  display.setTextColor(1);  
  display.fillRoundRect(posX[cx],posY[cy],boxW,boxH,2,0); 
  display.drawRoundRect(posX[cx],posY[cy],boxW,boxH,2,1); 
  display.setCursor(posX[cx]+(boxW/2)-2,posY[cy]+(boxH/2)-4); 
  display.print(buttons[cx][cy]); 

   display.setCursor(6,4);
   display.print("CALC");
  display.setCursor(10,20);
  display.setTextColor(0); 

  temp=num*10;
  if(temp%10==0)
  display.print((int)num);
  else 
  display.print(num); 
  display.display();
  display.setTextColor(1); 
  }

 void checkButtonsCalc()
  {
      if(digitalRead(up)==0){
  if(db1==0){
  db1=1;
  cx++;}
  }else db1=0;

  if(digitalRead(down)==0){
  if(db2==0){
  db2=1;
  cy++;}
  }else db2=0;

  if(cx==4)
  cx=0;
  if(cy==4)
  cy=0;

   if(digitalRead(presS)==0){
  if(db3==0){
  db3=1;

  if(buttons[cx][cy]=='0' || buttons[cx][cy]=='1' || buttons[cx][cy]=='2' || buttons[cx][cy]=='3' || buttons[cx][cy]=='4' || buttons[cx][cy]=='5' || buttons[cx][cy]=='6' || buttons[cx][cy]=='7' || buttons[cx][cy]=='8' ||  buttons[cx][cy]=='9' || buttons[cx][cy]=='.' )
  {
    num=num*(digit*10)+buttons[cx][cy]-'0';
    digit=1;
  }

  if(buttons[cx][cy]=='C')
  {num=0;cx=0;cy=0;operation=0;}

              if(buttons[cx][cy]=='+') {operation=1; n1=num; num=0;}
              if(buttons[cx][cy]=='-') {operation=2; n1=num; num=0;}
              if(buttons[cx][cy]=='*') {operation=3; n1=num; num=0;}
              if(buttons[cx][cy]=='/') {operation=4; n1=num; num=0;}

  if(buttons[cx][cy]=='=') {

               if(operation==1)
               {float r=n1+num; num=r; n1=num;
               
               }

                if(operation==2)
               {float r=n1-num; num=r; n1=num;
               }

                if(operation==3)
               {float r=n1*num; num=r; n1=num;
               }

                if(operation==4)
               {float r=n1/num; num=r; n1=num;
              }

               delay(200);
              }  
  }
  }else db3=0;
}

  void drawStop()
  {
    display.setFont();
    display.clearDisplay();
    display.setTextColor(1);
    display.setCursor(0,0);
    display.print("STOPWATCH");
    
    

  
    
    display.setCursor(0,20);
    display.setTextSize(2);
    display.print(s_min);
    display.setCursor(24,20);
    display.print(":");
    display.setCursor(34,20);
    display.print(s_sec);
    
    display.setTextSize(4);
    display.setCursor(6,64);
    display.print((int)s_milis);

    
    
    display.display();

    if(s_fase==1){
     s_milis=s_milis+3.5;

       if(s_milis>99)
      {
        s_sec++;
        s_milis=0;
      }

        if(s_sec>59)
      {
        s_min++;
        s_sec=0;
      }
    }
   display.setTextSize(0);   
  }  

  void checkButtonsStop()
  {
      if(digitalRead(presS)==0){
  if(db3==0){
  db3=1;
  s_fase++;
  if(s_fase==3)
  { s_fase=0; s_milis=0; s_sec=0; s_min=0;}
  }
  }else db3=0;

  
  }

  void drawGame()
  {
    display.clearDisplay();

    display.setCursor(40,0);
    display.print(gameScore);
    display.setCursor(2,0);
    display.print("Score:");
    display.drawLine(0,9,0,127,1);
    display.drawLine(63,9,63,127,1);
    display.drawLine(0,9,63,9,1);
    display.fillRect(playerX,118,playerW,2,1);
    display.fillCircle(ballX,ballY,1,1);

    for(int i=0;i<14;i++)
    if(enL[i]==1)
    display.fillRect(enX[i],enY[i],8,2,1);
    
    display.display();
    
    }

    void checkButtonsGame()
    {

         if(digitalRead(presS)==0){
         if(db3==0){
           db3=1;
           controler=!controler;
           digitalWrite(3,controler);
         }
         }else db3=0;

        
        if(controler==1)
        {
          playerX=map(analogRead(A0),0,1023,1,63-playerW);
        }
        
        if(controler==0)
        {
        if(digitalRead(down)==0)
        if(playerX>1)
        playerX--;

        if(digitalRead(up)==0)
        if(playerX<62-playerW)
        playerX++;
        }
 
    }

    void GameReset()
    {
      ballX=random(10,50);
      ballY=50;
       ballDirectionX=1;
       ballDirectionY=1;
      gameScore=0;
      for(int i=0;i<14;i++)
      enL[i]=1;
    }

    void gameOver()
    {
    display.clearDisplay();
    display.setCursor(6,20);
    display.print(gameScore);
    display.setTextSize(2); 
    display.setCursor(6,40);
    display.print("GAME");
    display.setCursor(6,60);
    display.print("OVER");
    display.setTextSize(0); 
    display.display();
    delay(3000);
    GameReset();
    }

    void checkColision()
    {
     if(ballX<4 || ballX>62)
     ballDirectionX=ballDirectionX*-1;
     if(ballY<10)
     ballDirectionY=ballDirectionY*-1;

     if(ballY>116 && ballX>playerX && ballX<playerX+playerW)
     ballDirectionY=ballDirectionY*-1;

    for(int i=0;i<14;i++)
    if(ballX>enX[i] && ballX<enX[i]+8 && ballY>enY[i] && ballY<enY[i]+2 && enL[i]==1)
      {
       ballDirectionY=ballDirectionY*-1;
       enL[i]=0; gameScore++;
      }
     
     ballX=ballX+ballDirectionX; 
     ballY=ballY+ballDirectionY;

     if(ballY>124)
     gameOver();
     
     
     if(gameScore%14==0 && gameScore!=0)
     GameReset();
     
      
    }

    void calendarDraw()
    {
      display.setRotation(0);
      display.clearDisplay();
      display.setCursor(0,0);
      display.print("Mo Tu We Th Fr Sa Su");
      int d=1;
      int dd=1;
      for(int i=0;i<6;i++)
      for(int j=0;j<7;j++)
      {
        if(dd<=dayS[chosenMonth] && d>=startDay[chosenMonth]){
        display.setCursor(j*18,(i+1)*9);
        display.print(dd);
        dd++;  
        }
        d++;
        
      }
      display.setCursor(70,55);
      display.print(chosenMonth+1);
      display.print("/2022");
      
      display.display(); 
      
      }

      void checkButtonsCalendar()
      {
  if(digitalRead(up)==0){
  if(db1==0){
  db1=1;
  if(chosenMonth>0)
  chosenMonth--;}
  }else db1=0;

  if(digitalRead(down)==0){
  if(db2==0){
  db2=1;
   if(chosenMonth<11)
  chosenMonth++;}
  }else db2=0; 
        
      }

      void drawMenu()
      {
        display.clearDisplay();

        display.setCursor(34,94);
        display.print("Beep");
        display.setCursor(38,104);
        if(sounds==1)
        display.print("ON");
        else
        display.print("OFF");
     
        display.setCursor(0,120);
        if(chosenMenu==0)
        display.print("Calculator");
         if(chosenMenu==1)
        display.print("Stopwatch");
        if(chosenMenu==2)
        display.print("Games");
        if(chosenMenu==3)
        display.print("Calendar");
         if(chosenMenu==4)
        display.print("PhoneBook");
        
        display.drawBitmap(0,0,epd_bitmap_logo,64,30,1);
        display.drawBitmap(menuX[0],menuY[0],myBitmapcalc,24,24,1);
        display.drawBitmap(menuX[1],menuY[1],myBitmapstop,24,24,1);
        display.drawBitmap(menuX[2],menuY[2],myBitmapgam,24,24,1);
        display.drawBitmap(menuX[3],menuY[3],myBitmapcalen,24,24,1);
        display.drawBitmap(menuX[4],menuY[4],myBitmapphone,24,24,1);
        display.drawRoundRect(menuX[chosenMenu]-2,menuY[chosenMenu]-2,28,28,2,1);
        display.display();
        
        
      }

        void checkButtonsMenu()
        {
        if(digitalRead(up)==0){
  if(db1==0){
  db1=1; if(sounds==1)tone(9,1100,50);
  if(chosenMenu>0)
  chosenMenu--;}
  }else db1=0;

  if(digitalRead(down)==0){
  if(db2==0){
  db2=1; if(sounds==1)tone(9,1100,50);
   if(chosenMenu<5)
  chosenMenu++;}
  }else db2=0; 

  if(digitalRead(presS)==0){
  if(chosenMenu==5)
  sounds=!sounds; 
  else 
  fase=chosenMenu+1; if(sounds==1)tone(9,1100,50);
  delay(400);}
          }

void resetAll()
{display.setFont();
 cy=0; cy=0;
 n1=0;
 n2=0;
 num=0;
 digit=0;
 operation=0;}

 void phoneDraw()
 {
  display.clearDisplay();
  display.setCursor(0,4);
  display.print("Mike");
  display.setCursor(0,14);
  display.print("0436789");
  display.drawLine(0,1,63,1,1);

   display.setCursor(0,28);
  display.print("Betty");
  display.setCursor(0,38);
  display.print("6574834");
  display.drawLine(0,25,63,25,1);

  display.setCursor(0,52);
  display.print("JohnConor");
  display.setCursor(0,62);
  display.print("6453722");
  display.drawLine(0,49,63,49,1);

   display.setCursor(0,76);
  display.print("Luke");
  display.setCursor(0,86);
  display.print("2275849");
  display.drawLine(0,73,63,73,1);
  
  display.display();
  
  }


    
