#include <AFMotor.h>

// Aviso! Não usar o servo 2, porque o pino 10 que precisa pra usar o servo 2 está sendo usado pra controlar um dos sensores ultrassônicos

// Usar as funções de calibração pra medir e calcular os valores
const int TIME_PER_DEGREE = 4000;
const int TIME_PER_CM = 40; 

class IRSensor {
  private:
    int pin;

  public:
    // Construtor
    IRSensor(int pin) : pin(pin) {
      pinMode(pin, INPUT);
    }

    // Métodos
    int detectLine() {
      return digitalRead(pin);
    }
};

class UltrasonicSensor {
  private:
    int trigPin;
    int echoPin;

  public:
    // Construtor
    UltrasonicSensor(int trigPin, int echoPin) : trigPin(trigPin), echoPin(echoPin) {
      pinMode(trigPin, OUTPUT);
      pinMode(echoPin, INPUT);
    }

    // Métodos
    long measureDistance() {
      digitalWrite(trigPin, LOW);
      delayMicroseconds(2);
      digitalWrite(trigPin, HIGH);
      delayMicroseconds(10);
      digitalWrite(trigPin, LOW);

      long duration = pulseIn(echoPin, HIGH);
      long distance = duration * 0.034 / 2;
      return distance;
    }
};

class Wheel {
  private:
    AF_DCMotor *motor;
    int currentSpeed;
    int targetSpeed;
    unsigned long lastStepTime;
    int stepDelay;

  public:
    // Construtor
    Wheel(AF_DCMotor *motor) : motor(motor) {
      currentSpeed = 0;
      targetSpeed = 0;
      lastStepTime = 0;
      stepDelay = 10;
    }

    // Métodos
    int getSpeed() { return currentSpeed; }

    void setTargetSpeed(int target, int delay = 10) {
      targetSpeed = target;
      stepDelay = delay;
    }

    bool update() {
      if (currentSpeed == targetSpeed) return true;

      // Verifica se passou tempo o suficiente pra ele atualizar
      if (millis() - lastStepTime >= stepDelay) {
        if (targetSpeed > currentSpeed)
          currentSpeed++;  // Acelera
        else
          currentSpeed--; // Desacelera
        
        motor->setSpeed(abs(currentSpeed));  // Atualiza velocidade do motor
        
        // Diz pro motor que direção girar ou se parar
        if (currentSpeed > 0)
          motor->run(FORWARD);
        else if (currentSpeed < 0)
          motor->run(BACKWARD);
        else
          motor->run(RELEASE);
        
        lastStepTime = millis();  // Marca o tempo da última atualização
      }

      return currentSpeed == targetSpeed;
    }
};

// Sensores IR
IRSensor irR(A1);  // Right
IRSensor irC(A0);  // Center
IRSensor irL(A2);  // Left

// Sensores Ultrassônicos
UltrasonicSensor usL(A3, A4);   // Left
UltrasonicSensor usF(A5, 2);    // Front
UltrasonicSensor usR(13, 10);   // Right

// Motores
AF_DCMotor motorL(3);
AF_DCMotor motorR(2);

// Rodas
Wheel wheelL(&motorL);
Wheel wheelR(&motorR);

// Funções pra movimentar o robô
void accelerate(int targetSpeed, int stepDelay = 10) {
  wheelR.setTargetSpeed(targetSpeed, stepDelay);
  wheelL.setTargetSpeed(targetSpeed, stepDelay);

  while (!wheelR.update() | !wheelL.update()) { }
}

void brake(int stepDelay = 10) {
  wheelR.setTargetSpeed(0, stepDelay);
  wheelL.setTargetSpeed(0, stepDelay);

  while (!wheelR.update() | !wheelL.update()) { }
}

// Funções pra calibração
int calibrateAngularMovement(int speed, bool clockwise, int testTime = 2000) {
  // ajuste o delay até girar aproximadamente 360° (calibração)
  int t = 2000;

  if (clockwise) {
    wheelL.setTargetSpeed(speed);
    wheelR.setTargetSpeed(-speed);
  } else {
    wheelL.setTargetSpeed(-speed);
    wheelR.setTargetSpeed(speed);
  }

  while (!wheelR.update() | !wheelL.update()) { }

  Serial.print("Girando por ");
  Serial.print(testTime);
  Serial.println("ms...");

  delay(testTime); 

  brake();

  Serial.println("Giro concluído!");
}

void calibrateLinearMovement(int speed, int testTime = 2000) {
  Serial.println("Indo pra frente...");
  accelerate(speed);
  delay(testTime);
  brake();
  delay(500);

  Serial.println("Voltando...");
  accelerate(-speed);
  delay(testTime);
  brake();
  delay(500);

  Serial.println("Ida e volta concluída...");
}

// Funções de teste

void testSensors() {
  Serial.println("Testando sensores:");

  // Imprime cabeçalhos
  Serial.println("      L     C     R");

  // Imprime IR
  Serial.print("IR  ");
  Serial.print(irL.detectLine()); Serial.print("     ");
  Serial.print(irC.detectLine()); Serial.print("     ");
  Serial.println(irR.detectLine());

  // Imprime US
  Serial.print("US  ");
  Serial.print(usL.measureDistance()); Serial.print("cm  ");
  Serial.print(usF.measureDistance()); Serial.print("cm");
  Serial.print(usR.measureDistance()); Serial.println("cm  ");

  Serial.println("---------------------");

  delay(500);
}

void testMotors() {
  Serial.println("Testando motores: movimento para frente...");
  
  accelerate(255);
  delay(2000);

  Serial.println("Testando motores: parando...");

  brake();
  delay(1000);
  
  Serial.println("Testando motores: movimento para trás...");

  accelerate(-255);
  delay(2000);

  Serial.println("Testando motores: parando...");

  brake();
  delay(1000);

  Serial.println("Teste completo!");
}

void setup() {
  Serial.begin(9600);
  Serial.println("INICIALIZAÇÃO COMPLETA!");
}

void loop() {
  testMotors();
}
