#include "../include/drivetrain.h"

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
    rampToPercent(-1 * percent, percent);

    while((leftEncoder.Counts() + rightEncoder.Counts()) / 2.0 < counts);

    stopMotors();
}

void Drivetrain::turn(float angle, Direction dir, int percent) {
    // distance in inches to make a 360 degree turn
    constexpr float TURN_DIST = PI * TRACK_WIDTH;
    int counts = ceil(TURN_DIST * COUNTS_PER_INCH * (angle / 360.0));

    resetCounts();
    rampToPercent(dir * percent, dir * percent);

    while((leftEncoder.Counts() + rightEncoder.Counts()) / 2.0 < counts);

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
