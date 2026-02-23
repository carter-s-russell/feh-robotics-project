#include <FEHUtility.h>
#include <FEHLCD.h>
#include <FEHIO.h>
#include <FEHSD.h>
#include <Arduino.h>
#include <FEHServo.h>
#include <FEHMotor.h>

// encoder const
const int IGUWAN_COUNTS_PER_ROTATION = 318;
const int WHEEL_RADIUS = 1.5;
const int COUNTS_PER_INCH = IGUWAN_COUNTS_PER_ROTATION / (2 * PI * WHEEL_RADIUS);

// optosensor const
const float OPTOSENSOR_THRESHOLD = 2.5;

// pin const
const int RIGHT_ENCODER_PIN = FEHIO::Pin7;
const int LEFT_ENCODER_PIN = FEHIO::Pin8;

// motor const
const float MAX_VOLTAGE = 9.0;
const int RIGHT_MOTOR_PIN = FEHMotor::Motor0;
const int LEFT_MOTOR_PIN = FEHMotor::Motor1;

// enums
enum Direction {
    LEFT = -1,
    RIGHT = 1
};

// functions
void drive_forward(int inches, DigitalEncoder right_encoder, DigitalEncoder left_encoder, FEHMotor right_motor, FEHMotor left_motor, int percent)
{
    int counts = ceil(inches * COUNTS_PER_INCH);

    //Reset encoder counts
    right_encoder.ResetCounts();
    left_encoder.ResetCounts();

    //Set both motors to desired percent
    right_motor.SetPercent(-1 * percent);
    left_motor.SetPercent(percent);

    //While the average of the left and right encoder is less than counts,
    //keep running motors
    while((left_encoder.Counts() + right_encoder.Counts()) / 2. < counts);

    //Turn off motors
    right_motor.Stop();
    left_motor.Stop();
}

void turn(Direction dir, DigitalEncoder right_encoder, DigitalEncoder left_encoder, FEHMotor right_motor, FEHMotor left_motor, int percent) {
    int counts = ceil(6 * COUNTS_PER_INCH);

    //Reset encoder counts
    right_encoder.ResetCounts();
    left_encoder.ResetCounts();

    //Set both motors to desired percent
    right_motor.SetPercent(dir * percent);
    left_motor.SetPercent(dir * percent);

    //While the average of the left and right encoder is less than counts,
    //keep running motors
    while((left_encoder.Counts() + right_encoder.Counts()) / 2. < counts);

    //Turn off motors
    right_motor.Stop();
    left_motor.Stop();
}