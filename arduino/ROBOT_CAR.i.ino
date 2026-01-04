//Right Motor (A)
#define AIN1 9  //Right Motor Forward
#define AIN2 8  //Right Motor Backward
#define PWMA 3  //Right Motor Speed

//Left Motor (B)
#define BIN1 11  //Left Motor Forward
#define BIN2 12  //Left Motor Backward
#define PWMB 5   //Left Motor Speed


// UltraSonic Sensor 1
const uint8_t TRIG1 = 7;
const uint8_t ECHO1 = 6;

// UltraSonic Sensor 2
const uint8_t TRIG2 = 13;
const uint8_t ECHO2 = 10;

String data = "";

void FORWARD();
void BACKWARD();
void TURN_RIGHT();
void TURN_LEFT();
void STOP();




struct Ultrasonic {
  uint8_t trig;
  uint8_t echo;

  enum State { IDLE,
               TRIG_HIGH,
               WAIT_ECHO_HIGH,
               WAIT_ECHO_LOW } state;

  unsigned long tStart;
  unsigned long echoStart;
  unsigned long duration;

  float distanceCm;
  bool ready;
};
Ultrasonic sensors[2] = {
  { TRIG1, ECHO1, Ultrasonic::IDLE, 0, 0, 0, 0.0, false },
  { TRIG2, ECHO2, Ultrasonic::IDLE, 0, 0, 0, 0.0, false }
};

int activeSensor = 0;

const unsigned long TRIG_PULSE_US = 10;
const unsigned long TIMEOUT_US = 38000;
const unsigned long SENSOR_GAP_US = 60000;  // 60 ms
unsigned long lastSwitch = 0;

void updateUltrasonic(Ultrasonic &u);

void setup() {
  //Motor Pins are output
  pinMode(AIN1, OUTPUT);
  pinMode(AIN2, OUTPUT);
  pinMode(BIN1, OUTPUT);
  pinMode(BIN2, OUTPUT);
  pinMode(PWMA, OUTPUT);
  pinMode(PWMB, OUTPUT);
  Serial.begin(9600);
  Serial.println("test");

  // Sets up 2 UltraSonic Sensors
  for (int i = 0; i < 2; i++) {
    pinMode(sensors[i].trig, OUTPUT);
    pinMode(sensors[i].echo, INPUT);
    digitalWrite(sensors[i].trig, LOW);
  }
}

void loop() {
  unsigned long now = micros();

  if (sensors[activeSensor].state == Ultrasonic::IDLE && now - lastSwitch > SENSOR_GAP_US) {
    activeSensor = (activeSensor + 1) % 2;
    lastSwitch = now;
  }

  updateUltrasonic(sensors[activeSensor]);

  for (int i = 0; i < 2; i++) {
    if (sensors[i].ready) {
      Serial.print("Sensor ");
      Serial.print(i);
      Serial.print(": ");
      Serial.print(sensors[i].distanceCm);
      Serial.println(" cm");
      sensors[i].ready = false;
    }
  }

  while (Serial.available()) {
    char c = Serial.read();

    if (c == '\n') {
      data.trim();  // removes \r, spaces, hidden chars

      Serial.println(data);
      Serial.println(data.length());
      Serial.println("found the work");  // Print full string
                                         //OUR LOGIC HERE
      if (data == "FORWARD") {
        FORWARD();
      } else if (data == "BACKWARD") {
        BACKWARD();
      } else if (data == "TURN_RIGHT") {
        TURN_RIGHT();
      } else if (data == "TURN_LEFT") {
        TURN_LEFT();
      } else if (data == "STOP") {
        STOP();
      } else {
        // STOP();
      }
      data = "";  // Clear buffer
    } else {
      data += c;  // Append character
    }
  }
}


void FORWARD() {

  int speed = 100;
  digitalWrite(BIN1, 0);
  digitalWrite(BIN2, 1);
  digitalWrite(AIN1, 0);
  digitalWrite(AIN2, 1);
  analogWrite(PWMB, speed);
  analogWrite(PWMA, speed);
}

void BACKWARD() {

  int speed = 100;
  digitalWrite(BIN1, 1);
  digitalWrite(BIN2, 0);
  digitalWrite(AIN1, 1);
  digitalWrite(AIN2, 0);
  analogWrite(PWMB, speed);
  analogWrite(PWMA, speed);
}


void STOP() {

  analogWrite(PWMB, 0);
  analogWrite(PWMA, 0);
}

void TURN_RIGHT() {

  int speed = 120;
  digitalWrite(BIN1, 0);
  digitalWrite(BIN2, 1);
  digitalWrite(AIN1, 1);
  digitalWrite(AIN2, 0);

  analogWrite(PWMB, speed);
  analogWrite(PWMA, speed);

  delay(1025);
}

void TURN_LEFT() {

  int speed = 120;
  digitalWrite(BIN1, 1);
  digitalWrite(BIN2, 0);
  digitalWrite(AIN1, 0);
  digitalWrite(AIN2, 1);

  analogWrite(PWMB, speed);
  analogWrite(PWMA, speed);

  delay(1025);
}

void updateUltrasonic(Ultrasonic &u) {
  unsigned long now = micros();

  switch (u.state) {

    case Ultrasonic::IDLE:
      digitalWrite(u.trig, HIGH);
      u.tStart = now;
      u.state = Ultrasonic::TRIG_HIGH;
      break;

    case Ultrasonic::TRIG_HIGH:
      if (now - u.tStart >= TRIG_PULSE_US) {
        digitalWrite(u.trig, LOW);
        u.tStart = now;
        u.state = Ultrasonic::WAIT_ECHO_HIGH;
      }
      break;

    case Ultrasonic::WAIT_ECHO_HIGH:
      if (digitalRead(u.echo) == HIGH) {
        u.echoStart = now;
        u.state = Ultrasonic::WAIT_ECHO_LOW;
      } else if (now - u.tStart > TIMEOUT_US) {
        u.state = Ultrasonic::IDLE;
      }
      break;

    case Ultrasonic::WAIT_ECHO_LOW:
      if (digitalRead(u.echo) == LOW) {
        u.duration = now - u.echoStart;
        u.distanceCm = u.duration / 58.0;
        u.ready = true;
        u.state = Ultrasonic::IDLE;
      } else if (now - u.echoStart > TIMEOUT_US) {
        u.state = Ultrasonic::IDLE;
      }
      break;
  }
}
