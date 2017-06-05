# include <stdio.h>
# include <stdlib.h>
# include <time.h>
# include <Wire.h> 
# include <LiquidCrystal_I2C.h> // LCD螢幕

// IR Obstacle Collision Detection Module
// Henry's Bench  //逆時針旋轉指針可以降低感應距離(目前預設左旋1/4)
// http://blog.ilc.edu.tw/blog/index.php?op=printView&articleId=621311&blogId=868
// http://henrysbench.capnfatz.com/henrys-bench/arduino-sensors-and-input/arduino-ir-obstacle-sensor-tutorial-and-manual/

const int greenLedPin = 4; // 不同顏色之led燈腳位
const int blueLedPin = 5;
const int redLedPin = 6;

const int ObstaclePin_green = 8;  // 障礙物腳位
const int ObstaclePin_blue = 9;
const int ObstaclePin_red = 10;

int Obstacle = HIGH;  // HIGH MEANS NO OBSTACLE 有障礙物: LOW 沒有障礙物: HIGH 設定初始值: 沒有障礙物

const int buzzerPin = 7; // 蜂鳴器腳位


int score = 0;

void showScreen(int score, String feedback);
void catch_ball_music(int speakerPin);
void fiveSecond(int speakerPin);

void setup() {
  
  pinMode(ObstaclePin_green, INPUT); //障礙物感測器 ---> 輸入模式
  pinMode(buzzerPin, OUTPUT);
  pinMode(greenLedPin, OUTPUT);
  pinMode(blueLedPin, OUTPUT);
  pinMode(redLedPin, OUTPUT);
  Serial.begin(9600); // 開啟溝通視窗

  int i;
  
  int a[11]; // 產生一陣列儲存每一個迴圈的亂數資料
  a[0]=0;
  a[1]=0;
  
  for(i = 1; i <= 10; i++){
    while(1){
    randomSeed(analogRead(A0)); // 宣告亂數種子(以數位訊號為亂數種)
    a[i] = random(1000)%3 + 1;
    if(a[i] != a[i-1])  // 準備產生亂數 以陣列比較前後兩次亂數是否一樣, 若一樣則重新產生一次亂數, 否則 break
      break;
    }

    if ( a[i] == 1 ){
      digitalWrite(greenLedPin, HIGH);
      const unsigned long previousMillis = millis(); // 紀錄迴圈開始時當下的時間

      while(1){
    
        Obstacle = digitalRead(ObstaclePin_green);
      
        //unsigned long currentMillis = millis();
        //Serial.println(currentMillis);
      
        if (Obstacle == LOW){
          delay(100);
          catch_ball_music(buzzerPin);
          Serial.println("Goal!!!");
          score += 1;
          showScreen(score, "Goal!!");
          digitalWrite(greenLedPin, LOW);
          break;
        }
      
        unsigned long currentMillis = millis();  // 紀錄沒感測到障礙物時的時間
        unsigned long timeInterval = currentMillis - previousMillis;
        //Serial.println(timeInterval);

        if(timeInterval == 15000){
          fiveSecond(buzzerPin);
        }
      
        if(timeInterval == 20000){
          Serial.println("Times up");
          showScreen(score, "Time's up!!");
          digitalWrite(greenLedPin, LOW);
          break;
        }
      }
     }

     else if ( a[i] == 2 ){
      digitalWrite(blueLedPin, HIGH);
      const unsigned long previousMillis = millis(); // 紀錄迴圈開始時當下的時間

      while(1){
    
        Obstacle = digitalRead(ObstaclePin_green);
      
        //unsigned long currentMillis = millis();
        //Serial.println(currentMillis);
      
        if (Obstacle == LOW){
          delay(100);
          catch_ball_music(buzzerPin);
          Serial.println("Goal!!!");
          score += 1;
          showScreen(score, "Goal!!");
          digitalWrite(blueLedPin, LOW);
          break;
        }
      
        unsigned long currentMillis = millis();  // 紀錄沒感測到障礙物時的時間
        unsigned long timeInterval = currentMillis - previousMillis;
        //Serial.println(timeInterval);

        if(timeInterval == 15000){
          fiveSecond(buzzerPin);
        }
      
        if(timeInterval == 20000){
          Serial.println("Times up");
          showScreen(score, "Time's up!!");
          digitalWrite(blueLedPin, LOW);
          break;
        }
      
      }
      
     }

     else if ( a[i] == 3 ){
      digitalWrite(redLedPin, HIGH);
      const unsigned long previousMillis = millis(); // 紀錄迴圈開始時當下的時間

      while(1){
    
        Obstacle = digitalRead(ObstaclePin_green);
      
        //unsigned long currentMillis = millis();
        //Serial.println(currentMillis);
      
        if (Obstacle == LOW){
          delay(100);
          catch_ball_music(buzzerPin);
          Serial.println("Goal!!!");
          score += 1;
          showScreen(score, "Goal!!");
          digitalWrite(redLedPin, LOW);
          break;
        }
      
        unsigned long currentMillis = millis();  // 紀錄沒感測到障礙物時的時間
        unsigned long timeInterval = currentMillis - previousMillis;
        //Serial.println(timeInterval);

        if(timeInterval == 15000){
          fiveSecond(buzzerPin);
        }
      
        if(timeInterval == 20000){
          Serial.println("Times up");
          showScreen(score, "Time's up!!");
          digitalWrite(redLedPin, LOW);
          break;
        }
      
      }
    }
    delay(1000);
  }
    /*const unsigned long previousMillis = millis(); // 紀錄迴圈開始時當下的時間

    while(1){
    
      Obstacle = digitalRead(ObstaclePin_green);
      
      //unsigned long currentMillis = millis();
      //Serial.println(currentMillis);
      
      if (Obstacle == LOW){
      delay(100);
      catch_ball_music(buzzerPin);
      Serial.println("Goal!!!");
      score += 1;
      break;
      }
      
      unsigned long currentMillis = millis();  // 紀錄沒感測到障礙物時的時間
      unsigned long timeInterval = currentMillis - previousMillis;
      //Serial.println(timeInterval);

      if(timeInterval == 15000){
        fiveSecond(buzzerPin);
      }
      
      if(timeInterval == 20000){
        Serial.println("Times up");
        break;
      }
      
    }
    delay(1000);
  }*/
  delay(100);
  Serial.print("score = ");
  Serial.println(score);
  if(score <= 4){
    showScreen(score, "Sharon");
    }
  if(score > 4 && score <= 9){
    showScreen(score, "Congratulations <3 ");
    }
  if(score == 10){
    showScreen(score, "OPOP");
   }
}

