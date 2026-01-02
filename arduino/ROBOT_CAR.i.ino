//Right Motor (A)
#define AIN1 9  //Right Motor Forward
#define AIN2 8  //Right Motor Backward
#define PWMA 3  //Right Motor Speed

//Left Motor (B)
#define BIN1 11  //Left Motor Forward
#define BIN2 12  //Left Motor Backward
#define PWMB 5   //Left Motor Speed

String data = "";

void FORWARD();
void BACKWARD();
void TURN_RIGHT();
void TURN_LEFT();
void STOP();


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
}

void loop() {

  while (Serial.available()) {
    char c = Serial.read();

      if (c == '\n') {
        data.trim();   // removes \r, spaces, hidden chars

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