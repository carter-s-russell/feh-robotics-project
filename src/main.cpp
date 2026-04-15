#include "robot.h"
#include "util.h"

// other declarations
Drivetrain drivetrain(FEHMotor::Motor0, FEHMotor::Motor1, FEHIO::Pin8, FEHIO::Pin9);
Robot robot(drivetrain, FEHIO::Pin0, FEHMotor::Motor2);
AnalogInputPin cds(FEHIO::Pin1);

void initiateRun() {
    //robot.waitForStartLight();
    //Sleep(1.0);
    //LCD.Clear();
    LCD.WriteLine("Starting");
    robot.completeCompostBinTask();
    robot.completeAppleBasketTask();
    //robot.completeHumidifierTask();
    //robot.completeFinalButtonTask();
}

void ERCMain() {
    initiateRun();
    //robot.testRobot();
}