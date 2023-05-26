/*** header block ***
* code file name: Robot arm Project
* code description: contolling a rob ot with 2 diffrent controls 
* arduino board (UNO/Nano/...etc): Mega 2560
* sensors and devices needed:potentiometers, Servo motors, Push Buttons, LED, Infrared sensor/remote.
* IDE version used to test code:2.0.2
* programmer(s) name: Jareth Balcazar
* date when code is created/modified: 12/21/22
* code version/revision:5
***/

// include libraries
#include <Servo.h>
#include <FiniteStateMachine.h>
#include <LED.h>
#include <IRremote.h>
#define LED_RED 8

// define pin names
int RECV_PIN = 11;
const int button_1 = 13;
const int button_2 = 12;

int Button_State_1 = 0;
int Button_State_2 = 0;

IRrecv irrecv(RECV_PIN);
decode_results results;

Servo Sholder;
Servo Arm;
Servo Elbow;

// define pin names and define pin names for Servos
int pot_pin_S = A0;
int pot_val_S;
int servo_pos_S;

int pot_pin_A = A1;
int pot_val_A;
int servo_pos_A;

int pot_pin_E = A2;
int pot_val_E;
int servo_pos_E;

// global variables
int angle_S = 90;
int angle_A = 90;
int angle_E = 90;

int New_angle_S = 0;
int New_angle_A = 0;
int New_angle_E = 0;

int angle_step = 20;

void setup() {
  // configure data communication
  Serial.begin(9600);
  // configure hardware peripherals
  // In case the interrupt driver crashes on setup, give a clue
  // to the user what's going on.
  Serial.println("Enabling IRin");
  irrecv.enableIRIn();  // Start the receiver
  Serial.println("Enabled IRin");

  // code that runs only once
  pinMode(LED_RED, OUTPUT);
  Sholder.attach(5);
  Arm.attach(4);
  Elbow.attach(3);
  pinMode(button_1, INPUT_PULLUP);
  pinMode(button_2, INPUT_PULLUP);
}

void loop() {
  // data input
  if (Button_State_1 == 0) {
    Button_State_1 = digitalRead(button_1);
    // data communication
    Serial.println("Using Physical Controls");

    digitalWrite(LED_RED, LOW);
    // data input
    pot_val_S = analogRead(pot_pin_S);               // read value of potentiometer (value from 0 to 1023)
    servo_pos_S = map(pot_val_S, 10, 1023, 0, 180);  // scale it to control the servo (value from 0 to 180)
    // data output
    Sholder.write(servo_pos_S);                      // set the servo position according to the scaled value

    // data input
    pot_val_A = analogRead(pot_pin_A);               // read value of potentiometer (value from 0 to 1023)
    servo_pos_A = map(pot_val_A, 10, 1023, 0, 180);  // scale it to control the servo (value from 0 to 180)
    // data output
    Arm.write(servo_pos_A);                           // set the servo position according to the scaled value

    // data input
    pot_val_E = analogRead(pot_pin_E);               // read value of potentiometer (value from 0 to 1023)
    servo_pos_E = map(pot_val_E, 10, 1023, 0, 180);  // scale it to control the servo (value from 0 to 180)
    // data output
    Elbow.write(servo_pos_E);                        // set the servo position according to the scaled value
  }
  if (Button_State_1 == 1) {
    // data communication
    Serial.println("Using IR Remote");
    Button_State_2 = digitalRead(button_2);
    digitalWrite(LED_RED, HIGH);
    if (irrecv.decode(&results)) {
      Serial.println(results.value, HEX);


      switch (results.value) {
        // data input
        case 0xFF10EF:
          New_angle_S = angle_S + angle_step;
          // data output
          Sholder.write(New_angle_S);  //Shoulder Left
          // data storage
          angle_S = New_angle_S;
          break;
        case 0xFF5AA5:
          New_angle_S = angle_S - angle_step;
          // data output
          Sholder.write(New_angle_S);  //Shoulder Right
          // data storage
          angle_S = New_angle_S;
          break;


        case 0xFF6897:
          New_angle_A = angle_A + angle_step;
          // data output
          Arm.write(New_angle_A);  //Arm Forward
          // data storage
          angle_A = New_angle_A;
          break;
        case 0xFFB04F:
          New_angle_A = angle_A - angle_step;
          // data output
          Arm.write(New_angle_A);  //Arm Backward
          // data storage
          angle_A = New_angle_A;
          break;


        case 0xFF18E7:
          New_angle_E = angle_E + angle_step;
          // data output
          Elbow.write(New_angle_E);  //Elbow UP
          // data storage
          angle_E = New_angle_E;
          break;
        case 0xFF4AB5:
          New_angle_E = angle_E - angle_step;
          // data output
          Elbow.write(New_angle_E);  //Elbow UP
          // data storage
          angle_E = New_angle_E;
          break;
      }
      irrecv.resume();  // Receive the next value
    }
    delay(100);
  }
  if (Button_State_2 == 1) {
    // data processing
    Button_State_1 = 0;
    Button_State_2 = 0;
  }
}
