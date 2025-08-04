# Tuning System Documentation

## Overview

The tuning system provides a way to create constants that can be adjusted at runtime via NetworkTables while maintaining transparent usage through operator overloading. This allows for easy parameter tuning during development and testing without requiring code recompilation.

## Features

- **Transparent Usage**: Tunables can be used exactly like regular constants thanks to implicit conversion operators
- **NetworkTables Integration**: Values are automatically published to and subscribed from NetworkTables
- **Type Safety**: Full support for primitive types (double, int, bool) and WPILib units types
- **Logging Integration**: Automatic logging of value changes through the existing logging system
- **Zero Performance Overhead**: When not being actively tuned, tunables behave like regular constants

## Basic Usage

### Including Headers

```cpp
#include <tuning/Tunable.h>
// For predefined tunable constants
#include <constants/TunableConstants.h>
```

### Creating Tunables

```cpp
// Basic types
nfr::TunableDouble speed("MySubsystem/Speed", 0.5);  // Default value 0.5
nfr::TunableInt encoderCPR("MySubsystem/EncoderCPR", 2048);
nfr::TunableBool enabled("MySubsystem/Enabled", true);

// Units types
nfr::Tunable<units::meters_per_second_t> maxVelocity("Drive/MaxVelocity", 3.0_mps);
nfr::Tunable<units::degree_t> armAngle("Arm/Angle", 45.0_deg);
```

### Using Tunables

Tunables can be used anywhere you would use the underlying type:

```cpp
// Direct usage (implicit conversion)
double currentSpeed = speed;  // Gets current value from NetworkTables
SetMotorSpeed(speed);         // Pass directly to functions

// Arithmetic operations
double scaledSpeed = speed * 0.8;
if (speed > 0.3) {
    // Conditional logic works naturally
}

// Units arithmetic
units::meter_t distance = maxVelocity * 2.0_s;
```

### Setting Values

```cpp
// Programmatically set values (also updates NetworkTables)
speed.Set(0.7);
speed = 0.7;  // Assignment operator

// Get current value explicitly
double currentValue = speed.Get();
```

### Periodic Updates

To receive updates from NetworkTables, call `Update()` periodically:

```cpp
void RobotPeriodic() {
    // Update individual tunables
    speed.Update();
    
    // Or use the convenience methods for groups
    TunableDriveConstants::UpdateFromNetworkTables();
}
```

## Advanced Features

### Using the TuningManager

For centralized management of multiple tunables:

```cpp
#include <tuning/TuningManager.h>

// Register tunables for automatic updates
nfr::TuningManager::GetInstance().RegisterTunable([&]() {
    myTunable.Update();
});

// Update all registered tunables
nfr::TuningManager::GetInstance().UpdateAll();
```

### Custom NetworkTables Tables

By default, tunables use the "Tuning" table. You can specify a custom table:

```cpp
nfr::TunableDouble customValue("MyKey", 1.0, "CustomTable");
```

### Integration with Existing Code

The system is designed to integrate seamlessly with existing constants:

```cpp
// Before (static constants)
static constexpr double kMotorSpeed = 0.5;

// After (tunable constants)  
static inline nfr::TunableDouble kMotorSpeed = nfr::TunableDouble("MotorSpeed", 0.5);

// Usage remains exactly the same!
SetMotorSpeed(kMotorSpeed);
```

## Example Implementation

See `TunableConstants.h` for a complete example of converting drive constants to tunables:

```cpp
class TunableDriveConstants {
public:
    static inline TunableDouble kTranslationP = TunableDouble("Drive/Translation/P", 0.5);
    static inline Tunable<units::meters_per_second_t> kMaxSpeed = 
        Tunable<units::meters_per_second_t>("Drive/MaxSpeed", 3.0_mps);
    
    static void UpdateFromNetworkTables() {
        kTranslationP.Update();
        kMaxSpeed.Update();
    }
};
```

## NetworkTables Interface

Tunables automatically create NetworkTables entries with the following structure:

```
/Tuning/
  ├── Drive/
  │   ├── Translation/
  │   │   ├── P (double)
  │   │   ├── I (double)
  │   │   └── D (double)
  │   └── MaxSpeed (double, units values stored as their numeric value)
  └── Example/
      ├── MotorSpeed (double)
      ├── MotorEnabled (boolean)
      └── EncoderCPR (integer)
```

These can be viewed and modified using:
- **Shuffleboard**: WPILib's dashboard application
- **NetworkTables clients**: Custom dashboards or command-line tools
- **Glass**: WPILib's robot simulation GUI

## Performance Considerations

- **Minimal Overhead**: Implicit conversions have virtually no performance cost
- **NetworkTables Updates**: Only call `Update()` as frequently as needed (typically 20-50Hz)
- **Memory Usage**: Each tunable stores the current value plus NetworkTables publishers/subscribers

## Best Practices

1. **Naming Convention**: Use hierarchical names like "Subsystem/Parameter/Subparameter"
2. **Grouping**: Create static update methods for related constants
3. **Default Values**: Always provide sensible default values
4. **Documentation**: Comment the purpose and valid ranges of tunable parameters
5. **Testing**: Include tunables in unit tests to ensure they work like regular constants

## Migration from Static Constants

Converting existing static constants to tunables is straightforward:

1. Change `static constexpr` to `static inline`
2. Wrap the type with `Tunable<>` or use type aliases
3. Add NetworkTables key as the first parameter
4. Add periodic `Update()` calls
5. Optionally group related constants with update methods

The usage code remains completely unchanged, making this a safe refactoring.