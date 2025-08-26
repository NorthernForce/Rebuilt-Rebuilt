# Northern Force FRC Team 172 Robot Code

FRC Team 172 "Northern Force" robot code for the 2025 season, built using WPILib's command-based framework in C++. This is a swerve drive robot with comprehensive logging, simulation capabilities, and Phoenix 6 motor controller integration.

Always reference these instructions first and fallback to search or bash commands only when you encounter unexpected information that does not match the info here.

## Working Effectively

### Bootstrap and Build the Repository
- **NEVER CANCEL builds or long-running commands** - All operations complete within 1-2 minutes
- Clone and build:
  - `git clone <repository-url>`
  - `cd Rebuilt-Rebuilt`
  - `./gradlew installRoboRioToolchain` -- takes ~5 seconds, downloads and installs RoboRIO cross-compilation toolchain
  - `./gradlew build` -- takes 1-2 minutes. Set timeout to 5+ minutes. NEVER CANCEL.
- **For simulation-only builds** (if RoboRIO toolchain not needed):
  - `./gradlew build -Ptoolchain-optional-roboRio` -- takes 1-2 minutes, skips RoboRIO builds

### Testing and Verification
- `./gradlew check` -- takes 1-2 minutes, runs all tests and verification. Set timeout to 5+ minutes. NEVER CANCEL.
- `./gradlew checkFrcUserProgramTestDebugGoogleTestExe` -- runs unit tests specifically
- Tests currently pass with 0 test cases (project uses integration testing via simulation)

### Robot Simulation
- **Start simulation**: `./gradlew simulateNative` -- starts immediately, runs indefinitely until stopped
- **ALWAYS manually validate robot functionality** by running simulation and checking:
  - Robot program starts successfully (`********** Robot program startup complete **********`)
  - NetworkTables server starts (ports 1735 and 5810)
  - Phoenix diagnostics server starts (port 1250)
  - CAN bus simulation connects (`[phoenix] CANbus Connected: sim`)
  - Data logging creates .wpilog files in `logs/` directory
- **Expected warnings in headless environment**:
  - `GLFW Error 65550: X11: The DISPLAY environment variable is missing` (GUI components)
  - `Warning: Joystick Button 7 missing` (no controllers connected)
- **Simulation GUI tools** (require X11 display):
  - `./gradlew Glass` -- Robot state visualization (fails in headless)
  - `./gradlew ShuffleBoard` -- Dashboard and data visualization (fails in headless)
  - `./gradlew OutlineViewer` -- NetworkTables viewer (fails in headless)

### Robot Deployment
- **Deploy to robot**: `./gradlew deploy`
- **IMPORTANT**: Deployment will fail without robot connection (expected behavior):
  - Robot discovery attempts multiple IP addresses (172.22.11.2, 10.1.72.2, etc.)
  - Times out after discovery phase if no robot found
  - This is normal behavior when not connected to a robot
- **Build for deployment**: `./gradlew frcUserProgramReleaseExecutable` -- creates robot executable

## Code Quality and Maintenance

### Formatting and Linting
- **ALWAYS run before committing**: `./gradlew format` -- takes ~5 seconds, formats all C++ files with clang-format
- Code style: Google C++ style with Allman braces (configured in `.clang-format`)
- **CI Requirements**: GitHub Actions will automatically check and fix formatting on PRs
- Format check runs on pushes to main/develop branches

### VS Code Integration
- **Generate IDE config**: `./gradlew generateVsCodeConfig` -- takes ~3 seconds
- IntelliSense configured for C++ with WPILib
- Pre-configured file associations for FRC development

## Validation Scenarios

### Complete Development Workflow Validation
After making changes, ALWAYS run this complete validation sequence:
1. **Build and test**: `./gradlew clean build` -- 1-2 minutes, verifies compilation
2. **Format code**: `./gradlew format` -- 5 seconds, ensures style compliance
3. **Run simulation**: `./gradlew simulateNative` -- verify robot starts and runs
4. **Check logs**: Verify `logs/*.wpilog` files are created during simulation
5. **Stop simulation**: Use Ctrl+C or stop command after validation

### Manual Testing Scenarios
- **Simulation startup**: Robot program should start and show "startup complete" message
- **Network services**: NetworkTables and Phoenix diagnostics servers should start
- **CAN simulation**: Phoenix library should connect to simulated CAN bus
- **Data logging**: Log files should appear in `logs/` directory during simulation

## Common Tasks and Solutions

### Build Issues
- **"No Toolchain Found for roboRio"**: Run `./gradlew installRoboRioToolchain`
- **Build fails with missing toolchain**: Use `-Ptoolchain-optional-roboRio` flag for simulation-only builds
- **Gradle daemon issues**: Run `./gradlew --stop` then retry

### Deployment Issues  
- **Robot not found during deployment**: Expected when not connected to robot network
- **Discover robot on network**: `./gradlew discoverroborio` (will timeout if no robot)

### Performance Settings
- Gradle is configured for parallel builds with 4 workers and 4GB heap
- Build cache enabled for faster subsequent builds
- All optimization settings in `gradle.properties`

## Repository Structure

### Key Directories
- `src/main/cpp/` -- Main robot code (8 C++ source files)
- `src/main/include/` -- Header files and constants
- `src/test/cpp/` -- Unit tests (GoogleTest framework)
- `src/main/deploy/` -- Files deployed to robot (PathPlanner configs)
- `vendordeps/` -- Third-party dependencies (Phoenix6, PathPlanner, Choreo)
- `logs/` -- Simulation and robot log files (auto-generated)

### Important Files
- `build.gradle` -- Build configuration with custom tasks
- `.clang-format` -- C++ formatting rules (Google style, Allman braces)
- `gradle.properties` -- Performance optimization settings
- `.github/workflows/` -- CI/CD pipelines (build.yml, format.yml)

### Core Robot Functionality
- **Swerve Drive**: Main robot drive system with 4 modules
- **Logging System**: WPILib data logging with NetworkTables integration
- **Auto Paths**: PathPlanner integration for autonomous routines
- **Phoenix 6**: CTRE motor controller and CAN device support

### Vendor Dependencies
- Phoenix6 (CTRE motor controllers) -- latest 2025 version
- PathplannerLib (autonomous path following)
- ChoreoLib (trajectory generation)
- WPILibNewCommands (command-based framework)

## CI/CD Integration

### GitHub Actions
- **Build workflow** (`.github/workflows/build.yml`): Builds robot code on ubuntu-22.04 with WPILib container
- **Format workflow** (`.github/workflows/format.yml`): Checks and auto-fixes C++ formatting
- **NEVER CANCEL**: All CI operations complete within 2-3 minutes

### Development Guidelines
- Always run `./gradlew format` before pushing changes
- Build validation happens automatically on all pushes
- Format fixes applied automatically on pull requests

## Team Information
- **Team Number**: 172 (Northern Force)
- **Season**: 2025
- **Framework**: WPILib Command-Based C++
- **Robot Type**: Swerve Drive Competition Robot