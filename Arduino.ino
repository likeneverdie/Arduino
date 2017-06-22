# include <stdio.h>
# include <stdlib.h>
# include <time.h>
# include <Wire.h>
# include <LiquidCrystal_I2C.h> // LCD螢幕

// 紅外線障礙物感應器
// 逆時針旋轉指針可以降低感應距離(目前預設左旋1/4)
// http://blog.ilc.edu.tw/blog/index.php?op=printView&articleId=621311&blogId=868
// http://henrysbench.capnfatz.com/henrys-bench/arduino-sensors-and-input/arduino-ir-obstacle-sensor-tutorial-and-manual/

const int buzzerPin = 13; // 蜂鳴器腳位

const int greenLedPin = 2; // 不同顏色之led燈腳位 1P
const int blueLedPin = 3;
const int redLedPin = 4;

const int pinkLedPin = 5;  // 2P
const int purpleLedPin = 6;
const int yellowLedPin = 7;

const int ObstaclePin_green = 8; // 障礙物腳位 1P
const int ObstaclePin_blue = 9;
const int ObstaclePin_red = 10;

const int ObstaclePin_pink = 11; // 2P
const int ObstaclePin_purple = 12;
const int ObstaclePin_yellow = 15; // 類比A1

int Obstacle_Correct = HIGH;  // HIGH MEANS NO OBSTACLE 有障礙物: LOW 沒有障礙物: HIGH 設定初始值: 沒有障礙物
int Obstacle_Wrong1 = HIGH;
int Obstacle_Wrong2 = HIGH;
int Obstacle_Wrong3 = HIGH;
int Obstacle_Wrong4 = HIGH;
int Obstacle_Wrong5 = HIGH;

void showScreen_1P(int score, String feedback);
void showScreen_2P(int health_1P, int health_2P, String feedback);
void catch_ball_music(int speakerPin);
void fiveSecond(int speakerPin);
void score_4(int speakerPin);
void score_5_9(int speakerPin);
void score_10(int speakerPin);
void minus_1P(int speakerPin);
void wrong_throw(int speakerPin);
void game_over(int speakerPin);

void setup() {

}


