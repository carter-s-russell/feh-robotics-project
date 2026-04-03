#include "arm.h"

Arm::Arm(FEHMotor::FEHMotorPort motorPort) : liftMotor(motorPort, 9.0) {}

void Arm::raise() {
    liftMotor.SetPercent(LIFT_POWER);
}

void Arm::lower() {
    liftMotor.SetPercent(-LOWER_POWER);
}

void Arm::stop() {
    liftMotor.Stop();
}