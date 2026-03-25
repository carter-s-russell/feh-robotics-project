#ifndef DRIVETRAIN_H
#define DRIVETRAIN_H

#include <FEHUtility.h>
#include <FEHLCD.h>
#include <FEHIO.h>
#include <FEHSD.h>
#include <Arduino.h>
#include <FEHServo.h>
#include <FEHMotor.h>
#include <math.h>

enum Direction {
    LEFT = -1,
    RIGHT = 1
};

class Drivetrain {
private:
    FEHMotor rightMotor;
    FEHMotor leftMotor;
    DigitalEncoder rightEncoder;
    DigitalEncoder leftEncoder;

    static constexpr int IGUWAN_COUNTS_PER_ROTATION = 318;
    static constexpr float WHEEL_RADIUS = 1.5;
    static constexpr float COUNTS_PER_INCH = IGUWAN_COUNTS_PER_ROTATION / (2 * PI * WHEEL_RADIUS);
    static constexpr float MAX_VOLTAGE = 9.0;
    static constexpr float TRACK_WIDTH = 7.75;

    /**
     * @brief Resets the pulse counts of both the right and left encoders to zero
     */
    void resetCounts();

    /**
     * @brief Ramps the motors up to a specified percent
     * 
     * @param targetRight desired final motor speed for right motor
     * @param targetLeft  desired final motor speed for left motor
     */
    void rampToPercent(int targetRight, int targetLeft);

public:
    /**
     * @brief Constructs a new Drivetrain object
     * 
     * @param rightMotorPort The port assignment for the right motor
     * @param leftMotorPort The port assignment for the left motor
     * @param rightEncoderPin The pin assignment for the right encoder
     * @param leftEncoderPin The pin assignment for the left encoder
     */
    Drivetrain(FEHMotor::FEHMotorPort rightMotorPort, FEHMotor::FEHMotorPort leftMotorPort, 
               FEHIO::FEHIOPin rightEncoderPin, FEHIO::FEHIOPin leftEncoderPin);

    /**
     * @brief Drives the robot forward a specified distance
     * @param inches The distance to drive forward in inches
     * @param percent The motor power percentage (0 to 100)
     */
    void drive(double inches, int percent);

/**
     * @brief Drives the robot continuously until physical resistance (a wall) stops the encoders
     * @param percent The motor power percentage (0 to 100). Note: Internally capped at 30% to prevent hardware damage.
     */
    void driveUntilWall(int percent);

    /**
     * @brief Turns the robot a specified angle in a given direction
     * 
     * @param angle The angle to turn in degrees
     * @param dir The direction to turn (LEFT or RIGHT)
     * @param percent The motor power percentage (0 to 100)
     */
    void turn(float angle, Direction dir, int percent);

    /**
     * @brief Stops power to both the right and left motors
     */
    void stopMotors();

    /**
     * @brief Sets the  power percentage for both motors
     * 
     * @param right The power percentage for the right motor (-100 to 100)
     * @param left The power percentage for the left motor (-100 to 100)
     */
    void setMotorPercent(int right, int left);
};

#endif