#ifndef LIGHT_INTERPRETER_H
#define LIGHT_INTERPRETER_H

#include <FEHIO.h>
#include <FEHLCD.h>
#include <FEHUtility.h>

class LightInterpreter {
private:
    AnalogInputPin sensor;
    
    float startThreshold;
    float colorThreshold;

public:
    LightInterpreter(FEHIO::FEHIOPin pin);

    bool isStartLightOn();
    int getHumidifierColor();
};

#endif