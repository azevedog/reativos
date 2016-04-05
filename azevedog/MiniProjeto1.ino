#include<LiquidCrystal.h>

//rs, e, d4, d5, d6, d7
LiquidCrystal lcd(2, 3, 4, 5, 6, 7);

const int RIGHT = 8;
const int UP = 9;
const int LEFT = 10;
const int DOWN = 11;

struct position_ {
  short x;
  short y;
};
typedef struct position_ Position;
Position food;

struct listElem {
  Position* pos;
  char draw;
  struct listElem* prev;
  struct listElem* next;
};
typedef struct listElem List;
List* snake = NULL;




void runIntro() {
  lcd.setCursor(7, 1);//(x,y)
  lcd.print("EAMCEMP");
  lcd.setCursor(6, 2);
  lcd.print("PRESENTS");
  delay(1500);
  lcd.clear();
  lcd.setCursor(5, 1);
  lcd.print("SNAKEDUINO");
  delay(1500);
  lcd.clear();
}

void startSnake() {
  List* head = (List*) malloc(sizeof(List));
  head->pos = (Position*) malloc(sizeof(Position));
  head->pos->x = 16;
  head->pos->y = 0;
  head->prev = NULL;
  head->draw = '@';

  snake = head;

  List* b0 = (List*) malloc(sizeof(List));
  b0->pos = (Position*) malloc(sizeof(Position));
  b0->pos->x = 17;
  b0->pos->y = 0;
  b0->prev = head;
  b0->draw = '*';

  head->next = b0;

  List* b1 = (List*) malloc(sizeof(List));
  b1->pos = (Position*) malloc(sizeof(Position));
  b1->pos->x = 18;
  b1->pos->y = 0;
  b1->prev = b0;
  b1->draw = '*';

  b0->next = b1;

  List* b2 = (List*) malloc(sizeof(List));
  b2->pos = (Position*) malloc(sizeof(Position));
  b2->pos->x = 19;
  b2->pos->y = 0;
  b2->prev = b1;
  b2->next = NULL;
  b2->draw = '*';

  b1->next = b2;
}


void setup() {
  pinMode(DOWN, INPUT);
  pinMode(RIGHT, INPUT);
  pinMode(LEFT, INPUT);
  pinMode(UP, INPUT);
  lcd.begin(20, 4);
  lcd.clear();
  runIntro();
  startSnake();
}


void drawSnake() {
  List* current = snake;
  while (current != NULL) {
    lcd.setCursor(current->pos->x, current->pos->y);
    lcd.print(current->draw);
    current = current->next;
  }
  lcd.clear();
}

void loop() {
  List* current = snake;
  while (current != NULL) {
    lcd.setCursor(current->pos->x, current->pos->y);
    lcd.print(current->draw);
    current = current->next;
  }

}
