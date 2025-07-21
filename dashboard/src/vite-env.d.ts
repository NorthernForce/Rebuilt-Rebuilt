/// <reference types="vite/client" />

// FRC Web Components type declarations
declare global {
  namespace JSX {
    interface IntrinsicElements {
      'frc-number-bar': {
        value?: number;
        min?: number;
        max?: number;
        precision?: number;
      };
      'frc-gyro': {
        value?: number;
      };
      'frc-field': {
        game?: string;
      };
      'frc-camera': {
        source?: string;
      };
    }
  }
}