void loop() {

  int score = 0; // 單人模式計分用int score = 0; // 單人模式計分用

  int health_1P = 100; // 雙人模式的血量
  int health_2P = 100;

  pinMode(buzzerPin, OUTPUT); // 蜂鳴器 ---> 輸出模式

  pinMode(greenLedPin, OUTPUT); // Led燈 ---> 輸出模式
  pinMode(purpleLedPin, OUTPUT);
  pinMode(redLedPin, OUTPUT);
  pinMode(pinkLedPin, OUTPUT);
  pinMode(blueLedPin, OUTPUT);
  pinMode(yellowLedPin, OUTPUT);

  pinMode(ObstaclePin_green, INPUT); //障礙物感測器 ---> 輸入模式
  pinMode(ObstaclePin_purple, INPUT);
  pinMode(ObstaclePin_red, INPUT);
  pinMode(ObstaclePin_pink, INPUT);
  pinMode(ObstaclePin_blue, INPUT);
  pinMode(ObstaclePin_yellow, INPUT);

  Serial.begin(9600); // 開啟溝通視窗


  //以下顯示LCD: 提示使用者選擇何種模式
  LiquidCrystal_I2C lcd(0x3F, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE); //設定 LCD I2C 位址 如果晶片是PCF8574AT

  lcd.begin(20, 4);      // 初始化 LCD，一行 20 的字元，共 4 行，預設開啟背光

  delay(100);
  lcd.setCursor(0, 0); // 設定游標位置在第一行行首
  lcd.print("1P: Green LED");
  delay(100);
  lcd.setCursor(0, 1); // 設定游標位置在第二行行首
  lcd.print("2P: Pink LED");
  delay(100);

  //以下點亮綠色以及粉色燈泡
  digitalWrite(greenLedPin, HIGH);
  digitalWrite(pinkLedPin, HIGH);

  while (1) {

    if (digitalRead(ObstaclePin_green) == LOW) {
      //進入單人模式
      digitalWrite(greenLedPin, LOW);
      digitalWrite(pinkLedPin, LOW);
      lcd.clear();
      delay(2000);

      showScreen_1P(score, "Game Start!");

      int i;

      int a[11]; // 產生一陣列儲存每一個迴圈的亂數資料
      a[0] = 0;
      a[1] = 0;

      for (i = 1; i <= 10; i++) {
        while (1) {
          randomSeed(analogRead(A0)); // 宣告亂數種子(以數位訊號為亂數種)
          a[i] = random(1000) % 6 + 1;
          if (a[i] != a[i - 1]) // 準備產生亂數 以陣列比較前後兩次亂數是否一樣, 若一樣則重新產生一次亂數, 否則 break
            break;
        }

        if ( a[i] == 1 ) {
          digitalWrite(greenLedPin, HIGH);
          const unsigned long previousMillis = millis(); // 紀錄迴圈開始時當下的時間

          while (1) {

            Obstacle_Correct = digitalRead(ObstaclePin_green);
            Obstacle_Wrong1 = digitalRead(ObstaclePin_blue);
            Obstacle_Wrong2 = digitalRead(ObstaclePin_red);
            Obstacle_Wrong3 = digitalRead(ObstaclePin_pink);
            Obstacle_Wrong4 = digitalRead(ObstaclePin_purple);
            Obstacle_Wrong5 = digitalRead(ObstaclePin_yellow);

            //unsigned long currentMillis = millis();
            //Serial.println(currentMillis);


            if (Obstacle_Correct == LOW) {
              delay(100);
              catch_ball_music(buzzerPin);
              Serial.println("Goal!!!");
              score += 1;
              showScreen_1P(score, "Goal!!");
              digitalWrite(greenLedPin, LOW);
              break;
            }
            else if (Obstacle_Wrong1 == LOW) {
              delay(100);
              wrong_throw(buzzerPin);
              Serial.println("Wrong hole");
              showScreen_1P(score, "Wrong hole");
              digitalWrite(greenLedPin, LOW);
              break;
            }
            else if (Obstacle_Wrong2 == LOW) {
              delay(100);
              wrong_throw(buzzerPin);
              Serial.println("Wrong hole");
              showScreen_1P(score, "Wrong hole");
              digitalWrite(greenLedPin, LOW);
              break;
            }
            else if (Obstacle_Wrong3 == LOW) {
              delay(100);
              wrong_throw(buzzerPin);
              Serial.println("Wrong hole");
              showScreen_1P(score, "Wrong hole");
              digitalWrite(greenLedPin, LOW);
              break;
            }
            else if (Obstacle_Wrong4 == LOW) {
              delay(100);
              wrong_throw(buzzerPin);
              Serial.println("Wrong hole");
              showScreen_1P(score, "Wrong hole");
              digitalWrite(greenLedPin, LOW);
              break;
            }
            else if (Obstacle_Wrong5 == LOW) {
              delay(100);
              wrong_throw(buzzerPin);
              Serial.println("Wrong hole");
              showScreen_1P(score, "Wrong hole");
              digitalWrite(greenLedPin, LOW);
              break;
            }

            unsigned long currentMillis = millis();  // 紀錄沒感測到障礙物時的時間
            unsigned long timeInterval = currentMillis - previousMillis;
            //Serial.println(timeInterval);

            if (timeInterval == 15000) {
              fiveSecond(buzzerPin);
            }

            if (timeInterval == 20000) {
              Serial.println("Times up");
              showScreen_1P(score, "Time's up!!");
              digitalWrite(greenLedPin, LOW);
              break;
            }
          }
        }

        else if ( a[i] == 2 ) {
          digitalWrite(blueLedPin, HIGH);
          const unsigned long previousMillis = millis(); // 紀錄迴圈開始時當下的時間

          while (1) {

            Obstacle_Correct = digitalRead(ObstaclePin_blue);
            Obstacle_Wrong1 = digitalRead(ObstaclePin_red);
            Obstacle_Wrong2 = digitalRead(ObstaclePin_green);
            Obstacle_Wrong3 = digitalRead(ObstaclePin_pink);
            Obstacle_Wrong4 = digitalRead(ObstaclePin_purple);
            Obstacle_Wrong5 = digitalRead(ObstaclePin_yellow);

            //unsigned long currentMillis = millis();
            //Serial.println(currentMillis);

            if (Obstacle_Correct == LOW) {
              delay(100);
              catch_ball_music(buzzerPin);
              Serial.println("Goal!!!");
              score += 1;
              showScreen_1P(score, "Goal!!");
              digitalWrite(blueLedPin, LOW);
              break;
            }
            else if (Obstacle_Wrong1 == LOW) {
              delay(100);
              wrong_throw(buzzerPin);
              Serial.println("Wrong hole");
              showScreen_1P(score, "Wrong hole");
              digitalWrite(blueLedPin, LOW);
              break;
            }
            else if (Obstacle_Wrong2 == LOW) {
              delay(100);
              wrong_throw(buzzerPin);
              Serial.println("Wrong hole");
              showScreen_1P(score, "Wrong hole");
              digitalWrite(blueLedPin, LOW);
              break;
            }
            else if (Obstacle_Wrong3 == LOW) {
              delay(100);
              wrong_throw(buzzerPin);
              Serial.println("Wrong hole");
              showScreen_1P(score, "Wrong hole");
              digitalWrite(blueLedPin, LOW);
              break;
            }
            else if (Obstacle_Wrong4 == LOW) {
              delay(100);
              wrong_throw(buzzerPin);
              Serial.println("Wrong hole");
              showScreen_1P(score, "Wrong hole");
              digitalWrite(blueLedPin, LOW);
              break;
            }
            else if (Obstacle_Wrong5 == LOW) {
              delay(100);
              wrong_throw(buzzerPin);
              Serial.println("Wrong hole");
              showScreen_1P(score, "Wrong hole");
              digitalWrite(blueLedPin, LOW);
              break;
            }

            unsigned long currentMillis = millis();  // 紀錄沒感測到障礙物時的時間
            unsigned long timeInterval = currentMillis - previousMillis;
            //Serial.println(timeInterval);

            if (timeInterval == 15000) {
              fiveSecond(buzzerPin);
            }

            if (timeInterval == 20000) {
              Serial.println("Times up");
              showScreen_1P(score, "Time's up!!");
              digitalWrite(blueLedPin, LOW);
              break;
            }
          }
        }

        else if ( a[i] == 3 ) {
          digitalWrite(redLedPin, HIGH);
          const unsigned long previousMillis = millis(); // 紀錄迴圈開始時當下的時間

          while (1) {

            Obstacle_Correct = digitalRead(ObstaclePin_red);
            Obstacle_Wrong1 = digitalRead(ObstaclePin_blue);
            Obstacle_Wrong2 = digitalRead(ObstaclePin_green);
            Obstacle_Wrong3 = digitalRead(ObstaclePin_pink);
            Obstacle_Wrong4 = digitalRead(ObstaclePin_purple);
            Obstacle_Wrong5 = digitalRead(ObstaclePin_yellow);

            //unsigned long currentMillis = millis();
            //Serial.println(currentMillis);

            if (Obstacle_Correct == LOW) {
              delay(100);
              catch_ball_music(buzzerPin);
              Serial.println("Goal!!!");
              score += 1;
              showScreen_1P(score, "Goal!!");
              digitalWrite(redLedPin, LOW);
              break;
            }
            else if (Obstacle_Wrong1 == LOW) {
              delay(100);
              wrong_throw(buzzerPin);
              Serial.println("Wrong hole");
              showScreen_1P(score, "Wrong hole");
              digitalWrite(redLedPin, LOW);
              break;
            }
            else if (Obstacle_Wrong2 == LOW) {
              delay(100);
              wrong_throw(buzzerPin);
              Serial.println("Wrong hole");
              showScreen_1P(score, "Wrong hole");
              digitalWrite(redLedPin, LOW);
              break;
            }
            else if (Obstacle_Wrong3 == LOW) {
              delay(100);
              wrong_throw(buzzerPin);
              Serial.println("Wrong hole");
              showScreen_1P(score, "Wrong hole");
              digitalWrite(redLedPin, LOW);
              break;
            }
            else if (Obstacle_Wrong4 == LOW) {
              delay(100);
              wrong_throw(buzzerPin);
              Serial.println("Wrong hole");
              showScreen_1P(score, "Wrong hole");
              digitalWrite(redLedPin, LOW);
              break;
            }
            else if (Obstacle_Wrong5 == LOW) {
              delay(100);
              wrong_throw(buzzerPin);
              Serial.println("Wrong hole");
              showScreen_1P(score, "Wrong hole");
              digitalWrite(redLedPin, LOW);
              break;
            }

            unsigned long currentMillis = millis();  // 紀錄沒感測到障礙物時的時間
            unsigned long timeInterval = currentMillis - previousMillis;
            //Serial.println(timeInterval);

            if (timeInterval == 15000) {
              fiveSecond(buzzerPin);
            }

            if (timeInterval == 20000) {
              Serial.println("Times up");
              showScreen_1P(score, "Time's up!!");
              digitalWrite(redLedPin, LOW);
              break;
            }
          }
        }
        delay(1000);

        if ( a[i] == 4 ) {
          digitalWrite(pinkLedPin, HIGH);
          const unsigned long previousMillis = millis(); // 紀錄迴圈開始時當下的時間

          while (1) {

            Obstacle_Correct = digitalRead(ObstaclePin_pink);
            Obstacle_Wrong1 = digitalRead(ObstaclePin_blue);
            Obstacle_Wrong2 = digitalRead(ObstaclePin_red);
            Obstacle_Wrong3 = digitalRead(ObstaclePin_green);
            Obstacle_Wrong4 = digitalRead(ObstaclePin_purple);
            Obstacle_Wrong5 = digitalRead(ObstaclePin_yellow);

            //unsigned long currentMillis = millis();
            //Serial.println(currentMillis);


            if (Obstacle_Correct == LOW) {
              delay(100);
              catch_ball_music(buzzerPin);
              Serial.println("Goal!!!");
              score += 1;
              showScreen_1P(score, "Goal!!");
              digitalWrite(pinkLedPin, LOW);
              break;
            }
            else if (Obstacle_Wrong1 == LOW) {
              delay(100);
              wrong_throw(buzzerPin);
              Serial.println("Wrong hole");
              score -= 1;
              showScreen_1P(score, "Wrong hole");
              digitalWrite(pinkLedPin, LOW);
              break;
            }
            else if (Obstacle_Wrong2 == LOW) {
              delay(100);
              wrong_throw(buzzerPin);
              Serial.println("Wrong hole");
              score -= 1;
              showScreen_1P(score, "Wrong hole");
              digitalWrite(pinkLedPin, LOW);
              break;
            }
            else if (Obstacle_Wrong3 == LOW) {
              delay(100);
              wrong_throw(buzzerPin);
              Serial.println("Wrong hole");
              score -= 1;
              showScreen_1P(score, "Wrong hole");
              digitalWrite(pinkLedPin, LOW);
              break;
            }
            else if (Obstacle_Wrong4 == LOW) {
              delay(100);
              wrong_throw(buzzerPin);
              Serial.println("Wrong hole");
              score -= 1;
              showScreen_1P(score, "Wrong hole");
              digitalWrite(pinkLedPin, LOW);
              break;
            }
            else if (Obstacle_Wrong5 == LOW) {
              delay(100);
              wrong_throw(buzzerPin);
              Serial.println("Wrong hole");
              score -= 1;
              showScreen_1P(score, "Wrong hole");
              digitalWrite(pinkLedPin, LOW);
              break;
            }

            unsigned long currentMillis = millis();  // 紀錄沒感測到障礙物時的時間
            unsigned long timeInterval = currentMillis - previousMillis;
            //Serial.println(timeInterval);

            if (timeInterval == 15000) {
              fiveSecond(buzzerPin);
            }

            if (timeInterval == 20000) {
              Serial.println("Times up");
              score -= 1;
              showScreen_1P(score, "Time's up!!");
              digitalWrite(pinkLedPin, LOW);
              break;
            }
          }
        }
        if ( a[i] == 5 ) {
          digitalWrite(purpleLedPin, HIGH);
          const unsigned long previousMillis = millis(); // 紀錄迴圈開始時當下的時間

          while (1) {

            Obstacle_Correct = digitalRead(ObstaclePin_purple);
            Obstacle_Wrong1 = digitalRead(ObstaclePin_blue);
            Obstacle_Wrong2 = digitalRead(ObstaclePin_red);
            Obstacle_Wrong3 = digitalRead(ObstaclePin_green);
            Obstacle_Wrong4 = digitalRead(ObstaclePin_pink);
            Obstacle_Wrong5 = digitalRead(ObstaclePin_yellow);

            //unsigned long currentMillis = millis();
            //Serial.println(currentMillis);


            if (Obstacle_Correct == LOW) {
              delay(100);
              catch_ball_music(buzzerPin);
              Serial.println("Goal!!!");
              score += 1;
              showScreen_1P(score, "Goal!!");
              digitalWrite(purpleLedPin, LOW);
              break;
            }
            else if (Obstacle_Wrong1 == LOW) {
              delay(100);
              wrong_throw(buzzerPin);
              Serial.println("Wrong hole");
              score -= 1;
              showScreen_1P(score, "Wrong hole");
              digitalWrite(purpleLedPin, LOW);
              break;
            }
            else if (Obstacle_Wrong2 == LOW) {
              delay(100);
              wrong_throw(buzzerPin);
              Serial.println("Wrong hole");
              score -= 1;
              showScreen_1P(score, "Wrong hole");
              digitalWrite(purpleLedPin, LOW);
              break;
            }
            else if (Obstacle_Wrong3 == LOW) {
              delay(100);
              wrong_throw(buzzerPin);
              Serial.println("Wrong hole");
              score -= 1;
              showScreen_1P(score, "Wrong hole");
              digitalWrite(purpleLedPin, LOW);
              break;
            }
            else if (Obstacle_Wrong4 == LOW) {
              delay(100);
              wrong_throw(buzzerPin);
              Serial.println("Wrong hole");
              score -= 1;
              showScreen_1P(score, "Wrong hole");
              digitalWrite(purpleLedPin, LOW);
              break;
            }
            else if (Obstacle_Wrong5 == LOW) {
              delay(100);
              wrong_throw(buzzerPin);
              Serial.println("Wrong hole");
              score -= 1;
              showScreen_1P(score, "Wrong hole");
              digitalWrite(purpleLedPin, LOW);
              break;
            }

            unsigned long currentMillis = millis();  // 紀錄沒感測到障礙物時的時間
            unsigned long timeInterval = currentMillis - previousMillis;
            //Serial.println(timeInterval);

            if (timeInterval == 15000) {
              fiveSecond(buzzerPin);
            }

            if (timeInterval == 20000) {
              Serial.println("Times up");
              score -= 1;
              showScreen_1P(score, "Time's up!!");
              digitalWrite(purpleLedPin, LOW);
              break;
            }
          }
        }
        if ( a[i] == 6 ) {
          digitalWrite(yellowLedPin, HIGH);
          const unsigned long previousMillis = millis(); // 紀錄迴圈開始時當下的時間

          while (1) {

            Obstacle_Correct = digitalRead(ObstaclePin_yellow);
            Obstacle_Wrong1 = digitalRead(ObstaclePin_blue);
            Obstacle_Wrong2 = digitalRead(ObstaclePin_red);
            Obstacle_Wrong3 = digitalRead(ObstaclePin_green);
            Obstacle_Wrong4 = digitalRead(ObstaclePin_pink);
            Obstacle_Wrong5 = digitalRead(ObstaclePin_purple);

            //unsigned long currentMillis = millis();
            //Serial.println(currentMillis);


            if (Obstacle_Correct == LOW) {
              delay(100);
              catch_ball_music(buzzerPin);
              Serial.println("Goal!!!");
              score += 1;
              showScreen_1P(score, "Goal!!");
              digitalWrite(yellowLedPin, LOW);
              break;
            }
            else if (Obstacle_Wrong1 == LOW) {
              delay(100);
              wrong_throw(buzzerPin);
              Serial.println("Wrong hole");
              score -= 1;
              showScreen_1P(score, "Wrong hole");
              digitalWrite(yellowLedPin, LOW);
              break;
            }
            else if (Obstacle_Wrong2 == LOW) {
              delay(100);
              wrong_throw(buzzerPin);
              Serial.println("Wrong hole");
              score -= 1;
              showScreen_1P(score, "Wrong hole");
              digitalWrite(yellowLedPin, LOW);
              break;
            }
            else if (Obstacle_Wrong3 == LOW) {
              delay(100);
              wrong_throw(buzzerPin);
              Serial.println("Wrong hole");
              score -= 1;
              showScreen_1P(score, "Wrong hole");
              digitalWrite(yellowLedPin, LOW);
              break;
            }
            else if (Obstacle_Wrong4 == LOW) {
              delay(100);
              wrong_throw(buzzerPin);
              Serial.println("Wrong hole");
              score -= 1;
              showScreen_1P(score, "Wrong hole");
              digitalWrite(yellowLedPin, LOW);
              break;
            }
            else if (Obstacle_Wrong5 == LOW) {
              delay(100);
              wrong_throw(buzzerPin);
              Serial.println("Wrong hole");
              score -= 1;
              showScreen_1P(score, "Wrong hole");
              digitalWrite(yellowLedPin, LOW);
              break;
            }

            unsigned long currentMillis = millis();  // 紀錄沒感測到障礙物時的時間
            unsigned long timeInterval = currentMillis - previousMillis;
            //Serial.println(timeInterval);

            if (timeInterval == 15000) {
              fiveSecond(buzzerPin);
            }

            if (timeInterval == 20000) {
              Serial.println("Times up");
              score -= 1;
              showScreen_1P(score, "Time's up!!");
              digitalWrite(yellowLedPin, LOW);
              break;
            }
          }
        }
      }

      delay(100);
      Serial.print("Your score: ");
      Serial.println(score);

      if (score <= 4 && score > 0) {
        showScreen_1P(score, "Poor U :(");
        score_4(buzzerPin);
      }
      if (score > 4 && score <= 9) {
        showScreen_1P(score, "Congratulations <3 ");
        score_5_9(buzzerPin);
      }
      if (score == 10) {
        showScreen_1P(score, "WOOOOOOW");
        score_10(buzzerPin);
      }
      if ( score <= 0 ) {
        showScreen_1P(score, "Retard?");
        minus_1P(buzzerPin);
      }
      break;
    }

    else if (digitalRead(ObstaclePin_pink) == LOW) {
      //進入雙人模式
      digitalWrite(greenLedPin, LOW);
      digitalWrite(pinkLedPin, LOW);
      lcd.clear();
      delay(2000);

      showScreen_2P(health_1P, health_2P, "Game Start!");

      int i;
      int a, b, c, d; // 產生一陣列儲存每一個迴圈的亂數資料

      a = 0;
      b = 0;
      c = 0;
      d = 0;
      //2P先攻

      while (1) {

        while (a == b) {
          randomSeed(analogRead(A0)); // 宣告亂數種子(以數位訊號為亂數種)
          a = random(1000) % 3 + 1;
        }

        b = a;

        if ( a == 1 ) {

          digitalWrite(greenLedPin, HIGH);
          const unsigned long previousMillis = millis(); // 紀錄迴圈開始時當下的時間

          while (1) {

            Obstacle_Correct = digitalRead(ObstaclePin_green);
            Obstacle_Wrong1 = digitalRead(ObstaclePin_blue);
            Obstacle_Wrong2 = digitalRead(ObstaclePin_red);

            //unsigned long currentMillis = millis();
            //Serial.println(currentMillis);


            if (Obstacle_Correct == LOW) {
              delay(100);
              catch_ball_music(buzzerPin);
              Serial.println("Goal!!!");
              health_1P -= 10;
              showScreen_2P(health_1P, health_2P, "Goal!!");
              digitalWrite(greenLedPin, LOW);
              break;
            }
            else if (Obstacle_Wrong1 == LOW) {
              delay(100);
              wrong_throw(buzzerPin);
              Serial.println("Wrong hole");
              showScreen_2P(health_1P, health_2P, "Wrong hole");
              digitalWrite(greenLedPin, LOW);
              break;
            }
            else if (Obstacle_Wrong2 == LOW) {
              delay(100);
              wrong_throw(buzzerPin);
              Serial.println("Wrong hole");
              showScreen_2P(health_1P, health_2P, "Wrong hole");
              digitalWrite(greenLedPin, LOW);
              break;
            }

            unsigned long currentMillis = millis();  // 紀錄沒感測到障礙物時的時間
            unsigned long timeInterval = currentMillis - previousMillis;
            //Serial.println(timeInterval);

            if (timeInterval == 15000) {
              fiveSecond(buzzerPin);
            }

            if (timeInterval == 20000) {
              Serial.println("Times up");
              showScreen_2P(health_1P, health_2P,  "Time's up!!");
              digitalWrite(greenLedPin, LOW);
              break;
            }
          }
        }

        else if ( a == 2 ) {
          digitalWrite(blueLedPin, HIGH);
          const unsigned long previousMillis = millis(); // 紀錄迴圈開始時當下的時間

          while (1) {

            Obstacle_Correct = digitalRead(ObstaclePin_blue);
            Obstacle_Wrong1 = digitalRead(ObstaclePin_red);
            Obstacle_Wrong2 = digitalRead(ObstaclePin_green);

            //unsigned long currentMillis = millis();
            //Serial.println(currentMillis);

            if (Obstacle_Correct == LOW) {
              delay(100);
              catch_ball_music(buzzerPin);
              Serial.println("Goal!!!");
              health_1P -= 10;
              showScreen_2P(health_1P, health_2P,  "Goal!!");
              digitalWrite(blueLedPin, LOW);
              break;
            }
            else if (Obstacle_Wrong1 == LOW) {
              delay(100);
              wrong_throw(buzzerPin);
              Serial.println("Wrong hole");
              showScreen_2P(health_1P, health_2P,  "Wrong hole");
              digitalWrite(blueLedPin, LOW);
              break;
            }
            else if (Obstacle_Wrong2 == LOW) {
              delay(100);
              wrong_throw(buzzerPin);
              Serial.println("Wrong hole");
              showScreen_2P(health_1P, health_2P,  "Wrong hole");
              digitalWrite(blueLedPin, LOW);
              break;
            }

            unsigned long currentMillis = millis();  // 紀錄沒感測到障礙物時的時間
            unsigned long timeInterval = currentMillis - previousMillis;
            //Serial.println(timeInterval);

            if (timeInterval == 15000) {
              fiveSecond(buzzerPin);
            }

            if (timeInterval == 20000) {
              Serial.println("Times up");
              showScreen_2P(health_1P, health_2P,  "Time's up!!");
              digitalWrite(blueLedPin, LOW);
              break;
            }
          }
        }

        else if ( a == 3 ) {
          digitalWrite(redLedPin, HIGH);
          const unsigned long previousMillis = millis(); // 紀錄迴圈開始時當下的時間

          while (1) {

            Obstacle_Correct = digitalRead(ObstaclePin_red);
            Obstacle_Wrong1 = digitalRead(ObstaclePin_blue);
            Obstacle_Wrong2 = digitalRead(ObstaclePin_green);

            //unsigned long currentMillis = millis();
            //Serial.println(currentMillis);

            if (Obstacle_Correct == LOW) {
              delay(100);
              catch_ball_music(buzzerPin);
              Serial.println("Goal!!!");
              health_1P -= 10;
              showScreen_2P(health_1P, health_2P,  "Goal!!");
              digitalWrite(redLedPin, LOW);
              break;
            }
            else if (Obstacle_Wrong1 == LOW) {
              delay(100);
              wrong_throw(buzzerPin);
              Serial.println("Wrong hole");
              showScreen_2P(health_1P, health_2P, "Wrong hole");
              digitalWrite(redLedPin, LOW);
              break;
            }
            else if (Obstacle_Wrong2 == LOW) {
              delay(100);
              wrong_throw(buzzerPin);
              Serial.println("Wrong hole");
              showScreen_2P(health_1P, health_2P,  "Wrong hole");
              digitalWrite(redLedPin, LOW);
              break;
            }

            unsigned long currentMillis = millis();  // 紀錄沒感測到障礙物時的時間
            unsigned long timeInterval = currentMillis - previousMillis;
            //Serial.println(timeInterval);

            if (timeInterval == 15000) {
              fiveSecond(buzzerPin);
            }

            if (timeInterval == 20000) {
              Serial.println("Times up");
              showScreen_2P(health_1P, health_2P, "Time's up!!");
              digitalWrite(redLedPin, LOW);
              break;
            }
          }
        }
        if (health_1P == 0) {
          game_over(buzzerPin);
          showScreen_2P(health_1P, health_2P, "Game Over!!!");
          break;
        }
        delay(1000);

        //1P攻

        while (c == d) {
          randomSeed(analogRead(A0)); // 宣告亂數種子(以數位訊號為亂數種)
          c = random(1000) % 3 + 4;
        }
        d = c;

        if ( c == 4 ) {
          digitalWrite(pinkLedPin, HIGH);
          const unsigned long previousMillis = millis(); // 紀錄迴圈開始時當下的時間

          while (1) {

            Obstacle_Correct = digitalRead(ObstaclePin_pink);
            Obstacle_Wrong1 = digitalRead(ObstaclePin_purple);
            Obstacle_Wrong2 = digitalRead(ObstaclePin_yellow);

            //unsigned long currentMillis = millis();
            //Serial.println(currentMillis);


            if (Obstacle_Correct == LOW) {
              delay(100);
              catch_ball_music(buzzerPin);
              Serial.println("Goal!!!");
              health_2P -= 10;
              showScreen_2P(health_1P, health_2P, "Goal!!");
              digitalWrite(pinkLedPin, LOW);
              break;
            }
            else if (Obstacle_Wrong1 == LOW) {
              delay(100);
              wrong_throw(buzzerPin);
              Serial.println("Wrong hole");
              health_1P -= 10;
              showScreen_2P(health_1P, health_2P, "Wrong hole");
              digitalWrite(pinkLedPin, LOW);
              break;
            }
            else if (Obstacle_Wrong2 == LOW) {
              delay(100);
              wrong_throw(buzzerPin);
              Serial.println("Wrong hole");
              health_1P -= 10;
              showScreen_2P(health_1P, health_2P, "Wrong hole");
              digitalWrite(pinkLedPin, LOW);
              break;
            }

            unsigned long currentMillis = millis();  // 紀錄沒感測到障礙物時的時間
            unsigned long timeInterval = currentMillis - previousMillis;
            //Serial.println(timeInterval);

            if (timeInterval == 15000) {
              fiveSecond(buzzerPin);
            }

            if (timeInterval == 20000) {
              Serial.println("Times up");
              health_1P -= 10;
              showScreen_2P(health_1P, health_2P, "Time's up!!");
              digitalWrite(pinkLedPin, LOW);
              break;
            }
          }
        }

        else if ( c == 5 ) {
          digitalWrite(purpleLedPin, HIGH);
          const unsigned long previousMillis = millis(); // 紀錄迴圈開始時當下的時間

          while (1) {

            Obstacle_Correct = digitalRead(ObstaclePin_purple);
            Obstacle_Wrong1 = digitalRead(ObstaclePin_pink);
            Obstacle_Wrong2 = digitalRead(ObstaclePin_yellow);

            //unsigned long currentMillis = millis();
            //Serial.println(currentMillis);

            if (Obstacle_Correct == LOW) {
              delay(100);
              catch_ball_music(buzzerPin);
              Serial.println("Goal!!!");
              health_2P -= 10;
              showScreen_2P(health_1P, health_2P, "Goal!!");
              digitalWrite(purpleLedPin, LOW);
              break;
            }
            else if (Obstacle_Wrong1 == LOW) {
              delay(100);
              wrong_throw(buzzerPin);
              Serial.println("Wrong hole");
              health_1P -= 10;
              showScreen_2P(health_1P, health_2P, "Wrong hole");
              digitalWrite(purpleLedPin, LOW);
              break;
            }
            else if (Obstacle_Wrong2 == LOW) {
              delay(100);
              wrong_throw(buzzerPin);
              Serial.println("Wrong hole");
              health_1P -= 10;
              showScreen_2P(health_1P, health_2P, "Wrong hole");
              digitalWrite(purpleLedPin, LOW);
              break;
            }

            unsigned long currentMillis = millis();  // 紀錄沒感測到障礙物時的時間
            unsigned long timeInterval = currentMillis - previousMillis;
            //Serial.println(timeInterval);

            if (timeInterval == 15000) {
              fiveSecond(buzzerPin);
            }

            if (timeInterval == 20000) {
              Serial.println("Times up");
              health_1P -= 10;
              showScreen_2P(health_1P, health_2P, "Time's up!!");
              digitalWrite(purpleLedPin, LOW);
              break;
            }

          }

        }

        else if ( c == 6 ) {
          digitalWrite(yellowLedPin, HIGH);
          const unsigned long previousMillis = millis(); // 紀錄迴圈開始時當下的時間

          while (1) {

            Obstacle_Correct = digitalRead(ObstaclePin_yellow);
            Obstacle_Wrong1 = digitalRead(ObstaclePin_pink);
            Obstacle_Wrong2 = digitalRead(ObstaclePin_purple);

            //unsigned long currentMillis = millis();
            //Serial.println(currentMillis);

            if (Obstacle_Correct == LOW) {
              delay(100);
              catch_ball_music(buzzerPin);
              Serial.println("Goal!!!");
              health_2P -= 10;
              showScreen_2P(health_1P, health_2P, "Goal!!");
              digitalWrite(yellowLedPin, LOW);
              break;
            }
            else if (Obstacle_Wrong1 == LOW) {
              delay(100);
              wrong_throw(buzzerPin);
              Serial.println("Wrong hole");
              health_1P -= 10;
              showScreen_2P(health_1P, health_2P, "Wrong hole");
              digitalWrite(yellowLedPin, LOW);
              break;
            }
            else if (Obstacle_Wrong2 == LOW) {
              delay(100);
              wrong_throw(buzzerPin);
              Serial.println("Wrong hole");
              health_1P -= 10;
              showScreen_2P(health_1P, health_2P, "Wrong hole");
              digitalWrite(yellowLedPin, LOW);
              break;
            }

            unsigned long currentMillis = millis();  // 紀錄沒感測到障礙物時的時間
            unsigned long timeInterval = currentMillis - previousMillis;
            //Serial.println(timeInterval);

            if (timeInterval == 15000) {
              fiveSecond(buzzerPin);
            }

            if (timeInterval == 20000) {
              Serial.println("Times up");
              health_1P -= 10;
              showScreen_2P(health_1P, health_2P, "Time's up!!");
              digitalWrite(yellowLedPin, LOW);
              break;
            }
          }
        }
        if (health_2P == 0) {
          game_over(buzzerPin);
          showScreen_2P(health_1P, health_2P, "Game Over!!!");
          break;
          delay(1000);
        }
      }

      delay(100);
      Serial.print("health(1P) = ");
      Serial.println(health_1P);
      Serial.print("health(2P) = ");
      Serial.println(health_2P);

      break;
    }
  }
  delay (3000);
}


