#include <AFMotor.h>
#include <Servo.h>
// =====================MOTOR=====================
AF_DCMotor motorKiri(4);
AF_DCMotor motorKanan(3);
// ========== HC-SR04/ =====================
#define TRIG A1
#define ECHO A2
// =====================// LINE FOLLOWER// =====================
#define LINE_KIRI  A3
#define LINE_KANAN A0
// =====================// SERVO// =====================
Servo servoSensor;
// =====================
void setup() {

  Serial.begin(9600);
  motorKiri.setSpeed(200);
  motorKanan.setSpeed(200);
  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);
  pinMode(LINE_KIRI, INPUT);
  pinMode(LINE_KANAN, INPUT);
  servoSensor.attach(10);
  servoSensor.write(90);
  delay(1000);
}
void loop() {
  // =====================// CEK HALANGAN// =====================
  int jarak = bacaJarak();
  if (jarak <= 20 && jarak > 0) {
    berhenti();
    delay(300);
    // Cek kiri
    servoSensor.write(150);
    delay(500);
    int kiri = bacaJarak();
    // Cek kanan
    servoSensor.write(30);
    delay(500);
    int kanan = bacaJarak();
    // Kembali ke tengah
    servoSensor.write(90);
    delay(300);
    // Kecepatan saat menghindari halangan
    motorKiri.setSpeed(150);
    motorKanan.setSpeed(150);
    if (kiri > kanan) {

      belokKiri();
      delay(700);
    } else {
      belokKanan();
      delay(700);
    }
    return;
  }
  // =====================LINE FOLLOWER// =====================
  int kiriLine = digitalRead(LINE_KIRI);
  int kananLine = digitalRead(LINE_KANAN);
  // PUTIH = LOW
  // HITAM = HIGH
  if (kiriLine == HIGH && kananLine == HIGH) {
    // Lurus
    motorKiri.setSpeed(200);
    motorKanan.setSpeed(200);
    maju();
  }
  else if (kiriLine == HIGH && kananLine == LOW) {
    // Belok kiri
    motorKiri.setSpeed(150);
    motorKanan.setSpeed(150);

    belokKiri();
    delay(100);
  }
  else if (kiriLine == LOW && kananLine == HIGH) {
    // Belok kanan
    motorKiri.setSpeed(150);
    motorKanan.setSpeed(150);

    belokKanan();
    delay(100);
  }
  else {
    // Tidak ada garis
    motorKiri.setSpeed(200);
    motorKanan.setSpeed(200);
    maju();
  }
}
// =====================
int bacaJarak() {
  digitalWrite(TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG, LOW);
  long durasi = pulseIn(ECHO, HIGH, 30000);

  if (durasi == 0) {
    return 999;
  }
  return durasi * 0.034 / 2;
}
// =====================
void maju() {
  motorKiri.run(FORWARD);
  motorKanan.run(FORWARD);
}
// =====================
void mundur() {
  motorKiri.run(BACKWARD);
  motorKanan.run(BACKWARD);
}
// =====================
void berhenti() {
  motorKiri.run(RELEASE);
  motorKanan.run(RELEASE);
}
// =====================
void belokKiri() {

  motorKiri.run(RELEASE);
  motorKanan.run(FORWARD);
}
// =====================
void belokKanan() {
  motorKiri.run(FORWARD);
  motorKanan.run(RELEASE);
}