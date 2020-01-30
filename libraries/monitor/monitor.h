#ifndef Monitor_h
#define Monitor_h

#include <Arduino.h>

class Monitor
{
    public:
        Monitor(){}

        void initialize()
        {
            Serial.begin(9600);
            this -> wait(1);
        }
        void log(String str)
        {
            Serial.println("<" + str + ">");
            Serial.flush();
        }
        void wait(int seconds)
        {
            delay(1000 * seconds);
        }
};

#endif