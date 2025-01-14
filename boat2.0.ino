#define BLYNK_TEMPLATE_ID           "asdasdasd"
#define BLYNK_TEMPLATE_NAME         "RC BOAT"
#define BLYNK_AUTH_TOKEN            "asdasdafa"

#include <WiFi.h>
#include <BlynkSimpleEsp32.h>
#include <ESP32Servo.h>

// Blynk credentials
char auth[] = "asdasdasdsad";  // Replace with your Blynk Auth Token
char ssid[] = "dark";        // Replace with your Wi-Fi SSID
char pass[] = "12345678";    // Replace with your Wi-Fi Password

// Motor control pins
#define IN1 2
#define IN2 12
#define IN3 13
#define IN4 4

// Set up the virtual pins for Blynk
#define vPinForward V1   // Forward button (virtual pin V1)
#define vPinBackward V2  // Backward button (virtual pin V2)
#define vPinLeft V3      // Left button (virtual pin V3)
#define vPinRight V4     // Right button (virtual pin V4)

// Define the Servo pin
#define servoPin 15       // Pin connected to the Servo (adjust as necessary)
Servo rudderServo;      // Create a Servo object for controlling the rudder

// Variables to store movement state
bool movingForward = false;
bool movingBackward = false;

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Blynk.begin(auth, ssid, pass);

  // Set motor control pins as outputs
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  // Attach the Servo to the pin
  rudderServo.attach(servoPin);
  rudderServo.write(87);  // Set the servo to a neutral (center) position at startup
}

void loop() {
  Blynk.run();  // Keeps Blynk running
}

// BLYNK_WRITE event handlers for each virtual pin

BLYNK_WRITE(vPinForward) {
  int pinValue = param.asInt(); // Get the value of the virtual pin
  if (pinValue == 1) {
    movingForward = true;
    forward();
  } else {
    movingForward = false;
    stopMotors();
  }
}

BLYNK_WRITE(vPinBackward) {
  int pinValue = param.asInt(); // Get the value of the virtual pin
  if (pinValue == 1) {
    movingBackward = true;
    backward();
  } else {
    movingBackward = false;
    stopMotors();
  }
}

BLYNK_WRITE(vPinLeft) {
  int pinValue = param.asInt(); // Get the value of the virtual pin
  if (pinValue == 1) {
    if (movingForward || movingBackward) {
      left();  // Turn the rudder if moving forward or backward
    } else {
      leftMotorOnly();  // If only left is pressed, move left motor only
    }
  } else {
    // Only stop rudder or left motor if movement is not active
    if (!(movingForward || movingBackward)) {
      stopMotors();
    }
  }
}

BLYNK_WRITE(vPinRight) {
  int pinValue = param.asInt(); // Get the value of the virtual pin
  if (pinValue == 1) {
    if (movingForward || movingBackward) {
      right();  // Turn the rudder if moving forward or backward
    } else {
      rightMotorOnly();  // If only right is pressed, move right motor only
    }
  } else {
    // Only stop rudder or right motor if movement is not active
    if (!(movingForward || movingBackward)) {
      stopMotors();
    }
  }
}

void forward() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  rudderServo.write(87); // Keep the rudder straight when moving forward
}

void backward() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  rudderServo.write(87); // Keep the rudder straight when moving backward
}

void left() {
  // Control the rudder for left turn
  rudderServo.write(135);  // Example angle for left turn
}

void right() {
  // Control the rudder for right turn
  rudderServo.write(45);  // Example angle for right turn
}

void rightMotorOnly() {
  // Move left motor only without affecting the rudder
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}

void leftMotorOnly() {
  // Move right motor only without affecting the rudder
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void stopMotors() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
  
  // Keep the rudder centered when stopping
  rudderServo.write(87);  // Set servo to neutral position
}
