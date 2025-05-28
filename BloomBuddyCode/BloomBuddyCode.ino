/*
 * Project  :  Bloom Buddy - Smart Plant Companion
 * Author   :  Mukesh Sankhla
 * Website  :  https://makerbrains.com
 * 
 * This code creates an interactive smart plant monitoring system that responds
 * with personality-driven animations and voice feedback based on plant needs.
 * 
 * Hardware: ESP32-based UNIHIKER K10
 * 
 * Features:
 * - Real-time soil moisture monitoring with analog probe
 * - Temperature and humidity monitoring (AHT20 sensor)
 * - Ambient light level detection for day/night cycles
 * - Interactive personality system with 12+ animations
 * - Contextual voice responses with randomized audio files
 * - Button-triggered sensor data display
 * - SD card storage for audio files and data logging
 * - Priority-based plant care logic (water > environment)
 * 
 * The device expresses emotions through animations:
 * - Happy when plant is watered
 * - Sad when soil is dry
 * - Sleepy during nighttime
 * - Cold/Hot reactions to temperature extremes
 * - Wake up responses in morning light
 * 
 * Repository: https://github.com/MukeshSankhla/Bloom-Buddy
 * License: MIT
 * Date: 2025
 */

#include "unihiker_k10.h"

// ========== GLOBAL OBJECTS ==========
UNIHIKER_K10 k10;              // Main controller object
AHT20        aht20;            // Temperature and humidity sensor
Music        music;            // Audio player for voice feedback

// ========== CONFIGURATION ==========
uint8_t screen_dir = 2;        // Screen orientation setting

// ========== SENSOR VARIABLES ==========
float Temperature = 0;         // Current temperature in Celsius
float Humidity = 0;            // Current relative humidity percentage
float Light = 0;               // Current light level
int   SoilMoisture = 0;        // Raw soil moisture sensor reading
int   moisturePercent = 0;     // Converted moisture percentage (0-100%)

// ========== SYSTEM VARIABLES ==========
int  randIndex = 1;            // Random index for voice file selection
bool wasDry = false;           // Flag to track if plant was previously dry
bool wasDark = false;          // Flag to track if environment was previously dark
bool wasTemp = false;          // Flag to track temperature-related state changes

// ========== THRESHOLDS ==========
const int DRY_THRESHOLD = 30;      // Moisture percentage below which plant is considered dry
const int DARK_THRESHOLD = 10;     // Light level below which environment is considered dark
const int COLD_THRESHOLD = 15;     // Temperature below which plant is considered cold
const int HOT_THRESHOLD = 40;      // Temperature above which plant is considered hot

// Soil moisture sensor calibration values
const int MOISTURE_WET_VALUE = 600;    // Sensor reading when soil is wet
const int MOISTURE_DRY_VALUE = 3046;   // Sensor reading when soil is dry

// ========== FUNCTION DECLARATIONS ==========
void ShowData();           // Display sensor data on screen
void IdealAnimation();     // Animation for ideal conditions
void SleepAnimation();     // Animation for nighttime/dark conditions
void ShyAnimation();       // Animation for shy state
void HappyAnimation();     // Animation for happy state (good watering)
void SadAnimation();       // Animation for sad state (needs water)
void BoaredAnimation();    // Animation for bored state
void ColdAnimation();      // Animation for cold conditions
void HotAnimation();       // Animation for hot conditions
void AngryAnimation();     // Animation for angry state
void WinkAnimation();      // Animation for winking
void LaughAnimation();     // Animation for laughing
void KissAnimation();      // Animation for kiss gesture
void BootAnimation();      // Animation played during system startup

/*
 * SETUP FUNCTION
 * Initializes all hardware components and plays startup sequence
 */
