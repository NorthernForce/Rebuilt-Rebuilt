import { Box, Paper, Typography, Grid, Card, CardContent } from '@mui/material'
import { useEffect, useState } from 'react'
import '@frc-web-components/fwc'

interface MatchTimeData {
  matchTime: number
  isEnabled: boolean
  mode: 'Teleop' | 'Autonomous' | 'Test' | 'Disabled'
}

function DriverPage() {
  const [matchData, setMatchData] = useState<MatchTimeData>({
    matchTime: 135,
    isEnabled: false,
    mode: 'Disabled'
  })

  // Simulate match time updates - in real implementation this would come from NetworkTables
  useEffect(() => {
    const interval = setInterval(() => {
      setMatchData(prev => ({
        ...prev,
        matchTime: Math.max(0, prev.matchTime - 1)
      }))
    }, 1000)

    return () => clearInterval(interval)
  }, [])

  const formatTime = (seconds: number) => {
    const mins = Math.floor(seconds / 60)
    const secs = seconds % 60
    return `${mins}:${secs.toString().padStart(2, '0')}`
  }

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
                {formatTime(matchData.matchTime)}
              </Typography>
              <Typography variant="body1" color={matchData.isEnabled ? 'success.main' : 'error.main'}>
                {matchData.mode} - {matchData.isEnabled ? 'ENABLED' : 'DISABLED'}
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
              <Typography variant="body1" color="success.main">
                🟢 Connected
              </Typography>
              <Typography variant="body2">
                Battery: 12.6V
              </Typography>
              <Typography variant="body2">
                CAN: OK
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
                Joystick 1: Connected
              </Typography>
              <Typography variant="body2">
                Joystick 2: Connected
              </Typography>
              <Typography variant="body2">
                Controller: Connected
              </Typography>
            </CardContent>
          </Card>
        </Grid>
      </Grid>

      {/* Camera Feed */}
      <Grid container spacing={2} sx={{ flex: '1 1 auto', minHeight: 0 }}>
        <Grid item xs={12} md={8}>
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

        {/* FRC Web Components Panel */}
        <Grid item xs={12} md={4}>
          <Paper elevation={3} sx={{ height: '100%', p: 2, display: 'flex', flexDirection: 'column' }}>
            <Typography variant="h6" gutterBottom>
              Robot Data
            </Typography>
            <Box sx={{ flex: 1, display: 'flex', flexDirection: 'column', gap: 2 }}>
              {/* Using FRC Web Components */}
              <Box>
                <Typography variant="subtitle2" gutterBottom>Drive Speed</Typography>
                <div
                  dangerouslySetInnerHTML={{
                    __html: '<frc-number-bar value="0.75" min="-1" max="1" precision="2"></frc-number-bar>'
                  }}
                />
              </Box>
              
              <Box>
                <Typography variant="subtitle2" gutterBottom>Gyro Angle</Typography>
                <div
                  dangerouslySetInnerHTML={{
                    __html: '<frc-gyro value="45"></frc-gyro>'
                  }}
                />
              </Box>

              <Box>
                <Typography variant="subtitle2" gutterBottom>Alliance</Typography>
                <Typography variant="body1" color="primary">
                  Blue Alliance - Position 2
                </Typography>
              </Box>
            </Box>
          </Paper>
        </Grid>
      </Grid>
    </Box>
  )
}

export default DriverPage