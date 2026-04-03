#ifndef ARM_H
#define ARM_H

#include <FEHMotor.h>
#include <FEHIO.h>
#include <FEHUtility.h>

class Arm {
private:
    FEHMotor liftMotor;

    static constexpr int LIFT_POWER = 30;
    static constexpr int LOWER_POWER = 20;

public:
    Arm(FEHMotor::FEHMotorPort motorPort); 

    void raise();
    void lower();
    void stop();
};

#endif