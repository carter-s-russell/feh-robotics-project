#include "robot.h"
#include <FEHLCD.h>

Robot::Robot(FEHMotor::FEHMotorPort driveRightMotor, FEHMotor::FEHMotorPort driveLeftMotor, 
             FEHIO::FEHIOPin driveRightEncoder, FEHIO::FEHIOPin driveLeftEncoder,
             FEHIO::FEHIOPin cdsPin)
    : drivetrain(driveRightMotor, driveLeftMotor, driveRightEncoder, driveLeftEncoder),
      lightInterpreter(cdsPin) 
{
}

Robot::Robot(Drivetrain& dt, FEHIO::FEHIOPin cdsPin)
    : drivetrain(dt),
      lightInterpreter(cdsPin)
{
}

void Robot::waitForStartLight() {

}

void Robot::completeCompostBinTask() {
    
}

void Robot::completeAppleBasketTask() {
    
}

void Robot::completeFertilizerTask() {
    
}

void Robot::completeHumidifierTask() {

}

void Robot::completeWindowTask() {
    
}

void Robot::completeFinalButtonTask() {
    
}

void Robot::testRobot() {
    Util::waitForTouch();
    drivetrain.driveForward(39, 40);
    Sleep(0.5);
    drivetrain.turn(90, LEFT, 40);
    Sleep(0.5);
    drivetrain.driveForward(15, 40);
    Util::waitForTouch();
    drivetrain.turn(180, LEFT, 40);
    Sleep(0.5);
    drivetrain.driveForward(15, 40);
    Sleep(0.5);
    drivetrain.turn(90, RIGHT, 40);
    Sleep(0.5);
    drivetrain.driveForward(35, 40);
}