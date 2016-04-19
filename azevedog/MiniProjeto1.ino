#include<LiquidCrystal.h>

/* Constante para indicar que uma posicao precisa ser regerada */
const short INVALID = -1;

/* Botao direito */
const int RIGHT = 8;

/* Botao superior */
const int UP = 9;

/* Botao inferior */
const int DOWN = 10;

/* Botao esquerdo */
const int LEFT = 11;

/* (RS, E, D4, D5, D6, D7) */
LiquidCrystal lcd(2, 3, 4, 5, 6, 7);

/* Estrutura de posicao das partes da cobra */
struct position_ {
  short x;
  short y;
};
typedef struct position_ Position;

/* Posicao da comida no grid, a comida faz a cobra crescer */
Position food;

/* Posicao das partes da cobra no grid, como o grid tem 80 posicoes, esse eh o tamanho maximo da cobra */
Position snake[80];

/* Indice do rabo da cobra no vetor de posicoes */
short tail;

/* Indice da cabeca da cobra no vetor de posicoes */
short head;

/* Tamanho da cobra */
short sizeSnake;

/* Tempo da ultima atualizacao de tela */
long lastDrawTime;

/* Intervalo de atualizacoes de tela */
const short FRAME_RATE = 600;

/* Direcao/Sentido de movimentacao da cobra, utiliza as constantes dos botoes por praticidade */
short currentDirection = LEFT;

/* Ultimo botao apertado, proxima Direcao/Sentido de movimentacao da cobra */
short lastBtnClicked = LEFT;

/* Configura as portas e estruturas */
void setup() {
  pinMode(DOWN, INPUT);
  pinMode(RIGHT, INPUT);
  pinMode(LEFT, INPUT);
  pinMode(UP, INPUT);
  lcd.begin(20, 4);
  lcd.clear();
  runIntro();
  startSnake();
  startFood();
}

/* Funcao de desenho da introducao do jogo */
void runIntro() {
  Serial.begin(9600);
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

/* Define a posicao inical da cobra */
void startSnake() {
  int i;
  for (i = 0; i < 4; i++) {
    snake[i].x = 19 - i;
    snake[i].y = 0;
  }
  tail = 0;
  head = 3;
  sizeSnake = 4;
  drawSnake();
}

/* Define a posição inicial da comida */
void startFood() {
  randomSeed(analogRead(0));
  generateFoodPosition();
}

/* Desenha a mensagem de fim de jogo */
void gameOver() {
  lcd.setCursor(6, 1);//(x,y)
  lcd.print("GAME OVER");
  lcd.setCursor(5, 2);
  lcd.print("Press Reset");
  lcd.setCursor(4, 3);
  lcd.print("to try again");
  while (1);
}

void youWin() {
  lcd.setCursor(7, 1);//(x,y)
  lcd.print("YOU WIN");
  lcd.setCursor(5, 2);
  lcd.print(":) :) :) :)");
  while (1);
}

/* Gera uma coordenada x,y aleatoriamente no grid, para posicionar a comida */
void generateFoodPosition () {
  food.x = random(0, 20);
  food.y = random(0, 4);
}

/* Verifica se uma dada posicao eh valida*/
boolean isPositionValid(short x, short y) {
  if (x < 0 || x > 19 || y < 0 || y > 3) {
    return false;
  }

  short j = 0;
  short index = tail;
  while (j < (sizeSnake - 1)) {
    if ((snake[index].x == x) && (snake[index].y == y)) {
      return false;
    }
    index = (index + 1) % 80;
    j++;
  }
  return true;
}

/* Verifica se ha sinal dos botoes */
void btnPolling() {
  if (digitalRead(DOWN) == HIGH) {
    if (currentDirection != UP) {
      lastBtnClicked = DOWN;
    }
  }
  if (digitalRead(UP) == HIGH) {
    if (currentDirection != DOWN) {
      lastBtnClicked = UP;
    }
  }
  if (digitalRead(RIGHT) == HIGH) {
    if (currentDirection != LEFT) {
      lastBtnClicked = RIGHT;
    }
  }
  if (digitalRead(LEFT) == HIGH) {
    if (currentDirection != RIGHT) {
      lastBtnClicked = LEFT;
    }
  }
}

/* Atualiza a posicao da cobra no grid */
void walkSnake() {
  Position currentHead = snake[head];

  head = (head + 1) % 80;
  snake[head].x = currentHead.x;
  snake[head].y = currentHead.y;

  currentDirection = lastBtnClicked;
  switch (currentDirection) {
    case DOWN:
      snake[head].y = currentHead.y + 1;
      break;
    case UP:
      snake[head].y = currentHead.y - 1;
      break;
    case RIGHT:
      snake[head].x = currentHead.x + 1;
      break;
    case LEFT:
      snake[head].x = currentHead.x - 1;
      break;
  }
  if (!isPositionValid(snake[head].x, snake[head].y)) {
    gameOver();
  }else if ((snake[head].x == food.x) && (snake[head].y == food.y)) {
    food.x = INVALID;
    sizeSnake++;
    if (sizeSnake >= 80) {
      youWin();
    }
  } else {
    tail = (tail + 1) % 80;
  }
}

/* Desenha a comida */
void drawFood() {
  if (isPositionValid(food.x, food.y)) {
    lcd.setCursor(food.x, food.y);
    lcd.print("$");
  } else {
    food.x = INVALID;
  }
}

/* Desenha a cobra no grid */
void drawSnake() {
  short j = 0;
  short index = tail;
  lcd.clear();
  while (j < sizeSnake) {
    lcd.setCursor(snake[index].x, snake[index].y);
    lcd.print("*");
    index = (index + 1) % 80;
    j++;
  }
  lcd.setCursor(snake[head].x, snake[head].y);
  lcd.print("@");
  lastDrawTime = millis();
}

/* Laco de processamento */
void loop() {
  btnPolling();
  if ((millis() - lastDrawTime) > FRAME_RATE) {
    if (food.x == INVALID) {
      generateFoodPosition();
    }
    walkSnake();
    drawSnake();
    drawFood();
  }
}

