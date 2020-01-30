#ifndef Network_h
#define Network_h

#include <Arduino.h>
#include <SoftwareSerial.h>
#include <monitor.h>
#include <at.h>
#include <stringer.h>

class Network
{
    public:
        Network(){}

        void initialize()
        {
            this -> serial = new SoftwareSerial(this -> rx, this -> tx);
            this -> serial -> begin(9600);
            this -> monitor.wait(1);
        }
        void setPins(int rx, int tx)
        { 
            this -> rx = rx;
            this -> tx = tx;
        }
        void setMonitor(Monitor m)
        { 
            this -> monitor = m;
        }
        bool setAPN(String apn)
        {
            if(this -> at.send(this -> serial, "AT+SAPBR=3,1,\"APN\",\"" + apn + "\"") == F("OK")) return true;
            return false;
        }
        bool isConnected()
        {
            String atResponse = this -> at.send(this -> serial, F("AT+SAPBR=2,1"));
            if(atResponse == F("ERROR")) return false;
            if(this -> stringer.split(atResponse, ',', 2) == F("\"0.0.0.0\"")) return false;
            return true;
        }
        bool activeMobileData()
        {
            if(this -> at.send(this -> serial, F("AT+SAPBR=1,1")) == F("OK")) return true;
            return false;
        }
        String request(String method, bool useSSL, String host, String endpoint, String data)
        {
            this -> at.send(this -> serial, F("AT+HTTPTERM"));
            if(this -> at.send(this -> serial, F("AT+HTTPINIT")) == F("ERROR")){
                this -> at.send(this -> serial, F("AT+HTTPTERM"));
                return F("A");
            }

            if(useSSL){
                if(this -> at.send(this -> serial, F("AT+HTTPSSL=1")) == F("ERROR")){
                    this -> at.send(this -> serial, F("AT+HTTPTERM"));
                    return F("0x01");
                }
            }
            else{
                if(this -> at.send(this -> serial, F("AT+HTTPSSL=0")) == F("ERROR")){
                    this -> at.send(this -> serial, F("AT+HTTPTERM"));
                    return F("0x02");
                }
            }

            if(this -> at.send(this -> serial, F("AT+HTTPPARA=\"CID\",1")) == F("ERROR")){
                this -> at.send(this -> serial, F("AT+HTTPTERM"));
                return F("0x03");
            }

            if(this -> at.send(this -> serial, "AT+HTTPPARA=\"URL\",\"" + (String)(useSSL ? "https" : "http") + "://" + host + "/" + endpoint + "\"") == F("ERROR")){
                this -> at.send(this -> serial, F("AT+HTTPTERM"));
                return F("0x04");
            }

            method.toUpperCase();
            if(method == F("POST"))
                if(this -> at.send(this -> serial, "AT+HTTPACTION=1") == F("ERROR")){
                    this -> at.send(this -> serial, F("AT+HTTPTERM"));
                    return F("0x05");
                }

            if(method != F("POST"))
                if(this -> at.send(this -> serial, "AT+HTTPACTION=0") == F("ERROR")){
                    this -> at.send(this -> serial, F("AT+HTTPTERM"));
                    return F("0x06");
                }
            method.toLowerCase();

            String response = this -> at.send(this -> serial, "AT+HTTPREAD", true);

            if(response == F("ERROR")){
                this -> at.send(this -> serial, F("AT+HTTPTERM"));
                return F("0x07");
            }

            this -> at.send(this -> serial, F("AT+HTTPTERM"));

            return response;
        }
        void debug()
        {
            if(Serial.available()) this -> serial -> write(Serial.read());
            if(this -> serial -> available()) Serial.write(this -> serial -> read());
        }




