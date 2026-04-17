#include "drivetrain.h"

// constructor
Drivetrain::Drivetrain(FEHMotor::FEHMotorPort rightMotorPort, FEHMotor::FEHMotorPort leftMotorPort, 
                       FEHIO::FEHIOPin rightEncoderPin, FEHIO::FEHIOPin leftEncoderPin)
    : rightMotor(rightMotorPort, MAX_VOLTAGE), 
      leftMotor(leftMotorPort, MAX_VOLTAGE),
      rightEncoder(rightEncoderPin), 
      leftEncoder(leftEncoderPin) 
{}

// private functions
void Drivetrain::resetCounts() {
    rightEncoder.ResetCounts();
    leftEncoder.ResetCounts();
}

void Drivetrain::rampToPercent(int targetRight, int targetLeft) {
    constexpr int START_POWER = 10;
    constexpr int INCREMENT = 2;
    constexpr float RAMP_DELAY_SEC = 0.02;

    int maxMagnitude = abs(targetRight);

    // assumes symmetric power magnitude for standard point turns and straight drives
    // this might have to be changed later but idk
    for (int p = START_POWER; p <= maxMagnitude; p += INCREMENT) {
        int r = (targetRight > 0) ? p : -p;
        int l = (targetLeft > 0) ? p : -p;
        
        setMotorPercent(r, l);
        Sleep(RAMP_DELAY_SEC);
    }
    
    setMotorPercent(targetRight, targetLeft);
}

// public functions
void Drivetrain::drive(double inches, int percent) {
    // test if robot is moving forward or not
    int forward = 1;
    if (inches < 0) {
        inches = -1 * inches;
        forward = -1;
    }

    int counts = ceil(inches * COUNTS_PER_INCH);
    resetCounts();
    
    if (inches > 3) {
        rampToPercent(forward * -1 * percent, forward * percent);
    } else {
        percent = 15; 
        setMotorPercent(forward * -1 * percent, forward * percent);
    }

    // constants for tuning the control loop
    constexpr float Kp_decel = 0.5;
    constexpr int MIN_POWER = 15;
    constexpr float Kp_straight = 1.0;
    
    float DECEL_THRESHOLD = 150.0;
    if (counts / 2.0 < 150.0) {
        DECEL_THRESHOLD = counts / 2.0;
    }

    float currentAvgCounts = 0.0;

    while(currentAvgCounts < counts) {
        currentAvgCounts = (leftEncoder.Counts() + rightEncoder.Counts()) / 2.0;
        
        float error = counts - currentAvgCounts;
        int currentPower = percent;

        if (error < DECEL_THRESHOLD) {
            currentPower = round(error * Kp_decel);
            
            if (currentPower > percent) {
                currentPower = percent;
            }

            if (currentPower < MIN_POWER) {
                currentPower = MIN_POWER;
            }
        }

        int encoderDiff = leftEncoder.Counts() - rightEncoder.Counts();
        int adjustment = round(encoderDiff * Kp_straight);

        setMotorPercent(forward * -(currentPower + adjustment), forward * (currentPower - adjustment));
    }

    stopMotors();
}

void Drivetrain::driveUntilWall(int percent) {
    resetCounts();
    
    // Safety check: Cap the power so you don't damage the robot or the course wall
    int safePower = percent;
    if (safePower > 20) {
        safePower = 20; 
    }

    // Start driving
    setMotorPercent(-safePower, safePower);

    float lastAvgCounts = 0.0;
    float lastCheckTime = TimeNow();
    
    constexpr float Kp_straight = 1.0;

    // Infinite loop that only breaks when the wall is hit
    while(true) {
        float currentAvgCounts = (leftEncoder.Counts() + rightEncoder.Counts()) / 2.0;

        // 1. Wall Detection: Check progress every 100ms
        if (TimeNow() - lastCheckTime >= 0.1) {
            // If the robot moved 1 count or less in 0.1 seconds, it is stuck
            if (currentAvgCounts - lastAvgCounts <= 1.0) {
                break; // Break the infinite loop!
            }
            // Update tracking variables for the next 0.1s window
            lastAvgCounts = currentAvgCounts;
            lastCheckTime = TimeNow();
        }

        // 2. Straight-line correction to ensure the robot stays perfectly straight
        int encoderDiff = leftEncoder.Counts() - rightEncoder.Counts();
        int adjustment = round(encoderDiff * Kp_straight);

        setMotorPercent(-(safePower + adjustment), safePower - adjustment);
    }

    stopMotors();
}

