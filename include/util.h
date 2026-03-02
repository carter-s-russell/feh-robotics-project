#ifndef UTIL_H
#define UTIL_H

#include <FEHLCD.h>

class Util {
public:
    /**
     * @brief pauses the program until ERC screen is touched
     * 
     * @param print should the ERC print to screen while waiting 
     */
    static void waitForTouch(bool print) {
        if (print) LCD.Write("waiting for touch..");

        int x, y;
        while(!LCD.Touch(&x, &y));
        while(LCD.Touch(&x, &y));

        if (print) LCD.Clear();
    }

    /**
     * @brief waitForTouch method without parameter, assumes printing is desired
     * 
     */
    static void waitForTouch() {
        waitForTouch(true);
    }
};

#endif