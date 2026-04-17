#include "robot.h"
#include <FEHLCD.h>

Robot::Robot(FEHMotor::FEHMotorPort driveRightMotor, FEHMotor::FEHMotorPort driveLeftMotor, 
             FEHIO::FEHIOPin driveRightEncoder, FEHIO::FEHIOPin driveLeftEncoder,
             FEHIO::FEHIOPin cdsPin, FEHMotor::FEHMotorPort liftMotor)
    : drivetrain(driveRightMotor, driveLeftMotor, driveRightEncoder, driveLeftEncoder),
      lightInterpreter(cdsPin), arm(liftMotor)
{
}

Robot::Robot(Drivetrain& dt, FEHIO::FEHIOPin cdsPin, FEHMotor::FEHMotorPort liftMotor)
    : drivetrain(dt),
      lightInterpreter(cdsPin),
      arm(liftMotor)
{
}

void Robot::waitForStartLight() {
    /* positioning */
    // robot starts at start point so no positioning needed

    /* action */
    while(!lightInterpreter.isStartLightOn());
    drivetrain.drive(-1, m_MOTOR_SPEED);
    arm.lower(1.9);
    drivetrain.driveUntilWall(m_MOTOR_SPEED);
    drivetrain.drive(-1, m_MOTOR_SPEED);
    drivetrain.turn(90, RIGHT, m_MOTOR_SPEED);
    drivetrain.turn(90, RIGHT, m_MOTOR_SPEED);
    drivetrain.drive(-5, m_MOTOR_SPEED);
    arm.raise(2.0);
}

void Robot::completeCompostBinTask() {
    /* positioning */
    drivetrain.drive(2, m_MOTOR_SPEED/2);
    drivetrain.turn(90, LEFT, m_MOTOR_SPEED);
    drivetrain.drive(3.75, m_MOTOR_SPEED);
    drivetrain.turn(20, RIGHT, m_MOTOR_SPEED/2);
    drivetrain.drive(1.75, m_MOTOR_SPEED/2);
    drivetrain.turn(25, RIGHT, m_MOTOR_SPEED/2);
    drivetrain.driveUntilWall(m_MOTOR_SPEED);
    drivetrain.drive(-2.2, m_MOTOR_SPEED/2);

    /* action */
    Sleep(1.0);
    int n = 1;
    for (int i = 0; i < n; i++) {
        drivetrain.drive(2, m_MOTOR_SPEED/2);
        arm.lower(1.9);
        drivetrain.drive(-2, m_MOTOR_SPEED/2);
        arm.raise(2.0);
    }
}

void Robot::completeAppleBasketTask() {
    /* positioning (pick up) */
    drivetrain.drive(-2, m_MOTOR_SPEED);
    drivetrain.turn(90, RIGHT, m_MOTOR_SPEED);
    drivetrain.drive(5, m_MOTOR_SPEED);
    drivetrain.turn(45, LEFT, m_MOTOR_SPEED);
    arm.lower(1.9);
    drivetrain.drive(14, m_MOTOR_SPEED);
    drivetrain.turn(45, LEFT, m_MOTOR_SPEED);

    /* action (pick up) */
    drivetrain.driveUntilWall(m_MOTOR_SPEED);
    drivetrain.drive(-3, m_MOTOR_SPEED/2);
    Sleep(1.0);


    /* positioning (drop off) */
    /*
    drivetrain.turn(45, RIGHT, m_MOTOR_SPEED);
    drivetrain.drive(-8, m_MOTOR_SPEED);
    drivetrain.turn(45, LEFT, m_MOTOR_SPEED);
    drivetrain.drive(-10, m_MOTOR_SPEED);
    drivetrain.turn(90, RIGHT, m_MOTOR_SPEED);
    drivetrain.drive(30, m_MOTOR_SPEED);
    drivetrain.turn(90, LEFT, m_MOTOR_SPEED);
    drivetrain.drive(10, m_MOTOR_SPEED);
    drivetrain.turn(90, RIGHT, m_MOTOR_SPEED);
    arm.raise(2.0);
    drivetrain.drive(20, m_MOTOR_SPEED);
    arm.lower(1.9);
    */

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
    drivetrain.drive(26, m_MOTOR_SPEED);
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
    Sleep(5.5);

    // press button
    drivetrain.turn(9, dir, m_MOTOR_SPEED/2);
    drivetrain.driveUntilWall(m_MOTOR_SPEED);

    // return to light for next step
    drivetrain.drive(-5.5, m_MOTOR_SPEED/2);
    int directionInt = (dir == LEFT) ? -1 : 1;
    drivetrain.turn(180 + (directionInt * 9), LEFT, m_MOTOR_SPEED);
}

void Robot::completeWindowTask() {
    /* positioning */
    // bottom of ramp
    drivetrain.drive(-2, m_MOTOR_SPEED);
    drivetrain.turn(90, LEFT, m_MOTOR_SPEED);
    drivetrain.drive(7.1, m_MOTOR_SPEED);
    drivetrain.turn(45, LEFT, m_MOTOR_SPEED);

    // up ramp and open window
    drivetrain.drive(24.5, m_MOTOR_SPEED);
    drivetrain.turn(90, RIGHT, m_MOTOR_SPEED);
    Util::waitForTouch();
    drivetrain.drive(-20, m_MOTOR_SPEED);

    /* action */
    // reposition and close window
    drivetrain.turn(12, LEFT, m_MOTOR_SPEED/2);
    Util::waitForTouch();
    drivetrain.drive(20, m_MOTOR_SPEED);

}

void Robot::completeFinalButtonTask() {
    /* positioning */
    drivetrain.turn(135, LEFT, m_MOTOR_SPEED);
    drivetrain.driveUntilWall(m_MOTOR_SPEED);

    /* action */
}

void Robot::testRobot() {
    drivetrain.drive(-1, m_MOTOR_SPEED);
    arm.lower(1.9);
    drivetrain.driveUntilWall(m_MOTOR_SPEED);
    drivetrain.drive(-1, m_MOTOR_SPEED);
    drivetrain.turn(90, RIGHT, m_MOTOR_SPEED);
    drivetrain.turn(90, RIGHT, m_MOTOR_SPEED);
    drivetrain.drive(-5, m_MOTOR_SPEED);
    arm.raise(2.0);
}