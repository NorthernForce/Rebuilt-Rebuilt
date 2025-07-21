import { useState } from 'react'
import { Tab, Tabs } from '@mui/material'
import { useEntry } from '@frc-web-components/react'
import DriverPage from './components/DriverPage'
import AutonomousChooser from './components/AutonomousChooser'
import SubsystemTreeView from './components/SubsystemTreeView'
import ConnectionStatus from './components/ConnectionStatus'
import DisconnectedNotice from './components/DisconnectedNotice'
import './App.css'

function TabPanel(props: { children?: React.ReactNode, selected: number, index: number }) {
  return <div hidden={props.selected !== props.index}>
    {props.children}
  </div>
}

function App() {
  const [selected, setSelected] = useState(0)
  const [robotConnected, setRobotConnected] = useState(false)
  const [tabEntry, _setSelectedTab] = useEntry('/FWC/selectedTab', 0)
  const [tabsLocked] = useState(false)
  const [hasCoral] = useEntry('/FWC/HasCoral', false)
  
  const handleTabChange = (_event: React.ChangeEvent<{}>, newValue: number) => {
    if (tabsLocked) return
    setSelected(newValue)
  }
  
  const color = hasCoral ? "#026afa" : ""

  return (
    <div className="main-container" style={{ backgroundColor: color }}>
      <div className="header">
        <Tabs id="header-tabs" value={tabsLocked ? tabEntry : selected} onChange={handleTabChange}>
          <Tab label="Teleop" />
          <Tab label="Autonomous" />
          <Tab label="Settings" />
        </Tabs>
        <ConnectionStatus sourceKey="/FWC/ConnectionStatus" changeConnectionStatus={setRobotConnected} />
      </div>
      <div style={{height: '100%'}}>
        <TabPanel selected={tabsLocked ? tabEntry : selected} index={0}>
          <DriverPage />
        </TabPanel>
        <TabPanel selected={tabsLocked ? tabEntry : selected} index={1}>
          <AutonomousChooser />
        </TabPanel>
        <TabPanel selected={tabsLocked ? tabEntry : selected} index={2}>
          <SubsystemTreeView />
        </TabPanel>
      </div>
      <DisconnectedNotice isDisconnected={!robotConnected} />
    </div>
  )
}

export default App
