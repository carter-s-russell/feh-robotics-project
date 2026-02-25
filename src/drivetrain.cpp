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

public:
    Drivetrain(FEHMotor::FEHMotorPort rightMotorPort, FEHMotor::FEHMotorPort leftMotorPort, 
               FEHIO::FEHIOPin rightEncoderPin, FEHIO::FEHIOPin leftEncoderPin)
        : rightMotor(rightMotorPort, MAX_VOLTAGE), 
          leftMotor(leftMotorPort, MAX_VOLTAGE),
          rightEncoder(rightEncoderPin), 
          leftEncoder(leftEncoderPin) 
    {}

    void driveForward(int inches, int percent) {
        int counts = ceil(inches * COUNTS_PER_INCH);

        resetCounts();

        setMotorPercent(-1 * percent, percent);

        while((leftEncoder.Counts() + rightEncoder.Counts()) / 2.0 < counts);

        stopMotors();
    }

    void turn(float angle, Direction dir, int percent) {
        // for some reason 6 inches worked perfectly for the crayola bot but I'll calculate this with the actual robot
        constexpr int TURN_DIST = 6;
        int counts = ceil(TURN_DIST * COUNTS_PER_INCH * (angle / 90.0));

        resetCounts();

        setMotorPercent(dir * percent, dir * percent);

        while((leftEncoder.Counts() + rightEncoder.Counts()) / 2.0 < counts);

        stopMotors();
    }

    void driveInDirection(int inches, double angle, int percent) {
        
    }

private:
    void resetCounts() {
        rightEncoder.ResetCounts();
        leftEncoder.ResetCounts();
    }

    void stopMotors() {
        rightMotor.Stop();
        leftMotor.Stop();
    }

    void setMotorPercent(int right, int left) {
        rightMotor.SetPercent(right);
        leftMotor.SetPercent(left);
    }
};