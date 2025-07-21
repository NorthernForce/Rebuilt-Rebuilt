import React, { useState, useEffect } from 'react';
import './App.css';
import ReefDisplay from './components/ReefDisplay';
import StatusPanel from './components/StatusPanel';
import AutonomousControls from './components/AutonomousControls';

function App() {
  const [robotData, setRobotData] = useState({
    alliance: 'blue',
    robotPose: { x: 0, y: 0, rotation: 0 },
    autoMode: false,
    hasGamePiece: false,
    selectedReefPosition: null,
    superstructureHeight: 'L1',
    isConnected: false
  });

  // Simulate NetworkTables connection (in real implementation, this would connect to robot)
  useEffect(() => {
    const interval = setInterval(() => {
      // Simulate robot data updates
      setRobotData(prev => ({
        ...prev,
        robotPose: {
          x: prev.robotPose.x + (Math.random() - 0.5) * 0.1,
          y: prev.robotPose.y + (Math.random() - 0.5) * 0.1,
          rotation: prev.robotPose.rotation + (Math.random() - 0.5) * 5
        },
        isConnected: true
      }));
    }, 100);

    return () => clearInterval(interval);
  }, []);

  const handleReefPositionSelect = (position) => {
    setRobotData(prev => ({
      ...prev,
      selectedReefPosition: position
    }));
    console.log('Selected reef position:', position);
  };

  const handleAutonomousToggle = () => {
    setRobotData(prev => ({
      ...prev,
      autoMode: !prev.autoMode
    }));
  };

  return (
    <div className="App">
      <header className="App-header">
        <h1>Team 172 Northern Force - Reefscape Dashboard</h1>
        <div className="connection-status">
          Status: {robotData.isConnected ? 'Connected' : 'Disconnected'}
        </div>
      </header>
      
      <div className="dashboard-layout">
        <div className="main-panel">
          <ReefDisplay 
            robotPose={robotData.robotPose}
            alliance={robotData.alliance}
            selectedPosition={robotData.selectedReefPosition}
            onPositionSelect={handleReefPositionSelect}
            superstructureHeight={robotData.superstructureHeight}
          />
        </div>
        
        <div className="side-panel">
          <StatusPanel 
            robotData={robotData}
          />
          
          <AutonomousControls 
            autoMode={robotData.autoMode}
            onToggle={handleAutonomousToggle}
            selectedPosition={robotData.selectedReefPosition}
          />
        </div>
      </div>
    </div>
  );
}

export default App;