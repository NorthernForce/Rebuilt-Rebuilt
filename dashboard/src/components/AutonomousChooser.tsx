import { useState, useEffect } from 'react'
import {
  Box,
  Paper,
  Typography,
  FormControl,
  InputLabel,
  Select,
  MenuItem,
  Button,
  Card,
  CardContent,
  Grid,
  Chip,
  SelectChangeEvent
} from '@mui/material'
import { PlayArrow, Stop, Warning } from '@mui/icons-material'
import { useEntry, SendableChooser } from '@frc-web-components/react'

interface AutonomousRoute {
  id: string
  name: string
  description: string
  duration: number
  complexity: 'Simple' | 'Medium' | 'Complex'
  startPosition: '1' | '2' | '3' | 'Center'
}

const autonomousRoutes: AutonomousRoute[] = [
  {
    id: 'leave_only',
    name: 'Leave Community',
    description: 'Simply drive out of the community zone',
    duration: 3,
    complexity: 'Simple',
    startPosition: '2'
  },
  {
    id: 'score_leave',
    name: 'Score and Leave',
    description: 'Score preloaded piece and leave community',
    duration: 8,
    complexity: 'Medium',
    startPosition: '2'
  },
  {
    id: 'two_piece',
    name: 'Two Piece Auto',
    description: 'Score preloaded piece, pick up another, and score again',
    duration: 12,
    complexity: 'Complex',
    startPosition: '1'
  },
  {
    id: 'balance_only',
    name: 'Balance Only',
    description: 'Drive onto charge station and balance',
    duration: 10,
    complexity: 'Medium',
    startPosition: '2'
  },
  {
    id: 'score_balance',
    name: 'Score and Balance',
    description: 'Score preloaded piece and balance on charge station',
    duration: 13,
    complexity: 'Complex',
    startPosition: '2'
  }
]

