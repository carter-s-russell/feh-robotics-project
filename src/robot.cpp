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
    /* positioning */
    // robot starts at start point so no positioning needed

    /* action */
    while(!lightInterpreter.isStartLightOn());
    drivetrain.driveForward(1, m_MOTOR_SPEED);
    Sleep(0.5);
    drivetrain.driveForward(-1, m_MOTOR_SPEED);
}

void Robot::completeCompostBinTask() {
    /* positioning */

    /* action */

}

void Robot::completeAppleBasketTask() {
    /* positioning */

    /* action */

}

void Robot::completeFertilizerTask() {
    /* positioning */

    /* action */

}

void Robot::completeHumidifierTask() {
    /* positioning */
    // bottom of ramp
    drivetrain.turn(270, RIGHT, m_MOTOR_SPEED);
    drivetrain.driveForward(4, m_MOTOR_SPEED);
    drivetrain.turn(45, LEFT, m_MOTOR_SPEED);

    // up ramp and to humidifier light
    drivetrain.driveForward(37, m_MOTOR_SPEED);
    drivetrain.turn(90, LEFT, m_MOTOR_SPEED);
    drivetrain.driveForward(16, m_MOTOR_SPEED);

    /* action */
    // read light
    Sleep(0.5);
    Direction dir = (lightInterpreter.getHumidifierColor() == 2) ? LEFT : RIGHT;

    // press button
    drivetrain.turn(11, dir, m_MOTOR_SPEED);
    drivetrain.driveForward(5.5, m_MOTOR_SPEED);
    Sleep(0.5);

    // return to light for next step
    drivetrain.driveForward(-5.5, m_MOTOR_SPEED);
}

void Robot::completeWindowTask() {
    /* positioning */

    /* action */

}

void Robot::completeFinalButtonTask() {
    /* positioning */
    // return to bottom light
    drivetrain.turn(180, RIGHT, m_MOTOR_SPEED);
    drivetrain.driveForward(16, m_MOTOR_SPEED);
    drivetrain.turn(90, RIGHT, m_MOTOR_SPEED);
    drivetrain.driveForward(37, m_MOTOR_SPEED);
    drivetrain.turn(45, RIGHT, m_MOTOR_SPEED);
    drivetrain.driveForward(4, m_MOTOR_SPEED);
    drivetrain.turn(90, LEFT, m_MOTOR_SPEED);

    /* action */
    // drive forward to press button
    drivetrain.driveForward(1, m_MOTOR_SPEED);

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