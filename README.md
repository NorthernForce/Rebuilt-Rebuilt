# Rebuilt-Rebuilt

A C++ WPILib robotics project.

## Building

This project uses Gradle with the WPILib plugin for building and deployment.

### Prerequisites

- WPILib 2025.1.1-beta-3 or later
- Gradle (included via Gradle Wrapper)

### Build Commands

```bash
# Give execute permission to gradlew (Linux/macOS)
chmod +x gradlew

# Install RoboRIO toolchain
./gradlew installRoboRioToolchain

# Build the project
./gradlew build

# Deploy to robot
./gradlew deploy
```

## Code Formatting

This project uses clang-format for code formatting. The formatting workflow will automatically:

- Check code formatting on all pushes and pull requests
- Auto-format code in pull requests when needed

To manually format code locally:

```bash
# Install clang-format
sudo apt-get install clang-format  # Ubuntu/Debian
# or
brew install clang-format  # macOS

# Format all files
find src -name "*.cpp" -o -name "*.h" -o -name "*.hpp" | xargs clang-format -i
```

## Project Structure

```
src/
├── main/
│   ├── cpp/           # C++ source files
│   ├── include/       # Header files
│   └── deploy/        # Files to deploy to robot
└── test/
    └── cpp/           # Test source files
```

## Development

This project follows the standard WPILib C++ project structure. The main robot code is in `src/main/cpp/Robot.cpp` and `src/main/cpp/RobotContainer.cpp`.