void setup() {
  // Initialize the main controller
  k10.begin();
  
  // Initialize SD card for file storage
  k10.initSDFile();
  
  // Initialize display with specified orientation
  k10.initScreen(screen_dir);
  k10.creatCanvas();
  
  // Play random boot sound (B1.wav to B6.wav)
  randIndex = random(1, 7);
  String filePath = "S:Voice/B" + String(randIndex) + ".wav";
  music.playTFCardAudio(filePath.c_str());
  
  // Play boot animation sequence
  BootAnimation();
  BootAnimation();
}

/*
 * MAIN LOOP FUNCTION
 * Continuously monitors sensors and responds with appropriate animations/sounds
 */
void loop() {
  // ========== SENSOR READINGS ==========
  Temperature = aht20.getData(AHT20::eAHT20TempC);    // Get temperature in Celsius
  Humidity = aht20.getData(AHT20::eAHT20HumiRH);      // Get relative humidity
  Light = k10.readALS();                              // Get ambient light level
  SoilMoisture = analogRead(P0);                      // Read soil moisture sensor
  
  // Convert raw moisture reading to percentage (0% = dry, 100% = wet)
  moisturePercent = map(SoilMoisture, MOISTURE_DRY_VALUE, MOISTURE_WET_VALUE, 0, 100);
  
  // ========== BUTTON INTERACTION ==========
  // Display sensor data when button B is pressed
  if (k10.buttonB->isPressed()) {
    ShowData();
    delay(5000);  // Prevent button bounce
  }
  
  // ========== PLANT CARE LOGIC ==========
  
  // PRIORITY 1: Check if plant needs water (most critical)
  if (moisturePercent < DRY_THRESHOLD) {
    // Plant is dry and needs water
    if (!wasDry) {
      // First time detecting dry condition - play sad voice
      randIndex = random(1, 6);  // D1.wav to D5.wav (dry/sad sounds)
      String filePath = "S:Voice/D" + String(randIndex) + ".wav";
      music.playTFCardAudio(filePath.c_str());
    }
    SadAnimation();
    wasDry = true;
  }
  else {
    // Plant has adequate water
    if (wasDry) {
      // Plant was just watered - celebrate!
      wasDry = false;
      randIndex = random(1, 7);  // W1.wav to W6.wav (happy/watered sounds)
      String filePath = "S:Voice/W" + String(randIndex) + ".wav";
      music.playTFCardAudio(filePath.c_str());
      
      // Play multiple happy animations to show excitement
      HappyAnimation();
      HappyAnimation();
    }
    
    // PRIORITY 2: Check environmental conditions when plant is watered
    
    if (Light < DARK_THRESHOLD) {
      // Dark environment - check temperature for additional comfort
      
      if (Temperature < COLD_THRESHOLD) {
        // Cold and dark conditions
        if (!wasTemp) {
          randIndex = random(1, 3);  // C1.wav to C2.wav (cold sounds)
          String filePath = "S:Voice/C" + String(randIndex) + ".wav";
          music.playTFCardAudio(filePath.c_str());
        }
        ColdAnimation();
        wasTemp = true;
      }
      else if (Temperature > HOT_THRESHOLD) {
        // Hot and dark conditions
        if (!wasTemp) {
          randIndex = random(1, 3);  // H1.wav to H2.wav (hot sounds)
          String filePath = "S:Voice/H" + String(randIndex) + ".wav";
          music.playTFCardAudio(filePath.c_str());
        }
        HotAnimation();
        wasTemp = true;
      }
      else {
        // Normal temperature but dark (nighttime/sleep mode)
        if (!wasDark) {
          randIndex = random(1, 6);  // N1.wav to N5.wav (night/sleep sounds)
          String filePath = "S:Voice/N" + String(randIndex) + ".wav";
          music.playTFCardAudio(filePath.c_str());
        }
        SleepAnimation();
        wasDark = true;
        wasTemp = false;  // Reset temperature flag since we're in normal temp range
      }
    }
    else {
      // Bright environment - ideal conditions
      if (wasDark) {
        // Just woke up from dark conditions
        randIndex = random(1, 5);  // M1.wav to M4.wav (morning/wake up sounds)
        String filePath = "S:Voice/M" + String(randIndex) + ".wav";
        music.playTFCardAudio(filePath.c_str());
      }
      IdealAnimation();
      wasDark = false;  // Reset dark flag
    }
  }
}

