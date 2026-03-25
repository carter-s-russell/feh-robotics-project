#ifndef ROBOT_H
#define ROBOT_H

#include <FEHMotor.h>
#include <FEHIO.h>
#include <FEHServo.h>

#include "drivetrain.h"
#include "lightInterpreter.h"
#include "util.h"

class Robot {
private:
    Drivetrain drivetrain;
    LightInterpreter lightInterpreter;
    
    // FEHServo armServo;

public:
    // general constructor
    Robot(FEHMotor::FEHMotorPort driveRightMotor, FEHMotor::FEHMotorPort driveLeftMotor, 
          FEHIO::FEHIOPin driveRightEncoder, FEHIO::FEHIOPin driveLeftEncoder,
          FEHIO::FEHIOPin cdsPin);

    // constructor with drivetrain instead of individual motors and encoders
    Robot(Drivetrain& dt, FEHIO::FEHIOPin cdsPin);

    // member variable so I can change it in main if needed
    int m_MOTOR_SPEED = 30;

    void waitForStartLight();
    void completeCompostBinTask();
    void completeAppleBasketTask();
    void completeFertilizerTask();
    void completeHumidifierTask();
    void completeWindowTask();
    void completeFinalButtonTask();

    void testRobot();
};

#endif