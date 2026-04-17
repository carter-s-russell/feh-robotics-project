#include "arm.h"

Arm::Arm(FEHMotor::FEHMotorPort motorPort) : liftMotor(motorPort, 9.0) {}

void Arm::raise(double seconds) {
    liftMotor.SetPercent(-LIFT_POWER);
    Sleep(seconds);
    liftMotor.Stop();
}

void Arm::lower(double seconds) {
    liftMotor.SetPercent(LOWER_POWER);
    Sleep(seconds);
    liftMotor.Stop();
}

void Arm::stop() {
    liftMotor.Stop();
}