void ShowData(){
  Temperature = aht20.getData(AHT20::eAHT20TempC);
  Humidity = aht20.getData(AHT20::eAHT20HumiRH);
  Light = k10.readALS();
  SoilMoisture = analogRead(P0);
  moisturePercent = map(SoilMoisture, 3046, 600, 0, 100);
  
  k10.canvas->canvasDrawImage(0, 0, "S:/Emoji/SD.png");
  k10.canvas->canvasText(String(Temperature)+" C", 100, 30, 0x333333, k10.canvas->eCNAndENFont24, 50, false);
  k10.canvas->canvasText(String(Humidity)+" %", 100, 110, 0x333333, k10.canvas->eCNAndENFont24, 50, false);
  k10.canvas->canvasText(String(Light)+" Lux", 100, 185, 0x333333, k10.canvas->eCNAndENFont24, 50, false);
  k10.canvas->canvasText(String(moisturePercent)+" %", 100, 260, 0x333333, k10.canvas->eCNAndENFont24, 50, false);
  k10.canvas->updateCanvas();
}
void BootAnimation() {
  k10.canvas->canvasDrawImage(0, 0, "S:/Emoji/G1.png");
  k10.canvas->updateCanvas();
  k10.canvas->canvasDrawImage(0, 0, "S:/Emoji/G2.png");
  k10.canvas->updateCanvas();
  k10.canvas->canvasDrawImage(0, 0, "S:/Emoji/G3.png");
  k10.canvas->updateCanvas();
  k10.canvas->canvasDrawImage(0, 0, "S:/Emoji/G4.png");
  k10.canvas->updateCanvas();
  k10.canvas->canvasDrawImage(0, 0, "S:/Emoji/G5.png");
  k10.canvas->updateCanvas();
  k10.canvas->canvasDrawImage(0, 0, "S:/Emoji/G6.png");
  k10.canvas->updateCanvas();
  k10.canvas->canvasDrawImage(0, 0, "S:/Emoji/G6.png");
  k10.canvas->updateCanvas();
  k10.canvas->canvasDrawImage(0, 0, "S:/Emoji/G5.png");
  k10.canvas->updateCanvas();
  k10.canvas->canvasDrawImage(0, 0, "S:/Emoji/G4.png");
  k10.canvas->updateCanvas();
  k10.canvas->canvasDrawImage(0, 0, "S:/Emoji/G3.png");
  k10.canvas->updateCanvas();
  k10.canvas->canvasDrawImage(0, 0, "S:/Emoji/G2.png");
  k10.canvas->updateCanvas();
  k10.canvas->canvasDrawImage(0, 0, "S:/Emoji/G1.png");
  k10.canvas->updateCanvas();
}

void IdealAnimation() {
  k10.canvas->canvasDrawImage(0, 0, "S:/Emoji/HA1.png");
  k10.canvas->updateCanvas();
  k10.canvas->canvasDrawImage(0, 0, "S:/Emoji/HA2.png");
  k10.canvas->updateCanvas();
  k10.canvas->canvasDrawImage(0, 0, "S:/Emoji/HA3.png");
  k10.canvas->updateCanvas();
  k10.canvas->canvasDrawImage(0, 0, "S:/Emoji/HA4.png");
  k10.canvas->updateCanvas();
  k10.canvas->canvasDrawImage(0, 0, "S:/Emoji/HA5.png");
  k10.canvas->updateCanvas();
  k10.canvas->canvasDrawImage(0, 0, "S:/Emoji/HA5.png");
  k10.canvas->updateCanvas();
  k10.canvas->canvasDrawImage(0, 0, "S:/Emoji/HA4.png");
  k10.canvas->updateCanvas();
  k10.canvas->canvasDrawImage(0, 0, "S:/Emoji/HA3.png");
  k10.canvas->updateCanvas();
  k10.canvas->canvasDrawImage(0, 0, "S:/Emoji/HA2.png");
  k10.canvas->updateCanvas();
  k10.canvas->canvasDrawImage(0, 0, "S:/Emoji/HA1.png");
  k10.canvas->updateCanvas();
}

