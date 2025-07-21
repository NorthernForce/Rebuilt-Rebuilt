import { useState } from 'react'
import { ThemeProvider, createTheme } from '@mui/material/styles'
import { CssBaseline, AppBar, Toolbar, Typography, Tabs, Tab, Box, Container } from '@mui/material'
import { TabContext, TabPanel } from '@mui/lab'
import DriverPage from './components/DriverPage'
import AutonomousChooser from './components/AutonomousChooser'
import SubsystemTreeView from './components/SubsystemTreeView'

const darkTheme = createTheme({
  palette: {
    mode: 'dark',
    primary: {
      main: '#1976d2',
    },
    secondary: {
      main: '#dc004e',
    },
  },
})

function App() {
  const [tabValue, setTabValue] = useState('driver')

  const handleTabChange = (_event: React.SyntheticEvent, newValue: string) => {
    setTabValue(newValue)
  }

  return (
    <ThemeProvider theme={darkTheme}>
      <CssBaseline />
      <Box sx={{ flexGrow: 1, height: '100vh', display: 'flex', flexDirection: 'column' }}>
        <AppBar position="static">
          <Toolbar>
            <Typography variant="h6" component="div" sx={{ flexGrow: 1 }}>
              Northern Force - Team 172 Dashboard
            </Typography>
          </Toolbar>
        </AppBar>
        
        <TabContext value={tabValue}>
          <Box sx={{ borderBottom: 1, borderColor: 'divider' }}>
            <Tabs
              value={tabValue}
              onChange={handleTabChange}
              variant="fullWidth"
              centered
            >
              <Tab label="Driver Station" value="driver" />
              <Tab label="Autonomous" value="autonomous" />
              <Tab label="Subsystems" value="subsystems" />
            </Tabs>
          </Box>
          
          <Container maxWidth={false} sx={{ flex: 1, py: 2 }}>
            <TabPanel value="driver" sx={{ height: '100%', p: 2 }}>
              <DriverPage />
            </TabPanel>
            <TabPanel value="autonomous" sx={{ height: '100%', p: 2 }}>
              <AutonomousChooser />
            </TabPanel>
            <TabPanel value="subsystems" sx={{ height: '100%', p: 2 }}>
              <SubsystemTreeView />
            </TabPanel>
          </Container>
        </TabContext>
      </Box>
    </ThemeProvider>
  )
}

export default App
