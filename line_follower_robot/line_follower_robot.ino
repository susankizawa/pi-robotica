#include <AFMotor.h>

// Aviso! Não usar o servo 2, porque o pino 10 que precisa pra usar o servo 2 está sendo usado pra controlar um dos sensores ultrassônicos

// Sensores IR
const int irR = A1;
const int irC = A0;
const int irL = A2;

// Sensores Ultrassônicos

// Motores
AF_DCMotor motorL(1, MOTOR12_1KHZ);
AF_DCMotor motorR(2, MOTOR12_1KHZ);

const int speed = 150;

void setup() {
  Serial.begin(9600);
  pinMode(irR, INPUT);
  pinMode(irC, INPUT);
  pinMode(irL, INPUT);
  Serial.println("INICIALIZAÇÃO COMPLETA!");
}

void loop() {
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
