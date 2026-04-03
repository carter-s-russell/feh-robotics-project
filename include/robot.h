#ifndef ROBOT_H
#define ROBOT_H

#include <FEHMotor.h>
#include <FEHIO.h>
#include <FEHServo.h>

#include "drivetrain.h"
#include "lightInterpreter.h"
#include "arm.h"
#include "util.h"

class Robot {
private:
    Drivetrain drivetrain;
    LightInterpreter lightInterpreter;
    Arm arm;
    
    // FEHServo armServo;

public:
    // general constructor
    Robot(FEHMotor::FEHMotorPort driveRightMotor, FEHMotor::FEHMotorPort driveLeftMotor, 
          FEHIO::FEHIOPin driveRightEncoder, FEHIO::FEHIOPin driveLeftEncoder,
          FEHIO::FEHIOPin cdsPin, FEHMotor::FEHMotorPort liftMotor);

    // constructor with drivetrain instead of individual motors and encoders
    Robot(Drivetrain& dt, FEHIO::FEHIOPin cdsPin, FEHMotor::FEHMotorPort liftMotor);

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