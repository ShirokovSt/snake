#include "LedControl.h"
#include <EEPROM.h>

int gameMode = 0;


class SnakeBot {
private:
  class Point { 
  public:
    int x;
    int y;
    Point() {x = 0; y = 0;}
    Point(int x, int y) { this->x = x; this->y = y;}
  };
  const int frequency = 2;
  int counter;
  int directionSnakeBot; 
  int sizeMatrix = 8; 
  int lengthSnakeBot = 4;
  Point snakeBotHead;
  int** positions;
  int scoreBot = 0;
  const int up = 0;
  const int down = 1;
  const int left = 2;
  const int right = 3;
  void changePositionBot() {
    positions[snakeBotHead.x][snakeBotHead.y] = lengthSnakeBot + 1;
    for(int i = 0; i < sizeMatrix; i++)
      for(int j = 0; j < sizeMatrix; j++){
        if(positions[i][j] > 0)
          positions[i][j]--;
      }
  }
  void moveBot() { 
    if(directionSnakeBot == right ) {
      snakeBotHead.y++;
    correctionOfCoordinates();
    } else if (directionSnakeBot == left) {
      snakeBotHead.y--;
        correctionOfCoordinates();
    } else if (directionSnakeBot == down) {
      snakeBotHead.x++;
        correctionOfCoordinates();
    } else if (directionSnakeBot == up) {
      snakeBotHead.x--;
        correctionOfCoordinates();
    }
  }
  void correctionOfCoordinates() {
    if(snakeBotHead.x < 0)
      snakeBotHead.x = sizeMatrix - 1;
    if(snakeBotHead.x >= sizeMatrix)
      snakeBotHead.x = 0;
    if(snakeBotHead.y < 0)
      snakeBotHead.y = sizeMatrix - 1;
    if(snakeBotHead.y >= sizeMatrix)
      snakeBotHead.y = 0;
  }
public:
  SnakeBot(int sizeMatrix) {
    snakeBotHead = Point(0, 3);
    directionSnakeBot = down;
    counter = 0;
    this->sizeMatrix = sizeMatrix;
    positions = new int*[sizeMatrix]();
    for(int i = 0; i < sizeMatrix; i++)
      positions[i] = new int[sizeMatrix]();
    positions[0][0] = 1;
    positions[0][1] = 2;
    positions[0][2] = 3;
    positions[0][3] = 4;
  }
  ~SnakeBot() {
    for(int i = 0; i < sizeMatrix; i++)
      delete[] positions[i];
    delete[] positions;
  }
  int getScoreBot(){ return scoreBot;}
  void incrScoreBot(){ scoreBot++;}
  bool stateForEatFruits(int xFruit, int yFruit) {
    if((yFruit == snakeBotHead.y) && (xFruit == snakeBotHead.x)) {
      lengthSnakeBot++;
      moveBot();
      changePositionBot();
      return true;
    }
    if(yFruit == snakeBotHead.y) {
      if((directionSnakeBot == up) || (directionSnakeBot == down)) {
        randomSeed(analogRead(A4));
        directionSnakeBot = random(2);
      }
    }
    if(xFruit == snakeBotHead.x) {
      if((directionSnakeBot == right) || (directionSnakeBot == left)) {
        randomSeed(analogRead(A4));
        directionSnakeBot = random(2) + 2;
      }
    }
    moveBot();
    changePositionBot();
    return false;
  }
  void state() {
    counter++;
    if(counter == frequency) { 
      counter = 0;
      randomSeed(analogRead(A4));
      int movement = random(4);
      if((movement == left && directionSnakeBot == right) 
      ||( movement == right && directionSnakeBot == left)
      ||(movement == down && directionSnakeBot == up)
      ||(movement == up && directionSnakeBot == down))
      {} else directionSnakeBot = movement;
    }
    moveBot();
    changePositionBot();
  }
  bool isOccupied(int x, int y) {
    return positions[x][y];
  }
  int getHeadX(){
    return snakeBotHead.x;
  }
  int getHeadY(){
    return snakeBotHead.y;
  }
  void reset() {
    for(int i = 0; i < sizeMatrix; i++)
      for(int j = 0; j < sizeMatrix; j++)
        positions[i][j] = 0;
    snakeBotHead = Point(0, 3);
    positions[0][0] = 1;
    positions[0][1] = 2;
    positions[0][2] = 3;
    positions[0][3] = 4;
    directionSnakeBot = down;
    counter = 0;
  }
};