        /*void setSerialBaud(int serialBaud) { serialBaud_ = serialBaud; }
        
        
String getProviderName()
        {
            String atResponse = at.sendCommand(this -> softwareSerial);


            bool error = false;
            if(!at.sendCommand(this -> softwareSerial, "AT+COPS?")) error = true;
            if(!error && at.getStatus() == "ERROR") error = true;
            if(error){
                //monitor.error("Não foi possível obter de \"" + name_ + "\" o nome do provedor da rede móvel!");
                return "";
            }
            String data = at.getData();
            String providerName = this -> stringer.split(data, ',', 2);
            return providerName;
        }
        bool isReady(){
            bool error = false;
            if(!at.sendCommand(this -> softwareSerial, "AT")) error = true;
            if(!error && at.getStatus() == "ERROR") error = true;
            if(error){
                //monitor.error("Não foi possível verificar se \"" + name_ + "\" finalizou a inicialização interna!");
                return false;
            }
            return true;
        }
        int getPowerSypplyPercent()
        {
            bool error = false;
            if(!at.sendCommand(this -> softwareSerial, "AT+CBC")) error = true;
            if(!error && at.getStatus() == "ERROR") error = true;
            if(error){
                //monitor.error("Não foi possível obter de \"" + name_ + "\" a porcentagem da bateria!");
                return 0;
            }
            String data = at.getData();
            int batteryPercent = this -> stringer.split(data, ',', 1).toInt();
            return batteryPercent;
        }
        float getPowerSypplyTension()
        {
            bool error = false;
            if(!at.sendCommand(this -> softwareSerial, "AT+CBC")) error = true;
            if(!error && at.getStatus() == "ERROR") error = true;
            if(error){
                //monitor.error("Não foi possível obter de \"" + name_ + "\" a tensão da bateria!");
                return 0;
            }
            String data = at.getData();
            float batteryTension = this -> stringer.split(data, ',', 2).toFloat() / 1000;
            return batteryTension;
        }
        String getBoardIMEI(){
            bool error = false;
            if(!at.sendCommand(this -> softwareSerial, "AT+CGSN")) error = true;
            if(!error && at.getStatus() == "ERROR") error = true;
            if(error){
                //monitor.error("Não foi possível obter de \"" + name_ + "\" o IMEI da placa!");
                return "";
            }
            String data = at.getData();
            return data;
        }
        
        bool isRegistered(){
            bool error = false;
            if(!at.sendCommand(this -> softwareSerial, "AT+CGREG?")) error = true;
            if(!error && at.getStatus() == "ERROR") error = true;
            if(error){
                //monitor.error("Não foi possível verificar se \"" + name_ + "\" está registrada no provedor da rede móvel!");
                return false;
            }
            String data = at.getData();
            int registrationMode = data.substring(data.indexOf(",") + 1, data.length()).toInt();
            if(registrationMode != 1 && registrationMode != 5) return false;
            else return true;
        }
        int getSignalQuality(){
            bool error = false;
            if(!at.sendCommand(this -> softwareSerial, "AT+CSQ")) error = true;
            if(!error && at.getStatus() == "ERROR") error = true;
            if(error){
                //monitor.error("Não foi possível obter a qualidade do sinal de \"" + name_ + "\"!");
                return 0;
            }
            String data = at.getData();
            float signalQuality = data.substring(6, data.indexOf(",")).toFloat();
            if(signalQuality == 99) signalQuality = 0;
            signalQuality = (int)((signalQuality * 100) / 31);
            return signalQuality;
        }
        bool setFunctionalityMode(int functionalityMode)
        {
            if(functionalityMode < 0 || functionalityMode > 4) {
                //monitor.error("O modo de funcionamento \"" + (String)functionalityMode + "\" é inválido!");
                return false;
            }
            bool error = false;
            if(!at.sendCommand(this -> softwareSerial, "AT+CFUN=" + (String)functionalityMode)) error = true;
            if(!error && at.getStatus() == "ERROR") error = true;
            if(error){
                //monitor.error("Não foi possível definir o modo de funcionamento de \"" + name_ + "\"");
                return false;
            }
            return true;
        }
        bool isSIMInserted()
        {
            bool error = false;
            if(!at.sendCommand(this -> softwareSerial, "AT+CPIN?")) error = true;
            if(!error && at.getStatus() == "ERROR") error = true;
            if(error){
                //monitor.error("Não foi possível verificar se o cartão SIM está inserido no \"" + name_ + "\"!");
                return false;
            }

            String data = at.getData();
            if(data.indexOf("READY") >= 1) return true;
            else {
                //monitor.error("Não foi detectado nenhum cartão SIM, por favor verifique se está inserido corretamente!");
                return false;
            }
        }

        
        */

    private:
        int rx;
        int tx;

        Monitor monitor;
        AT at;
        SoftwareSerial *serial;
        Stringer stringer;
};

#endif