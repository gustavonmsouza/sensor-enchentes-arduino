/*
Projeto: Sistema Anti-Enchente com Arduino
Disciplina: Trabalho Interdisciplinar I
Curso: Ciência da Computação - PUC Minas

Integrantes:
- Gustavo Norberto Medeiros de Souza
- Bernardo Rocha Ázara
- Eduardo Augusto Freitas Nogueira
- Pedro Gabriel Rezende de Freitas
*/

#include <Servo.h>

Servo comporta;

const int trigPin = 9;
const int echoPin = 8;

const int chuvaPin = A0;
const int calibPin = A2;

const int ledVermelho = 10;
const int ledAmarelo  = 11;
const int ledVerde    = 12;

const int buzzer = 13;
const int servoPin = 7;

int distancia = 0;
int nivelGrau = 0;

int mediaUltrassonico() {

  long soma = 0;

  for (int i = 0; i < 10; i++) {

    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);

    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);

    digitalWrite(trigPin, LOW);

    long duracao = pulseIn(echoPin, HIGH, 30000);

    if (duracao == 0) {
      continue;
    }

    int dist = duracao * 0.034 / 2;

    soma += dist;

    delay(10);
  }

  return soma / 10;
}

void setup() {

  Serial.begin(9600);

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  pinMode(ledVerde, OUTPUT);
  pinMode(ledAmarelo, OUTPUT);
  pinMode(ledVermelho, OUTPUT);

  pinMode(buzzer, OUTPUT);

  comporta.attach(servoPin);

  comporta.write(0);
}

void loop() {

  distancia = mediaUltrassonico();

  int chuva = analogRead(chuvaPin);
  int calib = analogRead(calibPin);

  int limiteNormal  = map(calib, 0, 1023, 120, 200);
  int limiteAtencao = map(calib, 0, 1023, 60, 119);

  if (distancia > limiteNormal) {

    nivelGrau = 1;

  } 
  else if (distancia > limiteAtencao) {

    nivelGrau = 2;

  } 
  else {

    nivelGrau = 3;
  }

  digitalWrite(ledVerde, LOW);
  digitalWrite(ledAmarelo, LOW);
  digitalWrite(ledVermelho, LOW);

  noTone(buzzer);

  switch (nivelGrau) {

    case 1:

      digitalWrite(ledVerde, HIGH);

      comporta.write(0);

      break;

    case 2:

      digitalWrite(ledAmarelo, HIGH);

      comporta.write(90);

      break;

    case 3:

      digitalWrite(ledVermelho, HIGH);

      tone(buzzer, 1200);

      comporta.write(180);

      break;
  }

  Serial.print("Distancia: ");
  Serial.print(distancia);
  Serial.print(" cm");

  Serial.print(" | Chuva: ");
  Serial.print(chuva);

  Serial.print(" | Calibracao: ");
  Serial.print(calib);

  Serial.print(" | Nivel: ");

  if (nivelGrau == 1) {
    Serial.println("NORMAL");
  }
  else if (nivelGrau == 2) {
    Serial.println("ATENCAO");
  }
  else {
    Serial.println("PERIGO");
  }

  delay(300);
}