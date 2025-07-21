import { Box, Paper, Typography, Grid, Card, CardContent } from '@mui/material'
import { useEffect, useState } from 'react'
import '@frc-web-components/fwc'
import { 
  useEntry, 
  useNt4,
  BasicFmsInfo,
  NumberBar,
  Gyro 
} from '@frc-web-components/react'

interface RobotPose {
  x: number
  y: number
  rotation: number
}

function DriverPage() {
  const { nt4Provider } = useNt4()
  const [connected, setConnected] = useState(false)
  
  // NetworkTables entries using useEntry hook
  const [matchTime] = useEntry('/FMSInfo/MatchTime', 135)
  const [isEnabled] = useEntry('/FMSInfo/FMSControlData', false)
  const [robotMode] = useEntry('/FMSInfo/GameSpecificMessage', 'Teleop')
  const [batteryVoltage] = useEntry('/SmartDashboard/Battery Voltage', 12.6)
  const [driveSpeed] = useEntry('/SmartDashboard/Drive Speed', 0)
  const [gyroAngle] = useEntry('/SmartDashboard/Gyro Angle', 0)
  const [robotPose] = useEntry('/SmartDashboard/Robot Pose', [0, 0, 0])
  const [leftMotorCurrent] = useEntry('/SmartDashboard/Left Motor Current', 0)
  const [rightMotorCurrent] = useEntry('/SmartDashboard/Right Motor Current', 0)

  // Check connection status
  useEffect(() => {
    const interval = setInterval(() => {
      setConnected(nt4Provider.isConnected())
    }, 1000)

    return () => clearInterval(interval)
  }, [nt4Provider])

  const formatTime = (seconds: number) => {
    const mins = Math.floor(seconds / 60)
    const secs = seconds % 60
    return `${mins}:${secs.toString().padStart(2, '0')}`
  }

  const getRobotStatus = () => {
    if (!connected) return { status: '🔴 Disconnected', color: 'error.main' }
    return { status: '🟢 Connected', color: 'success.main' }
  }

  const status = getRobotStatus()

  return (
    <Box sx={{ height: '100%', display: 'flex', flexDirection: 'column', gap: 2 }}>
      <Grid container spacing={2} sx={{ flex: '0 0 auto' }}>
        {/* Match Status */}
        <Grid item xs={12} md={4}>
          <Card elevation={3}>
            <CardContent>
              <Typography variant="h6" gutterBottom>
                Match Status
              </Typography>
              <Typography variant="h3" color="primary" gutterBottom>
                {formatTime(matchTime)}
              </Typography>
              <Typography variant="body1" color={isEnabled ? 'success.main' : 'error.main'}>
                {robotMode} - {isEnabled ? 'ENABLED' : 'DISABLED'}
              </Typography>
            </CardContent>
          </Card>
        </Grid>

        {/* Robot Status */}
        <Grid item xs={12} md={4}>
          <Card elevation={3}>
            <CardContent>
              <Typography variant="h6" gutterBottom>
                Robot Status
              </Typography>
              <Typography variant="body1" color={status.color}>
                {status.status}
              </Typography>
              <Typography variant="body2">
                Battery: {batteryVoltage.toFixed(1)}V
              </Typography>
              <Typography variant="body2">
                CAN: {connected ? 'OK' : 'Error'}
              </Typography>
            </CardContent>
          </Card>
        </Grid>

        {/* Driver Controls */}
        <Grid item xs={12} md={4}>
          <Card elevation={3}>
            <CardContent>
              <Typography variant="h6" gutterBottom>
                Driver Controls
              </Typography>
              <Typography variant="body2">
                Drive Speed: {(driveSpeed * 100).toFixed(0)}%
              </Typography>
              <Typography variant="body2">
                Left Motor: {leftMotorCurrent.toFixed(1)}A
              </Typography>
              <Typography variant="body2">
                Right Motor: {rightMotorCurrent.toFixed(1)}A
              </Typography>
            </CardContent>
          </Card>
        </Grid>
      </Grid>

      {/* Camera Feed and FMS Info */}
      <Grid container spacing={2} sx={{ flex: '1 1 auto', minHeight: 0 }}>
        <Grid item xs={12} md={6}>
          <Paper elevation={3} sx={{ height: '100%', p: 2, display: 'flex', flexDirection: 'column' }}>
            <Typography variant="h6" gutterBottom>
              Driver Camera
            </Typography>
            <Box
              sx={{
                flex: 1,
                backgroundColor: 'grey.900',
                borderRadius: 1,
                display: 'flex',
                alignItems: 'center',
                justifyContent: 'center',
                minHeight: 300
              }}
            >
              <Typography variant="h4" color="grey.500">
                📹 Camera Feed
              </Typography>
            </Box>
            <Typography variant="caption" color="text.secondary" sx={{ mt: 1 }}>
              Stream: mjpeg://roborio-172-frc.local:1181/stream.mjpg
            </Typography>
          </Paper>
        </Grid>

        <Grid item xs={12} md={6}>
          <Box sx={{ height: '100%', display: 'flex', flexDirection: 'column', gap: 2 }}>
            {/* FMS Info */}
            <Paper elevation={3} sx={{ p: 2 }}>
              <Typography variant="h6" gutterBottom>
                FMS Information
              </Typography>
              <BasicFmsInfo source-key="/FMSInfo" />
            </Paper>

            {/* Robot Data with FWC Components */}
            <Paper elevation={3} sx={{ flex: 1, p: 2, display: 'flex', flexDirection: 'column' }}>
              <Typography variant="h6" gutterBottom>
                Robot Data
              </Typography>
              <Box sx={{ flex: 1, display: 'flex', flexDirection: 'column', gap: 2 }}>
                {/* Drive Speed Bar */}
                <Box>
                  <Typography variant="subtitle2" gutterBottom>Drive Speed</Typography>
                  <NumberBar 
                    value={driveSpeed} 
                    min={-1} 
                    max={1} 
                    precision={2}
                    style={{ width: '100%' }}
                  />
                </Box>
                
                {/* Gyro */}
                <Box>
                  <Typography variant="subtitle2" gutterBottom>Gyro Angle</Typography>
                  <Gyro 
                    value={gyroAngle}
                    style={{ width: '150px', height: '150px', margin: '0 auto' }}
                  />
                </Box>

                {/* Alliance */}
                <Box>
                  <Typography variant="subtitle2" gutterBottom>Alliance</Typography>
                  <Typography variant="body1" color="primary">
                    Blue Alliance - Position 2
                  </Typography>
                </Box>
              </Box>
            </Paper>
          </Box>
        </Grid>
      </Grid>
    </Box>
  )
}

export default DriverPage