void KissAnimation() {
  k10.canvas->canvasDrawImage(0, 0, "S:/Emoji/K1.png");
  k10.canvas->updateCanvas();
  k10.canvas->canvasDrawImage(0, 0, "S:/Emoji/K2.png");
  k10.canvas->updateCanvas();
  k10.canvas->canvasDrawImage(0, 0, "S:/Emoji/K3.png");
  k10.canvas->updateCanvas();
  k10.canvas->canvasDrawImage(0, 0, "S:/Emoji/K4.png");
  k10.canvas->updateCanvas();
  k10.canvas->canvasDrawImage(0, 0, "S:/Emoji/K5.png");
  k10.canvas->updateCanvas();
  k10.canvas->canvasDrawImage(0, 0, "S:/Emoji/K6.png");
  k10.canvas->updateCanvas();
  k10.canvas->canvasDrawImage(0, 0, "S:/Emoji/K7.png");
  k10.canvas->updateCanvas();
  k10.canvas->canvasDrawImage(0, 0, "S:/Emoji/K8.png");
  k10.canvas->updateCanvas();
  k10.canvas->canvasDrawImage(0, 0, "S:/Emoji/K9.png");
  k10.canvas->updateCanvas();
}

void SleepAnimation() {
  k10.canvas->canvasDrawImage(0, 0, "S:/Emoji/N1.png");
  k10.canvas->updateCanvas();
  k10.canvas->canvasDrawImage(0, 0, "S:/Emoji/N2.png");
  k10.canvas->updateCanvas();
  k10.canvas->canvasDrawImage(0, 0, "S:/Emoji/N3.png");
  k10.canvas->updateCanvas();
  k10.canvas->canvasDrawImage(0, 0, "S:/Emoji/N4.png");
  k10.canvas->updateCanvas();
  k10.canvas->canvasDrawImage(0, 0, "S:/Emoji/N5.png");
  k10.canvas->updateCanvas();
  k10.canvas->canvasDrawImage(0, 0, "S:/Emoji/N6.png");
  k10.canvas->updateCanvas();
  k10.canvas->canvasDrawImage(0, 0, "S:/Emoji/N7.png");
  k10.canvas->updateCanvas();
  k10.canvas->canvasDrawImage(0, 0, "S:/Emoji/N8.png");
  k10.canvas->updateCanvas();
}

void ShyAnimation() {
  k10.canvas->canvasDrawImage(0, 0, "S:/Emoji/SY1.png");
  k10.canvas->updateCanvas();
  k10.canvas->canvasDrawImage(0, 0, "S:/Emoji/SY2.png");
  k10.canvas->updateCanvas();
  k10.canvas->canvasDrawImage(0, 0, "S:/Emoji/SY3.png");
  k10.canvas->updateCanvas();
  k10.canvas->canvasDrawImage(0, 0, "S:/Emoji/SY4.png");
  k10.canvas->updateCanvas();
  k10.canvas->canvasDrawImage(0, 0, "S:/Emoji/SY5.png");
  k10.canvas->updateCanvas();
  k10.canvas->canvasDrawImage(0, 0, "S:/Emoji/SY6.png");
  k10.canvas->updateCanvas();
  k10.canvas->canvasDrawImage(0, 0, "S:/Emoji/SY6.png");
  k10.canvas->updateCanvas();
  k10.canvas->canvasDrawImage(0, 0, "S:/Emoji/SY5.png");
  k10.canvas->updateCanvas();
  k10.canvas->canvasDrawImage(0, 0, "S:/Emoji/SY4.png");
  k10.canvas->updateCanvas();
  k10.canvas->canvasDrawImage(0, 0, "S:/Emoji/SY3.png");
  k10.canvas->updateCanvas();
  k10.canvas->canvasDrawImage(0, 0, "S:/Emoji/SY2.png");
  k10.canvas->updateCanvas();
  k10.canvas->canvasDrawImage(0, 0, "S:/Emoji/SY1.png");
  k10.canvas->updateCanvas();
}

