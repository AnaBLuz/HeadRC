// INCLUSÃO DE BIBLIOTECAS
#include <MPU6050_tockn.h>
#include <Wire.h>
#include <IRremote.h>

// DEFINIÇÕES
//#define MPU6050_ADDR         0x68 // ENDEREÇO QUANDO O PINO AD0 ESTIVER LIGADO AO GND
//#define MPU6050_ADDR         0x69 // ENDEREÇO QUANDO O PINO AD0 ESTIVER LIGADO AO VCC


#define DEBUG
// INSTANCIANDO OBJETOS
MPU6050 mpu6050(Wire);

// DECLARAÇÃO DE VARIÁVEIS  
float anguloX;
float anguloY;

IRsend irsend;
unsigned long valor;
decode_results res;

// ACIONADOR EXTERNO
int piezoPin = a; // sensor piezo no pino a
int threshold = b; // valor mínimo a ser lido pelo sensor para ativação
bool ligado = False; // armazena o estado do dispositivo


void setup() {
  Serial.begin(9600);
  
  Wire.begin();
  mpu6050.begin();
  mpu6050.calcGyroOffsets(false);// MUDAR PARA "true" SE QUISER VISUALIZAR INFORMAÇÕES DE CALIBRAÇÃO NO MONITOR SERIAL
  
  #ifdef DEBUG
    Serial.println("Fim Setup");
  #endif

    
}

void loop() {
   if ((analogRead(piezoPin)) >= threshold && (ligado == False)){ //se o valor do sensor atingir o threshold e estiver desligado, o dispositivo é acionado
    ligado = True; // muda o estado do dispositivo
    mpu6050.update();
    
    // GUARDA NA MEMÓRIA OS VALORES ENVIADOS PELO GIROSCOPIO
    anguloX = mpu6050.getAngleX();
    anguloY = mpu6050.getAngleY();
    
    
    // VERIFICA SE GIROU NO EIXO X
    if (anguloX >= 25){
                             valor=0xE0E09E61;// SMART HUB
                             irsend.sendNEC(0xE0E09E61, 32);
                             Serial.println(valor,HEX);
                             Serial.println("Para baixo - SMART HUB");
      
                      }
    
    else if (anguloX <= -25){
                              valor=0xE0E040BF;// POWER
                              irsend.sendNEC(E0E040BF, 32);
                              Serial.println(valor,HEX);
                              Serial.println("Para Cima - liga a TV");
      
                             }
    
    // VERIFICA SE GIROU NO EIXO Y
    if (anguloY >= 25){
                               valor=0xE0E0D02F;//  VOL- 
                               irsend.sendNEC(0xE0E0D02F, 32);
                               Serial.println(valor,HEX);
                               Serial.println("ESQUERDA - DIMINUI O VOLUME");
                      } 
                      
    else if (anguloY <= -25){
                              valor=0xE0E0E01F;// VOL+
                              irsend.sendNEC(0xE0E0E01F, 32);
                              Serial.println(valor,HEX);
                              Serial.println("DIREITA - AUMENTA VOLUME");
      
                            } 
  }
  if ((analogRead(piezoPin) >= threshold) && (ligado == True)){ //~se o valor do sensor atingir o threshold e estiver ligado, o dispositivo é acionado
    ligado = False; // muda o estado do dispositivo
  }
 
}
