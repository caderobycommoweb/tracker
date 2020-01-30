#ifndef Stringer_h
#define Stringer_h

#include <Arduino.h>

class Stringer
{
    public:
        Stringer() { }

        void fingerprint(String str)
        {
            for(int i = 0; i < str.length(); i++){
                char character = str[i];
                int decimal = character;
                Serial.println((String)i + "." + str[i] + "-" + (String)decimal);
            }
        }
        String remove(String str, String chars)
        {
            String cleanedStr;
            for(int i = 0; i < str.length(); i++){
                bool addChar = true;
                for(int n = 0; n < chars.length(); n++)
                    if(str[i] == chars[n]) addChar = false;
                if(addChar) cleanedStr += str[i];
            }
            return cleanedStr;
        }
        String split(String str, char separator, int index)
        {
            int found = 0;
            int strIndex[] = {0, -1};
            int maxIndex = str.length()-1;
            for(int i=0; i<=maxIndex && found<=index; i++){
                if(str.charAt(i)==separator || i==maxIndex){
                    found++;
                    strIndex[0] = strIndex[1]+1;
                    strIndex[1] = (i == maxIndex) ? i+1 : i;
                }
            }
            return found>index ? str.substring(strIndex[0], strIndex[1]) : "";
        }
};

#endif