void  HappyAnimation() {
  k10.canvas->canvasDrawImage(0, 0, "S:/Emoji/H1.png");
  k10.canvas->updateCanvas();
  k10.canvas->canvasDrawImage(0, 0, "S:/Emoji/H2.png");
  k10.canvas->updateCanvas();
  k10.canvas->canvasDrawImage(0, 0, "S:/Emoji/H3.png");
  k10.canvas->updateCanvas();
  k10.canvas->canvasDrawImage(0, 0, "S:/Emoji/H4.png");
  k10.canvas->updateCanvas();
  k10.canvas->canvasDrawImage(0, 0, "S:/Emoji/H4.png");
  k10.canvas->updateCanvas();
  k10.canvas->canvasDrawImage(0, 0, "S:/Emoji/H3.png");
  k10.canvas->updateCanvas();
  k10.canvas->canvasDrawImage(0, 0, "S:/Emoji/H2.png");
  k10.canvas->updateCanvas();
  k10.canvas->canvasDrawImage(0, 0, "S:/Emoji/H1.png");
  k10.canvas->updateCanvas();
}

void SadAnimation() {
  k10.canvas->canvasDrawImage(0, 0, "S:/Emoji/S1.png");
  k10.canvas->updateCanvas();
  k10.canvas->canvasDrawImage(0, 0, "S:/Emoji/S2.png");
  k10.canvas->updateCanvas();
  k10.canvas->canvasDrawImage(0, 0, "S:/Emoji/S3.png");
  k10.canvas->updateCanvas();
  k10.canvas->canvasDrawImage(0, 0, "S:/Emoji/S4.png");
  k10.canvas->updateCanvas();
  k10.canvas->canvasDrawImage(0, 0, "S:/Emoji/S5.png");
  k10.canvas->updateCanvas();
  k10.canvas->canvasDrawImage(0, 0, "S:/Emoji/S4.png");
  k10.canvas->updateCanvas();
  k10.canvas->canvasDrawImage(0, 0, "S:/Emoji/S3.png");
  k10.canvas->updateCanvas();
  k10.canvas->canvasDrawImage(0, 0, "S:/Emoji/S2.png");
  k10.canvas->updateCanvas();
  k10.canvas->canvasDrawImage(0, 0, "S:/Emoji/S1.png");
  k10.canvas->updateCanvas();
}

void BoaredAnimation() {
  k10.canvas->canvasDrawImage(0, 0, "S:/Emoji/B1.png");
  k10.canvas->updateCanvas();
  k10.canvas->canvasDrawImage(0, 0, "S:/Emoji/B2.png");
  k10.canvas->updateCanvas();
  k10.canvas->canvasDrawImage(0, 0, "S:/Emoji/B3.png");
  k10.canvas->updateCanvas();
  k10.canvas->canvasDrawImage(0, 0, "S:/Emoji/B3.png");
  k10.canvas->updateCanvas();
  k10.canvas->canvasDrawImage(0, 0, "S:/Emoji/B2.png");
  k10.canvas->updateCanvas();
  k10.canvas->canvasDrawImage(0, 0, "S:/Emoji/B1.png");
  k10.canvas->updateCanvas();
}

