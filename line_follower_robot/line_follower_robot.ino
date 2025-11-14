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
  delay(750);
  
  Serial.println("Dando a volta no obstaculo...");
  motorR.setSpeed(speed);
  motorL.setSpeed(speed);
  motorR.run(FORWARD);
  motorL.run(BACKWARD);
  delay(750);

  motorR.setSpeed(speed);
  motorL.setSpeed(speed);
  motorR.run(FORWARD);
  motorL.run(FORWARD);
  delay(1250);

  Serial.println("Voltando pra linha...");
  motorR.setSpeed(speed);
  motorL.setSpeed(speed);
  motorR.run(BACKWARD);
  motorL.run(FORWARD);
  delay(1700);

  motorR.setSpeed(speed);
  motorL.setSpeed(speed);
  motorR.run(FORWARD);
  motorL.run(FORWARD);
}

void goAroundObstacle() {
  unsigned int frontDist = usF.read();
  unsigned int rightDist = usR.read();
  
  const int TARGET_DISTANCE = 20;
  const int SAFE_FRONT = 30;
  
  // Obstáculo na frente - vira esquerda
  if(frontDist < SAFE_FRONT) {
    Serial.println("Obstáculo na frente");
    motorR.setSpeed(speed);
    motorL.setSpeed(speed);
    motorR.run(FORWARD);
    motorL.run(BACKWARD);
  }
  // Muito perto - afasta
  else if(rightDist < TARGET_DISTANCE - 5) {
    Serial.println("Afasta");
    motorR.setSpeed(speed);
    motorL.setSpeed(speed * 0.6);
    motorR.run(FORWARD);
    motorL.run(BACKWARD);
  }
  // Muito longe - aproxima
  else if(rightDist > TARGET_DISTANCE + 5) {
    Serial.println("Aproxima");
    motorR.setSpeed(speed * 0.6);
    motorL.setSpeed(speed);
    motorR.run(BACKWARD);
    motorL.run(FORWARD);
  }
  // Distância boa - reto
  else {
    Serial.println("Em frente");
    motorR.setSpeed(speed);
    motorL.setSpeed(speed);
    motorR.run(FORWARD);
    motorL.run(FORWARD);
  }
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

      //Serial.println(distance);

      if(distance > 5 && distance < 15) {
        Serial.println("Obstaculo detectado!");
        currentState = AVOIDING_OBSTACLE;
        break;
      }

      followLine();
      break;
    case AVOIDING_OBSTACLE:
      //Serial.println("Desviando do obstáculo...");
      goAroundObstacle();
      if(usR.read() > 50 && (digitalRead(irL) || digitalRead(irC) || digitalRead(irR))) {
        Serial.println("Linha recuperada!");
        currentState = FOLLOWING_LINE;
      }
      break;
  }
}