class Joystick {
public:
  int x;
  int y;
  int touch;
  const int joystickBase = 500;
  const int joystickSensitivity = 160;
  Joystick() {x = 0; x = 0; touch = 0;}
  Joystick(int x, int y, int touch) { this->x = x; this->y = y, this->touch = touch; }
  Joystick& operator=(const Joystick& other) {
    x = other.x;
    y = other.y;
    touch = other.touch;
    return *this;
  }
};

class Snake {
private:
  class Point {
  public:
    int x;
    int y;
    Point() {x = 0; y = 0;}
    Point(int x, int y) { this->x = x; this->y = y;}
  };
  const int up = 0;
  const int down = 1;
  const int left = 2;
  const int right = 3;
  int gameOver = 0;
  int sizeMatrix = 8; 
  const int SpeedGame = 200;
  int directionSnake; 
  int initLenghtSnake = 3;
  int scorePlaer = 0;
  int lengthSnake = initLenghtSnake; 
  Point snakeHead;
  Point fruit;
  Joystick joystick;
  int** positions;  
  LedControl* matrix;
  void changePosition() {
    positions[snakeHead.x][snakeHead.y] = lengthSnake + 1;
      for(int i = 0; i < sizeMatrix; i++)
        for(int j = 0; j < sizeMatrix; j++){
          if(positions[i][j] > 0)
          positions[i][j]--;
        }
  }
  void moveSnake() {
    if(directionSnake == right) {
      snakeHead.y++;
      correctionOfCoordinates();
    } else if (directionSnake == left) {
      snakeHead.y--;
      correctionOfCoordinates();
    } else if (directionSnake == down) {
      snakeHead.x++;
      correctionOfCoordinates();
    } else if (directionSnake == up) {
      snakeHead.x--;
      correctionOfCoordinates();
    }
  }
  void displaySnakeWithFruit() {
    if((positions[snakeHead.x][snakeHead.y] > 0) && (directionSnake != -1)) {
      gameOver = 1;
      return;
    }
    if((snakeHead.x == fruit.x) && (snakeHead.y == fruit.y)) {
      lengthSnake++;
      fruit.x = -1;
      for(int i = 0; i < sizeMatrix; i++)
        for(int j = 0; j < sizeMatrix; j++){
          if(positions[i][j] > 0)
          positions[i][j]++;
        }
    }
    positions[snakeHead.x][snakeHead.y] = lengthSnake + 1;
    for(int i = 0; i < sizeMatrix; i++)
      for(int j = 0; j < sizeMatrix; j++){
        if(positions[i][j] > 0)
          positions[i][j]--;
        if(positions[i][j] > 0)
          matrix->setLed(0, i, j, true);
        else
          matrix->setLed(0, i, j, false);
    }
  }
  void correctionOfCoordinates() {
    if(snakeHead.x < 0)
      snakeHead.x = sizeMatrix - 1;
    if(snakeHead.x >= sizeMatrix)
      snakeHead.x = 0;
    if(snakeHead.y < 0)
      snakeHead.y = sizeMatrix - 1;
    if(snakeHead.y >= sizeMatrix)
      snakeHead.y = 0;
  }
  void newFruit() {
    if(lengthSnake == sizeMatrix * sizeMatrix) {
      gameOver = 2;
      return;
    }
    if(fruit.x == -1) {
      do {
        randomSeed(analogRead(A4));
        fruit.x = random(sizeMatrix);
        fruit.y = random(sizeMatrix);
      } while (positions[fruit.x][fruit.y] > 0); 
    }
  }
  void pause() {
    if(digitalRead(joystick.touch) == LOW) {
      delay(150);
      while(true) {
        if(digitalRead(joystick.touch) == LOW)
          break;
      }
      delay(150);
    }
  }
public:
  Snake(LedControl* matrix, int x, int y, int touch, int sizeMatrix) {
    randomSeed(analogRead(A4));
    snakeHead = Point(random(sizeMatrix), random(sizeMatrix));
    joystick = Joystick(x, y, touch);
    directionSnake = -1;
    lengthSnake = 1;
    this->matrix = matrix;
    fruit = Point(-1, -1);
    this->sizeMatrix = sizeMatrix;
    positions = new int*[sizeMatrix]();
    for(int i = 0; i < sizeMatrix; i++)
      positions[i] = new int[sizeMatrix]();
  }
  Snake(int x, int y, int touch, int sizeMatrix) {
    this->sizeMatrix = sizeMatrix;
    positions = new int*[sizeMatrix]();
    for(int i = 0; i < sizeMatrix; i++)
      positions[i] = new int[sizeMatrix]();
    snakeHead = Point(sizeMatrix - 1, sizeMatrix - 3);
    joystick = Joystick(x, y, touch);
    positions[sizeMatrix - 1][sizeMatrix - 1] = 1;
    positions[sizeMatrix - 1][sizeMatrix - 2] = 2;
    positions[sizeMatrix - 1][sizeMatrix - 3] = 3;
    directionSnake = -1;
    this->matrix = NULL;
  }
  ~Snake() {
    for(int i = 0; i < sizeMatrix; i++)
      delete[] positions[i];
    delete[] positions;
  }
  int getInitLenghtSnake(){ return initLenghtSnake;}
  int getLengthSnake(){ return lengthSnake;}
  int getScorePlaer(){ return scorePlaer;}
  void incrScorePlaer(){ scorePlaer++;}
  void stateWithFruit() {
    newFruit();
    if(gameOver == 2)
      return;
    int prevDir = directionSnake;
    long timeForSpeed = millis();
    while(millis() < timeForSpeed + SpeedGame) {// от этого цикла будет зависеть скорость игры, чем меньше SpeedGame, тем больше скорость игры {
      if(analogRead(joystick.x) < joystick.joystickBase - joystick.joystickSensitivity) {
        directionSnake = left;
      }
      if(analogRead(joystick.x) > joystick.joystickBase + joystick.joystickSensitivity) {
        directionSnake = right;
      }
      if(analogRead(joystick.y) < joystick.joystickBase - joystick.joystickSensitivity) {
        directionSnake = up;
      }
      if(analogRead(joystick.y) >joystick.joystickBase + joystick.joystickSensitivity) {
        directionSnake = down;
      }
      pause();
      randomSeed(analogRead(A4));
      if(random(2))
        matrix->setLed(0, fruit.x, fruit.y, 1);
      else
        matrix->setLed(0, fruit.x, fruit.y, 0);
    }
    if((prevDir == left && directionSnake == right) //если вдруг невозможные варианты (типо шла вверх, а тут вниз), то оставляем направление как есть
    ||(prevDir == right && directionSnake == left)
    ||(prevDir == down && directionSnake == up)
    ||(prevDir == up && directionSnake == down)) {
      directionSnake = prevDir;
    }
    moveSnake();
    displaySnakeWithFruit(); 
  }
  void state() {
    while(directionSnake == -1) {
      if(analogRead(joystick.x) < joystick.joystickBase - joystick.joystickSensitivity) {
        directionSnake = left;
        moveSnake();
        changePosition();
        return;
      }
      if(analogRead(joystick.y) < joystick.joystickBase - joystick.joystickSensitivity) {
        directionSnake = up;
        moveSnake(); 
        changePosition(); 
        return;
      }
    }
    long timeForSpeed = millis();
    int prevDir = directionSnake;
    while(millis() < timeForSpeed + SpeedGame) {
      if(analogRead(joystick.x) < joystick.joystickBase - joystick.joystickSensitivity) {
        directionSnake = left;
      }
      if(analogRead(joystick.x) > joystick.joystickBase + joystick.joystickSensitivity) {
        directionSnake = right;
      }
      if(analogRead(joystick.y) < joystick.joystickBase - joystick.joystickSensitivity) {
        directionSnake = up;
      }
      if(analogRead(joystick.y) > joystick.joystickBase + joystick.joystickSensitivity) {
        directionSnake = down;
      }
      pause();
    }
    if((prevDir == left && directionSnake == right)
    ||(prevDir == right && directionSnake == left)
    ||(prevDir == up && directionSnake == down)
    ||(prevDir == down && directionSnake == up)) {
      directionSnake = prevDir;
    }
    moveSnake();
    changePosition(); 
  }
  bool isOccupied(int x, int y) {
    return positions[x][y];
  }
  int getHeadX(){
    return snakeHead.x;
  }
  int getHeadY(){
    return snakeHead.y;
  }
  int isGameOver() {
    return gameOver;
  }
  void resetAll() { 
    directionSnake = -1;
    lengthSnake = 1;
    for(int i = 0; i < sizeMatrix; i++)
      for(int j = 0; j < sizeMatrix; j++)
        positions[i][j] = 0;
    randomSeed(analogRead(A4));
    snakeHead = Point(random(sizeMatrix), random(sizeMatrix));
    fruit = Point(-1, -1);
    gameOver = 0;
  }
  void resetForBattleMode() {
    for(int i = 0; i < sizeMatrix; i++)
      for(int j = 0; j < sizeMatrix; j++)
        positions[i][j] = 0;
    snakeHead = Point(sizeMatrix - 1, sizeMatrix - 3);
    positions[sizeMatrix - 1][sizeMatrix - 1] = 1;
    positions[sizeMatrix - 1][sizeMatrix - 2] = 2;
    positions[sizeMatrix - 1][sizeMatrix - 3] = 3;
    directionSnake = -1;
  }
};

