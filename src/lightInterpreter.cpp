#include "lightInterpreter.h"

LightInterpreter::LightInterpreter(FEHIO::FEHIOPin pin) : sensor(pin) {
    startThreshold = 2.0;
    redThreshold = 2.0;
    blueThreshold = 0.8;
}

bool LightInterpreter::isStartLightOn() {
    LCD.WriteLine(sensor.Value());
    Sleep(0.1);
    return sensor.Value() > startThreshold;
}

int LightInterpreter::getHumidifierColor() {
    float voltage = sensor.Value();
    LCD.WriteLine(voltage);
    
    // Returns 1 for Red, 2 for Blue, 0 for undetected
    if (voltage >= redThreshold) {
        return 1; 
    } else if (voltage <= blueThreshold) {
        return 2; 
    }
    
    return 0; 
}