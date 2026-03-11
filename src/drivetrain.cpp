#include "drivetrain.h"

// constructor
Drivetrain::Drivetrain(FEHMotor::FEHMotorPort rightMotorPort, FEHMotor::FEHMotorPort leftMotorPort, 
                       FEHIO::FEHIOPin rightEncoderPin, FEHIO::FEHIOPin leftEncoderPin)
    : rightMotor(rightMotorPort, MAX_VOLTAGE), 
      leftMotor(leftMotorPort, MAX_VOLTAGE),
      rightEncoder(rightEncoderPin), 
      leftEncoder(leftEncoderPin) 
{}

// private functions
void Drivetrain::resetCounts() {
    rightEncoder.ResetCounts();
    leftEncoder.ResetCounts();
}

void Drivetrain::rampToPercent(int targetRight, int targetLeft) {
    constexpr int START_POWER = 10;
    constexpr int INCREMENT = 2;
    constexpr float RAMP_DELAY_SEC = 0.02;

    int maxMagnitude = abs(targetRight);

    // assumes symmetric power magnitude for standard point turns and straight drives
    // this might have to be changed later but idk
    for (int p = START_POWER; p <= maxMagnitude; p += INCREMENT) {
        int r = (targetRight > 0) ? p : -p;
        int l = (targetLeft > 0) ? p : -p;
        
        setMotorPercent(r, l);
        Sleep(RAMP_DELAY_SEC);
    }
    
    setMotorPercent(targetRight, targetLeft);
}

// public functions
void Drivetrain::driveForward(int inches, int percent) {
    int counts = ceil(inches * COUNTS_PER_INCH);

    resetCounts();
    
    // 1. Acceleration: Break static friction smoothly
    rampToPercent(-1 * percent, percent);

    // Constants for tuning the control loop
    constexpr float Kp_decel = 0.5;          // Proportional gain for braking
    constexpr int MIN_POWER = 15;            // Minimum power to prevent stalling
    constexpr float DECEL_THRESHOLD = 150.0; // Begin braking when 150 counts away
    constexpr float Kp_straight = 1.0;       // Proportional gain for straight-line correction

    float currentAvgCounts = 0.0;

    // 2. The Control Loop
    while(currentAvgCounts < counts) {
        currentAvgCounts = (leftEncoder.Counts() + rightEncoder.Counts()) / 2.0;
        
        float error = counts - currentAvgCounts;
        int currentPower = percent;

        // 3. Deceleration: Proportionally reduce power as error approaches zero
        if (error < DECEL_THRESHOLD) {
            currentPower = round(error * Kp_decel);
            
            // THE FIX: Cap the power so it never spikes above your cruising speed
            if (currentPower > percent) {
                currentPower = percent;
            }

            // Prevent the robot from stalling out before hitting the target
            if (currentPower < MIN_POWER) {
                currentPower = MIN_POWER;
            }
        }

        // 4. Straight-line correction
        // If the left wheel spins faster, encoderDiff is positive. 
        // We subtract the adjustment from the left motor and add it to the right motor.
        int encoderDiff = leftEncoder.Counts() - rightEncoder.Counts();
        int adjustment = round(encoderDiff * Kp_straight);

        // Apply dynamically calculated power (maintaining your -1 polarity for the right motor)
        setMotorPercent(-(currentPower + adjustment), currentPower - adjustment);
    }

    // Print diagnostic data to the screen
    LCD.WriteLine(counts);
    LCD.WriteLine(rightEncoder.Counts());
    LCD.WriteLine(leftEncoder.Counts());

    stopMotors();
}

void Drivetrain::turn(float angle, Direction dir, int percent) {
    // distance in inches to make a 360 degree turn
    constexpr float TURN_DIST = PI * TRACK_WIDTH;
    int counts = ceil(TURN_DIST * COUNTS_PER_INCH * (angle / 360.0));

    resetCounts();
    
    // 1. Acceleration
    rampToPercent(dir * percent, dir * percent);

    // Constants for tuning the control loop
    constexpr float Kp_decel = 0.5; 
    constexpr int MIN_POWER = 15; 
    constexpr float DECEL_THRESHOLD = 150.0; 

    float currentAvgCounts = 0.0;

    // 2. The Control Loop
    while(currentAvgCounts < counts) {
        currentAvgCounts = (leftEncoder.Counts() + rightEncoder.Counts()) / 2.0;
        
        float error = counts - currentAvgCounts;
        int currentPower = percent;

        // 3. Deceleration: Proportionally reduce power as error approaches zero
        if (error < DECEL_THRESHOLD) {
            currentPower = round(error * Kp_decel);
            
            // THE FIX: Cap the power so it never spikes above your cruising speed
            if (currentPower > percent) {
                currentPower = percent;
            }

            // Prevent the robot from stalling out before hitting the target
            if (currentPower < MIN_POWER) {
                currentPower = MIN_POWER;
            }
        }

        // Straight-line correction is omitted because the wheels should spin in opposite directions
        setMotorPercent(dir * currentPower, dir * currentPower);
    }

    // Print diagnostic data to the screen
    LCD.WriteLine(counts);
    LCD.WriteLine(rightEncoder.Counts());
    LCD.WriteLine(leftEncoder.Counts());

    stopMotors();
}

void Drivetrain::stopMotors() {
    rightMotor.Stop();
    leftMotor.Stop();
}

void Drivetrain::setMotorPercent(int right, int left) {
    rightMotor.SetPercent(right);
    leftMotor.SetPercent(left);
}
