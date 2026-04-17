#ifndef ARM_H
#define ARM_H

#include <FEHMotor.h>
#include <FEHIO.h>
#include <FEHUtility.h>

class Arm {
private:
    FEHMotor liftMotor;

    static constexpr int LIFT_POWER = 40;
    static constexpr int LOWER_POWER = 40;

public:
    Arm(FEHMotor::FEHMotorPort motorPort); 

    void raise(double seconds);
    void lower(double seconds);
    void stop();
};

#endif