void Drivetrain::turn(float angle, Direction dir, int percent) {
    // distance in inches to make a 360 degree turn
    constexpr float TURN_DIST = PI * TRACK_WIDTH;
    // for some reason the robot needs this to turn accurately idk why lol prob something to do with the track width so maybe ill fix that
    // TODO: remove this when fixed
    constexpr float SCALAR = 1.0175;
    int counts = ceil(TURN_DIST * COUNTS_PER_INCH * (angle / 360.0) * SCALAR);
    resetCounts();

    if (angle > 15.0) {
        rampToPercent(dir * percent, dir * percent);
    } else {
        percent = 20; 
        setMotorPercent(dir * percent, dir * percent);
    }

    // constants for tuning the control loop
    constexpr float Kp_decel = 0.5; 
    constexpr int MIN_POWER = 15; 
    float DECEL_THRESHOLD = 150.0; 
    if (counts / 2.0 < 150.0) {
        DECEL_THRESHOLD = counts / 2.0;
    }

    float currentAvgCounts = 0.0;

    while(currentAvgCounts < counts) {
        currentAvgCounts = (leftEncoder.Counts() + rightEncoder.Counts()) / 2.0;
        
        float error = counts - currentAvgCounts;
        int currentPower = percent;

        if (error < DECEL_THRESHOLD) {
            currentPower = round(error * Kp_decel);
            
            if (currentPower > percent) {
                currentPower = percent;
            }

            if (currentPower < MIN_POWER) {
                currentPower = MIN_POWER;
            }
        }

        setMotorPercent(dir * currentPower, dir * currentPower);
    }

    stopMotors();
}

void Drivetrain::pivotTurn(float angle, Direction dir, int percent) {
    constexpr float TURN_DIST = 2.0 * PI * TRACK_WIDTH;
    constexpr float SCRUB_FACTOR = 0.95; 
    int counts = ceil(TURN_DIST * COUNTS_PER_INCH * (angle / 360.0) * SCRUB_FACTOR);

    resetCounts();

    constexpr float Kp_decel = 0.5;
    constexpr int MIN_POWER = 15;

    float dynamic_threshold = 150.0;
    if (counts / 2.0 < 150.0) {
        dynamic_threshold = counts / 2.0;
    }

    float currentCounts = 0.0;

    while(currentCounts < counts) {
        // Novel situation: The center of rotation has shifted to the stationary wheel, so we only track the encoder of the outer driving wheel.
        if (dir == LEFT) {
            currentCounts = rightEncoder.Counts();
        } else {
            currentCounts = leftEncoder.Counts();
        }

        float error = counts - currentCounts;
        int currentPower = percent;

        if (error < dynamic_threshold) {
            currentPower = round(error * Kp_decel);

            if (currentPower > percent) {
                currentPower = percent;
            }
            if (currentPower < MIN_POWER) {
                currentPower = MIN_POWER;
            }
        }

        if (dir == LEFT) {
            setMotorPercent(0, currentPower);
        } else {
            setMotorPercent(currentPower, 0);
        }
    }

    stopMotors();
}

void Drivetrain::stopMotors() {
    rightMotor.Stop();
    leftMotor.Stop();
    Sleep(0.25);
}

void Drivetrain::setMotorPercent(int right, int left) {
    rightMotor.SetPercent(right);
    leftMotor.SetPercent(left);
}