namespace pin {
  const int CLK = 10;
  const int CS = 11;
  const int DIN = 12;

  const int X = A2;
  const int Y = A3;
  const int touch = 2;
}

int sizeDisplay = 8;
LedControl matrix(pin::DIN, pin::CLK, pin::CS, 1);
Snake sn(pin::X, pin::Y, pin::touch, sizeDisplay);
SnakeBot sb(sizeDisplay);
Snake s(&matrix, pin::X, pin::Y, pin::touch, sizeDisplay);

int messageSpeed = 20;
const PROGMEM bool winMessage[8][40] = {
{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
{0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 1, 1, 1, 0, 0, 1, 1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0},
{0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 1, 0, 0, 0, 1, 1, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0},
{0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 1, 0, 0, 0, 1, 1, 1, 1, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0},
{0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 1, 0, 1, 0, 0, 0, 0, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0},
{0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 1, 0, 1, 0, 0, 0, 0, 1, 1, 0, 0, 0, 1, 1, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0},
{0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 1, 0, 1, 0, 0, 0, 0, 1, 1, 0, 0, 0, 1, 1, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0},
{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 1, 1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0}
};


const PROGMEM bool loseMessage[8][43] = {
{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0},
{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 1, 1, 0, 0, 1, 1, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0},
{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0},
{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0},
{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0},
{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0},
{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0}
};


const PROGMEM bool snakeMessage[8][64] = {
{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 1, 1, 0, 0, 0, 1, 1, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 1, 1, 0, 0, 0, 1, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0},
{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 1, 1, 0, 0, 0, 0,
 
0, 0, 1, 1, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0},
{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 1, 1, 0, 1, 1, 1, 1, 0, 0, 1, 0, 0, 0, 1, 1, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 1, 0, 0, 1, 0, 0, 0, 1, 1, 0, 0, 1, 1, 0, 1, 1, 0, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0},
{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 1, 1, 0, 0, 0, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
};

const PROGMEM bool varGame1[8][8] = {
  {0, 0, 0, 0, 0, 0, 0, 0 },
  {0, 0, 0, 1, 1, 1, 0, 0 },
  {0, 0, 1, 1, 1, 1, 0, 0 },
  {0, 1, 1, 0, 1, 1, 0, 0 },
  {0, 0, 0, 0, 1, 1, 0, 0 },
  {0, 0, 0, 0, 1, 1, 0, 0 },
  {0, 0, 0, 0, 1, 1, 0, 0 },
  {0, 0, 0, 0, 1, 1, 0, 0 }
};

const PROGMEM bool varGame2[8][8] = {
  {0, 0, 0, 0, 0, 0, 0, 0 },
  {0, 0, 0, 1, 1, 1, 0, 0 },
  {0, 0, 1, 1, 0, 1, 1, 0 },
  {0, 0, 0, 0, 0, 1, 1, 0 },
  {0, 0, 0, 0, 1, 1, 0, 0 },
  {0, 0, 0, 1, 1, 0, 0, 0 },
  {0, 0, 1, 1, 0, 0, 0, 0 },
  {0, 0, 1, 1, 1, 1, 1, 0 }
};

const PROGMEM bool varGame3[8][8] = {
  {0, 0, 0, 0, 0, 0, 0, 0 },
  {0, 0, 1, 1, 1, 1, 0, 0 },
  {0, 1, 0, 1, 1, 0, 1, 0 },
  {1, 0, 0, 1, 1, 0, 0, 1 },
  {1, 0, 0, 1, 1, 0, 0, 1 },
  {1, 0, 0, 0, 0, 0, 0, 1 },
  {0, 1, 0, 0, 0, 0, 1, 0 },
  {0, 0, 1, 1, 1, 1, 0, 0 }
};

const PROGMEM bool bestMessage[8][58] = {
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 1, 0, 0, 0, 0, 1, 1, 0, 0, 0, 1, 0, 1, 1, 0, 0, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 0, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 1, 0, 0, 0, 0, 1, 1, 0, 0, 0, 1, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 1, 0, 0, 0, 1, 1, 0, 0, 1, 1, 0, 1, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0}
};


const PROGMEM bool scoreMessage[8][58] = {
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 0, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 0, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 1, 1, 0, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
};

const PROGMEM bool digits[][8][8] = {
  {
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 1, 1, 1, 1, 0, 0},
    {0, 1, 1, 0, 0, 1, 1, 0},
    {0, 1, 1, 0, 1, 1, 1, 0},
    {0, 1, 1, 1, 0, 1, 1, 0},
    {0, 1, 1, 0, 0, 1, 1, 0},
    {0, 1, 1, 0, 0, 1, 1, 0},
    {0, 0, 1, 1, 1, 1, 0, 0}
  },
  {
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 1, 1, 1, 0, 0},
    {0, 0, 1, 1, 1, 1, 0, 0},
    {0, 1, 1, 0, 1, 1, 0, 0},
    {0, 0, 0, 0, 1, 1, 0, 0},
    {0, 0, 0, 0, 1, 1, 0, 0},
    {0, 0, 0, 0, 1, 1, 0, 0},
    {0, 0, 0, 0, 1, 1, 0, 0}
  },
  {
    {0, 0, 0, 0, 0, 0, 0, 0 },
    {0, 0, 0, 1, 1, 1, 0, 0 },
    {0, 0, 1, 1, 0, 1, 1, 0 },
    {0, 0, 0, 0, 0, 1, 1, 0 },
    {0, 0, 0, 0, 1, 1, 0, 0 },
    {0, 0, 0, 1, 1, 0, 0, 0 },
    {0, 0, 1, 1, 0, 0, 0, 0 },
    {0, 0, 1, 1, 1, 1, 1, 0 }
  },
  {
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 1, 1, 1, 1, 0, 0},
    {0, 1, 1, 0, 0, 1, 1, 0},
    {0, 0, 0, 0, 0, 1, 1, 0},
    {0, 0, 0, 1, 1, 1, 0, 0},
    {0, 0, 0, 0, 0, 1, 1, 0},
    {0, 1, 1, 0, 0, 1, 1, 0},
    {0, 0, 1, 1, 1, 1, 0, 0}
  },
  {
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 1, 1, 0, 0},
    {0, 0, 0, 1, 1, 1, 0, 0},
    {0, 0, 1, 0, 1, 1, 0, 0},
    {0, 1, 0, 0, 1, 1, 0, 0},
    {0, 1, 1, 1, 1, 1, 1, 0},
    {0, 0, 0, 0, 1, 1, 0, 0},
    {0, 0, 0, 0, 1, 1, 0, 0}
  },
  {
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 1, 1, 1, 1, 1, 1, 0},
    {0, 1, 1, 0, 0, 0, 0, 0},
    {0, 1, 1, 1, 1, 1, 0, 0},
    {0, 0, 0, 0, 0, 1, 1, 0},
    {0, 0, 0, 0, 0, 1, 1, 0},
    {0, 1, 1, 0, 0, 1, 1, 0},
    {0, 0, 1, 1, 1, 1, 0, 0}
  },
  {
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 1, 1, 1, 1, 0, 0},
    {0, 1, 1, 0, 0, 1, 1, 0},
    {0, 1, 1, 0, 0, 0, 0, 0},
    {0, 1, 1, 1, 1, 1, 0, 0},
    {0, 1, 1, 0, 0, 1, 1, 0},
    {0, 1, 1, 0, 0, 1, 1, 0},
    {0, 0, 1, 1, 1, 1, 0, 0}
  },
  {
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 1, 1, 1, 1, 1, 1, 0},
    {0, 1, 1, 0, 0, 1, 1, 0},
    {0, 0, 0, 0, 1, 1, 0, 0},
    {0, 0, 0, 0, 1, 1, 0, 0},
    {0, 0, 0, 1, 1, 0, 0, 0},
    {0, 0, 0, 1, 1, 0, 0, 0},
    {0, 0, 0, 1, 1, 0, 0, 0}
  },
  {
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 1, 1, 1, 1, 0, 0},
    {0, 1, 1, 0, 0, 1, 1, 0},
    {0, 1, 1, 0, 0, 1, 1, 0},
    {0, 0, 1, 1, 1, 1, 0, 0},
    {0, 1, 1, 0, 0, 1, 1, 0},
    {0, 1, 1, 0, 0, 1, 1, 0},
    {0, 0, 1, 1, 1, 1, 0, 0}
  },
  {
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 1, 1, 1, 1, 0, 0},
    {0, 1, 1, 0, 0, 1, 1, 0},
    {0, 1, 1, 0, 0, 1, 1, 0},
    {0, 0, 1, 1, 1, 1, 1, 0},
    {0, 0, 0, 0, 0, 1, 1, 0},
    {0, 1, 1, 0, 0, 1, 1, 0},
    {0, 0, 1, 1, 1, 1, 0, 0}
  }
};

