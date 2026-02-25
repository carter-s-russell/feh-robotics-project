#include "../include/drivetrain.h"

Drivetrain::Drivetrain(FEHMotor::FEHMotorPort rightMotorPort, FEHMotor::FEHMotorPort leftMotorPort, 
                       FEHIO::FEHIOPin rightEncoderPin, FEHIO::FEHIOPin leftEncoderPin)
    : rightMotor(rightMotorPort, MAX_VOLTAGE), 
      leftMotor(leftMotorPort, MAX_VOLTAGE),
      rightEncoder(rightEncoderPin), 
      leftEncoder(leftEncoderPin) 
{}

void Drivetrain::driveForward(int inches, int percent) {
    int counts = ceil(inches * COUNTS_PER_INCH);

    resetCounts();
    setMotorPercent(-1 * percent, percent);

    while((leftEncoder.Counts() + rightEncoder.Counts()) / 2.0 < counts);

    stopMotors();
}

void Drivetrain::turn(float angle, Direction dir, int percent) {
    // for some reason 6 inches worked perfectly for the crayola bot but I'll calculate this with the actual robot
    constexpr int TURN_DIST = 6;
    int counts = ceil(TURN_DIST * COUNTS_PER_INCH * (angle / 90.0));

    resetCounts();
    setMotorPercent(dir * percent, dir * percent);

    while((leftEncoder.Counts() + rightEncoder.Counts()) / 2.0 < counts);

    stopMotors();
}

void Drivetrain::driveInDirection(int inches, double angle, int percent) {
    
}

void Drivetrain::resetCounts() {
    rightEncoder.ResetCounts();
    leftEncoder.ResetCounts();
}

void Drivetrain::stopMotors() {
    rightMotor.Stop();
    leftMotor.Stop();
}

void Drivetrain::setMotorPercent(int right, int left) {
    rightMotor.SetPercent(right);
    leftMotor.SetPercent(left);
}