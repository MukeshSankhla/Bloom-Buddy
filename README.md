# 🌱 Bloom Buddy - Your Smart Plant Companion

<div align="center">
  <img src="https://via.placeholder.com/600x400/F4A261/000000?text=Bloom+Buddy" alt="Bloom Buddy Device">
  
  [![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
  [![Platform](https://img.shields.io/badge/Platform-ESP32-blue.svg)](https://www.espressif.com/en/products/socs/esp32)
  [![Status](https://img.shields.io/badge/Status-Active-brightgreen.svg)]()
</div>

## 🌟 Meet Your New Plant's Best Friend

Bloom Buddy is an intelligent, interactive soil moisture probe that transforms plant care into a delightful experience. This adorable ESP32-powered device doesn't just monitor your plants—it communicates with you through expressive animations, voice feedback, and real-time environmental monitoring.

Watch as Bloom Buddy celebrates when you water your plants, gets sad when they're thirsty, sleeps during the night, and reacts to temperature changes with personality-filled animations and sounds!

## ✨ Features

### 🎭 **Interactive Personality System**
- **Expressive Animations**: 12 different animated emotions (Happy, Sad, Sleep, Cold, Hot, and more)
- **Voice Feedback**: Contextual audio responses with randomized sound files
- **Smart State Management**: Remembers previous conditions to provide appropriate reactions

### 📊 **Comprehensive Environmental Monitoring**
- **Soil Moisture**: Precise moisture level detection with percentage conversion
- **Temperature & Humidity**: Real-time atmospheric monitoring (AHT20 sensor)
- **Light Detection**: Automatic day/night cycle recognition
- **Data Display**: On-demand sensor readings via button press

### 🎵 **Audio-Visual Experience**
- **Built-in Speaker**: Plays contextual voice responses
- **SD Card Storage**: Stores multiple audio files for varied responses
- **OLED Display**: Shows animations and sensor data
- **Button Interaction**: Manual data display trigger

### 🧠 **Intelligent Behavior**
- **Priority-Based Logic**: Water needs take precedence over environmental comfort
- **Adaptive Responses**: Different reactions for first-time vs. ongoing conditions
- **Environmental Awareness**: Adjusts behavior based on light and temperature

## 🛠️ Hardware Components

### Main Board
- **ESP32-based Controller** (UNIHIKER K10)
- **Built-in OLED Display**
- **SD Card Slot**
- **Integrated Sensors**: Light, Temperature, Humidity
- **Audio System**: Microphone and Speaker

### Sensors
- **AHT20**: Temperature and Humidity sensor
- **Analog Soil Moisture Probe**: Connected to pin P0
- **Ambient Light Sensor**: Built-in light detection
- **Interactive Button**: Manual data display trigger

### Audio System
- **Built-in Speaker**: Voice feedback playback
- **SD Card Audio Storage**: Multiple voice response files
- **Randomized Responses**: Varied audio for same conditions

## 📁 SD Structure

```
SD/
  ├── Voice/
  │   ├── All Audio Files
  ├── Emoji/
  │   ├── All Frames
```

## 🚀 Quick Start

### Prerequisites
- Arduino IDE with ESP32 board support
- UNIHIKER K10 library
- AHT20 sensor library
- SD card (for audio files)

### Installation

1. **Clone the Repository**
   ```bash
   git clone https://github.com/yourusername/bloom-buddy.git
   cd bloom-buddy
   ```

2. **Install Required Libraries**
   ```bash
   # Install via Arduino Library Manager:
   # - UNIHIKER K10
   # - AHT20 Sensor Library
   ```

3. **Prepare Audio Files**
   - Copy audio files to SD card in the structure shown above
   - Insert SD card into device

4. **Upload Code**
   - Open `BloomBuddy.ino` in Arduino IDE
   - Select ESP32 board
   - Upload to device

5. **Insert into Soil**
   - Place moisture probe into plant soil
   - Power on device
   - Watch Bloom Buddy come to life!

## 🎮 How It Works

### Behavior Logic

```mermaid
graph TD
    A[Start] --> B[Read Sensors]
    B --> C{Soil Moisture < 30%?}
    C -->|Yes| D[Play Sad Animation]
    C -->|No| E{Was Previously Dry?}
    E -->|Yes| F[Celebrate with Happy Animation]
    E -->|No| G{Light Level < 10?}
    G -->|Yes| H{Temperature Check}
    G -->|No| I[Ideal Animation]
    H -->|< 15°C| J[Cold Animation]
    H -->|> 40°C| K[Hot Animation]
    H -->|Normal| L[Sleep Animation]
```

### Response Categories

| Condition | Animation | Voice Files | Behavior |
|-----------|-----------|-------------|----------|
| **Dry Soil** | Sad | D1-D5.wav | Continuous sad display until watered |
| **Just Watered** | Happy (4x) | W1-W6.wav | Celebration sequence |
| **Nighttime** | Sleep | N1-N5.wav | Calm, sleepy behavior |
| **Morning** | Ideal | M1-M4.wav | Wake up and active |
| **Cold** | Cold | C1-C2.wav | Shivering animation |
| **Hot** | Hot | H1-H2.wav | Overheated animation |

## 🔧 Configuration

### Sensor Thresholds
```cpp
const int DRY_THRESHOLD = 30;      // Moisture % threshold
const int DARK_THRESHOLD = 10;     // Light level threshold
const int COLD_THRESHOLD = 15;     // Cold temperature (°C)
const int HOT_THRESHOLD = 40;      // Hot temperature (°C)
```

### Calibration
Adjust moisture sensor calibration values based on your soil type:
```cpp
const int MOISTURE_WET_VALUE = 600;    // Wet soil reading
const int MOISTURE_DRY_VALUE = 3046;   // Dry soil reading
```

## 🎨 Customization

### Adding New Animations
1. Create new animation function in code
2. Add appropriate voice files to SD card
3. Integrate into main behavior logic

### Custom Voice Responses
- Record your own WAV files
- Follow naming convention (B1.wav, D1.wav, etc.)
- Place in `S:Voice/` directory on SD card

### Behavior Modification
- Adjust thresholds in configuration section
- Modify animation sequences
- Add new sensor-based conditions

## 🤝 Contributing

We welcome contributions! Here's how you can help:

1. **Fork the repository**
2. **Create a feature branch** (`git checkout -b feature/amazing-feature`)
3. **Commit your changes** (`git commit -m 'Add amazing feature'`)
4. **Push to branch** (`git push origin feature/amazing-feature`)
5. **Open a Pull Request**

### Areas for Contribution
- Additional animation sequences
- New voice response packs
- Mobile app integration
- IoT connectivity features
- Advanced plant care algorithms

## 📸 Gallery

| State | Animation | Description |
|-------|-----------|-------------|
| 😊 Happy | ![Happy](https://github.com/MukeshSankhla/Bloom-Buddy/blob/main/Gifs/12.gif?text=😊) | Plant is well-watered and content |
| 😢 Sad | ![Sad](https://github.com/MukeshSankhla/Bloom-Buddy/blob/main/Gifs/10.gif?text=😢) | Plant needs water |
| 😴 Sleep | ![Sleep](https://github.com/MukeshSankhla/Bloom-Buddy/blob/main/Gifs/17.gif?text=😴) | Nighttime rest mode |
| 🥶 Cold | ![Cold](https://github.com/MukeshSankhla/Bloom-Buddy/blob/main/Gifs/7.gif?text=🥶) | Temperature too low |
| 🥵 Hot | ![Hot](https://github.com/MukeshSankhla/Bloom-Buddy/blob/main/Gifs/5.gif?text=🥵) | Temperature too high |

## 📊 Technical Specifications

- **Microcontroller**: ESP32
- **Display**: OLED (integrated)
- **Sensors**: AHT20 (temp/humidity), Analog moisture probe, Light sensor
- **Audio**: Built-in speaker and microphone
- **Storage**: SD card support
- **Power**: USB/Battery powered
- **Dimensions**: Compact soil probe form factor

## 🐛 Troubleshooting

### Common Issues

**Device doesn't respond**
- Check power connection
- Verify SD card is properly inserted
- Ensure audio files are in correct directory

**Inaccurate moisture readings**
- Calibrate sensor values for your soil type
- Clean moisture probe contacts
- Check probe placement depth

**No audio playback**
- Verify audio files are in WAV format
- Check SD card file structure
- Ensure speaker connections

## 📄 License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## 🙏 Acknowledgments

- Thanks to the Arduino and ESP32 communities
- UNIHIKER team for the excellent development board
- Plant enthusiasts who inspired this project
- Contributors and testers
- 
---

🌱 Happy Gardening with Bloom Buddy! 🌱

Made with ❤️ for plants and their humans
