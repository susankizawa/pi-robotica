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
    bool detectLine() {
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
    bool _needsKick;

  public:
    // Construtor
    Wheel(AF_DCMotor *motor) : motor(motor) {
      _currentSpeed = 0;
      _targetSpeed = 0;
      _lastUpdateTime = 0;
      _step = 3;
      _timeDelay = 1;
      _needsKick = false;
    }

    // Métodos
    int getSpeed() { return _currentSpeed; }

    void setTargetSpeed(int target) {
      if (_currentSpeed == 0 && target != 0) {
        _needsKick = true;
      }
      _targetSpeed = target;
    }

    void setStep(int step) {
      _step = step;
    }

    void setTimeDelay(int timeDelay) {
      _timeDelay = timeDelay;
    }

    bool update() {
      if (_needsKick) {
        int kickSpeed = (_targetSpeed > 0) ? 255 : -255;  // Velocidade máxima pro kick
        motor->setSpeed(abs(kickSpeed));
        motor->run((kickSpeed > 0) ? FORWARD : BACKWARD);
        delay(250);  // Kick de 150ms (ajuste esse valor!)
        _needsKick = false;
        _currentSpeed = kickSpeed * 0.6;  // Começa em ~60% da velocidade depois do kick
        _lastUpdateTime = millis();
      }

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
  FOLLOWING_LINE,
  AVOIDING_OBSTACLE,
  TEST_TURN,
  TEST_STRAIGHT
};

enum LastDirection {
  NONE,
  LAST_LEFT,
  LAST_RIGHT
};

// Usar as funções de calibração pra medir e calcular os valores
const int TIME_PER_DEGREE = 4000;
const int TIME_PER_CM = 40; 
const float FACTOR_SMOOTH = 0.6;
const float FACTOR_SHARP = -1;
const float MIN_OBSTACLE_DISTANCE = 60;

State currentState = IDLE;
LastDirection lastCorrection = NONE;

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
}

void brake() {
  wheelR.setTargetSpeed(0);
  wheelL.setTargetSpeed(0);
}

void turnLeftSharp(int targetSpeed) {
  wheelL.setTargetSpeed(targetSpeed * FACTOR_SHARP);
  wheelR.setTargetSpeed(targetSpeed);
}

void turnLeftSmooth(int speed) {
  wheelL.setTargetSpeed(speed * FACTOR_SMOOTH);
  wheelR.setTargetSpeed(speed);
}

void turnRightSharp(int targetSpeed) {
  wheelL.setTargetSpeed(targetSpeed);
  wheelR.setTargetSpeed(targetSpeed * FACTOR_SHARP);
}

void turnRightSmooth(int speed) {
  wheelL.setTargetSpeed(speed);
  wheelR.setTargetSpeed(speed * FACTOR_SMOOTH); 
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
  currentState = TEST_STRAIGHT;
}

void loop() {
  // testSensors();
  wheelL.update();
  wheelR.update();
  
  bool L, C, R, obstacleDetected;
  int speed = 150;
  static unsigned long testStartTime = 0;
  static unsigned long elapsed;
  static bool goingForward = true;
  static bool clockwise = true;

  switch(currentState) {
    case IDLE:
      //L = irL.detectLine();
      //C = irC.detectLine();
      //R = irR.detectLine();

      //if (L || C || R) {
      //  currentState = FOLLOWING_LINE;
      //}
      break;
    
    case FOLLOWING_LINE:
      L = irL.detectLine();
      C = irC.detectLine();
      R = irR.detectLine();
      obstacleDetected = usF.measureDistance() < MIN_OBSTACLE_DISTANCE;

      if (obstacleDetected) {
        //Serial.println("Obstáculo detectado!");
        brake();
        currentState = AVOIDING_OBSTACLE;
        break;
      }

      if (C) {
        // 1. Linha Centralizada (ou centralizada com leve desvio)
        if (L && !R) { 
            // Linha levemente à esquerda. Vira para a direita (SUAVE)
            turnRightSmooth(speed);
            lastCorrection = LAST_RIGHT; 
        } else if (!L && R) { 
            // Linha levemente à direita. Vira para a esquerda (SUAVE)
            turnLeftSmooth(speed);
            lastCorrection = LAST_LEFT; 
        } else { // (F, T, F) ou (T, T, T) - (F, T, T) já foi pego
            // Perfeito: Linha no centro.
            accelerate(speed);
        } 
      } else if (!C && (L || R)) {
        // 2. Linha fora do centro (correção ACENTUADA)
        if (L && !R) { // (T, F, F)
            // Linha totalmente à esquerda. Vira para a direita (ACENTUADA)
            turnRightSharp(speed); 
            lastCorrection = LAST_RIGHT; 
        } else if (!L && R) { // (F, F, T)
            // Linha totalmente à direita. Vira para a esquerda (ACENTUADA)
            turnLeftSharp(speed);
            lastCorrection = LAST_LEFT; 
        } 
      } else { // (!L && !C && !R) -- 3. Linha Perdida  
        // Rotina de BUSCA (usa o giro acentuado na última direção conhecida)
        if (lastCorrection == LAST_RIGHT) {
            turnLeftSharp(speed); // Gira para a esquerda bruscamente (Busca)
        } else if (lastCorrection == LAST_LEFT) {
            turnRightSharp(speed); // Gira para a direita bruscamente (Busca)
        } else {
            // Se for a primeira vez que perde a linha, gire em uma direção padrão (ex: direita)
            turnRightSharp(speed);
        }
      }
      break;

    case AVOIDING_OBSTACLE:
      if (!obstacleDetected) {
        currentState = FOLLOWING_LINE;
        break;
      }
      break;
    case TEST_TURN:
      // Inicializa o timer na primeira execução
      if (testStartTime == 0) {
        testStartTime = millis();
        turnRightSharp(speed);
      }
      
      elapsed = millis() - testStartTime;
      
      if (clockwise && elapsed >= 10000) {
        // Passou 2s indo pra frente, inverte
        turnLeftSharp(speed);
        clockwise = false;
        testStartTime = millis(); // Reseta o timer
      } else if (!clockwise && elapsed >= 10000) {
        // Passou 2s indo pra trás, terminou o teste
        brake();
        clockwise = true; // Reset pro próximo teste
        testStartTime = 0;
        // Opcional: muda de estado aqui se quiser
        currentState = IDLE;
      }
      
      break;
    case TEST_STRAIGHT:
      // Inicializa o timer na primeira execução
      if (testStartTime == 0) {
        testStartTime = millis();
        accelerate(speed);
      }
      
      elapsed = millis() - testStartTime;
      
      if (goingForward && elapsed >= 2000) {
        // Passou 2s indo pra frente, inverte
        accelerate(-speed);
        goingForward = false;
        testStartTime = millis(); // Reseta o timer
      } else if (!goingForward && elapsed >= 2000) {
        // Passou 2s indo pra trás, terminou o teste
        brake();
        goingForward = true; // Reset pro próximo teste
        testStartTime = 0;
        // Opcional: muda de estado aqui se quiser
        currentState = TEST_TURN;
      }
      break;
  }
}