void showWinMessage() {
  [&] {
        for (int d = 0; d < sizeof(winMessage[0]) ; d++) {
          for (int col = 0; col < 8; col++) {
            delay(messageSpeed);
            for (int row = 0; row < 8; row++) {
              matrix.setLed(0, row, col, pgm_read_byte(&(winMessage[row][col + d])));
            }
          }
        }
  }();
  matrix.clearDisplay(0);
}

void showLoseMessage() {
  [&] {
        for (int d = 0; d < sizeof(loseMessage[0]) ; d++) {
          for (int col = 0; col < 8; col++) {
            delay(messageSpeed);
            for (int row = 0; row < 8; row++) {
              matrix.setLed(0, row, col, pgm_read_byte(&(loseMessage[row][col + d])));
            }
          }
        }
  }();
  matrix.clearDisplay(0);
}

void showSnakeMessage() {
  [&] {
        for (int d = 0; d < sizeof(snakeMessage[0]) ; d++) {
          for (int col = 0; col < 8; col++) {
            delay(messageSpeed);
            for (int row = 0; row < 8; row++) {
              matrix.setLed(0, row, col, pgm_read_byte(&(snakeMessage[row][col + d])));
            }
          }
        }
  }();
  matrix.clearDisplay(0);
}

void showVarGame1() {
  [&] {
        for (int d = 0; d < sizeof(varGame1[0]) ; d++) {
          for (int col = 0; col < 8; col++) {
            for (int row = 0; row < 8; row++) {
              matrix.setLed(0, row, col, pgm_read_byte(&(varGame1[row][col])));
            }
          }
        }
  }();
}

