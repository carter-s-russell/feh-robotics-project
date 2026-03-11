#ifndef LIGHT_INTERPRETER_H
#define LIGHT_INTERPRETER_H

#include <FEHIO.h>

class LightInterpreter {
private:
    AnalogInputPin sensor;
    
    float startThreshold;
    float redThreshold;
    float blueThreshold;

public:
    LightInterpreter(FEHIO::FEHIOPin pin);

    bool isStartLightOn();
    int getHumidifierColor();
};

#endif