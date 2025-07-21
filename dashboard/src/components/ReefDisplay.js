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

const ReefDisplay = ({ robotPose, alliance, selectedPosition, onPositionSelect, superstructureHeight }) => {
  const scale = 50; // Scale factor for field display
  const fieldWidth = 600;
  const fieldHeight = 483;
  
  // Convert robot pose to display coordinates
  const robotDisplayX = (robotPose.x * scale) + (fieldWidth / 2);
  const robotDisplayY = fieldHeight - ((robotPose.y * scale) + (fieldHeight / 2));

  const renderReefPosition = (position, coords, type = 'coral') => {
    const isSelected = selectedPosition === position;
    const className = `reef-position ${type} ${isSelected ? 'selected' : ''} ${alliance}`;
    
    // Show 4-level scoring visualization
    const levelIndicator = type === 'coral' ? (
      <div className="level-indicators">
        {['L1', 'L2', 'L3', 'L4'].map(level => (
          <div
            key={level}
            className={`level-dot ${superstructureHeight === level ? 'current' : ''}`}
            title={`Level ${level}`}
          />
        ))}
      </div>
    ) : null;
    
    return (
      <div
        key={position}
        className={className}
        style={{
          left: coords.x - 12,
          top: coords.y - 12,
          transform: `rotate(${coords.angle}deg)`
        }}
        onClick={() => onPositionSelect(position)}
        title={`Position ${position} - ${type === 'coral' ? 'Coral Scoring' : 'Algae Removal'}`}
      >
        <div className="position-label">{position}</div>
        {levelIndicator}
      </div>
    );
  };

  const renderCoralStation = (name, coords) => {
    return (
      <div
        key={name}
        className={`coral-station ${alliance}`}
        style={{
          left: coords.x - 12,
          top: coords.y - 12,
          transform: `rotate(${coords.angle}deg)`
        }}
        onClick={() => onPositionSelect(name)}
        title={`Coral Station ${name}`}
      >
        <div className="station-icon">🪸</div>
        <div className="station-label">CS</div>
      </div>
    );
  };

  return (
    <div className="reef-display">
      <h2>🏁 Field View - {alliance.toUpperCase()} Alliance</h2>
      
      <div className="field-container">
        <svg className="field-svg" viewBox="0 0 600 483">
          {/* Field background gradient */}
          <defs>
            <radialGradient id="fieldGradient" cx="50%" cy="50%" r="50%">
              <stop offset="0%" stopColor="#2d5a2d" stopOpacity="1" />
              <stop offset="100%" stopColor="#1a3d1a" stopOpacity="1" />
            </radialGradient>
            <pattern id="fieldPattern" patternUnits="userSpaceOnUse" width="20" height="20">
              <rect width="20" height="20" fill="none"/>
              <circle cx="10" cy="10" r="1" fill="rgba(0,255,65,0.1)"/>
            </pattern>
          </defs>
          
          {/* Field outline with enhanced styling */}
          <rect x="10" y="10" width="580" height="463" 
                fill="url(#fieldGradient)" 
                stroke="#00ff41" 
                strokeWidth="3" 
                rx="8" />
          
          {/* Field pattern overlay */}
          <rect x="10" y="10" width="580" height="463" 
                fill="url(#fieldPattern)" 
                rx="8" />
          
          {/* Reef center (hexagonal outline with glow) */}
          <polygon 
            points="250,200 350,200 400,241 350,282 250,282 200,241"
            fill="rgba(0,255,65,0.1)" 
            stroke="#00ff41" 
            strokeWidth="3"
            strokeDasharray="8,4"
            filter="url(#glow)"
          />
          
          {/* Add glow effect definition */}
          <defs>
            <filter id="glow" x="-20%" y="-20%" width="140%" height="140%">
              <feGaussianBlur stdDeviation="3" result="coloredBlur"/>
              <feMerge> 
                <feMergeNode in="coloredBlur"/>
                <feMergeNode in="SourceGraphic"/>
              </feMerge>
            </filter>
          </defs>
          
          {/* Robot position with enhanced visualization */}
          <g transform={`translate(${robotDisplayX}, ${robotDisplayY}) rotate(${robotPose.rotation})`}>
            {/* Robot body */}
            <polygon points="-15,0 10,-10 10,10" 
                     fill={alliance === 'blue' ? '#0080ff' : '#ff0040'} 
                     stroke="white" 
                     strokeWidth="2" 
                     filter="url(#glow)" />
            {/* Robot direction indicator */}
            <circle cx="8" cy="0" r="3" fill="white" opacity="0.8" />
            {/* Robot range circle */}
            <circle cx="0" cy="0" r="20" fill="none" stroke="white" strokeWidth="1" strokeDasharray="4,4" opacity="0.6" />
            {/* Team number */}
            <text x="0" y="25" textAnchor="middle" fill="white" fontSize="12" fontWeight="bold">172</text>
          </g>
          
          {/* Selected position indicator with enhanced animation */}
          {selectedPosition && REEF_POSITIONS[selectedPosition] && (
            <g>
              <circle
                cx={REEF_POSITIONS[selectedPosition].x}
                cy={REEF_POSITIONS[selectedPosition].y}
                r="25"
                fill="none"
                stroke="#ffff00"
                strokeWidth="3"
                strokeDasharray="8,4"
                opacity="0.8"
              >
                <animateTransform
                  attributeName="transform"
                  attributeType="XML"
                  type="rotate"
                  from={`0 ${REEF_POSITIONS[selectedPosition].x} ${REEF_POSITIONS[selectedPosition].y}`}
                  to={`360 ${REEF_POSITIONS[selectedPosition].x} ${REEF_POSITIONS[selectedPosition].y}`}
                  dur="2s"
                  repeatCount="indefinite"/>
              </circle>
              <circle
                cx={REEF_POSITIONS[selectedPosition].x}
                cy={REEF_POSITIONS[selectedPosition].y}
                r="15"
                fill="rgba(255,255,0,0.2)"
                stroke="none"
              >
                <animate attributeName="r" values="15;20;15" dur="1.5s" repeatCount="indefinite"/>
              </circle>
            </g>
          )}
          
          {/* Field zones visualization */}
          <text x="50" y="50" fill="#00ff41" fontSize="14" fontWeight="bold" opacity="0.7">Red Zone</text>
          <text x="520" y="450" fill="#00ff41" fontSize="14" fontWeight="bold" opacity="0.7">Blue Zone</text>
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
        <div className="level-selector">
          <label>Current Level:</label>
          <div className="level-buttons">
            {['L1', 'L2', 'L3', 'L4'].map(level => (
              <div
                key={level}
                className={`level-button ${superstructureHeight === level ? 'active' : ''}`}
                title={`Level ${level} - ${level === 'L1' ? 'Ground' : level === 'L2' ? 'Low' : level === 'L3' ? 'Mid' : 'High'} Scoring`}
              >
                {level}
              </div>
            ))}
          </div>
        </div>
        
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
            <span className="selected-label">🎯 Selected:</span>
            <strong className="selected-position">{selectedPosition}</strong>
            <button 
              className="clear-button"
              onClick={() => onPositionSelect(null)}
              title="Clear selection"
            >
              ✕
            </button>
          </div>
        )}
      </div>
    </div>
  );
};

export default ReefDisplay;