void showScreen_1P(int score, String feedback) {

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
     LCD螢幕:
     GND接GND
     VCC接5V
     SDA接A4
     SCL接A5
  */
}


void showScreen_2P(int health_1P, int health_2P, String feedback) {

  LiquidCrystal_I2C lcd(0x3F, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE); //設定 LCD I2C 位址 如果晶片是PCF8574AT

  lcd.begin(20, 4);      // 初始化 LCD，一行 20 的字元，共 4 行，預設開啟背光

  delay(100);
  lcd.setCursor(0, 0); // 設定游標位置在第一行行首
  lcd.print("1P HP: ");
  lcd.print(health_1P);
  delay(100);
  lcd.setCursor(0, 1); // 設定游標位置在第二行行首
  lcd.print("2P HP: ");
  lcd.print(health_2P);
  delay(100);
  lcd.setCursor(0, 2); // 設定游標位置在第二行行首
  lcd.print(feedback);
  //lcd.setCursor(0, 3); // 設定游標位置在第二行行首
  //lcd.clear(); //清空畫面
}

/*
    蜂鳴器
    +(長的)接任意pin腳
    -(短的)接地GND
    利用tone函數控制(noTone)
*/

void catch_ball_music(int speakerPin)
{
  // notes in the melody:
  int /*NOTE_C2=65,*/ NOTE_E4 = 330;
  int melody[] = {NOTE_E4, NOTE_E4, NOTE_E4, NOTE_E4};
  int duration = 200;
  int thisNote;

  for (thisNote = 0; thisNote < 4; thisNote++)  // 在 pin7   上輸出聲音，每個音階響 0.5 秒
  {
    tone(speakerPin, melody[thisNote], duration);

    delay(300);  // 間隔一段時間後再播放下一個音階

  }
}

