#include "../include/drivetrain.h"

// constructor
Drivetrain::Drivetrain(FEHMotor::FEHMotorPort rightMotorPort, FEHMotor::FEHMotorPort leftMotorPort, 
                       FEHIO::FEHIOPin rightEncoderPin, FEHIO::FEHIOPin leftEncoderPin)
    : rightMotor(rightMotorPort, MAX_VOLTAGE), 
      leftMotor(leftMotorPort, MAX_VOLTAGE),
      rightEncoder(rightEncoderPin), 
      leftEncoder(leftEncoderPin) 
{}

// public functions
void Drivetrain::driveForward(int inches, int percent) {
    int counts = ceil(inches * COUNTS_PER_INCH);

    resetCounts();
    setMotorPercent(-1 * percent, percent);

    while((leftEncoder.Counts() + rightEncoder.Counts()) / 2.0 < counts);

    stopMotors();
}

void Drivetrain::turn(float angle, Direction dir, int percent) {
    // distance in inches to make a 360 degree turn
    constexpr float TURN_DIST = PI * TRACK_WIDTH;
    int counts = ceil(TURN_DIST * COUNTS_PER_INCH * (angle / 360.0));

    resetCounts();
    setMotorPercent(dir * percent, dir * percent);

    while((leftEncoder.Counts() + rightEncoder.Counts()) / 2.0 < counts);

    stopMotors();
}

void Drivetrain::driveInDirection(int inches, double angle, int percent) {
    // might not be necesary im not sure lowk
}

void Drivetrain::stopMotors() {
    rightMotor.Stop();
    leftMotor.Stop();
}

void Drivetrain::setMotorPercent(int right, int left) {
    rightMotor.SetPercent(right);
    leftMotor.SetPercent(left);
}

// private functions
void Drivetrain::resetCounts() {
    rightEncoder.ResetCounts();
    leftEncoder.ResetCounts();
}