void showVarGame2() {
  [&] {
        for (int d = 0; d < sizeof(varGame2[0]) ; d++) {
          for (int col = 0; col < 8; col++) {
            for (int row = 0; row < 8; row++) {
              matrix.setLed(0, row, col, pgm_read_byte(&(varGame2[row][col])));
            }
          }
        }
  }();
}

void showVarGame3() {
  [&] {
        for (int d = 0; d < sizeof(varGame3[0]) ; d++) {
          for (int col = 0; col < 8; col++) {
            for (int row = 0; row < 8; row++) {
              matrix.setLed(0, row, col, pgm_read_byte(&(varGame3[row][col])));
            }
          }
        }
  }();
}

int gameModeVar(){
  Joystick joysticks;
  joysticks = Joystick(A2, A3, 2);
  pinMode(2, HIGH);
  int var = 1 ;
  while(digitalRead(2) != LOW){
    if(analogRead(joysticks.x) < joysticks.joystickBase - joysticks.joystickSensitivity) {
      matrix.clearDisplay(0);
      showVarGame1();
      var = 1;
    }
    if(var == 2 && (analogRead(joysticks.x) > joysticks.joystickBase + joysticks.joystickSensitivity)) {
      matrix.clearDisplay(0);
      showVarGame3();
      var = 3;
    }
    if(var != 2 && var != 3 &&( analogRead(joysticks.x) > joysticks.joystickBase + joysticks.joystickSensitivity)) {
      matrix.clearDisplay(0);
      showVarGame2();
      var = 2;
    }
    if(var == 3 && (analogRead(joysticks.x) < joysticks.joystickBase - joysticks.joystickSensitivity)) {
      matrix.clearDisplay(0);
      showVarGame2();
      var = 2;
    }
  }
  matrix.clearDisplay(0);
  return var;
}

