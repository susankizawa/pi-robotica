#include <AFMotor.h>

// Aviso! Não usar o servo 2, porque o pino 10 que precisa pra usar o servo 2 está sendo usado pra controlar um dos sensores ultrassônicos

class IRSensor {
  private:
    int _pin;

  public:
    // Construtor
    IRSensor(int pin) : _pin(pin) {
      pinMode(_pin, INPUT);
    }

    // Métodos
    int detectLine() {
      return digitalRead(_pin);
    }
};

class UltrasonicSensor {
  private:
    int _trigPin;
    int _echoPin;

  public:
    // Construtor
    UltrasonicSensor(int trigPin, int echoPin) : _trigPin(trigPin), _echoPin(echoPin) {
      pinMode(_trigPin, OUTPUT);
      pinMode(_echoPin, INPUT);
    }

    // Métodos
    long measureDistance() {
      digitalWrite(_trigPin, LOW);
      delayMicroseconds(2);
      digitalWrite(_trigPin, HIGH);
      delayMicroseconds(10);
      digitalWrite(_trigPin, LOW);

      long duration = pulseIn(_echoPin, HIGH);
      long distance = duration * 0.034 / 2;
      return distance;
    }
};

class Wheel {
  private:
    AF_DCMotor *motor;
    int _currentSpeed;
    int _targetSpeed;
    unsigned long _lastUpdateTime;
    int _step;
    int _timeDelay;

  public:
    // Construtor
    Wheel(AF_DCMotor *motor) : motor(motor) {
      _currentSpeed = 0;
      _targetSpeed = 0;
      _lastUpdateTime = 0;
      _step = 3;
      _timeDelay = 1;
    }

    // Métodos
    int getSpeed() { return _currentSpeed; }

    void setTargetSpeed(int target) {
      _targetSpeed = target;
    }

    void setStep(int step) {
      _step = step;
    }

    void setTimeDelay(int timeDelay) {
      _timeDelay = timeDelay;
    }

    bool update() {
      if (_currentSpeed == _targetSpeed) return true;

      // Verifica se passou tempo o suficiente pra ele atualizar
      if (millis() - _lastUpdateTime >= _timeDelay) {
        if (_targetSpeed > _currentSpeed)
          _currentSpeed = min(_currentSpeed + _step, _targetSpeed);  // Acelera
        else
          _currentSpeed = max(_currentSpeed - _step, _targetSpeed); // Desacelera

        if(_currentSpeed > 255) {
          _currentSpeed = 255;
        } else if(_currentSpeed < -255) {
          _currentSpeed = -255;
        }

        motor->setSpeed(abs(_currentSpeed));  // Atualiza velocidade do motor

        // Diz pro motor que direção girar ou se parar
        if (_currentSpeed > 0)
          motor->run(FORWARD);
        else if (_currentSpeed < 0)
          motor->run(BACKWARD);
        else
          motor->run(RELEASE);
        
        _lastUpdateTime = millis();  // Marca o tempo da última atualização
      }

      return _currentSpeed == _targetSpeed;
    }
};

enum State {
  IDLE,
  MOVING,
  FOLLOWING_LINE,
  AVOIDING_OBSTACLE,
  TURNING
};

// Usar as funções de calibração pra medir e calcular os valores
const int TIME_PER_DEGREE = 4000;
const int TIME_PER_CM = 40; 

const int speed = 200;
const int turningSpeed = 255;

State currentState = IDLE;
bool obstacleDetected;


// Sensores IR
IRSensor irR(A1);  // Right
IRSensor irC(A0);  // Center
IRSensor irL(A2);  // Left

// Sensores Ultrassônicos
UltrasonicSensor usL(A3, A4);   // Left
UltrasonicSensor usF(A5, 2);    // Front
UltrasonicSensor usR(13, 10);   // Right

// Motores
AF_DCMotor motorL(1, MOTOR12_1KHZ);
AF_DCMotor motorR(2, MOTOR12_1KHZ);

// Rodas
Wheel wheelL(&motorL);
Wheel wheelR(&motorR);

// Funções pra movimentar o robô
void accelerate(int targetSpeed) {
  wheelR.setTargetSpeed(targetSpeed);
  wheelL.setTargetSpeed(targetSpeed);

  while (!wheelR.update() | !wheelL.update()) { }
}

void brake() {
  wheelR.setTargetSpeed(0);
  wheelL.setTargetSpeed(0);

  while (!wheelR.update() | !wheelL.update()) { }
}

void turnAround(int targetSpeed) {
  wheelL.setTargetSpeed(targetSpeed);
  wheelR.setTargetSpeed(0);

  while (!wheelR.update() | !wheelL.update()) { }
}

// Funções pra calibração
int calibrateAngularMovement(int speed, bool clockwise, int testTime = 2000) {
  // ajuste o delay até girar aproximadamente 360° (calibração)
  int t = 2000;

  if (clockwise) {
    wheelL.setTargetSpeed(speed);
  } else {
    wheelL.setTargetSpeed(-speed);
  }
  wheelR.setTargetSpeed(0);

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

void testMotors(int speed, int timeDelay) {
  Serial.println("Testando motores: movimento para frente...");
  
  accelerate(speed);
  delay(timeDelay);

  Serial.println("Testando motores: parando...");

  brake();
  delay(1000);
  
  Serial.println("Testando motores: movimento para trás...");

  accelerate(-speed);
  delay(timeDelay);

  Serial.println("Testando motores: parando...");

  brake();
  delay(1000);

  Serial.println("Teste completo!");
}

void setup() {
  Serial.begin(9600);
  Serial.println("INICIALIZAÇÃO COMPLETA!");
  currentState = MOVING;
}

void loop() {
  testSensors();

  obstacleDetected = usF.measureDistance() < 60;

  switch(currentState) {
    case IDLE:
      // espera comando
      break;
      
    case MOVING:
      if (obstacleDetected) {
        Serial.println("Obstáculo detectado! Parando...");
        currentState = AVOIDING_OBSTACLE;
        break;
      }

      Serial.println("Nenhum obstáculo detectado. Avançando...");
      accelerate(speed);

    case FOLLOWING_LINE:
      break;

    case AVOIDING_OBSTACLE:
      brake();
      accelerate(-speed);
      delay(1000);
      turnAround(turningSpeed);
      delay(1000);
      brake();
      currentState = MOVING;
      break;
  }
}
