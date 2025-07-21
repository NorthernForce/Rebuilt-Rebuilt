import React from 'react';
import './ReefDisplay.css';

// Reef positions based on field coordinates (scaled for display)
const REEF_POSITIONS = {
  A: { x: 180, y: 251, angle: 0 },
  B: { x: 180, y: 231, angle: 0 },
  C: { x: 222, y: 178, angle: 60 },
  D: { x: 237, y: 167, angle: 60 },
  E: { x: 301, y: 169, angle: 120 },
  F: { x: 318, y: 178, angle: 120 },
  G: { x: 351, y: 232, angle: 180 },
  H: { x: 351, y: 251, angle: 180 },
  I: { x: 318, y: 307, angle: 240 },
  J: { x: 301, y: 318, angle: 240 },
  K: { x: 237, y: 318, angle: 300 },
  L: { x: 219, y: 307, angle: 300 },
};

// Algae positions
const ALGAE_POSITIONS = {
  AB: { x: 180, y: 241, angle: 0 },
  CD: { x: 228, y: 172, angle: 60 },
  EF: { x: 309, y: 172, angle: 120 },
  GH: { x: 351, y: 241, angle: 180 },
  IJ: { x: 309, y: 312, angle: 240 },
  KL: { x: 229, y: 312, angle: 300 },
};

// Coral stations
const CORAL_STATIONS = {
  LEFT: { x: 71, y: 425, angle: 307.5 },
  RIGHT: { x: 67, y: 60, angle: 52.5 }
};

const ReefDisplay = ({ robotPose, alliance, selectedPosition, onPositionSelect }) => {
  const scale = 50; // Scale factor for field display
  const fieldWidth = 600;
  const fieldHeight = 483;
  
  // Convert robot pose to display coordinates
  const robotDisplayX = (robotPose.x * scale) + (fieldWidth / 2);
  const robotDisplayY = fieldHeight - ((robotPose.y * scale) + (fieldHeight / 2));

  const renderReefPosition = (position, coords, type = 'coral') => {
    const isSelected = selectedPosition === position;
    const className = `reef-position ${type} ${isSelected ? 'selected' : ''} ${alliance}`;
    
    return (
      <div
        key={position}
        className={className}
        style={{
          left: coords.x - 8,
          top: coords.y - 8,
          transform: `rotate(${coords.angle}deg)`
        }}
        onClick={() => onPositionSelect(position)}
        title={`Position ${position}`}
      >
        <div className="position-label">{position}</div>
      </div>
    );
  };

  const renderCoralStation = (name, coords) => {
    return (
      <div
        key={name}
        className={`coral-station ${alliance}`}
        style={{
          left: coords.x - 10,
          top: coords.y - 10,
          transform: `rotate(${coords.angle}deg)`
        }}
        onClick={() => onPositionSelect(name)}
        title={`Coral Station ${name}`}
      >
        CS
      </div>
    );
  };

  return (
    <div className="reef-display">
      <h2>Field View - {alliance.toUpperCase()} Alliance</h2>
      
      <div className="field-container">
        <svg className="field-svg" viewBox="0 0 600 483">
          {/* Field outline */}
          <rect x="10" y="10" width="580" height="463" 
                fill="#2d4a2d" stroke="#00ff41" strokeWidth="2" />
          
          {/* Reef center (hexagonal outline) */}
          <polygon 
            points="250,200 350,200 400,241 350,282 250,282 200,241"
            fill="none" 
            stroke="#00ff41" 
            strokeWidth="2"
            strokeDasharray="5,5"
          />
          
          {/* Robot position */}
          <g transform={`translate(${robotDisplayX}, ${robotDisplayY}) rotate(${robotPose.rotation})`}>
            <polygon points="-12,0 8,-8 8,8" fill={alliance === 'blue' ? '#0080ff' : '#ff0040'} stroke="white" strokeWidth="2" />
            <circle cx="0" cy="0" r="15" fill="none" stroke="white" strokeWidth="1" strokeDasharray="3,3" />
          </g>
          
          {/* Selected position indicator */}
          {selectedPosition && REEF_POSITIONS[selectedPosition] && (
            <circle
              cx={REEF_POSITIONS[selectedPosition].x}
              cy={REEF_POSITIONS[selectedPosition].y}
              r="20"
              fill="none"
              stroke="#ffff00"
              strokeWidth="3"
              strokeDasharray="5,5"
            />
          )}
        </svg>
        
        {/* Overlay positioned elements */}
        <div className="field-overlay">
          {/* Render coral scoring positions */}
          {Object.entries(REEF_POSITIONS).map(([position, coords]) =>
            renderReefPosition(position, coords, 'coral')
          )}
          
          {/* Render algae positions */}
          {Object.entries(ALGAE_POSITIONS).map(([position, coords]) =>
            renderReefPosition(position, coords, 'algae')
          )}
          
          {/* Render coral stations */}
          {Object.entries(CORAL_STATIONS).map(([name, coords]) =>
            renderCoralStation(name, coords)
          )}
        </div>
      </div>
      
      <div className="field-controls">
        <div className="alliance-toggle">
          <label>
            <input 
              type="checkbox" 
              checked={alliance === 'red'}
              onChange={() => {/* Alliance would be set from robot */}}
              disabled 
            />
            Red Alliance
          </label>
        </div>
        
        {selectedPosition && (
          <div className="selected-info">
            Selected: <strong>{selectedPosition}</strong>
            <button onClick={() => onPositionSelect(null)}>Clear Selection</button>
          </div>
        )}
      </div>
    </div>
  );
};

export default ReefDisplay;