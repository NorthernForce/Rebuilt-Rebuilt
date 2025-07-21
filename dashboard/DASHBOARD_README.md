# Northern Force Team 172 Dashboard

A modern React-based dashboard for FRC Team 172's robot built with Material-UI and proper FIRST Web Components NetworkTables integration.

## 🚀 Quick Start

```bash
cd dashboard
npm install
npm run dev
```

The dashboard will be available at `http://localhost:5173`

## 🎯 Features

### Driver Station ✅ Updated with NetworkTables
- **Real-time match timer**: Connected to `/FMSInfo/MatchTime` 
- **Live robot status**: Connection monitoring with NT4Provider
- **Battery monitoring**: Live voltage from `/SmartDashboard/Battery Voltage`
- **FRC Web Components**: Proper React integration using `useEntry` hooks
  - `NumberBar` for drive speed visualization
  - `Gyro` component for robot heading
  - `BasicFmsInfo` for FMS data

### Autonomous Chooser ✅ Updated with NetworkTables
- **SendableChooser integration**: Uses `/SmartDashboard/Autonomous`
- **Live status monitoring**: Real-time autonomous execution state
- **Test mode**: Safe autonomous testing with simulation mode
- **NetworkTables sync**: Bidirectional data communication

### Subsystems Monitor ✅ Updated with NetworkTables  
- **Live status monitoring**: All subsystem states from NetworkTables
- **Dynamic data visualization**: Real motor currents, speeds, sensor readings
- **Interactive controls**: Enable/disable subsystems via NetworkTables
- **FWC integration**: Proper React-wrapped web components

## 🔧 NetworkTables Integration

### Key Features
- **NT4 Protocol**: Full bidirectional communication
- **Real-time Updates**: Live data binding with `useEntry` hook
- **Connection Monitoring**: Automatic reconnection and offline state
- **Type Safety**: Full TypeScript support

### Usage Example
```typescript
import { useEntry, useNt4, NT4Provider } from '@frc-web-components/react'

// Real-time data binding
const [batteryVoltage] = useEntry('/SmartDashboard/Battery Voltage', 12.6)
const [matchTime] = useEntry('/FMSInfo/MatchTime', 135)
const [driveEnabled, setDriveEnabled] = useEntry('/SmartDashboard/Drive Enabled', true)

// Connection monitoring
const { nt4Provider } = useNt4()
const connected = nt4Provider.isConnected()
```

### Robot Integration
```java
// In Robot.java - publish telemetry data
@Override
public void robotPeriodic() {
  SmartDashboard.putNumber("Battery Voltage", RobotController.getBatteryVoltage());
  SmartDashboard.putNumber("Drive Speed", drivetrain.getCurrentSpeed());
  SmartDashboard.putNumber("Gyro Angle", gyro.getAngle());
  SmartDashboard.putBoolean("Drive Enabled", drivetrain.isEnabled());
}
```

## 🛠️ Development

### Build Commands
```bash
npm run dev          # Start development server
npm run build        # Build for production  
npm run preview      # Preview production build
npm run lint         # Run ESLint
```

### Project Structure
```
src/
├── components/           # React components
│   ├── DriverPage.tsx   # Driver station interface
│   ├── AutonomousChooser.tsx  # Auto route selection
│   └── SubsystemTreeView.tsx  # Subsystem monitoring
├── App.tsx              # Main application with tabs
└── main.tsx             # Application entry point
```

### Technologies
- **React 19** with TypeScript
- **Material-UI v5** for UI components
- **FIRST Web Components** for robot data visualization
- **Vite** for fast development and building

## 🔌 Integration

### NetworkTables Connection
To connect to live robot data, modify the components to use NetworkTables:

```typescript
// Example NetworkTables integration
import { NetworkTables } from '@frc-web-components/fwc'

// Subscribe to robot data
NetworkTables.addRobotConnectionListener((connected) => {
  setRobotConnected(connected)
})
```

### Camera Streams
Update the camera URL in `DriverPage.tsx`:
```typescript
// Current placeholder
Stream: mjpeg://roborio-172-frc.local:1181/stream.mjpg

// Update to your camera server
Stream: http://your-camera-server:port/stream
```

## 📦 Deployment

### Production Build
```bash
npm run build
```
Generates optimized static files in `dist/` folder.

### Hosting Options
- Static file hosting (Netlify, Vercel, GitHub Pages)
- Web server on driver station laptop
- RoboRIO web server (for pit scouting)

## 🤝 Contributing

1. Follow the existing component structure
2. Use Material-UI components for consistency
3. Add TypeScript types for all props
4. Test responsive design on mobile/desktop
5. Include FRC Web Components where appropriate

## 🏁 Team Information
- **Team**: 172 Northern Force
- **Season**: 2025 
- **Framework**: WPILib + React Dashboard