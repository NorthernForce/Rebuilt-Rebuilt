# Build Performance Optimization

This project has been optimized for faster build times during development.

## Performance Improvements

### Before Optimization
- Clean build: ~2 minutes 16 seconds
- Built for both desktop and RoboRio platforms on every build
- No parallel compilation optimization
- No build caching

### After Optimization
- Clean build: ~1 minute 37 seconds (39 second improvement)
- Incremental build: ~1.6 seconds
- Only builds for desktop by default (faster development)
- Parallel compilation enabled
- Build caching enabled
- Optimized compiler flags

## Build Commands

### Development (Fast)
```bash
./gradlew build
```
Builds only for desktop simulation - fastest for development and testing.

### Deployment (Complete)
```bash
./gradlew build -PbuildRoboRio=true
```
Builds for both desktop and RoboRio - use when deploying to robot.

### Other Commands
```bash
./gradlew clean build     # Clean build (development)
./gradlew deploy          # Deploy to RoboRio (automatically includes RoboRio build)
./gradlew format          # Format C++ code
```

## Performance Features

1. **Conditional Platform Building**: Only builds for RoboRio when explicitly requested
2. **Gradle Daemon**: Keeps Gradle running for faster subsequent builds
3. **Parallel Builds**: Utilizes multiple CPU cores
4. **Build Cache**: Reuses compilation results when possible
5. **Optimized Memory**: Increased JVM memory for faster compilation
6. **Optimized Git Properties**: Reduces git metadata generation overhead

## Configuration

Performance settings are configured in `gradle.properties`:
- Parallel builds enabled
- 4GB JVM memory allocation
- 4 worker processes
- Build caching enabled