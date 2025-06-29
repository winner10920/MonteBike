
// --- Servo Configuration ---
#define SERVO_NUM 2  // Number of servos
SMS_STS st;  // Servo control object
#define COMSerial Serial1

byte ID[SERVO_NUM] = { 1, 2 };          // IDs of the servos
u16 Speed[SERVO_NUM] = { 1500, 1500 };  // Set a medium speed for the servos
byte ACC[SERVO_NUM] = { 50, 50 };       // Set a medium acceleration for the servos
s16 Pos[SERVO_NUM] = { 2048, 2048 };    // Servo position array, initialized to the midpoint (2048)


void setupServo() {
  Serial.println("--- Servo Control Program Start ---");

  // Start the serial port for controlling the servos
  COMSerial.begin(1000000, SERIAL_8N1);
  st.pSerial = &COMSerial;  // Associate the control object with the serial port

  Serial.println("Checking servo connection status...");
  for (int i = 0; i < SERVO_NUM; i++) {
    if (st.Ping(ID[i]) != -1) {
      Serial.print("Servo with ID ");
      Serial.print(ID[i]);
      Serial.println(" is connected.");
    } else {
      Serial.print("Error: Servo with ID ");
      Serial.print(ID[i]);
      Serial.println(" is not responding!");
    }
  }

  // --- Power-on Self-Test ---
  // This section makes the servos move automatically on power-up to confirm they are working correctly.
  Serial.println("\nExecuting power-on self-test movement...");

  // 1. Move to position 1024
  Serial.println("Moving to position 1024...");
  for (int i = 0; i < SERVO_NUM; i++) {
    Pos[i] = 1024;
  }
  st.SyncWritePosEx(ID, SERVO_NUM, Pos, Speed, ACC);
  delay(2000);  // Wait for the movement to complete

  // 2. Move to position 3072
  Serial.println("Moving to position 3072...");
  for (int i = 0; i < SERVO_NUM; i++) {
    Pos[i] = 3072;
  }
  st.SyncWritePosEx(ID, SERVO_NUM, Pos, Speed, ACC);
  delay(2000);  // Wait for the movement to complete

  // 3. Return to center position (2048) to prepare for user commands
  Serial.println("Returning to center position (2048)...");
  for (int i = 0; i < SERVO_NUM; i++) {
    Pos[i] = 2048;
  }
  st.SyncWritePosEx(ID, SERVO_NUM, Pos, Speed, ACC);
  delay(1500);

  Serial.println("\n--- Initialization Complete ---");
  Serial.println("Enter 'j' to decrease the angle, or 'k' to increase it.");
  Serial.println("-----------------------------------");
}


void runServo() {
  // Check if the user has sent a command via the Serial Monitor
  if (Serial.available()) {
    String input = Serial.readString();
    input.trim();  // Remove extra spaces or newlines

    bool shouldMove = false;  // Flag to indicate if a valid command was received

    if (input.startsWith("j")) {
      Serial.println("Received command: 'j'. Decreasing angle.");
      for (int i = 0; i < SERVO_NUM; i++) {
        Pos[i] -= 512;  // Move a small step for easy observation
        if (Pos[i] < 0) {
          Pos[i] = 0;  // Prevent going below the minimum range
        }
      }
      shouldMove = true;
    } else if (input.startsWith("k")) {
      Serial.println("Received command: 'k'. Increasing angle.");
      for (int i = 0; i < SERVO_NUM; i++) {
        Pos[i] += 512;  // Move a small step
        if (Pos[i] > 4095) {
          Pos[i] = 4095;  // Prevent going above the maximum range
        }
      }
      shouldMove = true;
    } else {
      Serial.print("Unknown command: '");
      Serial.print(input);
      Serial.println("'. Please enter 'j' or 'k'.");
    }

    // If a valid command was received, send the new positions to the servos
    if (shouldMove) {
      Serial.print("Moving servos to new positions: [");
      for (int i = 0; i < SERVO_NUM; i++) {
        Serial.print(Pos[i]);
        if (i < SERVO_NUM - 1) Serial.print(", ");
      }
      Serial.println("]");

      st.SyncWritePosEx(ID, SERVO_NUM, Pos, Speed, ACC);
    }
  }
}