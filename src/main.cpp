#include "robot.h"
#include "util.h"

// other declarations
Drivetrain drivetrain(FEHMotor::Motor0, FEHMotor::Motor1, FEHIO::Pin8, FEHIO::Pin9);
Robot robot(drivetrain, FEHIO::Pin0);

void initiateRun() {
    robot.waitForStartLight();
    robot.completeHumidifierTask();
    robot.completeFinalButtonTask();
}

void ERCMain() {
    initiateRun();
}