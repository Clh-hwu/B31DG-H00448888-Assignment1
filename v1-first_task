// Include necessary libraries
#include <Arduino.h>

// Define compile-time switch for DEBUG timing
#define DEBUG_TIMING

// Timing parameters
#ifdef DEBUG_TIMING
    #define TON1_DEBUG    100000  // DEBUG on-time in milliseconds (factor of 1000)
    #define TOFF_DEBUG    100000  // DEBUG off-time in milliseconds (factor of 1000)
    #define NUM_PULSES_DEBUG  10  // DEBUG number of pulses
    #define IDLE_TIME_DEBUG   200000 // DEBUG idle time between DATA and SYNC pulses
    #define TSYNC_ON_DEBUG    500000 // DEBUG SYNC on-time
#else
    #define TON1_PROD    100    // PRODUCTION on-time in microseconds
    #define TOFF_PROD    100    // PRODUCTION off-time in microseconds
    #define NUM_PULSES_PROD  5   // PRODUCTION number of pulses
    #define IDLE_TIME_PROD   200 // PRODUCTION idle time between DATA and SYNC pulses
    #define TSYNC_ON_PROD    5  // PRODUCTION SYNC on-time
#endif

// Define pin numbers for push buttons and outputs
const int PB1_PIN = 2; // Pin for Push Button 1
const int PB2_PIN = 3; // Pin for Push Button 2
const int DATA_PIN = 9; // Pin for DATA output
const int SYNC_PIN = 10; // Pin for SYNC output

// Operational state
enum SystemState {
    STATE_IDLE,
    STATE_OUTPUT_ENABLED,
    STATE_ALTERNATIVE_SELECTED
};

// Variable to store the current state
SystemState currentState = STATE_IDLE;

// Variable to store the User's Surname
String userSurname = "ab";

// Variable to store the alternative behaviour
int alternativeBehavior;
int* params;

// Function prototypes
void handlePushButton1();
void handlePushButton2();
void generateDataWaveform();
void generateAlternativeDataWaveform();
void generateSyncPulse();
void calculateTimingParameters();
void generateAlternativeDataWaveformWithSurname();

// Setup function
void setup() {
    // Set pin modes
    pinMode(PB1_PIN, INPUT_PULLUP);
    pinMode(PB2_PIN, INPUT_PULLUP);
    pinMode(DATA_PIN, OUTPUT);
    pinMode(SYNC_PIN, OUTPUT);

    // Set up the variables 
    alternativeBehavior = 0;
    
    Serial.begin(9600);
}

// Loop function
void loop() {
    // Check and handle push button inputs
    delayMicroseconds(100);
    handlePushButton1();
    handlePushButton2();
    Serial.print("Current State : ");
    Serial.println(currentState);
    params = calculateTimingParameters(userSurname);
    delay(500);  // Delay for better readability in the serial monitor
    // Generate appropriate waveforms based on the current state
    switch (currentState) {
        case STATE_OUTPUT_ENABLED:
            generateDataWaveform();
            break;
        case STATE_ALTERNATIVE_SELECTED:
            generateAlternativeDataWaveform(alternativeBehavior);
            //generateAlternativeDataWaveformWithSurname(alternativeBehavior, params);

            break;
        default:
            // Generate SYNC pulse
            generateSyncPulse();
            break;
    }
}

// Function to handle the first push button
void handlePushButton1() {
  static unsigned long lastDebounceTimeUp = 0;
  static unsigned long debounceDelay = 500;

  // Check for a valid button state change (debouncing)
  if (digitalRead(PB1_PIN) == HIGH && currentState == STATE_IDLE) {
    lastDebounceTimeUp = millis();    
  }

  // Check if the button state has remained stable for the debounce delay
  if ((millis() - lastDebounceTimeUp) > debounceDelay) {
    if (digitalRead(PB1_PIN) == LOW) {
      // Implementation to handle momentary press and change state
      currentState = STATE_OUTPUT_ENABLED;
      // For demonstration purposes, just print a message
      Serial.println("PushButton1 pressed");
    }
  }
}

// Function to handle the second push button
void handlePushButton2() {
  static unsigned long lastDebounceTimeUp = 0;
  static unsigned long debounceDelay = 500;

  // Check for a valid button state change (debouncing)
  if (digitalRead(PB2_PIN) == HIGH && currentState == STATE_IDLE) {
    lastDebounceTimeUp = millis(); 
  }
  
  // Check if the button state has remained stable for the debounce delay
  if ((millis() - lastDebounceTimeUp) > debounceDelay) {
    //Serial.println("stage2");
    if (digitalRead(PB2_PIN) == LOW) {
      // Implementation to handle momentary press and change state
      currentState = STATE_ALTERNATIVE_SELECTED;
      alternativeBehavior += 1;
      alternativeBehavior = (alternativeBehavior % 4);

      // For demonstration purposes, just print a message
      Serial.println("PushButton2 pressed");
      Serial.print("Alternativre behaviour : ");
      Serial.println(alternativeBehavior);
    }
  }
}