void fiveSecond(int speakerPin)
{
  // notes in the melody:
  int NOTE_C1 = 33, NOTE_G5 = 784;
  int melody[] = {NOTE_C1, NOTE_C1, NOTE_C1, NOTE_G5};
  int duration = 200;
  int thisNote;

  for (thisNote = 0; thisNote < 4; thisNote++)  // 在 pin7   上輸出聲音，每個音階響 0.5 秒
  {
    tone(speakerPin, melody[thisNote], duration);

    delay(1000);  // 間隔一段時間後再播放下一個音階

  }
}

void score_4(int speakerPin)
{
  // notes in the melody:
  int NOTE_C2 = 65, NOTE_E3 = 165, NOTE_G4 = 392;
  int melody[] = {NOTE_C2, NOTE_C2, NOTE_E3, NOTE_E3, NOTE_G4, NOTE_G4, NOTE_E3};
  int duration = 200;
  int thisNote;

  for (int thisNote = 0; thisNote < 7; thisNote++)  // 輸出聲音，每個音階響 0.5 秒
  {
    tone(speakerPin, melody[thisNote], duration);

    delay(500);  // 間隔一段時間後再播放下一個音階

  }
}

void score_5_9(int speakerPin)
{
  // notes in the melody:
  int NOTE_C5 = 523, NOTE_E7 = 2637;
  int melody[] = {NOTE_E7, NOTE_E7, NOTE_E7, NOTE_C5};
  int duration = 200;
  int thisNote;

  for (int thisNote = 0; thisNote < 4; thisNote++)  // 輸出聲音，每個音階響 0.5 秒
  {
    tone(speakerPin, melody[thisNote], duration);

    delay(500);  // 間隔一段時間後再播放下一個音階

  }
}