void loop() {
  
}

/*
    +(長的)接任意pin腳
    -(短的)接地GND
    利用tone函數控制(noTone)
 */

void showScreen(int score, String feedback){
 
LiquidCrystal_I2C lcd(0x3F, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE); //設定 LCD I2C 位址 如果晶片是PCF8574AT
 

  lcd.begin(20, 4);      // 初始化 LCD，一行 20 的字元，共 4 行，預設開啟背光
 
  delay(100);
  lcd.setCursor(0, 0); // 設定游標位置在第一行行首
  lcd.print("Your score: ");
  lcd.print(score);
  delay(200);
  lcd.setCursor(0, 1); // 設定游標位置在第二行行首
  lcd.print(feedback);
  //delay(200);
  lcd.setCursor(0, 2); // 設定游標位置在第二行行首
  //lcd.print("I am stupid ><!");
  lcd.setCursor(0, 3); // 設定游標位置在第二行行首
  //lcd.print("I am retard ><!");
  //lcd.clear(); //清空畫面
/* 
   GND接GND
   VCC接5V
   SDA接A4
   SCL接A5 
*/
}

void catch_ball_music(int speakerPin)
{
  // notes in the melody:
  int /*NOTE_C2=65,*/ NOTE_E4=330;
  int melody[] = {NOTE_E4, NOTE_E4};
  int duration = 200;  
  int thisNote;

  for (thisNote = 0; thisNote < 2; thisNote++)  // 在 pin7   上輸出聲音，每個音階響 0.5 秒
  {  
    tone(speakerPin, melody[thisNote], duration);

    delay(300);  // 間隔一段時間後再播放下一個音階

   }
}

void fiveSecond(int speakerPin)
{
// notes in the melody:
int NOTE_C1=33, NOTE_G5=784;
int melody[] = {NOTE_C1, NOTE_C1, NOTE_C1, NOTE_G5};
int duration = 200;  
int thisNote;

  for (thisNote = 0; thisNote < 4; thisNote++)  // 在 pin7   上輸出聲音，每個音階響 0.5 秒
  {  
    tone(speakerPin, melody[thisNote], duration);

    delay(1000);  // 間隔一段時間後再播放下一個音階

   }
}

