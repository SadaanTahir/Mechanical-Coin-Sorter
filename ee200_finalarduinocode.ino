// Analog IR Sensors for security
int IRSensor1 = A0; // Analog IR sensor #1
int IRSensor2 = A2; // Analog IR sensor #2
int IRSensor3 = A4; // Analog IR sensor #3

int BUZZER = 13;    // Digital pin for the buzzer
int common_threshold = 600; // Threshold for intrusion detection

// Digital IR Sensors
const int DigitalIRSensors[4] = {9, 11, 19, 20}; // Digital IR sensors
const int RELAY_PIN = 2; // Pin to activate the relay
const int detectionThreshold = 10; // Number of detections needed to activate the relay

int detectionCounts[4] = {0, 0, 0, 0}; // Count for each digital IR sensor
bool relayActivated = false; // To track if the relay has already been activated

void setup() {
  Serial.begin(9600);             // Initialize Serial at 9600 Baud
  pinMode(BUZZER, OUTPUT);        // Buzzer pin as output
  pinMode(RELAY_PIN, OUTPUT);     // Relay pin as output

  // Set all digital IR sensors as input
  for (int i = 0; i < 4; i++) {
    pinMode(DigitalIRSensors[i], INPUT);
  }
  digitalWrite(RELAY_PIN, LOW);
}

void loop() {
  // Code for Analog IR Sensors
  int sensor1Value = analogRead(IRSensor1); 
  int sensor2Value = analogRead(IRSensor2); 
  int sensor3Value = analogRead(IRSensor3); 
  
  bool allAboveThreshold = true;

  // Check if any analog sensor is below the threshold
  if (sensor1Value <= common_threshold || 
      sensor2Value <= common_threshold || 
      sensor3Value <= common_threshold) {
    allAboveThreshold = false;
  }

  // Control the buzzer based on the analog IR sensor state
  digitalWrite(BUZZER, allAboveThreshold ? LOW : HIGH);

  // Code for Digital IR Sensors
  for (int i = 0; i < 4; i++) {
    int sensorValue = digitalRead(DigitalIRSensors[i]); // Read the digital value of the sensor

    if (sensorValue == LOW) { // Obstruction detected
      detectionCounts[i]++;
      Serial.print("Digital Sensor ");
      Serial.print(i + 1);
      Serial.print(" detection count: ");
      Serial.println(detectionCounts[i]);

      // Check if the detection threshold is reached
      if (detectionCounts[i] >= detectionThreshold && !relayActivated) {
        digitalWrite(RELAY_PIN, HIGH); // Activate relay
        relayActivated = true;
        Serial.println("Relay Activated! - Machine should turn off now...");

        // Reset all detection counts
        for (int j = 0; j < 4; j++) {
          detectionCounts[j] = 0;
        }
      }
    }
  }
}
