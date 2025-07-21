# LED Subsystem Documentation

## Overview

The LED subsystem provides comprehensive visual feedback for robot states using a CTRE CANdle LED controller. It supports various animations and patterns to indicate game conditions such as alliance color, piece detection, autonomous mode, and endgame status.

## Features

- **CANdle Integration**: Uses CTRE Phoenix CANdle LED controller
- **Multiple Animation Types**: Rainbow, strobe, fire, and custom particle effects
- **Game State Indication**: Visual feedback for different robot states
- **Command-Based Architecture**: Easy integration with robot code using WPILib commands

## Hardware Requirements

- CTRE CANdle LED controller (CAN ID 30 by default)
- RGB LED strip (72 LEDs by default)
- Proper power supply for LED strip

## Usage Examples

### Basic Usage in RobotContainer

```cpp
#include "RobotContainer.h"

RobotContainer::RobotContainer() {
  ConfigureBindings();
}

void RobotContainer::ConfigureBindings() {
  // Set default LED behavior
  m_leds.SetDefaultCommand(m_leds.NoAlliance());
}
```

### Game State Commands

```cpp
// During autonomous
m_leds.Auto().Schedule();

// When piece is detected  
m_leds.Happy().Schedule();

// When ready to place piece
m_leds.ReadyToPlace().Schedule();

// When wanting a piece
m_leds.Hungry().Schedule();

// Alliance-specific colors
m_leds.RedAlliance().Schedule();
m_leds.BlueAlliance().Schedule();

// During endgame
m_leds.Endgame().Schedule();
```

### Manual LED Control

```cpp
// Set solid color
m_leds.SetColor(frc::Color::kBlue);

// Set specific range
m_leds.SetColor(frc::Color::kRed, 0, 10);

// Rainbow animation
m_leds.RainbowAnimation(0.75, 0.1);

// Clear all LEDs
m_leds.ResetLEDs();
```

## Available Commands

| Command | Description | Use Case |
|---------|-------------|----------|
| `AutoLEDs` | Rainbow animation | Autonomous mode |
| `Blinking` | Blinking team color | Ready to place piece |
| `EveryOther` | Alternating alliance/team colors | Alliance indication |
| `EndgameLEDs` | Fire animation | Endgame period |
| `NoAllianceLEDs` | Rainbow animation | No alliance selected |
| `PiecePresent` | Solid purple | Piece detected |
| `Runway` | Particle effect | Wanting/seeking piece |

## Configuration

LED constants are defined in `Constants.h`:

```cpp
namespace Constants::LEDConstants {
  constexpr int kCANId = 30;           // CANdle CAN ID
  constexpr int kLEDCount = 72;        // Number of LEDs
  constexpr double kLEDRate = 0.1;     // Update rate (seconds)
  
  // Team color (pink/magenta)
  const frc::Color kTeamColor{238.0/255.0, 10.0/255.0, 154.0/255.0};
}
```

## Dependencies

- CTRE Phoenix library (added via `vendordeps/Phoenix.json`)
- WPILib New Commands framework
- C++17 or later

## Integration Example

Here's how to integrate the LED subsystem into different robot modes:

```cpp
class Robot : public frc::TimedRobot {
private:
  RobotContainer m_container;

public:
  void AutonomousInit() override {
    // Start rainbow animation during auto
    m_container.GetLEDs()->Auto().Schedule();
  }
  
  void TeleopInit() override {
    // Return to default behavior
    m_container.GetLEDs()->GetDefaultCommand()->Schedule();
  }
  
  void DisabledInit() override {
    // Show alliance colors when disabled
    if (/* red alliance */) {
      m_container.GetLEDs()->RedAlliance().Schedule();
    } else if (/* blue alliance */) {
      m_container.GetLEDs()->BlueAlliance().Schedule();
    } else {
      m_container.GetLEDs()->NoAlliance().Schedule();
    }
  }
};
```

## Customization

The LED subsystem can be easily customized by:

1. **Changing colors**: Modify constants in `Constants.h`
2. **Adding new patterns**: Create new command classes following existing examples
3. **Adjusting timing**: Modify update rates and animation speeds
4. **Hardware changes**: Update CAN ID and LED count in constants

This LED subsystem provides a complete foundation for visual feedback that can be extended as needed for specific robot requirements.