void ColdAnimation() {
  k10.canvas->canvasDrawImage(0, 0, "S:/Emoji/C1.png");
  k10.canvas->updateCanvas();
  k10.canvas->canvasDrawImage(0, 0, "S:/Emoji/C2.png");
  k10.canvas->updateCanvas();
  k10.canvas->canvasDrawImage(0, 0, "S:/Emoji/C3.png");
  k10.canvas->updateCanvas();
  k10.canvas->canvasDrawImage(0, 0, "S:/Emoji/C4.png");
  k10.canvas->updateCanvas();
  k10.canvas->canvasDrawImage(0, 0, "S:/Emoji/C5.png");
  k10.canvas->updateCanvas();
  k10.canvas->canvasDrawImage(0, 0, "S:/Emoji/C6.png");
  k10.canvas->updateCanvas();
  k10.canvas->canvasDrawImage(0, 0, "S:/Emoji/C6.png");
  k10.canvas->updateCanvas();
  k10.canvas->canvasDrawImage(0, 0, "S:/Emoji/C5.png");
  k10.canvas->updateCanvas();
  k10.canvas->canvasDrawImage(0, 0, "S:/Emoji/C4.png");
  k10.canvas->updateCanvas();
  k10.canvas->canvasDrawImage(0, 0, "S:/Emoji/C3.png");
  k10.canvas->updateCanvas();
  k10.canvas->canvasDrawImage(0, 0, "S:/Emoji/C2.png");
  k10.canvas->updateCanvas();
  k10.canvas->canvasDrawImage(0, 0, "S:/Emoji/C1.png");
  k10.canvas->updateCanvas();
}

void HotAnimation() {
  k10.canvas->canvasDrawImage(0, 0, "S:/Emoji/D1.png");
  k10.canvas->updateCanvas();
  k10.canvas->canvasDrawImage(0, 0, "S:/Emoji/D2.png");
  k10.canvas->updateCanvas();
  k10.canvas->canvasDrawImage(0, 0, "S:/Emoji/D3.png");
  k10.canvas->updateCanvas();
  k10.canvas->canvasDrawImage(0, 0, "S:/Emoji/D4.png");
  k10.canvas->updateCanvas();
  k10.canvas->canvasDrawImage(0, 0, "S:/Emoji/D5.png");
  k10.canvas->updateCanvas();
  k10.canvas->canvasDrawImage(0, 0, "S:/Emoji/D6.png");
  k10.canvas->updateCanvas();
  k10.canvas->canvasDrawImage(0, 0, "S:/Emoji/D7.png");
  k10.canvas->updateCanvas();
  k10.canvas->canvasDrawImage(0, 0, "S:/Emoji/D6.png");
  k10.canvas->updateCanvas();
  k10.canvas->canvasDrawImage(0, 0, "S:/Emoji/D5.png");
  k10.canvas->updateCanvas();
  k10.canvas->canvasDrawImage(0, 0, "S:/Emoji/D4.png");
  k10.canvas->updateCanvas();
  k10.canvas->canvasDrawImage(0, 0, "S:/Emoji/D3.png");
  k10.canvas->updateCanvas();
  k10.canvas->canvasDrawImage(0, 0, "S:/Emoji/D2.png");
  k10.canvas->updateCanvas();
  k10.canvas->canvasDrawImage(0, 0, "S:/Emoji/D1.png");
  k10.canvas->updateCanvas();
}

