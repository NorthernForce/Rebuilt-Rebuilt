import React, { useState } from 'react';
import './AutonomousControls.css';

const AutonomousControls = ({ autoMode, onToggle, selectedPosition }) => {
  const [autoCommand, setAutoCommand] = useState('');
  const [superstructureTarget, setSuperstructureTarget] = useState('L1');

  const autonomousCommands = [
    { value: 'moveToPosition', label: 'Move to Selected Position' },
    { value: 'scoreAndReturn', label: 'Score and Return' },
    { value: 'collectFromStation', label: 'Collect from Coral Station' },
    { value: 'removeAlgae', label: 'Remove Algae' },
    { value: 'simpleLeave', label: 'Simple Leave' },
    { value: 'doNothing', label: 'Do Nothing' }
  ];

  const superstructureHeights = ['L1', 'L2', 'L3', 'L4', 'CORAL_STATION'];

  const handleExecuteCommand = () => {
    if (!autoCommand) {
      alert('Please select a command first');
      return;
    }
    
    if (autoCommand === 'moveToPosition' && !selectedPosition) {
      alert('Please select a reef position first');
      return;
    }

    // In real implementation, this would send commands to NetworkTables
    console.log('Executing autonomous command:', {
      command: autoCommand,
      position: selectedPosition,
      superstructureHeight: superstructureTarget
    });
    
    alert(`Executing: ${autoCommand}${selectedPosition ? ` to ${selectedPosition}` : ''}`);
  };

  const handleEmergencyStop = () => {
    // In real implementation, this would send emergency stop to robot
    console.log('EMERGENCY STOP ACTIVATED');
    alert('EMERGENCY STOP ACTIVATED');
  };

  return (
    <div className="autonomous-controls">
      <h3>Autonomous Controls</h3>
      
      <div className="control-section">
        <div className="mode-toggle">
          <label className="toggle-switch">
            <input
              type="checkbox"
              checked={autoMode}
              onChange={onToggle}
            />
            <span className="toggle-slider"></span>
            <span className="toggle-label">
              {autoMode ? 'Autonomous Mode' : 'Manual Mode'}
            </span>
          </label>
        </div>
      </div>

      <div className="control-section">
        <h4>Command Selection</h4>
        <select
          value={autoCommand}
          onChange={(e) => setAutoCommand(e.target.value)}
          className="command-select"
          disabled={!autoMode}
        >
          <option value="">Select Command...</option>
          {autonomousCommands.map(cmd => (
            <option key={cmd.value} value={cmd.value}>
              {cmd.label}
            </option>
          ))}
        </select>
      </div>

      <div className="control-section">
        <h4>Target Position</h4>
        <div className="position-display">
          {selectedPosition ? (
            <span className="selected-position">{selectedPosition}</span>
          ) : (
            <span className="no-position">No position selected</span>
          )}
        </div>
      </div>

      <div className="control-section">
        <h4>Superstructure Height</h4>
        <div className="height-buttons">
          {superstructureHeights.map(height => (
            <button
              key={height}
              className={`height-btn ${superstructureTarget === height ? 'active' : ''}`}
              onClick={() => setSuperstructureTarget(height)}
              disabled={!autoMode}
            >
              {height}
            </button>
          ))}
        </div>
      </div>

      <div className="control-section">
        <h4>Actions</h4>
        <div className="action-buttons">
          <button
            className="execute-btn"
            onClick={handleExecuteCommand}
            disabled={!autoMode || !autoCommand}
          >
            Execute Command
          </button>
          
          <button
            className="stop-btn"
            onClick={handleEmergencyStop}
          >
            EMERGENCY STOP
          </button>
        </div>
      </div>

      <div className="control-section">
        <h4>Quick Actions</h4>
        <div className="quick-actions">
          <button 
            className="quick-btn"
            disabled={!autoMode}
            onClick={() => {
              setAutoCommand('collectFromStation');
              handleExecuteCommand();
            }}
          >
            Go to Coral Station
          </button>
          
          <button 
            className="quick-btn"
            disabled={!autoMode}
            onClick={() => {
              setAutoCommand('simpleLeave');
              handleExecuteCommand();
            }}
          >
            Leave Starting Zone
          </button>
        </div>
      </div>

      {selectedPosition && (
        <div className="control-section">
          <h4>Selected Position Info</h4>
          <div className="position-info">
            <div>Position: <strong>{selectedPosition}</strong></div>
            <div>Type: {selectedPosition.length === 1 ? 'Coral Scoring' : 
                      selectedPosition.includes('_') ? 'Algae Removal' : 'Station'}</div>
            <div>Target Height: <strong>{superstructureTarget}</strong></div>
          </div>
        </div>
      )}
    </div>
  );
};

export default AutonomousControls;