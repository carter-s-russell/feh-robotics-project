#include "../include/drivetrain.h"
#include "../include/util.h"

// declarations for encoders & motors
DigitalEncoder right_encoder(FEHIO::Pin8);
DigitalEncoder left_encoder(FEHIO::Pin9);
FEHMotor right_motor(FEHMotor::Motor0,9.0);
FEHMotor left_motor(FEHMotor::Motor1,9.0);

// other declarations
Drivetrain driveChain(FEHMotor::Motor0, FEHMotor::Motor1, FEHIO::Pin8, FEHIO::Pin9);

void ERCMain()
{
    Util::waitForTouch();
    driveChain.driveForward(13, -40);
    Sleep(0.5);
    driveChain.turn(85, LEFT, 40);
    driveChain.driveForward(10, -40);
    driveChain.turn(38, LEFT, 40);
    Sleep(0.5);
    Util::waitForTouch();
    driveChain.driveForward(15, -40);
    driveChain.turn(85, LEFT, 40);
    driveChain.driveForward(15, -40);
}