void AngryAnimation() {
  k10.canvas->canvasDrawImage(0, 0, "S:/Emoji/A1.png");
  k10.canvas->updateCanvas();
  k10.canvas->canvasDrawImage(0, 0, "S:/Emoji/A2.png");
  k10.canvas->updateCanvas();
  k10.canvas->canvasDrawImage(0, 0, "S:/Emoji/A3.png");
  k10.canvas->updateCanvas();
  k10.canvas->canvasDrawImage(0, 0, "S:/Emoji/A4.png");
  k10.canvas->updateCanvas();
  k10.canvas->canvasDrawImage(0, 0, "S:/Emoji/A5.png");
  k10.canvas->updateCanvas();
  k10.canvas->canvasDrawImage(0, 0, "S:/Emoji/A6.png");
  k10.canvas->updateCanvas();
  k10.canvas->canvasDrawImage(0, 0, "S:/Emoji/A6.png");
  k10.canvas->updateCanvas();
  k10.canvas->canvasDrawImage(0, 0, "S:/Emoji/A5.png");
  k10.canvas->updateCanvas();
  k10.canvas->canvasDrawImage(0, 0, "S:/Emoji/A4.png");
  k10.canvas->updateCanvas();
  k10.canvas->canvasDrawImage(0, 0, "S:/Emoji/A3.png");
  k10.canvas->updateCanvas();
  k10.canvas->canvasDrawImage(0, 0, "S:/Emoji/A2.png");
  k10.canvas->updateCanvas();
  k10.canvas->canvasDrawImage(0, 0, "S:/Emoji/A1.png");
  k10.canvas->updateCanvas();
}

void WinkAnimation() {
  k10.canvas->canvasDrawImage(0, 0, "S:/Emoji/V1.png");
  k10.canvas->updateCanvas();
  k10.canvas->canvasDrawImage(0, 0, "S:/Emoji/V2.png");
  k10.canvas->updateCanvas();
  k10.canvas->canvasDrawImage(0, 0, "S:/Emoji/V3.png");
  k10.canvas->updateCanvas();
  k10.canvas->canvasDrawImage(0, 0, "S:/Emoji/V4.png");
  k10.canvas->updateCanvas();
  k10.canvas->canvasDrawImage(0, 0, "S:/Emoji/V5.png");
  k10.canvas->updateCanvas();
  k10.canvas->canvasDrawImage(0, 0, "S:/Emoji/V6.png");
  k10.canvas->updateCanvas();
  k10.canvas->canvasDrawImage(0, 0, "S:/Emoji/V7.png");
  k10.canvas->updateCanvas();
  k10.canvas->canvasDrawImage(0, 0, "S:/Emoji/V8.png");
  k10.canvas->updateCanvas();
  k10.canvas->canvasDrawImage(0, 0, "S:/Emoji/V8.png");
  k10.canvas->updateCanvas();
  k10.canvas->canvasDrawImage(0, 0, "S:/Emoji/V7.png");
  k10.canvas->updateCanvas();
  k10.canvas->canvasDrawImage(0, 0, "S:/Emoji/V6.png");
  k10.canvas->updateCanvas();
  k10.canvas->canvasDrawImage(0, 0, "S:/Emoji/V5.png");
  k10.canvas->updateCanvas();
  k10.canvas->canvasDrawImage(0, 0, "S:/Emoji/V4.png");
  k10.canvas->updateCanvas();
  k10.canvas->canvasDrawImage(0, 0, "S:/Emoji/V3.png");
  k10.canvas->updateCanvas();
  k10.canvas->canvasDrawImage(0, 0, "S:/Emoji/V2.png");
  k10.canvas->updateCanvas();
  k10.canvas->canvasDrawImage(0, 0, "S:/Emoji/V1.png");
  k10.canvas->updateCanvas();
}

void LaughAnimation() {
  k10.canvas->canvasDrawImage(0, 0, "S:/Emoji/L1.png");
  k10.canvas->updateCanvas();
  k10.canvas->canvasDrawImage(0, 0, "S:/Emoji/L2.png");
  k10.canvas->updateCanvas();
  k10.canvas->canvasDrawImage(0, 0, "S:/Emoji/L3.png");
  k10.canvas->updateCanvas();
  k10.canvas->canvasDrawImage(0, 0, "S:/Emoji/L3.png");
  k10.canvas->updateCanvas();
  k10.canvas->canvasDrawImage(0, 0, "S:/Emoji/L2.png");
  k10.canvas->updateCanvas();
  k10.canvas->canvasDrawImage(0, 0, "S:/Emoji/L1.png");
  k10.canvas->updateCanvas();
}