// Function to generate the normal DATA waveform
void generateDataWaveform() {
  int i = 0; 
  int delay_on;
  int delay_off;
  #ifdef DEBUG_TIMING
    const unsigned long TDATA_ON = TON1_DEBUG;
    const unsigned long TDATA_OFF = TOFF_DEBUG;
    const unsigned long NUM_PULSES = NUM_PULSES_DEBUG;
    const unsigned long IDLE_TIME = IDLE_TIME_DEBUG;
  #else
    const unsigned long TDATA_ON = TON1_PROD;
    const unsigned long TDATA_OFF = TOFF_PROD;
    const unsigned long NUM_PULSES = NUM_PULSES_PROD;
    const unsigned long IDLE_TIME = IDLE_TIME_PROD;
  #endif
  while(i < NUM_PULSES) {
    delay_on = (TDATA_ON + i*50000); 
    delay_off = (TDATA_OFF);
    i+=1;
    digitalWrite(DATA_PIN, HIGH);  // Set DATA_PIN high for the pulse on-time
    delayMicroseconds(delay_on);
        
    digitalWrite(DATA_PIN, LOW);   // Set DATA_PIN low for the pulse off-time
    delayMicroseconds(delay_off);
    Serial.println("generateDataWaveform");
  }
  
  // Add idle time between the end of the final pulse off-time and the start of the next SYNC pulse
  currentState = STATE_IDLE;
  delayMicroseconds(IDLE_TIME);
}

// Function to generate the alternative DATA waveform based on selected behavior
void generateAlternativeDataWaveform(int alternativeBehavior) {
  int delay_on;
  int delay_off;
  #ifdef DEBUG_TIMING
    const unsigned long TDATA_ON = TON1_DEBUG;
    const unsigned long TDATA_OFF = TOFF_DEBUG;
    const unsigned long NUM_PULSES = NUM_PULSES_DEBUG;
    const unsigned long IDLE_TIME = IDLE_TIME_DEBUG;
  #else
    const unsigned long TDATA_ON = TON1_PROD;
    const unsigned long TDATA_OFF = TOFF_PROD;
    const unsigned long NUM_PULSES = NUM_PULSES_PROD;
    const unsigned long IDLE_TIME = IDLE_TIME_PROD;
  #endif
  delay_off = (TDATA_OFF);
  
    switch (alternativeBehavior) {
        case 1:
            // Behavior: Remove the final 3 pulses from each data waveform cycle
            for (int i = 0; i < NUM_PULSES - 3; ++i) {
                delay_on = (TDATA_ON + i*50000); 
                digitalWrite(DATA_PIN, HIGH);
                delayMicroseconds(delay_on);

                digitalWrite(DATA_PIN, LOW);
                delayMicroseconds(delay_off);
            }
            break;

        case 2:
            // Behavior: Generate a reversed form of the data waveform
            for (int i = NUM_PULSES; i > 0; --i) {
                delay_on = (TDATA_ON + i*50000); 
                digitalWrite(DATA_PIN, HIGH);
                delayMicroseconds(delay_on);

                digitalWrite(DATA_PIN, LOW);
                delayMicroseconds(delay_off);
            }
            break;

        case 3:
            // Behavior: Insert an extra 3 pulses into each data waveform cycle
            for (int i = 0; i < NUM_PULSES + 3; ++i) {
                delay_on = (TDATA_ON + i*50000); 
                digitalWrite(DATA_PIN, HIGH);
                delayMicroseconds(delay_on);

                digitalWrite(DATA_PIN, LOW);
                delayMicroseconds(delay_off);
            }
            break;
/*
        case 4:
            // Behavior: Half the b and d time intervals
            TON1_PROD /= 2;
            TOFF_PROD /= 2;
            // Continue with normal data waveform generation
            generateDataWaveform();
            // Restore the original values
            TON1_PROD *= 2;
            TOFF_PROD *= 2;
            break;
*/
        default:
            // Default: Generate the normal data waveform
            generateDataWaveform();
            break;
    }

    // Add idle time between the end of the final pulse off-time and the start of the next SYNC pulse
    currentState = STATE_IDLE;
    delayMicroseconds(IDLE_TIME);
}


// Function to generate the SYNC pulse
void generateSyncPulse() {
  int TON_us;
#ifdef DEBUG_TIMING
    const unsigned long TSYNC_ON = TSYNC_ON_DEBUG;
#else
    const unsigned long TSYNC_ON = TSYNC_ON_PROD;
#endif
    TON_us = TSYNC_ON;
    // Set SYNC_PIN high for the SYNC on-time
    digitalWrite(SYNC_PIN, HIGH);
    delayMicroseconds(TON_us);

    // Set SYNC_PIN low to complete the SYNC pulse
    digitalWrite(SYNC_PIN, LOW);
}

