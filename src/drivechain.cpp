/*
#include <FEHUtility.h>
#include <FEHLCD.h>
#include <FEHIO.h>
#include <FEHSD.h>
#include <Arduino.h>
#include <FEHServo.h>
#include <FEHMotor.h>

AnalogInputPin right_opto(FEHIO::Pin2);
AnalogInputPin middle_opto(FEHIO::Pin3);
AnalogInputPin left_opto(FEHIO::Pin4);

FEHMotor right_motor(FEHMotor::Motor0, 9.0);
FEHMotor left_motor(FEHMotor::Motor1, 9.0);

enum LineStates {
    MIDDLE,
    RIGHT,
    LEFT
};

void MoveAlongBlackLine() { 
    int state = MIDDLE;
    float THRESHOLD = 2.5;
    
    while (true) {
        switch(state) { 
            case MIDDLE:  
                left_motor.SetPercent(25);
                right_motor.SetPercent(25);
                
                if (right_opto.Value() > THRESHOLD) {  
                    state = RIGHT; 
                } 
                else if (left_opto.Value() > THRESHOLD) {
                    state = LEFT;
                }
                break; 
 
            case RIGHT:  
                left_motor.SetPercent(25);
                right_motor.SetPercent(5);
                
                if(right_opto.Value() < THRESHOLD && middle_opto.Value() > THRESHOLD) { 
                    state = MIDDLE; 
                } 
                break; 
 
            case LEFT:  
                left_motor.SetPercent(5);
                right_motor.SetPercent(25);
                
                if (left_opto.Value() < THRESHOLD && middle_opto.Value() > THRESHOLD) {
                    state = MIDDLE;
                }
                break; 
 
            default:
                state = MIDDLE;
                break; 
        } 
        Sleep(20);
    } 
}
*/