void score_10(int speakerPin)
{
  // notes in the melody:
  int NOTE_C2 = 65, NOTE_E3, NOTE_G4 = 2637;
  int melody[] = {NOTE_C2, NOTE_E3, NOTE_G4, NOTE_E3, NOTE_G4};
  int duration[] = {4, 4, 2, 4, 2};
  int thisNote, duration_second;

  for (int thisNote = 0; thisNote < 5; thisNote++)  // 輸出聲音，每個音階響 0.5 秒
  {
    duration_second = 1000 / duration[thisNote];
    tone(speakerPin, melody[thisNote], duration_second);

    delay(500);  // 間隔一段時間後再播放下一個音階

  }
}

void wrong_throw(int speakerPin)
{

  int NOTE_E1 = 41, NOTE_C1 = 33;
  int melody[] = {NOTE_E1, NOTE_C1};
  int duration = 200;
  int thisNote;

  for (int thisNote = 0; thisNote < 2; thisNote++)
  {
    tone(speakerPin, melody[thisNote], duration);

    delay(500);
  }
}

void game_over(int speakerPin)
{

  int NOTE_C4 = 262, NOTE_D4 = 294, NOTE_E4 = 330, NOTE_G4 = 392;
  int melody[] = {NOTE_C4, NOTE_D4, NOTE_E4, NOTE_D4};
  int duration = 200;
  int thisNote;

  for (int thisNote = 0; thisNote < 4; thisNote++)
  {
    tone(speakerPin, melody[thisNote], duration);

    delay(500);
  }
  for (int thisNote = 0; thisNote < 4; thisNote++)
  {
    tone(speakerPin, melody[thisNote], duration);

    delay(500);
  }
}

void minus_1P(int speakerPin)
{
  // notes in the melody:
  int NOTE_C3 = 131, NOTE_D3 = 147, NOTE_E3 = 165, NOTE_G3 = 196;
  int melody[] = {NOTE_E3, NOTE_E3, NOTE_E3, NOTE_E3, NOTE_E3, NOTE_E3, NOTE_E3, NOTE_G3, NOTE_C3, NOTE_D3, NOTE_E3};
  int duration[] = {20, 20, 4, 20, 20, 4, 20, 20, 20, 20, 4};
  int thisNote, duration_second;

  for (thisNote = 0; thisNote < 11; thisNote++)
  {
    duration_second = 1000 / duration[thisNote];
    tone(speakerPin, melody[thisNote], duration_second);

    delay(500);  // 間隔一段時間後再播放下一個音階

  }
}