// Part 2
// Function to calculate the Output Timing Parameters
int* calculateTimingParameters(const String& surname) {
    int* params = new int[4];

    // Ensure the surname has at least one character
    if (surname.length() == 0) {
        Serial.println("Error: Surname is empty.");
    }

    // Extend the surname with the final letter if it has less than four letters
    String extendedSurname = surname;
    while (extendedSurname.length() < 4) {
        extendedSurname += surname.charAt(surname.length() - 1);
    }

    // Extract the first four letters
    String firstFourLetters = extendedSurname.substring(0, 4);

    // Map each letter to its corresponding numeric value
    int mapping[4];
    for (int i = 0; i < 4; ++i) {
        char currentLetter = firstFourLetters[i];
        int numericValue = 0;

        // Map letters a-z to numeric values
        if (currentLetter >= 'a' && currentLetter <= 'z') {
            numericValue = currentLetter - 'a' + 1;
        }
        // Map letters A-Z to numeric values
        else if (currentLetter >= 'A' && currentLetter <= 'Z') {
            numericValue = currentLetter - 'A' + 1;
        }

        mapping[i] = numericValue;
    }

    // Calculate Output Timing Parameters
    unsigned long parameter1 = mapping[0] * 100;      // First Letter Numerical Mapping x 100us
    unsigned long parameter2 = mapping[1] * 100;      // Second Letter Numerical Mapping x 100us
    unsigned long parameter3 = (mapping[2] + 4) * 100; // Third Letter Numerical Mapping + 4
    unsigned long parameter4 = mapping[3] * 500;      // Fourth Letter Numerical Mapping x 500us

    params[0] = parameter1;
    params[1] = parameter2;
    params[2] = parameter3;
    params[3] = parameter4;
    
    // Print the calculated parameters
    Serial.print("Parameter 1: ");
    Serial.print(parameter1);
    Serial.println(" us");

    Serial.print("Parameter 2: ");
    Serial.print(parameter2);
    Serial.println(" us");

    Serial.print("Parameter 3: ");
    Serial.print(parameter3);
    Serial.println(" us");

    Serial.print("Parameter 4: ");
    Serial.print(parameter4);
    Serial.println(" us");

    return params;
}

// Function to generate the alternative DATA waveform based on selected behavior
void generateAlternativeDataWaveformWithSurname(int alternativeBehavior, int* params) {
  int delay_on;
  int delay_off;
  int Pulses;
  #ifdef DEBUG_TIMING
    const unsigned long TDATA_ON = TON1_DEBUG;
    const unsigned long TDATA_OFF = TOFF_DEBUG;
    const unsigned long NUM_PULSES = NUM_PULSES_DEBUG;
    const unsigned long IDLE_TIME = IDLE_TIME_DEBUG;
  #else
    const unsigned long TDATA_ON = TON1_PROD;
    const unsigned long TDATA_OFF = TOFF_PROD;
    const unsigned long NUM_PULSES = NUM_PULSES_PROD;
    const unsigned long IDLE_TIME = IDLE_TIME_PROD;
  #endif
  delay_off = (TDATA_OFF)/1000;
  Pulses = 4;
    switch (alternativeBehavior) {
        case 1:
            // Behavior: Remove the final 3 pulses from each data waveform cycle
            for (int i = 0; i < Pulses - 3; ++i) {
                delay_on = (TDATA_ON + i*50000)/1000; 
                digitalWrite(DATA_PIN, HIGH);
                delayMicroseconds(params[i]*10);
                Serial.print("Parameter 111: ");
                Serial.print(params[i]*10);


                digitalWrite(DATA_PIN, LOW);
                delayMicroseconds(delay_off);
            }
            break;

        case 2:
            // Behavior: Generate a reversed form of the data waveform
            for (int i = Pulses; i > 0; --i) {
                delay_on = (TDATA_ON + i*50000)/1000; 
                digitalWrite(DATA_PIN, HIGH);
                delayMicroseconds(params[i]*10);
                Serial.print("Parameter 222: ");
                Serial.print(params[i]*10);


                digitalWrite(DATA_PIN, LOW);
                delayMicroseconds(delay_off);
            }
            break;

        case 3:
            // Behavior: Insert an extra 3 pulses into each data waveform cycle
            for (int i = 0; i < Pulses + 3; ++i) {
                delay_on = (TDATA_ON + i*50000)/1000; 
                digitalWrite(DATA_PIN, HIGH);
                delayMicroseconds(params[i]*10);
                Serial.print("Parameter 333: ");
                Serial.print(params[i]*10);

                digitalWrite(DATA_PIN, LOW);
                delayMicroseconds(delay_off);
            }
            break;
/*
        case 4:
            // Behavior: Half the b and d time intervals
            TON1_PROD /= 2;
            TOFF_PROD /= 2;
            // Continue with normal data waveform generation
            generateDataWaveform();
            // Restore the original values
            TON1_PROD *= 2;
            TOFF_PROD *= 2;
            break;
*/
        default:
            // Default: Generate the normal data waveform
            for (int i = 0; i < Pulses; ++i) { 
                digitalWrite(DATA_PIN, HIGH);
                delayMicroseconds(params[i]*10);
                Serial.print("Parameter 444: ");
                Serial.print(params[i]*10);

                digitalWrite(DATA_PIN, LOW);
                delayMicroseconds(delay_off);
            }
            break;
    }
    // Add idle time between the end of the final pulse off-time and the start of the next SYNC pulse
    currentState = STATE_IDLE;
    delayMicroseconds(IDLE_TIME);
}
