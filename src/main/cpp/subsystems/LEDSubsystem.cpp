#include "subsystems/LEDSubsystem.h"

#include <frc/smartdashboard/SmartDashboard.h>

#include "logging/LogTypes.h"
#include "subsystems/CustomLEDState.h"
#include "subsystems/RalphLEDStates.h"

using namespace nfr;

LEDSubsystem::LEDSubsystem(int canID, const std::string& canBus, int numLEDs)
    : m_candle(canID, canBus),
      m_numLEDs(numLEDs),
      m_initialized(false),
      m_brightness(1.0)
{
    // Configure the CANdle
    // Note: In Phoenix 6, brightness is controlled directly through animation
    // controls No need to set brightness in configuration

    // Initialize default states
    InitializeDefaultStates();

    // Set initial state to OFF
    SetState(m_states["OFF"]);

    m_initialized = true;
}

void LEDSubsystem::Periodic()
{
    // Apply the animation for the current state
    if (m_initialized && m_currentState)
    {
        m_currentState->Animate(m_candle);
    }

    // Log subsystem data
    nfr::logger["LEDSubsystem"] << *this;
}

void LEDSubsystem::SetState(LEDStatePtr state)
{
    if (state)
    {
        m_currentState = state;
    }
}

void LEDSubsystem::SetStateEnum(LEDStateEnum stateEnum)
{
    auto it = m_enumToStateName.find(stateEnum);
    if (it != m_enumToStateName.end())
    {
        auto stateIt = m_states.find(it->second);
        if (stateIt != m_states.end())
        {
            SetState(stateIt->second);
        }
    }
}

std::string LEDSubsystem::GetStateName() const
{
    return m_currentState ? m_currentState->GetStateName() : "UNKNOWN";
}

LEDStatePtr LEDSubsystem::GetState() const
{
    return m_currentState;
}

LEDStatePtr LEDSubsystem::RegisterState(const std::string& name,
                                        LEDStatePtr state)
{
    if (state)
    {
        // Add the state to the map
        m_states[name] = state;
    }
    return state;
}

void LEDSubsystem::InitializeDefaultStates()
{
    // Register common states
    RegisterState("OFF", LEDStateFactory::CreateOffState());
    RegisterState("DEFAULT", LEDStateFactory::CreateDefaultState());
    RegisterState("ENABLED", LEDStateFactory::CreateEnabledState());
    RegisterState("DISABLED", LEDStateFactory::CreateDisabledState());
    RegisterState("ERROR", LEDStateFactory::CreateErrorState());
    RegisterState("WARNING", LEDStateFactory::CreateWarningState());
    RegisterState("SUCCESS", LEDStateFactory::CreateSuccessState());

    // Register Ralph-specific states
    RegisterState("AUTONOMOUS",
                  ralph::RalphLEDStateFactory::CreateAutonomousState());
    RegisterState("TELEOP", ralph::RalphLEDStateFactory::CreateTeleopState());
    RegisterState("ALIGNMENT",
                  ralph::RalphLEDStateFactory::CreateAlignmentState());

    // Set up enum to state name mapping for backward compatibility
    m_enumToStateName[LEDStateEnum::OFF] = "OFF";
    m_enumToStateName[LEDStateEnum::DEFAULT] = "DEFAULT";
    m_enumToStateName[LEDStateEnum::ENABLED] = "ENABLED";
    m_enumToStateName[LEDStateEnum::DISABLED] = "DISABLED";
    m_enumToStateName[LEDStateEnum::AUTONOMOUS] = "AUTONOMOUS";
    m_enumToStateName[LEDStateEnum::TELEOP] = "TELEOP";
    m_enumToStateName[LEDStateEnum::ALIGNMENT] = "ALIGNMENT";
    m_enumToStateName[LEDStateEnum::ERROR] = "ERROR";
    m_enumToStateName[LEDStateEnum::WARNING] = "WARNING";
    m_enumToStateName[LEDStateEnum::SUCCESS] = "SUCCESS";
    m_enumToStateName[LEDStateEnum::CUSTOM1] = "CUSTOM1";
    m_enumToStateName[LEDStateEnum::CUSTOM2] = "CUSTOM2";
    m_enumToStateName[LEDStateEnum::CUSTOM3] = "CUSTOM3";
}

void LEDSubsystem::Log(const nfr::LogContext& log) const
{
    log << "Current State: " << GetStateName();
    log << "Brightness: " << m_brightness;
    log << "Initialized: " << (m_initialized ? "true" : "false");
    log << "Num LEDs: " << m_numLEDs;
}

void LEDSubsystem::SetBrightness(double brightness)
{
    m_brightness = brightness;

    // Update the CANdle configuration with the new brightness
    ctre::phoenix6::configs::CANdleConfiguration config;
    m_candle.GetConfigurator().Refresh(config);
    config.LED.BrightnessScalar = m_brightness;
    m_candle.GetConfigurator().Apply(config);
}

void LEDSubsystem::TurnOff()
{
    // Set state to OFF
    SetStateEnum(LEDStateEnum::OFF);
}
