#include "../include/drivetrain.h"
#include "../include/util.h"
#include "../include/rcs.h"

#include <FEHLCD.h>
#include <FEHIO.h>
#include <FEHUtility.h>
#include <FEHMotor.h>
#include <FEHRCS.h>
#include <FEHSD.h>

// RCS Delay time
#define RCS_WAIT_TIME_IN_SEC 0.35

// Shaft encoding counts for CrayolaBots
#define COUNTS_PER_INCH 40.5
#define COUNTS_PER_DEGREE 2.48

// Defines for pulsing the robot
#define PULSE_TIME 0.05
#define PULSE_POWER 20

// Define for the motor power
#define POWER 30

// Orientation of AruCo Code
#define PLUS 0
#define MINUS 1

//Declarations for encoders & motors
DigitalEncoder right_encoder(FEHIO::Pin8);
DigitalEncoder left_encoder(FEHIO::Pin9);
FEHMotor right_motor(FEHMotor::Motor1, 9.0);
FEHMotor left_motor(FEHMotor::Motor0, 9.0);

/*
 * Pulse forward a short distance using time
 */
void pulse_forward(int percent, float seconds) 
{
    // Set both motors to desired percent
    right_motor.SetPercent(percent);
    left_motor.SetPercent(-percent);

    // Wait for the correct number of seconds
    Sleep(seconds);

    // Turn off motors
    right_motor.Stop();
    left_motor.Stop();
}

/*
 * Pulse counterclockwise a short distance using time
 */
void pulse_counterclockwise(int percent, float seconds) 
{
    // Set both motors to desired percent
    right_motor.SetPercent(percent);
    left_motor.SetPercent(percent);

    // Wait for the correct number of seconds
    Sleep(seconds);

    // Turn off motors
    right_motor.Stop();
    left_motor.Stop();
}

/*
 * Move forward using shaft encoders where percent is the motor percent and counts is the distance to travel
 */
void move_forward(int percent, int counts) //using encoders
{
    // Reset encoder counts
    right_encoder.ResetCounts();
    left_encoder.ResetCounts();

    // Set both motors to desired percent
    right_motor.SetPercent(percent);
    left_motor.SetPercent(-percent);

    // While the average of the left and right encoder are less than counts,
    // keep running motors
    while((left_encoder.Counts() + right_encoder.Counts()) / 2. < counts);

    // Turn off motors
    right_motor.Stop();
    left_motor.Stop();
}

/*
 * Turn counterclockwise using shaft encoders where percent is the motor percent and counts is the distance to travel
 */
void turn_counterclockwise(int percent, int counts) 
{
    // Reset encoder counts
    right_encoder.ResetCounts();
    left_encoder.ResetCounts();

    // Set both motors to desired percent
    right_motor.SetPercent(percent);
    left_motor.SetPercent(percent);

    // While the average of the left and right encoder are less than counts,
    // keep running motors
    while((left_encoder.Counts() + right_encoder.Counts()) / 2. < counts);

    // Turn off motors
    right_motor.Stop();
    left_motor.Stop();
}

/* 
 * Use RCS to move to the desired x_coordinate based on the orientation of the AruCo code
 */
void check_x(float x_coordinate, int orientation)
{
    int power = PULSE_POWER;
    if(orientation == MINUS){
        power = -PULSE_POWER;
    }

    RCSPose* pose = RCS.RequestPosition();

    for (int i = 0; i < 10; i++) {
        if (pose != nullptr && pose->x >= 0 && (pose->x < x_coordinate - 1.0 || pose->x > x_coordinate + 1.0))
        {
            if(pose->x > x_coordinate)
            {
                pulse_forward(-power, PULSE_TIME);
            }
            else if(pose->x < x_coordinate)
            {
                pulse_forward(power, PULSE_TIME);
            }
            Sleep(RCS_WAIT_TIME_IN_SEC);

            pose = RCS.RequestPosition();
        }
        else if (pose != nullptr && pose->x >= 0)
        {
            break;
        }
    }
}

void check_y(float y_coordinate, int orientation)
{
    int power = PULSE_POWER;
    if(orientation == MINUS){
        power = -PULSE_POWER;
    }

    RCSPose* pose = RCS.RequestPosition();

    for (int i = 0; i < 10; i++) {
        if (pose != nullptr && pose->y >= 0 && (pose->y < y_coordinate - 1.0 || pose->y > y_coordinate + 1.0))
        {
            if(pose->y > y_coordinate)
            {
                pulse_forward(-power, PULSE_TIME);
            }
            else if(pose->y < y_coordinate)
            {
                pulse_forward(power, PULSE_TIME);
            }
            Sleep(RCS_WAIT_TIME_IN_SEC);
            
            pose = RCS.RequestPosition();
        }
        else if (pose != nullptr && pose->y >= 0)
        {
            break;
        }
    }   
}

void check_heading(float heading)
{
    RCSPose* pose = RCS.RequestPosition();
    
    for (int i = 0; i < 10; i++) 
    {
        if (pose != nullptr && pose->heading >= 0) 
        {
            float error = heading - pose->heading;
            
            // Normalizing error to handle the 0/360 degree wrap-around edge case
            while (error > 180.0) error -= 360.0;
            while (error < -180.0) error += 360.0;
            
            if (abs(error) > 2.0) 
            {
                if (error > 0) 
                {
                    pulse_counterclockwise(PULSE_POWER, PULSE_TIME);
                } 
                else 
                {
                    pulse_counterclockwise(-PULSE_POWER, PULSE_TIME);
                }
                Sleep(RCS_WAIT_TIME_IN_SEC);
                pose = RCS.RequestPosition();
            } 
            else 
            {
                break;
            }
        }
    }
}

void ERCMain()
{
    int touch_x,touch_y;
    float A_x, A_y, B_x, B_y, C_x, C_y, D_x, D_y;
    float A_heading, B_heading, C_heading, D_heading;
    int B_C_counts, C_D_counts, turn_90_counts;

    RCS.InitializeTouchMenu("Z1TESTING");

    LCD.WriteLine("RCS & Data Logging Test");
    LCD.WriteLine("Press Screen To Start");
    while(!LCD.Touch(&touch_x,&touch_y));
    while(LCD.Touch(&touch_x,&touch_y));

    FEHFile* fptr = SD.FOpen("test.txt", "r");
    SD.FScanf(fptr, "%f%f", &A_x, &A_y);
    SD.FScanf(fptr, "%f%f", &B_x, &B_y);
    SD.FScanf(fptr, "%f%f", &C_x, &C_y);
    SD.FScanf(fptr, "%f%f", &D_x, &D_y);
    SD.FClose(fptr);

    A_heading = 90.0;
    B_heading = 180.0;
    C_heading = 270.0;
    D_heading = 0.0;

    B_C_counts = 12.79 * COUNTS_PER_INCH; 
    C_D_counts = 12.00 * COUNTS_PER_INCH; 

    turn_90_counts = 90 * COUNTS_PER_DEGREE;

    check_y(B_y, PLUS);
    check_heading(B_heading);

    move_forward(POWER, B_C_counts);
    check_x(C_x, MINUS);
    turn_counterclockwise(POWER, turn_90_counts);
    check_heading(C_heading);

    move_forward(POWER, C_D_counts);
    check_y(D_y, MINUS);
    turn_counterclockwise(POWER, turn_90_counts);
    check_heading(D_heading);
}
