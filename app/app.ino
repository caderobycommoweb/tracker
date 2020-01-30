#include <monitor.h>
#include <network.h>

int networkReceptorPin = 2;
int networkTransmitterPin = 3;

String networkAPN = "internet.br";

bool isReady = false;

Monitor monitor;
Network network;

void setup()
{
  // Aguarda a inicialização da placa para que não ocorra
  // problemas na transmissão dos dados pelas portas seriais
  monitor.wait(2);


  // Inicia o monitor serial
  monitor.initialize();
  monitor.log("Monitor iniciado");


  // Define algumas propriedades do módulo de rede móvel
  network.setPins(networkReceptorPin, networkTransmitterPin);
  network.setMonitor(monitor);
  

  // Inicia o módulo de rede móvel
  network.initialize();
  monitor.log("Rede iniciada");
    

  // Define algumas propriedades do módulo de rede móvel
  if(!network.isConnected()){
    bool apnIsSetted = network.setAPN(networkAPN);
    bool mobileDataIsActived = network.activeMobileData();

      // Valida as operações executadas
    if(!apnIsSetted){
      monitor.log("Não foi possível definir a APN da rede");
      return;
    }
    if(!mobileDataIsActived){
      monitor.log("Não foi possível ativar os dados móveis");
      return;
    }
  }

  monitor.log(network.request("GET", true, "google.com", "", "Hello World!"));

  isReady = true;
}

void loop()
{  
  if(!isReady)
    return;
  
  network.debug();
}
