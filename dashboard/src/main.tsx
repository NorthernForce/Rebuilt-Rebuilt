import { StrictMode } from 'react'
import { createRoot } from 'react-dom/client'
import './index.css'
import App from './App.tsx'

// Import FRC Web Components
import '@frc-web-components/fwc'
import { NT4Provider } from '@frc-web-components/react'

createRoot(document.getElementById('root')!).render(
  <StrictMode>
    <NT4Provider address="roborio-172-frc.local">
      <App />
    </NT4Provider>
  </StrictMode>,
)
