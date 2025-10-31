#include <AFMotor.h>

// Aviso! Não usar o servo 2, porque o pino 10 que precisa pra usar o servo 2 está sendo usado pra controlar um dos sensores ultrassônicos

// Struct para sensor IR
struct IRSensor {
  int pin;
};

// Struct para sensor Ultrassônico
struct UltrasonicSensor {
  int trigPin;
  int echoPin;
};

// Sensores IR
IRSensor IR_C = {A0};  // Center
IRSensor IR_R = {A1};  // Right
IRSensor IR_L = {A2};  // Left

// Sensores Ultrassônicos
UltrasonicSensor US_L = {A3, A4};   // Left
UltrasonicSensor US_M = {A5, 2};    // Middle
UltrasonicSensor US_R = {13, 10};   // Right

AF_DCMotor motorL(3); // Left
AF_DCMotor motorR(2); // Right

void initPins() {
  // IR como entrada
  pinMode(IR_C.pin, INPUT);
  pinMode(IR_R.pin, INPUT);
  pinMode(IR_L.pin, INPUT);

  // Ultrassônicos: trigger como saída, echo como entrada
  pinMode(US_L.trigPin, OUTPUT);
  pinMode(US_L.echoPin, INPUT);

  pinMode(US_M.trigPin, OUTPUT);
  pinMode(US_M.echoPin, INPUT);

  pinMode(US_R.trigPin, OUTPUT);
  pinMode(US_R.echoPin, INPUT);
}

void initMotors() {
  motorR.setSpeed(255);
  motorL.setSpeed(255);
}

long measureDistance(UltrasonicSensor sensor) {
  digitalWrite(sensor.trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(sensor.trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(sensor.trigPin, LOW);

  long duration = pulseIn(sensor.echoPin, HIGH);
  long distance = duration * 0.034 / 2;
  return distance;
}

int detectLine(IRSensor sensor) {
  return digitalRead(sensor.pin);
}

void testSensors() {
  // Leitura dos sensores IR
  int irL = detectLine(IR_L);
  int irR = detectLine(IR_R);
  int irC = detectLine(IR_C);

  // Leitura dos sensores ultrassônicos
  long usL = measureDistance(US_L);
  long usR = measureDistance(US_R);
  long usC = measureDistance(US_M);  // Middle

  // Imprime cabeçalhos
  Serial.println("      L     C     R");

  // Imprime IR
  Serial.print("IR  ");
  Serial.print(irL); Serial.print("     ");
  Serial.print(irC); Serial.print("     ");
  Serial.println(irR);

  // Imprime US
  Serial.print("US  ");
  Serial.print(usL); Serial.print("cm  ");
  Serial.print(usC); Serial.print("cm");
  Serial.print(usR); Serial.println("cm  ");

  Serial.println("---------------------");

  delay(500);
}

void testMotors() {
  //Serial.println("Testando motores: movimento para frente...");
  motorR.run(FORWARD);
  motorL.run(FORWARD);
  delay(500);

  /*
  Serial.println("Parando motores...");
  motorR.run(RELEASE);
  motorL.run(RELEASE);
  delay(5000);

  Serial.println("Testando motores: movimento para trás...");
  motorR.run(BACKWARD);
  motorL.run(BACKWARD);
  delay(10000);

  Serial.println("Parando motores...");
  motorR.run(RELEASE);
  motorL.run(RELEASE);
  delay(5000);
  */
}

void setup() {
  Serial.begin(9600);

  Serial.println("Inicializando pinos...");
  initPins();
  Serial.println("Pinos inicializados!");

  Serial.println("Inicializando motores...");
  initMotors();
  Serial.println("Motores inicializados!");

  Serial.println("INICIALIZAÇÃO COMPLETA!");
}

void loop() {
  testMotors();
}
