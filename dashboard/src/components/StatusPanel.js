import React from 'react';
import './StatusPanel.css';

const StatusPanel = ({ robotData }) => {
  const {
    alliance,
    robotPose,
    autoMode,
    hasGamePiece,
    superstructureHeight,
    isConnected
  } = robotData;

  const formatCoordinate = (value) => {
    return value.toFixed(2);
  };

  const formatAngle = (value) => {
    return (value % 360).toFixed(1);
  };

  return (
    <div className="status-panel">
      <h3>Robot Status</h3>
      
      <div className="status-section">
        <h4>Connection</h4>
        <div className={`status-indicator ${isConnected ? 'connected' : 'disconnected'}`}>
          {isConnected ? 'Connected' : 'Disconnected'}
        </div>
      </div>

      <div className="status-section">
        <h4>Alliance</h4>
        <div className={`alliance-display ${alliance}`}>
          {alliance.toUpperCase()} ALLIANCE
        </div>
      </div>

      <div className="status-section">
        <h4>Robot Position</h4>
        <div className="coordinate-display">
          <div>X: {formatCoordinate(robotPose.x)} m</div>
          <div>Y: {formatCoordinate(robotPose.y)} m</div>
          <div>Θ: {formatAngle(robotPose.rotation)}°</div>
        </div>
      </div>

      <div className="status-section">
        <h4>Control Mode</h4>
        <div className={`mode-indicator ${autoMode ? 'auto' : 'manual'}`}>
          {autoMode ? 'AUTONOMOUS' : 'MANUAL'}
        </div>
      </div>

      <div className="status-section">
        <h4>Manipulator</h4>
        <div className={`game-piece-indicator ${hasGamePiece ? 'has-piece' : 'no-piece'}`}>
          {hasGamePiece ? 'HAS CORAL' : 'NO CORAL'}
        </div>
      </div>

      <div className="status-section">
        <h4>Superstructure</h4>
        <div className="height-display">
          Height: <span className="height-value">{superstructureHeight}</span>
        </div>
        <div className="height-bars">
          {['L1', 'L2', 'L3', 'L4'].map(level => (
            <div
              key={level}
              className={`height-bar ${superstructureHeight === level ? 'active' : ''}`}
            >
              {level}
            </div>
          ))}
        </div>
      </div>

      <div className="status-section">
        <h4>System Health</h4>
        <div className="health-indicators">
          <div className="health-item">
            <span className="health-label">Drive:</span>
            <span className="health-status good">OK</span>
          </div>
          <div className="health-item">
            <span className="health-label">Vision:</span>
            <span className="health-status good">OK</span>
          </div>
          <div className="health-item">
            <span className="health-label">Manipulator:</span>
            <span className="health-status good">OK</span>
          </div>
          <div className="health-item">
            <span className="health-label">Elevator:</span>
            <span className="health-status good">OK</span>
          </div>
        </div>
      </div>
    </div>
  );
};

export default StatusPanel;