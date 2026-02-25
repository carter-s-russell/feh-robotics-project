#include "drivetrain.cpp"
#include <FEHLCD.h>
#include <FEHIO.h>
#include <FEHMotor.h>


// declarations for encoders & motors
DigitalEncoder right_encoder(FEHIO::Pin8);
DigitalEncoder left_encoder(FEHIO::Pin9);
FEHMotor right_motor(FEHMotor::Motor0,9.0);
FEHMotor left_motor(FEHMotor::Motor1,9.0);

// other declariations
Drivetrain driveChain(FEHMotor::Motor0, FEHMotor::Motor1, FEHIO::Pin7, FEHIO::Pin8);

void ERCMain()
{
    driveChain.driveForward(6, 25);
}

