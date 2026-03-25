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
void Drivetrain::drive(double inches, int percent) {
    // test if robot is moving forward or not
    int forward = 1;
    if (inches < 0) {
        inches = -1 * inches;
        forward = -1;
    }

    int counts = ceil(inches * COUNTS_PER_INCH);
    resetCounts();
    
    if (inches > 3) {
        rampToPercent(forward * -1 * percent, forward * percent);
    } else {
        percent = 15; 
        setMotorPercent(forward * -1 * percent, forward * percent);
    }

    // constants for tuning the control loop
    constexpr float Kp_decel = 0.5;
    constexpr int MIN_POWER = 15;
    constexpr float Kp_straight = 1.0;
    
    float DECEL_THRESHOLD = 150.0;
    if (counts / 2.0 < 150.0) {
        DECEL_THRESHOLD = counts / 2.0;
    }

    float currentAvgCounts = 0.0;

    while(currentAvgCounts < counts) {
        currentAvgCounts = (leftEncoder.Counts() + rightEncoder.Counts()) / 2.0;
        
        float error = counts - currentAvgCounts;
        int currentPower = percent;

        if (error < DECEL_THRESHOLD) {
            currentPower = round(error * Kp_decel);
            
            if (currentPower > percent) {
                currentPower = percent;
            }

            if (currentPower < MIN_POWER) {
                currentPower = MIN_POWER;
            }
        }

        int encoderDiff = leftEncoder.Counts() - rightEncoder.Counts();
        int adjustment = round(encoderDiff * Kp_straight);

        setMotorPercent(forward * -(currentPower + adjustment), forward * (currentPower - adjustment));
    }

    stopMotors();
}

void Drivetrain::turn(float angle, Direction dir, int percent) {
    // distance in inches to make a 360 degree turn
    constexpr float TURN_DIST = PI * TRACK_WIDTH;
    int counts = ceil(TURN_DIST * COUNTS_PER_INCH * (angle / 360.0) * 0.98);
    resetCounts();

    if (angle > 15.0) {
        rampToPercent(dir * percent, dir * percent);
    } else {
        percent = 20; 
        setMotorPercent(dir * percent, dir * percent);
    }

    // constants for tuning the control loop
    constexpr float Kp_decel = 0.5; 
    constexpr int MIN_POWER = 15; 
    float DECEL_THRESHOLD = 150.0; 
    if (counts / 2.0 < 150.0) {
        DECEL_THRESHOLD = counts / 2.0;
    }

    float currentAvgCounts = 0.0;

    while(currentAvgCounts < counts) {
        currentAvgCounts = (leftEncoder.Counts() + rightEncoder.Counts()) / 2.0;
        
        float error = counts - currentAvgCounts;
        int currentPower = percent;

        if (error < DECEL_THRESHOLD) {
            currentPower = round(error * Kp_decel);
            
            if (currentPower > percent) {
                currentPower = percent;
            }

            if (currentPower < MIN_POWER) {
                currentPower = MIN_POWER;
            }
        }

        setMotorPercent(dir * currentPower, dir * currentPower);
    }

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
