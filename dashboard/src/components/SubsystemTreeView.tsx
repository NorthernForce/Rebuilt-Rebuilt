import { useState, useEffect } from 'react'
import {
  Box,
  Paper,
  Typography,
  Grid,
  Accordion,
  AccordionSummary,
  AccordionDetails,
  Card,
  CardContent,
  LinearProgress,
  Chip,
  FormControlLabel,
  Switch
} from '@mui/material'
import { ExpandMore, DriveEta, PrecisionManufacturing, CameraAlt, Sensors, Error } from '@mui/icons-material'
import '@frc-web-components/fwc'
import { useEntry, NumberBar } from '@frc-web-components/react'

interface SubsystemStatus {
  name: string
  statusKey: string
  enabledKey: string
  icon: React.ReactNode
}

function SubsystemTreeView() {
  const subsystemConfigs: SubsystemStatus[] = [
    { 
      name: 'Drive Train', 
      statusKey: '/SmartDashboard/Drive Train Status', 
      enabledKey: '/SmartDashboard/Drive Train Enabled',
      icon: <DriveEta /> 
    },
    { 
      name: 'Arm Mechanism', 
      statusKey: '/SmartDashboard/Arm Status', 
      enabledKey: '/SmartDashboard/Arm Enabled',
      icon: <PrecisionManufacturing /> 
    },
    { 
      name: 'Vision', 
      statusKey: '/SmartDashboard/Vision Status', 
      enabledKey: '/SmartDashboard/Vision Enabled',
      icon: <CameraAlt /> 
    },
    { 
      name: 'Sensors', 
      statusKey: '/SmartDashboard/Sensors Status', 
      enabledKey: '/SmartDashboard/Sensors Enabled',
      icon: <Sensors /> 
    }
  ]

  // Drive Train NetworkTables entries
  const [leftMotorSpeed] = useEntry('/SmartDashboard/Left Motor Speed', 0.65)
  const [rightMotorSpeed] = useEntry('/SmartDashboard/Right Motor Speed', 0.68)
  const [gyroAngle] = useEntry('/SmartDashboard/Gyro Angle', 45.2)
  const [odometryX] = useEntry('/SmartDashboard/Odometry X', 2.5)
  const [odometryY] = useEntry('/SmartDashboard/Odometry Y', 1.2)
  const [maxSpeed] = useEntry('/SmartDashboard/Max Speed', 4.5)
  const [currentSpeed] = useEntry('/SmartDashboard/Current Speed', 2.1)
  const [leftMotorCurrent] = useEntry('/SmartDashboard/Left Motor Current', 15.2)
  const [rightMotorCurrent] = useEntry('/SmartDashboard/Right Motor Current', 16.8)

  // Arm Mechanism NetworkTables entries  
  const [armAngle] = useEntry('/SmartDashboard/Arm Angle', 32.5)
  const [armTargetAngle] = useEntry('/SmartDashboard/Arm Target Angle', 35.0)
  const [armMotorCurrent] = useEntry('/SmartDashboard/Arm Motor Current', 8.2)
  const [wristAngle] = useEntry('/SmartDashboard/Wrist Angle', 15.0)
  const [intakeSpeed] = useEntry('/SmartDashboard/Intake Speed', 0.8)
  const [hasGamePiece] = useEntry('/SmartDashboard/Has Game Piece', true)

  // Vision NetworkTables entries
  const [targetVisible] = useEntry('/SmartDashboard/Target Visible', true)
  const [targetDistance] = useEntry('/SmartDashboard/Target Distance', 2.3)
  const [targetAngle] = useEntry('/SmartDashboard/Target Angle', -5.2)
  const [cameraConnected] = useEntry('/SmartDashboard/Camera Connected', true)
  const [processingLatency] = useEntry('/SmartDashboard/Vision Latency', 15)

  // Sensor NetworkTables entries
  const [batteryVoltage] = useEntry('/SmartDashboard/Battery Voltage', 12.6)
  const [totalCurrent] = useEntry('/SmartDashboard/Total Current', 45.2)
  const [canStatus] = useEntry('/SmartDashboard/CAN Status', 'OK')
  const [pdpTemperature] = useEntry('/SmartDashboard/PDP Temperature', 32)
  const [compressorPressure] = useEntry('/SmartDashboard/Compressor Pressure', 110)

  // Subsystem status and enabled states
  const [driveTrainStatus] = useEntry('/SmartDashboard/Drive Train Status', 'OK')
  const [driveTrainEnabled, setDriveTrainEnabled] = useEntry('/SmartDashboard/Drive Train Enabled', true)
  const [armStatus] = useEntry('/SmartDashboard/Arm Status', 'OK')
  const [armEnabled, setArmEnabled] = useEntry('/SmartDashboard/Arm Enabled', true)
  const [visionStatus] = useEntry('/SmartDashboard/Vision Status', 'Warning')
  const [visionEnabled, setVisionEnabled] = useEntry('/SmartDashboard/Vision Enabled', true)
  const [sensorsStatus] = useEntry('/SmartDashboard/Sensors Status', 'OK')
  const [sensorsEnabled, setSensorsEnabled] = useEntry('/SmartDashboard/Sensors Enabled', true)

  const getSubsystemStatus = (name: string) => {
    switch (name) {
      case 'Drive Train': return driveTrainStatus
      case 'Arm Mechanism': return armStatus
      case 'Vision': return visionStatus
      case 'Sensors': return sensorsStatus
      default: return 'OK'
    }
  }

  const getSubsystemEnabled = (name: string) => {
    switch (name) {
      case 'Drive Train': return driveTrainEnabled
      case 'Arm Mechanism': return armEnabled
      case 'Vision': return visionEnabled
      case 'Sensors': return sensorsEnabled
      default: return true
    }
  }

  const setSubsystemEnabled = (name: string, enabled: boolean) => {
    switch (name) {
      case 'Drive Train': setDriveTrainEnabled(enabled); break
      case 'Arm Mechanism': setArmEnabled(enabled); break
      case 'Vision': setVisionEnabled(enabled); break
      case 'Sensors': setSensorsEnabled(enabled); break
    }
  }

  const getStatusColor = (status: string) => {
    switch (status) {
      case 'OK': return 'success'
      case 'Warning': return 'warning'
      case 'Error': return 'error'
      default: return 'default'
    }
  }

  const renderSubsystemDashboard = (subsystemName: string) => {
    switch (subsystemName) {
      case 'Drive Train':
        return (
          <Grid container spacing={2}>
            <Grid item xs={12} md={6}>
              <Card>
                <CardContent>
                  <Typography variant="h6" gutterBottom>Motor Speeds</Typography>
                  <Box sx={{ mb: 2 }}>
                    <Typography variant="body2">Left Motor: {(leftMotorSpeed * 100).toFixed(1)}%</Typography>
                    <LinearProgress variant="determinate" value={Math.abs(leftMotorSpeed * 100)} sx={{ mb: 1 }} />
                    <Typography variant="caption" color="text.secondary">
                      Current: {leftMotorCurrent.toFixed(1)}A
                    </Typography>
                  </Box>
                  <Box>
                    <Typography variant="body2">Right Motor: {(rightMotorSpeed * 100).toFixed(1)}%</Typography>
                    <LinearProgress variant="determinate" value={Math.abs(rightMotorSpeed * 100)} />
                    <Typography variant="caption" color="text.secondary">
                      Current: {rightMotorCurrent.toFixed(1)}A
                    </Typography>
                  </Box>
                </CardContent>
              </Card>
            </Grid>
            <Grid item xs={12} md={6}>
              <Card>
                <CardContent>
                  <Typography variant="h6" gutterBottom>Position & Speed</Typography>
                  <Typography variant="body2">Gyro: {gyroAngle.toFixed(1)}°</Typography>
                  <Typography variant="body2">X: {odometryX.toFixed(2)}m</Typography>
                  <Typography variant="body2">Y: {odometryY.toFixed(2)}m</Typography>
                  <Typography variant="body2">Speed: {currentSpeed.toFixed(1)} m/s</Typography>
                </CardContent>
              </Card>
            </Grid>
            <Grid item xs={12}>
              <Box sx={{ mt: 2 }}>
                <Typography variant="subtitle2" gutterBottom>Drive Speed</Typography>
                <NumberBar 
                  value={currentSpeed / maxSpeed} 
                  min={0} 
                  max={1} 
                  precision={2}
                  style={{ width: '100%' }}
                />
              </Box>
            </Grid>
          </Grid>
        )

      case 'Arm Mechanism':
        return (
          <Grid container spacing={2}>
            <Grid item xs={12} md={6}>
              <Card>
                <CardContent>
                  <Typography variant="h6" gutterBottom>Arm Position</Typography>
                  <Typography variant="body2">Current: {armAngle.toFixed(1)}°</Typography>
                  <Typography variant="body2">Target: {armTargetAngle.toFixed(1)}°</Typography>
                  <Typography variant="body2">Motor Current: {armMotorCurrent.toFixed(1)}A</Typography>
                  <Box sx={{ mt: 1 }}>
                    <LinearProgress 
                      variant="determinate" 
                      value={(armAngle / 180) * 100}
                      sx={{ height: 8, borderRadius: 1 }}
                    />
                  </Box>
                </CardContent>
              </Card>
            </Grid>
            <Grid item xs={12} md={6}>
              <Card>
                <CardContent>
                  <Typography variant="h6" gutterBottom>End Effector</Typography>
                  <Typography variant="body2">Wrist Angle: {wristAngle.toFixed(1)}°</Typography>
                  <Typography variant="body2">Intake Speed: {(intakeSpeed * 100).toFixed(0)}%</Typography>
                  <Typography variant="body2">
                    Game Piece: {hasGamePiece ? '✅ Detected' : '❌ None'}
                  </Typography>
                  <Box sx={{ mt: 1 }}>
                    <NumberBar 
                      value={intakeSpeed} 
                      min={-1} 
                      max={1} 
                      precision={2}
                      style={{ width: '100%', height: '20px' }}
                    />
                  </Box>
                </CardContent>
              </Card>
            </Grid>
          </Grid>
        )

      case 'Vision':
        return (
          <Grid container spacing={2}>
            <Grid item xs={12} md={6}>
              <Card>
                <CardContent>
                  <Typography variant="h6" gutterBottom>Target Tracking</Typography>
                  <Typography variant="body2">
                    Target: {targetVisible ? '✅ Visible' : '❌ Not Found'}
                  </Typography>
                  <Typography variant="body2">Distance: {targetDistance.toFixed(1)}m</Typography>
                  <Typography variant="body2">Angle: {targetAngle.toFixed(1)}°</Typography>
                  <Box sx={{ mt: 1 }}>
                    <Typography variant="caption" color="text.secondary">Target Angle</Typography>
                    <NumberBar 
                      value={targetAngle / 180} 
                      min={-1} 
                      max={1} 
                      precision={2}
                      style={{ width: '100%', height: '20px' }}
                    />
                  </Box>
                </CardContent>
              </Card>
            </Grid>
            <Grid item xs={12} md={6}>
              <Card>
                <CardContent>
                  <Typography variant="h6" gutterBottom>Camera Status</Typography>
                  <Typography variant="body2">
                    Connection: {cameraConnected ? '✅ Connected' : '❌ Disconnected'}
                  </Typography>
                  <Typography variant="body2">Latency: {processingLatency}ms</Typography>
                  {processingLatency > 20 && (
                    <Typography variant="body2" color="warning.main">
                      ⚠️ High latency detected
                    </Typography>
                  )}
                  <LinearProgress 
                    variant="determinate" 
                    value={Math.min((processingLatency / 50) * 100, 100)}
                    color={processingLatency > 20 ? 'warning' : 'success'}
                    sx={{ mt: 1 }}
                  />
                </CardContent>
              </Card>
            </Grid>
          </Grid>
        )

      case 'Sensors':
        return (
          <Grid container spacing={2}>
            <Grid item xs={12} md={6}>
              <Card>
                <CardContent>
                  <Typography variant="h6" gutterBottom>Power System</Typography>
                  <Typography variant="body2">Battery: {batteryVoltage.toFixed(1)}V</Typography>
                  <Typography variant="body2">Total Current: {totalCurrent.toFixed(1)}A</Typography>
                  <Typography variant="body2">Temperature: {pdpTemperature}°C</Typography>
                  <Box sx={{ mt: 1 }}>
                    <Typography variant="caption" color="text.secondary">Current Draw</Typography>
                    <LinearProgress 
                      variant="determinate" 
                      value={Math.min((totalCurrent / 120) * 100, 100)}
                      color={totalCurrent > 80 ? 'warning' : 'success'}
                    />
                  </Box>
                </CardContent>
              </Card>
            </Grid>
            <Grid item xs={12} md={6}>
              <Card>
                <CardContent>
                  <Typography variant="h6" gutterBottom>System Status</Typography>
                  <Typography variant="body2">CAN Bus: {canStatus}</Typography>
                  <Typography variant="body2">Pressure: {compressorPressure} PSI</Typography>
                  <Box sx={{ mt: 1 }}>
                    <Typography variant="caption" color="text.secondary">Air Pressure</Typography>
                    <LinearProgress 
                      variant="determinate" 
                      value={(compressorPressure / 120) * 100}
                      color={compressorPressure > 100 ? 'success' : 'warning'}
                    />
                  </Box>
                </CardContent>
              </Card>
            </Grid>
            <Grid item xs={12}>
              <Box sx={{ mt: 2 }}>
                <Typography variant="subtitle2" gutterBottom>Battery Voltage</Typography>
                <NumberBar 
                  value={batteryVoltage} 
                  min={10} 
                  max={13} 
                  precision={1}
                  style={{ width: '100%' }}
                />
              </Box>
            </Grid>
          </Grid>
        )

      default:
        return <Typography>No dashboard available for this subsystem</Typography>
    }
  }

  return (
    <Box sx={{ height: '100%' }}>
      <Grid container spacing={2} sx={{ height: '100%' }}>
        {/* Subsystem Overview */}
        <Grid item xs={12} md={4}>
          <Paper elevation={3} sx={{ p: 2, height: '100%' }}>
            <Typography variant="h5" gutterBottom>
              Subsystem Status
            </Typography>
            
            <Box sx={{ display: 'flex', flexDirection: 'column', gap: 1 }}>
              {subsystemConfigs.map((subsystem) => {
                const status = getSubsystemStatus(subsystem.name)
                const enabled = getSubsystemEnabled(subsystem.name)
                
                return (
                  <Card key={subsystem.name} elevation={2}>
                    <CardContent sx={{ py: 1.5, '&:last-child': { pb: 1.5 } }}>
                      <Box sx={{ display: 'flex', alignItems: 'center', gap: 2 }}>
                        {subsystem.icon}
                        <Box sx={{ flex: 1 }}>
                          <Typography variant="subtitle1">{subsystem.name}</Typography>
                          <Box sx={{ display: 'flex', alignItems: 'center', gap: 1 }}>
                            <Chip 
                              label={status}
                              size="small"
                              color={getStatusColor(status) as any}
                            />
                            <FormControlLabel
                              control={
                                <Switch 
                                  checked={enabled}
                                  size="small"
                                  onChange={(e) => setSubsystemEnabled(subsystem.name, e.target.checked)}
                                />
                              }
                              label="Enabled"
                              sx={{ ml: 'auto' }}
                            />
                          </Box>
                        </Box>
                      </Box>
                    </CardContent>
                  </Card>
                )
              })}
            </Box>
          </Paper>
        </Grid>

        {/* Subsystem Details */}
        <Grid item xs={12} md={8}>
          <Paper elevation={3} sx={{ p: 2, height: '100%', overflow: 'auto' }}>
            <Typography variant="h5" gutterBottom>
              Subsystem Dashboards
            </Typography>

            {subsystemConfigs.map((subsystem) => {
              const status = getSubsystemStatus(subsystem.name)
              
              return (
                <Accordion key={subsystem.name} elevation={2} sx={{ mb: 1 }}>
                  <AccordionSummary 
                    expandIcon={<ExpandMore />}
                    sx={{ backgroundColor: 'grey.900' }}
                  >
                    <Box sx={{ display: 'flex', alignItems: 'center', gap: 2 }}>
                      {subsystem.icon}
                      <Typography variant="h6">{subsystem.name}</Typography>
                      <Chip 
                        label={status}
                        size="small"
                        color={getStatusColor(status) as any}
                      />
                      {status === 'Error' && <Error color="error" />}
                    </Box>
                  </AccordionSummary>
                  <AccordionDetails sx={{ p: 3 }}>
                    {renderSubsystemDashboard(subsystem.name)}
                  </AccordionDetails>
                </Accordion>
              )
            })}
          </Paper>
        </Grid>
      </Grid>
    </Box>
  )
}

export default SubsystemTreeView