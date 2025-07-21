import { useState } from 'react'
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

interface SubsystemStatus {
  name: string
  status: 'OK' | 'Warning' | 'Error'
  enabled: boolean
  icon: React.ReactNode
}

interface SubsystemData {
  [key: string]: any
}

function SubsystemTreeView() {
  const [subsystems] = useState<SubsystemStatus[]>([
    { name: 'Drive Train', status: 'OK', enabled: true, icon: <DriveEta /> },
    { name: 'Arm Mechanism', status: 'OK', enabled: true, icon: <PrecisionManufacturing /> },
    { name: 'Vision', status: 'Warning', enabled: true, icon: <CameraAlt /> },
    { name: 'Sensors', status: 'OK', enabled: true, icon: <Sensors /> }
  ])

  const [driveData] = useState<SubsystemData>({
    leftMotorSpeed: 0.65,
    rightMotorSpeed: 0.68,
    gyroAngle: 45.2,
    odometryX: 2.5,
    odometryY: 1.2,
    maxSpeed: 4.5,
    currentSpeed: 2.1
  })

  const [armData] = useState<SubsystemData>({
    armAngle: 32.5,
    armTargetAngle: 35.0,
    armMotorCurrent: 8.2,
    wristAngle: 15.0,
    intakeSpeed: 0.8,
    hasGamePiece: true
  })

  const [visionData] = useState<SubsystemData>({
    targetVisible: true,
    targetDistance: 2.3,
    targetAngle: -5.2,
    cameraConnected: true,
    processingLatency: 15
  })

  const [sensorData] = useState<SubsystemData>({
    batteryVoltage: 12.6,
    totalCurrent: 45.2,
    canStatus: 'OK',
    pdpTemperature: 32,
    compressorPressure: 110
  })

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
                    <Typography variant="body2">Left Motor: {(driveData.leftMotorSpeed * 100).toFixed(1)}%</Typography>
                    <LinearProgress variant="determinate" value={Math.abs(driveData.leftMotorSpeed * 100)} sx={{ mb: 1 }} />
                  </Box>
                  <Box>
                    <Typography variant="body2">Right Motor: {(driveData.rightMotorSpeed * 100).toFixed(1)}%</Typography>
                    <LinearProgress variant="determinate" value={Math.abs(driveData.rightMotorSpeed * 100)} />
                  </Box>
                </CardContent>
              </Card>
            </Grid>
            <Grid item xs={12} md={6}>
              <Card>
                <CardContent>
                  <Typography variant="h6" gutterBottom>Position & Speed</Typography>
                  <Typography variant="body2">Gyro: {driveData.gyroAngle}°</Typography>
                  <Typography variant="body2">X: {driveData.odometryX}m</Typography>
                  <Typography variant="body2">Y: {driveData.odometryY}m</Typography>
                  <Typography variant="body2">Speed: {driveData.currentSpeed} m/s</Typography>
                </CardContent>
              </Card>
            </Grid>
            <Grid item xs={12}>
              <Box sx={{ mt: 2 }}>
                <Typography variant="subtitle2" gutterBottom>Drive Speed</Typography>
                <div
                  dangerouslySetInnerHTML={{
                    __html: `<frc-number-bar value="${driveData.currentSpeed / driveData.maxSpeed}" min="0" max="1" precision="2"></frc-number-bar>`
                  }}
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
                  <Typography variant="body2">Current: {armData.armAngle}°</Typography>
                  <Typography variant="body2">Target: {armData.armTargetAngle}°</Typography>
                  <Typography variant="body2">Motor Current: {armData.armMotorCurrent}A</Typography>
                </CardContent>
              </Card>
            </Grid>
            <Grid item xs={12} md={6}>
              <Card>
                <CardContent>
                  <Typography variant="h6" gutterBottom>End Effector</Typography>
                  <Typography variant="body2">Wrist Angle: {armData.wristAngle}°</Typography>
                  <Typography variant="body2">Intake Speed: {(armData.intakeSpeed * 100).toFixed(0)}%</Typography>
                  <Typography variant="body2">
                    Game Piece: {armData.hasGamePiece ? '✅ Detected' : '❌ None'}
                  </Typography>
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
                    Target: {visionData.targetVisible ? '✅ Visible' : '❌ Not Found'}
                  </Typography>
                  <Typography variant="body2">Distance: {visionData.targetDistance}m</Typography>
                  <Typography variant="body2">Angle: {visionData.targetAngle}°</Typography>
                </CardContent>
              </Card>
            </Grid>
            <Grid item xs={12} md={6}>
              <Card>
                <CardContent>
                  <Typography variant="h6" gutterBottom>Camera Status</Typography>
                  <Typography variant="body2">
                    Connection: {visionData.cameraConnected ? '✅ Connected' : '❌ Disconnected'}
                  </Typography>
                  <Typography variant="body2">Latency: {visionData.processingLatency}ms</Typography>
                  {visionData.processingLatency > 20 && (
                    <Typography variant="body2" color="warning.main">
                      ⚠️ High latency detected
                    </Typography>
                  )}
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
                  <Typography variant="body2">Battery: {sensorData.batteryVoltage}V</Typography>
                  <Typography variant="body2">Total Current: {sensorData.totalCurrent}A</Typography>
                  <Typography variant="body2">Temperature: {sensorData.pdpTemperature}°C</Typography>
                </CardContent>
              </Card>
            </Grid>
            <Grid item xs={12} md={6}>
              <Card>
                <CardContent>
                  <Typography variant="h6" gutterBottom>System Status</Typography>
                  <Typography variant="body2">CAN Bus: {sensorData.canStatus}</Typography>
                  <Typography variant="body2">Pressure: {sensorData.compressorPressure} PSI</Typography>
                </CardContent>
              </Card>
            </Grid>
            <Grid item xs={12}>
              <Box sx={{ mt: 2 }}>
                <Typography variant="subtitle2" gutterBottom>Battery Voltage</Typography>
                <div
                  dangerouslySetInnerHTML={{
                    __html: `<frc-number-bar value="${sensorData.batteryVoltage}" min="10" max="13" precision="1"></frc-number-bar>`
                  }}
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
              {subsystems.map((subsystem) => (
                <Card key={subsystem.name} elevation={2}>
                  <CardContent sx={{ py: 1.5, '&:last-child': { pb: 1.5 } }}>
                    <Box sx={{ display: 'flex', alignItems: 'center', gap: 2 }}>
                      {subsystem.icon}
                      <Box sx={{ flex: 1 }}>
                        <Typography variant="subtitle1">{subsystem.name}</Typography>
                        <Box sx={{ display: 'flex', alignItems: 'center', gap: 1 }}>
                          <Chip 
                            label={subsystem.status}
                            size="small"
                            color={getStatusColor(subsystem.status) as any}
                          />
                          <FormControlLabel
                            control={
                              <Switch 
                                checked={subsystem.enabled}
                                size="small"
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
              ))}
            </Box>
          </Paper>
        </Grid>

        {/* Subsystem Details */}
        <Grid item xs={12} md={8}>
          <Paper elevation={3} sx={{ p: 2, height: '100%', overflow: 'auto' }}>
            <Typography variant="h5" gutterBottom>
              Subsystem Dashboards
            </Typography>

            {subsystems.map((subsystem) => (
              <Accordion key={subsystem.name} elevation={2} sx={{ mb: 1 }}>
                <AccordionSummary 
                  expandIcon={<ExpandMore />}
                  sx={{ backgroundColor: 'grey.900' }}
                >
                  <Box sx={{ display: 'flex', alignItems: 'center', gap: 2 }}>
                    {subsystem.icon}
                    <Typography variant="h6">{subsystem.name}</Typography>
                    <Chip 
                      label={subsystem.status}
                      size="small"
                      color={getStatusColor(subsystem.status) as any}
                    />
                    {subsystem.status === 'Error' && <Error color="error" />}
                  </Box>
                </AccordionSummary>
                <AccordionDetails sx={{ p: 3 }}>
                  {renderSubsystemDashboard(subsystem.name)}
                </AccordionDetails>
              </Accordion>
            ))}
          </Paper>
        </Grid>
      </Grid>
    </Box>
  )
}

export default SubsystemTreeView