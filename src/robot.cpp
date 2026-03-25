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
    drivetrain.drive(1, m_MOTOR_SPEED);
    Sleep(0.5);
    drivetrain.drive(-1, m_MOTOR_SPEED);
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
    drivetrain.drive(-2, m_MOTOR_SPEED);
    drivetrain.turn(90, LEFT, m_MOTOR_SPEED);
    drivetrain.drive(8, m_MOTOR_SPEED);
    drivetrain.turn(45, LEFT, m_MOTOR_SPEED);

    // up ramp and to humidifier light
    drivetrain.drive(25, m_MOTOR_SPEED);
    // move 4 in forward and 5 in left while avoiding table
    constexpr float angle = 51.34;
    drivetrain.turn(angle, LEFT, m_MOTOR_SPEED);
    drivetrain.drive(6.4, m_MOTOR_SPEED);

    drivetrain.turn(90 - angle, LEFT, m_MOTOR_SPEED);
    drivetrain.drive(7, m_MOTOR_SPEED);

    /* action */
    // read light
    Sleep(0.5);
    Direction dir = (lightInterpreter.getHumidifierColor() == 2) ? LEFT : RIGHT;
    dir = LEFT;
    Sleep(1.5);

    // press button
    drivetrain.turn(11, dir, m_MOTOR_SPEED/2);
    drivetrain.drive(5.5, m_MOTOR_SPEED/2);
    Sleep(2.5);

    // return to light for next step
    drivetrain.drive(-5.5, m_MOTOR_SPEED/2);
}

void Robot::completeWindowTask() {
    /* positioning */

    /* action */

}

void Robot::completeFinalButtonTask() {
    /* positioning */
    // return to bottom light
    drivetrain.turn(180, RIGHT, m_MOTOR_SPEED);
    drivetrain.drive(16, m_MOTOR_SPEED);
    drivetrain.turn(90, RIGHT, m_MOTOR_SPEED);
    drivetrain.drive(37, m_MOTOR_SPEED);
    drivetrain.turn(45, RIGHT, m_MOTOR_SPEED);
    drivetrain.drive(4, m_MOTOR_SPEED);
    drivetrain.turn(90, LEFT, m_MOTOR_SPEED);

    /* action */
    // drive forward to press button
    drivetrain.drive(1, m_MOTOR_SPEED);

}

void Robot::testRobot() {
    Util::waitForTouch();
    drivetrain.turn(90, LEFT, 40);
    Sleep(0.5);
    drivetrain.turn(90, LEFT, 40);
    Sleep(0.5);
    drivetrain.turn(90, LEFT, 40);
    Sleep(0.5);
    drivetrain.turn(90, LEFT, 40);
    Sleep(0.5);
}