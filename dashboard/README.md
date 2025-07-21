# Team 172 Reefscape Dashboard

A modern React-based dashboard interface for FRC Team 172's 2025 Reefscape robot, featuring real-time field visualization and autonomous control capabilities.

![Dashboard Screenshot](https://github.com/user-attachments/assets/eceb944d-93a4-4104-9019-3b0e32c74e90)

## Features

### Interactive Field Display
- **Real-time robot position tracking** with field-relative visualization
- **Clickable reef positions** (A-L) for autonomous target selection
- **Algae removal locations** (AB, CD, EF, GH, IJ, KL) clearly marked
- **Coral station positions** for game piece collection
- **Alliance-specific color coding** (Blue/Red alliance support)
- **Field elements** including reef center and boundaries

### Robot Status Monitoring
- **Connection status** indicator with real-time updates
- **Robot position** display (X, Y coordinates and rotation)
- **Control mode** indicator (Autonomous vs Manual)
- **Game piece detection** (Has Coral/No Coral status)
- **Alliance identification** with visual indicators

### Autonomous Controls
- **Target selection** via interactive field positions
- **Command execution** interface with multiple autonomous routines:
  - Move to Selected Position
  - Score and Return
  - Collect from Coral Station
  - Remove Algae
  - Simple Leave
- **Emergency stop** functionality for safety
- **Quick action buttons** for common operations
- **Real-time command status** and feedback

### Safety Features
- **Emergency stop button** prominently displayed
- **Connection status monitoring** with visual alerts
- **Command validation** before execution
- **Manual override capabilities** during autonomous mode

## Technology Stack

- **Frontend**: React 18 with modern hooks
- **Styling**: CSS3 with responsive design
- **Communication**: NetworkTables integration (planned)
- **Build System**: Create React App
- **Target**: Team dashboard displays and driver stations

## Installation

### Prerequisites
- Node.js 16+ and npm
- React development environment
- Robot NetworkTables server (for live data)

### Setup
```bash
cd dashboard
npm install
npm start
```

The dashboard will be available at `http://localhost:3000`

### Demo Mode
For development and demonstration without a robot connection:
```bash
# Serve the standalone demo
python3 -m http.server 8080
# Navigate to http://localhost:8080/demo.html
```

## Robot Integration

The dashboard is designed to integrate with the robot's C++ codebase via NetworkTables:

### Data Flow
- **Robot → Dashboard**: Position, status, sensor data
- **Dashboard → Robot**: Target positions, autonomous commands
- **Bi-directional**: Emergency stops, mode changes

### NetworkTables Keys
```
/SmartDashboard/Robot Pose -> [x, y, rotation]
/SmartDashboard/Has Game Piece -> boolean
/SmartDashboard/Autonomous Mode -> boolean
/SmartDashboard/Alliance -> "Blue"/"Red"
/SmartDashboard/Selected Position -> "A"/"B"/etc.
```

## Architecture

### Component Structure
```
src/
├── App.js                 # Main application component
├── components/
│   ├── ReefDisplay.js     # Interactive field visualization
│   ├── StatusPanel.js     # Robot status monitoring
│   └── AutonomousControls.js # Command interface
└── App.css               # Styling and theme
```

### Key Design Principles
- **Real-time updates** with smooth animations
- **Intuitive interface** for drivers and operators
- **Safety-first** design with prominent emergency controls
- **Responsive layout** for various display sizes
- **High contrast** colors for competition lighting conditions

## Customization

### Team Branding
- Primary color: `#00ff41` (Team 172 green)
- Secondary colors: Blue/Red alliance themes
- Team name and number prominently displayed

### Field Configuration
Reef positions and field elements can be modified in:
- `ReefDisplay.js` - Position coordinates and layouts
- `FieldConstants.h` - C++ robot-side field definitions

## Competition Compliance

- **FRC-approved technologies** (React, NetworkTables)
- **Driver station compatibility** with standard displays
- **Network isolation** support for competition environments
- **Offline capability** for practice and testing

## Development

### Testing
```bash
npm test
```

### Building for Production
```bash
npm run build
```

### Code Style
- ESLint configuration for consistent formatting
- React best practices and hooks patterns
- Component-based architecture for maintainability

## Team 172 Integration

This dashboard is specifically designed for Northern Force FRC Team 172's 2025 Reefscape robot, featuring:
- **Swerve drive integration** with field-relative control
- **Multi-level scoring** with superstructure height selection
- **Vision-assisted positioning** for precise reef alignment
- **Autonomous path planning** with obstacle avoidance
- **Operator workflow optimization** for efficient gameplay

## License

This project is licensed under the WPILib BSD license, consistent with the robot codebase.