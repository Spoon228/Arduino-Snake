#include "LedControl.h"
#include <stdlib.h>

const int SW_pin = 2;
const int X_pin = 0;
const int Y_pin = 1;

LedControl lc = LedControl(12,10,11,1);
void setup() {
  lc.shutdown(0,false);
  lc.setIntensity(0,1);
  lc.clearDisplay(0);
  pinMode(SW_pin, INPUT);
  digitalWrite(SW_pin, HIGH);
  randomSeed(analogRead(0));
  SetStartVariables();
}

struct Coords {
    int x;
    int y;
};

//Character position X and Y
int HeadPosX;
int HeadPosY;
Coords SnakeBody[64]; 
int SnakeLength;
int MovementDirectionX;
int MovementDirectionY;
bool GameOver = false;

void SetStartVariables() {
    HeadPosX = 4;
    HeadPosY = 4;
    SnakeLength = 3;
    MovementDirectionX = 1;
    MovementDirectionY = 0;
    SnakeBody[0] = {4, 4};
    SnakeBody[1] = {3, 4};
    SnakeBody[2] = {2, 4};
    GameOver = false;
    lc.shutdown(0, false);
    lc.clearDisplay(0);
    FoodUpdate();
}

int FoodX = random(8);
int FoodY = random(8);

int PartOfFrame = 1;
int FramesPerMovement = 4;
int MSPerMovement = 600;


void loop() {
  if (PartOfFrame == FramesPerMovement){
    MovementUpdate();
    BodyUpdate();
    PartOfFrame = 1;
  }else{
    PartOfFrame++;
  }
  LEDViewUpdate();
  delay(MSPerMovement / FramesPerMovement);

  if(digitalRead(SW_pin) == 0 && GameOver){
    SetStartVariables();
  }
}

void MovementUpdate(){
  int XValue = analogRead(X_pin);
  int YValue = analogRead(Y_pin);
  if(XValue > 612 && MovementDirectionX == 0) {MovementDirectionX = -1; MovementDirectionY = 0;}
  else if(XValue < 412 && MovementDirectionX == 0) {MovementDirectionX = 1; MovementDirectionY = 0;}
  else if(YValue > 612 && MovementDirectionY == 0) {MovementDirectionY = 1; MovementDirectionX = 0;}
  else if(YValue < 412 && MovementDirectionY == 0) {MovementDirectionY = -1; MovementDirectionX = 0;}
}

void BodyUpdate() {
  HeadPosX += MovementDirectionX;
  if(HeadPosX == 8) HeadPosX = 0;
  if(HeadPosX == -1) HeadPosX = 7;
  HeadPosY += MovementDirectionY;
  if(HeadPosY == 8) HeadPosY = 0;
  if(HeadPosY == -1) HeadPosY = 7;

  for (int i = 0; i < SnakeLength; i++){
    if(HeadPosX == SnakeBody[i].x && HeadPosY == SnakeBody[i].y && i != 1){
      lc.shutdown(0,true);
      GameOver = true;
    }
  }

  if(HeadPosX == FoodX && HeadPosY == FoodY) {SnakeLength++; FoodUpdate();}

  Coords positionToPassDown = {HeadPosX, HeadPosY};
  Coords temp;

  for (int i = 0; i < SnakeLength; i++) {
    temp = SnakeBody[i];
    SnakeBody[i] = positionToPassDown;
    positionToPassDown = temp;
  }
}
bool Blink = true;
void LEDViewUpdate(){
  lc.clearDisplay(0);
  for (int i = 0; i < SnakeLength; i++){
    lc.setLed(0, SnakeBody[i].x, SnakeBody[i].y, true);
  }
  lc.setLed(0, FoodX, FoodY, Blink);
  if(Blink){
    Blink = false;
  }else{
    Blink = true;
  }
}

void FoodUpdate(){
  FoodX = random(8);
  FoodY = random(8);
  for (int i = 0; i < SnakeLength; i++){
    if(FoodX == SnakeBody[i].x && FoodY == SnakeBody[i].y) {FoodUpdate();}
  }
}


