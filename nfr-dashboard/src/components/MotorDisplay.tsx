import { useEntry } from "@frc-web-components/react";

interface MotorDisplayProps {
    motorId: number;
}

function MotorDisplay({ motorId }: MotorDisplayProps) {
    const [type] = useEntry(`FWC/Motors/${motorId}/Type`, 'Unknown');
    switch (type) {
    case 'TalonFX':
        return (
            <h1>TODO: Implement TalonFX Display</h1>
        );
    case 'TalonFXS':
        return (
            <h1>TODO: Implement TalonFXS Display</h1>
        );
    case 'SparkMax':
        return (
            <h1>TODO: Implement SparkMax Display</h1>
        );
    }
}

export default MotorDisplay;