void showScoreMessage(int score) {
  if (score < 0 || score > 99) 
    return;
  int second = score % 10;
  int first = (score / 10) % 10;
  [&] {
        for (int d = 0; d < sizeof(scoreMessage[0]) + 2 * sizeof(digits[0][0]); d++) {
          for (int col = 0; col < 8; col++) {
            delay(20);
            for (int row = 0; row < 8; row++) {
              if (d <= sizeof(scoreMessage[0]) - 8) {
                matrix.setLed(0, row, col, pgm_read_byte(&(scoreMessage[row][col + d])));
              }
              int c = col + d - sizeof(scoreMessage[0]) + 6;
              if (score < 10) 
                c += 8;
              if (c >= 0 && c < 8) {
                if (first > 0) 
                  matrix.setLed(0, row, col, pgm_read_byte(&(digits[first][row][c])));
              } else {
                c -= 8;
                if (c >= 0 && c < 8) {
                  matrix.setLed(0, row, col, pgm_read_byte(&(digits[second][row][c])));
                }
              }
            }
          }
        }
  }();
  matrix.clearDisplay(0);
}

void showBestScoreMessage(int score) {
  if (score < 0 || score > 99) 
    return;
  int second = score % 10;
  int first = (score / 10) % 10;
  [&] {
        for (int d = 0; d < sizeof(bestMessage[0]) + 2 * sizeof(digits[0][0]); d++) {
          for (int col = 0; col < 8; col++) {
            delay(20);
            for (int row = 0; row < 8; row++) {
              if (d <= sizeof(bestMessage[0]) - 8) {
                matrix.setLed(0, row, col, pgm_read_byte(&(bestMessage[row][col + d])));
              }
              int c = col + d - sizeof(bestMessage[0]) + 6;
              if (score < 10) 
                c += 8;
              if (c >= 0 && c < 8) {
                if (first > 0) matrix.setLed(0, row, col, pgm_read_byte(&(digits[first][row][c])));
              } else {
                c -= 8;
                if (c >= 0 && c < 8) {  
                  matrix.setLed(0, row, col, pgm_read_byte(&(digits[second][row][c])));
                }
              }
            }
          }
        }
  }();
  matrix.clearDisplay(0);
}

