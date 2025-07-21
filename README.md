# Rebuilt-Rebuilt

FRC Team 172 Robot Code - 2025 Season

This repository contains the robot code for Northern Force FRC Team 172's 2025 competition robot, built using WPILib's command-based framework in C++.

## Prerequisites

Before building this project, ensure you have the following installed:

### Required Software
- **WPILib**: Download and install the latest 2025 WPILib suite from [WPILib Releases](https://github.com/wpilibsuite/allwpilib/releases)
- **Java 17 or higher**: Required for Gradle build system
- **Git**: For version control

### Development Environment
- **Visual Studio Code** with the WPILib extension (recommended)
- Or any C++ IDE with proper toolchain support

## Building the Project

### Clone the Repository
```bash
git clone <repository-url>
cd Rebuilt-Rebuilt
```

### Build for Simulation (Desktop)
To build and run the robot code in simulation mode:

```bash
# Build the project for simulation
./gradlew build -Ptoolchain-optional-roboRio

# Run robot simulation (native C++)
./gradlew simulateNative
```

### Build for Robot Deployment

#### Install RoboRIO Toolchain
First, install the required toolchain for building RoboRIO binaries:
```bash
./gradlew installRoboRioToolchain
```

#### Build Robot Code
```bash
# Build robot executable
./gradlew build

# Or build release version
./gradlew frcUserProgramReleaseExecutable
```

## Deploying to Robot

### Prerequisites for Deployment
- Robot must be powered on and connected to your development machine
- Ensure the robot's team number is set correctly (Team 172)

### Deploy Commands
```bash
# Deploy robot code to RoboRIO
./gradlew deploy

# Deploy and start robot program
./gradlew deploy
```

## Testing

### Running Unit Tests
```bash
# Run all tests
./gradlew test

# Run specific test executables
./gradlew checkFrcUserProgramTestDebugGoogleTestExe
```

### Running Robot Simulation
```bash
# Start robot simulation (C++ native)
./gradlew simulateNative

# Start robot simulation in debug mode
./gradlew simulateNativeDebug

# Available simulation tools:
./gradlew Glass          # Robot state visualization
./gradlew ShuffleBoard   # Dashboard and data visualization
./gradlew OutlineViewer  # NetworkTables viewer
```

## Development Workflow

### Code Organization
- `src/main/cpp/`: Main robot code
- `src/main/include/`: Header files
- `src/test/cpp/`: Unit tests
- `src/main/deploy/`: Files deployed to robot

### Common Gradle Tasks
```bash
# Clean build artifacts
./gradlew clean

# Full build and test
./gradlew build

# Deploy to robot
./gradlew deploy

# Generate VS Code configuration
./gradlew generateVsCodeConfig

# List all available tasks
./gradlew tasks
```

### Vendor Dependencies
To add or update vendor dependencies:
```bash
# Install from URL
./gradlew vendordep --url=<vendor-json-url>

# View native dependency graph
./gradlew printNativeDependencyGraph
```

## Troubleshooting

### Common Issues

**"No Toolchain Found for roboRio"**
```bash
./gradlew installRoboRioToolchain
```

**Build fails with missing toolchain**
```bash
# Use optional toolchain flag for simulation-only builds
./gradlew build -Ptoolchain-optional-roboRio
```

**Robot not found during deployment**
```bash
# Discover robot on network
./gradlew discoverroborio
```

### Getting Help
- Check WPILib documentation: https://docs.wpilib.org
- FRC Programming resources: https://frc-docs.readthedocs.io
- Team 172 specific questions: Contact team mentors/programmers

## Team Information
- **Team Number**: 172 (Northern Force)
- **Season**: 2025
- **Framework**: WPILib Command-Based
- **Language**: C++

## License
This project is licensed under the WPILib BSD license. See `WPILib-License.md` for details.