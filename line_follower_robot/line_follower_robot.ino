#include <Ultrasonic.h>
#include <AFMotor.h>

// Aviso! Não usar o servo 2, porque o pino 10 que precisa pra usar o servo 2 está sendo usado pra controlar um dos sensores ultrassônicos

enum State {
  FOLLOWING_LINE,
  AVOIDING_OBSTACLE
};

// Sensores IR
const int irR = A1;
const int irC = A0;
const int irL = A2;

// Sensores Ultrassônicos
const int usRTrig = 13;
const int usREcho = 10;

const int usFTrig = A5;
const int usFEcho = 2;

const int usLTrig = A3;
const int usLEcho = A4;

Ultrasonic usR(usRTrig, usREcho);
Ultrasonic usF(usFTrig, usFEcho);
Ultrasonic usL(usLTrig, usLEcho);

// Motores
AF_DCMotor motorL(1, MOTOR12_1KHZ);
AF_DCMotor motorR(2, MOTOR12_1KHZ);

const int speed = 150;
unsigned int distance;

State currentState;

void followLine() {
  bool right = digitalRead(irR);
  bool center = digitalRead(irC);
  bool left = digitalRead(irL);

  if(center && !right && !left) {
    // Linha no centro - vai reto
    motorR.setSpeed(speed);
    motorL.setSpeed(speed);
    motorR.run(FORWARD);
    motorL.run(FORWARD);
    
  } else if(center && left && !right) {
    // Linha levemente à esquerda - curva suave pra esquerda
    motorR.setSpeed(speed);
    motorL.setSpeed(speed * 0.6);  // Diminui roda esquerda
    motorR.run(FORWARD);
    motorL.run(FORWARD);
    
  } else if(center && right && !left) {
    // Linha levemente à direita - curva suave pra direita
    motorR.setSpeed(speed * 0.6);  // Diminui roda direita
    motorL.setSpeed(speed);
    motorR.run(FORWARD);
    motorL.run(FORWARD);
    
  } else if(left && !center && !right) {
    // Linha totalmente à esquerda - gira no lugar pra esquerda
    motorR.setSpeed(speed);
    motorL.setSpeed(speed);
    motorR.run(FORWARD);
    motorL.run(BACKWARD);
    
  } else if(right && !center && !left) {
    // Linha totalmente à direita - gira no lugar pra direita
    motorR.setSpeed(speed);
    motorL.setSpeed(speed);
    motorR.run(BACKWARD);
    motorL.run(FORWARD);
    
  } else if(right && center && left) {
    // Todos detectam (cruzamento ou linha muito larga) - para
    motorR.run(RELEASE);
    motorL.run(RELEASE);
  }
}

void avoidObstacle() {
  Serial.println("Dando ré...");
  motorR.setSpeed(speed);
  motorL.setSpeed(speed);
  motorR.run(BACKWARD);
  motorL.run(BACKWARD);
  delay(2000);
  Serial.println("Desviando do obstáculo...");
  motorR.setSpeed(speed);
  motorL.setSpeed(speed);
  motorR.run(FORWARD);
  motorL.run(BACKWARD);
  delay(1000);
  motorR.setSpeed(speed);
  motorL.setSpeed(speed);
  motorR.run(BACKWARD);
  motorL.run(FORWARD);
  delay(1000);
  Serial.println("Andando do lado do obstáculo...");
  motorR.setSpeed(speed);
  motorL.setSpeed(speed);
  motorR.run(FORWARD);
  motorL.run(FORWARD);
  delay(2000);
  Serial.println("Voltando pra linha...");
  motorR.setSpeed(speed);
  motorL.setSpeed(speed);
  motorR.run(BACKWARD);
  motorL.run(FORWARD);
  delay(1000);
  motorR.setSpeed(speed);
  motorL.setSpeed(speed);
  motorR.run(FORWARD);
  motorL.run(BACKWARD);
  delay(1000);
}

void setup() {
  Serial.begin(9600);
  pinMode(irR, INPUT);
  pinMode(irC, INPUT);
  pinMode(irL, INPUT);
  currentState = FOLLOWING_LINE;
  Serial.println("INICIALIZAÇÃO COMPLETA!");
}

void loop() {
  switch(currentState) {
    case FOLLOWING_LINE:      
      distance = usF.read();

      if(distance < 10) {
        Serial.println("Obstaculo detectado!");
        currentState = AVOIDING_OBSTACLE;
        break;
      }

      followLine();
      break;
    case AVOIDING_OBSTACLE:
      Serial.println("Desviando do obstáculo...");
      avoidObstacle();
      currentState = FOLLOWING_LINE;
      break;
  }
}