void showScoreBotVsPlayerMessage(int scoreP, int scoreB){
  if (scoreP < 0 || scoreP > 99) 
    return;
  int secondP = scoreP % 10;
  int secondB = scoreB % 10;
  int first = (scoreP / 10) % 10;
  int two = (scoreB / 10) % 10;
  [&]{
      for (int d = 0; d < sizeof(digits[0][0]); d++) {
        for (int col = 0; col < 8; col++) {
          delay(50);
          for (int row = 0; row < 8; row++) {
            int c = col + d ;
              if (scoreP < 10) 
                c += 8;
              if (c >= 0 && c < 8) {
                if (first > 0) 
                  matrix.setLed(0, row, col, pgm_read_byte(&(digits[first][row][c])));
              } else {
                  c -= 8;
                  if (c >= 0 && c < 8) {
                    matrix.setLed(0, row, col, pgm_read_byte(&(digits[secondP][row][c])));
                  }
              }
          }
        }
      }
      for (int d = 0; d < sizeof(digits[0][0]); d++) {
        for (int col = 0; col < 8; col++) {
          delay(50);
          for (int row = 0; row < 8; row++) {
            int c = col + d ;
            if (scoreB < 10) 
              c += 8;
            if (c >= 0 && c < 8) {
              if (two > 0) 
                matrix.setLed(0, row, col, pgm_read_byte(&(digits[two][row][c]))); 
            } else {
              c -= 8;
              if (c >= 0 && c < 8) {
                matrix.setLed(0, row, col, pgm_read_byte(&(digits[secondB][row][c]))); // show always
              }
            }
          }
        }
      }
  }();
  matrix.clearDisplay(0);
}

