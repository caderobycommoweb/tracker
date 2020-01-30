#ifndef AT_h
#define AT_h

#include <Arduino.h>
#include <SoftwareSerial.h>
#include <stringer.h>

class AT
{
    public:
        AT() { }

        String send(SoftwareSerial *serial, String command, bool returnFullResponse = false)
        {
            serial -> println(command);
            serial -> flush();

            String response;

            while(true){
                if(serial -> available()){
                    char character = serial -> read();

                    response = response + character;

                    Serial.print(character);



                    //if(returnFullResponse) return data;
                    //if(data.indexOf(F("\r\n\r\n")) >= 1) return data.substring(data.indexOf(F("\r\r\n")) + 3, data.indexOf(F("\r\n\r\n")));
                    //else return data.substring(data.indexOf(F("\r\r\n")) + 3, data.length() - 2);
                }
            }
        }

    private:
        Stringer stringer;
};

#endif