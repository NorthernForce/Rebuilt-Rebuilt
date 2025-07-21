# Northern Force Team 172 Dashboard

A modern React-based dashboard for FRC Team 172's robot built with Material-UI and FIRST Web Components.

## 🚀 Quick Start

```bash
cd dashboard
npm install
npm run dev
```

The dashboard will be available at `http://localhost:5173`

## 🎯 Features

### Driver Station
- Real-time match timer and robot status
- Camera feed integration
- FRC Web Components for telemetry
- Battery and connection monitoring

### Autonomous Chooser  
- Multiple autonomous routes with descriptions
- Starting position selection
- Complexity indicators
- Test functionality with safety warnings

### Subsystems Monitor
- Tree view of all robot subsystems
- Individual status dashboards
- Real-time sensor data visualization
- Enable/disable controls

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