void softReset() {
  asm volatile ("jmp 0");
}

void displayS(Snake &s, SnakeBot &sb) {
  for(int i = 0; i < sizeDisplay; i++)
    for(int j = 0; j < sizeDisplay; j++)
      if((s.isOccupied(i,j) != 0) || (sb.isOccupied(i,j) != 0))
        matrix.setLed(0, i, j, true);
      else
        matrix.setLed(0, i, j, false);
}

void setup() {
  Serial.begin(115200);
  pinMode(pin::touch, INPUT);
  digitalWrite(pin::touch, HIGH);
  while(digitalRead(pin::touch) == HIGH) {}
  matrix.shutdown(0, false);
  matrix.setIntensity(0, 1);
  matrix.clearDisplay(0);
  showSnakeMessage();
  matrix.clearDisplay(0);
  if(EEPROM.read(0) == 255){
    EEPROM.put(0,0);
  }
}

void loop() {
  if(gameMode == 0){
    showVarGame1();
    gameMode = gameModeVar();
  }
  if(gameMode == 1){
    s.stateWithFruit();
    if(s.isGameOver()) {
      for(int i = 0; i < sizeDisplay; i++)
        for(int j = 0; j < sizeDisplay; j++)
          matrix.setLed(0, i, j, true);
      int score = s.getLengthSnake() - s.getInitLenghtSnake();
      delay(1000);
      matrix.clearDisplay(0);
      showScoreMessage(score);
      matrix.clearDisplay(0);
      if(EEPROM.read(0) < score){
        EEPROM.put(0, score);
      }
      showBestScoreMessage(EEPROM.read(0));
      matrix.clearDisplay(0);
      s.resetAll();
      matrix.clearDisplay(0);
      gameMode =0 ;
    }
  }
  if(gameMode == 2){
    sn.state(); 
    sb.state();
    displayS(sn, sb);
    if(sn.isOccupied(sb.getHeadX(), sb.getHeadY())) {
      delay(1000);
      showWinMessage();
      sn.incrScorePlaer();
      showScoreBotVsPlayerMessage(sn.getScorePlaer(),sb.getScoreBot());
      matrix.clearDisplay(0);
      sn.resetForBattleMode();
      sb.reset();
      gameMode =0 ;
    }
    if(sb.isOccupied(sn.getHeadX(), sn.getHeadY())) {
      delay(1000);
      showLoseMessage();
      sb.incrScoreBot();
      showScoreBotVsPlayerMessage(sn.getScorePlaer(),sb.getScoreBot());
      matrix.clearDisplay(0);
      sn.resetForBattleMode();
      sb.reset();
      gameMode =0 ;
    }
  }
  if(gameMode == 3){
    delay(500);
    matrix.shutdown(0, true);
    while(digitalRead(pin::touch) == HIGH) {}
    matrix.shutdown(0, false);
    softReset();
  }
}