function AutonomousChooser() {
  const [selectedRoute, setSelectedRoute] = useEntry('/SmartDashboard/Autonomous Selected', 'score_leave')
  const [startPosition, setStartPosition] = useEntry('/SmartDashboard/Starting Position', '2')
  const [autoRunning, setAutoRunning] = useEntry('/SmartDashboard/Auto Running', false)
  const [autoComplete, setAutoComplete] = useEntry('/SmartDashboard/Auto Complete', false)
  const [testModeEnabled, setTestModeEnabled] = useEntry('/SmartDashboard/Test Mode Enabled', false)
  
  const [localAutoStatus, setLocalAutoStatus] = useState<'ready' | 'running' | 'complete'>('ready')

  // Update local status based on NetworkTables
  useEffect(() => {
    if (autoRunning) {
      setLocalAutoStatus('running')
    } else if (autoComplete) {
      setLocalAutoStatus('complete')
    } else {
      setLocalAutoStatus('ready')
    }
  }, [autoRunning, autoComplete])

  const handleRouteChange = (event: SelectChangeEvent<string>) => {
    const routeId = event.target.value
    setSelectedRoute(routeId)
    const route = autonomousRoutes.find(r => r.id === routeId)
    if (route) {
      setStartPosition(route.startPosition)
    }
  }

  const handleStartPositionChange = (event: SelectChangeEvent<string>) => {
    setStartPosition(event.target.value)
  }

  const selectedRouteData = autonomousRoutes.find(r => r.id === selectedRoute)

  const getComplexityColor = (complexity: string) => {
    switch (complexity) {
      case 'Simple': return 'success'
      case 'Medium': return 'warning'  
      case 'Complex': return 'error'
      default: return 'default'
    }
  }

  const testAutonomous = () => {
    if (localAutoStatus === 'running') {
      // Stop the autonomous routine
      setAutoRunning(false)
      setTestModeEnabled(false)
    } else {
      // Start the autonomous routine
      setTestModeEnabled(true)
      setAutoRunning(true)
      setAutoComplete(false)
    }
  }

  return (
    <Box sx={{ height: '100%' }}>
      <Grid container spacing={3} sx={{ height: '100%' }}>
        {/* Selection Panel */}
        <Grid item xs={12} md={6}>
          <Paper elevation={3} sx={{ p: 3, height: '100%' }}>
            <Typography variant="h5" gutterBottom>
              Autonomous Route Selection
            </Typography>

            <Box sx={{ display: 'flex', flexDirection: 'column', gap: 3, mt: 3 }}>
              {/* NetworkTables SendableChooser */}
              <Box>
                <Typography variant="subtitle1" gutterBottom>
                  Robot Autonomous Chooser
                </Typography>
                <SendableChooser source-key="/SmartDashboard/Autonomous" />
              </Box>

              {/* Route Selector (Dashboard Override) */}
              <FormControl fullWidth>
                <InputLabel>Autonomous Route (Dashboard)</InputLabel>
                <Select
                  value={selectedRoute}
                  label="Autonomous Route (Dashboard)"
                  onChange={handleRouteChange}
                >
                  {autonomousRoutes.map((route) => (
                    <MenuItem key={route.id} value={route.id}>
                      {route.name}
                    </MenuItem>
                  ))}
                </Select>
              </FormControl>

              {/* Starting Position */}
              <FormControl fullWidth>
                <InputLabel>Starting Position</InputLabel>
                <Select
                  value={startPosition}
                  label="Starting Position"
                  onChange={handleStartPositionChange}
                >
                  <MenuItem value="1">Position 1 (Left)</MenuItem>
                  <MenuItem value="2">Position 2 (Center)</MenuItem>
                  <MenuItem value="3">Position 3 (Right)</MenuItem>
                  <MenuItem value="Center">Center</MenuItem>
                </Select>
              </FormControl>

              {/* Test Button */}
              <Box sx={{ display: 'flex', gap: 2 }}>
                <Button
                  variant="contained"
                  startIcon={localAutoStatus === 'running' ? <Stop /> : <PlayArrow />}
                  onClick={testAutonomous}
                  color={localAutoStatus === 'running' ? 'error' : 'primary'}
                  fullWidth
                >
                  {localAutoStatus === 'running' ? 'Stop Test' : localAutoStatus === 'complete' ? 'Test Complete!' : 'Test Autonomous'}
                </Button>
              </Box>

              {localAutoStatus === 'running' && (
                <Box sx={{ display: 'flex', alignItems: 'center', gap: 1, p: 2, backgroundColor: 'warning.dark', borderRadius: 1 }}>
                  <Warning />
                  <Typography variant="body2">
                    Autonomous routine is running. Do not enable robot during testing.
                  </Typography>
                </Box>
              )}

              {testModeEnabled && (
                <Box sx={{ display: 'flex', alignItems: 'center', gap: 1, p: 2, backgroundColor: 'info.dark', borderRadius: 1 }}>
                  <Typography variant="body2">
                    Test Mode: Robot will run autonomous in simulation mode
                  </Typography>
                </Box>
              )}
            </Box>
          </Paper>
        </Grid>

        {/* Route Details */}
        <Grid item xs={12} md={6}>
          <Paper elevation={3} sx={{ p: 3, height: '100%' }}>
            <Typography variant="h5" gutterBottom>
              Route Details
            </Typography>

            {selectedRouteData && (
              <Card elevation={2} sx={{ mt: 3 }}>
                <CardContent>
                  <Box sx={{ display: 'flex', justifyContent: 'space-between', alignItems: 'flex-start', mb: 2 }}>
                    <Typography variant="h6">
                      {selectedRouteData.name}
                    </Typography>
                    <Chip 
                      label={selectedRouteData.complexity}
                      color={getComplexityColor(selectedRouteData.complexity) as any}
                      size="small"
                    />
                  </Box>
                  
                  <Typography variant="body1" color="text.secondary" gutterBottom>
                    {selectedRouteData.description}
                  </Typography>

                  <Box sx={{ display: 'flex', gap: 3, mt: 3 }}>
                    <Box>
                      <Typography variant="subtitle2" color="text.secondary">
                        Duration
                      </Typography>
                      <Typography variant="body1">
                        {selectedRouteData.duration}s
                      </Typography>
                    </Box>
                    <Box>
                      <Typography variant="subtitle2" color="text.secondary">
                        Start Position
                      </Typography>
                      <Typography variant="body1">
                        {selectedRouteData.startPosition}
                      </Typography>
                    </Box>
                  </Box>
                </CardContent>
              </Card>
            )}

            {/* Field Visualization */}
            <Box sx={{ mt: 3 }}>
              <Typography variant="h6" gutterBottom>
                Field Position
              </Typography>
              <Box
                sx={{
                  width: '100%',
                  height: 200,
                  backgroundColor: 'grey.900',
                  borderRadius: 1,
                  display: 'flex',
                  alignItems: 'center',
                  justifyContent: 'center',
                  position: 'relative',
                  border: '2px solid',
                  borderColor: 'primary.main'
                }}
              >
                <Typography variant="h6" color="grey.500">
                  🏁 Field Layout
                </Typography>
                
                {/* Start position indicator */}
                <Box
                  sx={{
                    position: 'absolute',
                    bottom: 10,
                    left: startPosition === '1' ? '20%' : startPosition === '2' ? '50%' : startPosition === '3' ? '80%' : '50%',
                    transform: 'translateX(-50%)',
                    width: 12,
                    height: 12,
                    backgroundColor: 'primary.main',
                    borderRadius: '50%'
                  }}
                />
              </Box>
            </Box>
          </Paper>
        </Grid>
      </Grid>
    </Box>
  )
}

export default AutonomousChooser