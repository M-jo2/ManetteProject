#include <XInput.h>
#include <EEPROM.h>
#include "PacketButton.h"
#include <Arduino.h>

#define NB_BUTTONS 27

#define X 0
#define Y 1
#define B 2
#define A 3
#define AB 4
#define RBa 5
#define RBb 6
#define LBa 7
#define LBb 8
#define BLB 9
#define BRB 10
#define BJOYL 11
#define BJOYR 12
#define START 13
#define SELECT 14
#define XBOX 15
#define CONFIG_BUTTON 16
#define JOYLX 17
#define JOYLY 18
#define JOYRX 19
#define JOYRY 20
#define LT 21
#define RT 22
#define PADU 23
#define PADD 24
#define PADR 25
#define PADL 26

//Entrée Analogique A7
const int PatternPadDroit[4] =  {0,363,484,605};
int ValeurPadDroit=0;

//Entrée Analogique A2
const int PatternPadGauche[16] = {0,336,420,556,116,392,464,582,218,443,504,608,293,483,536,629};
int ValeurPadGauche=0;

//etat de tout les boutons
int btEtat[NB_BUTTONS];
int btLastEtat[NB_BUTTONS];

PacketButton PadDroit(20,10);
PacketButton PadGauche(0,4);

void setup() {
  for(int i = 0; i<NB_BUTTONS ; i++)
  {
    btEtat[i]=0;
    btLastEtat[i]=0;
  }
  
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(0, INPUT_PULLUP);
  pinMode(1, INPUT_PULLUP);
  pinMode(3, INPUT_PULLUP);
  pinMode(10, INPUT_PULLUP);
  pinMode(16, INPUT_PULLUP);
  pinMode(15, INPUT_PULLUP);
  pinMode(14, INPUT_PULLUP);
  pinMode(7, INPUT_PULLUP);
  pinMode(5, INPUT_PULLUP);
  pinMode(2, INPUT_PULLUP);
  //Serial.begin(57600);

  XInput.setTriggerRange(0, 255);
  XInput.setRange(JOY_LEFT, 0, 1023);
  XInput.setRange(JOY_RIGHT, 0, 1023);
  XInput.begin();
  
  //XInput.setAutoSend(false);
  
  PadDroit.fillVE(PatternPadDroit,16);
  PadGauche.fillVE(PatternPadGauche,16);
}


void loop() {
  ValeurPadDroit = analogRead(A7);
  ValeurPadGauche = analogRead(A2);
  
  PadDroit.positionVE(ValeurPadDroit);
  PadGauche.positionVE(ValeurPadGauche);
  
  btEtat[B] = PadDroit.Button1;
  btEtat[Y] = PadDroit.Button2;
  
  btEtat[LBb] = PadGauche.Button1;
  btEtat[LBa]   = PadGauche.Button2;
  btEtat[AB]    = PadGauche.Button3;
  btEtat[BJOYL]   = PadGauche.Button4;
  
  
    //Serial.println(analogRead(A7));
  
  if(!digitalRead(2)){
    btEtat[X] = 1;
  }
  if(!digitalRead(10)){
    btEtat[A] = 1;
  }
  
  if(!digitalRead(3)){
    btEtat[BJOYR] = 1;
  }
  
  if(!digitalRead(1)){
    btEtat[START] = 1;
  }
  
  if(!digitalRead(0)){
    btEtat[BRB] = 1;
  }
  
  if(!digitalRead(14)){
    btEtat[BLB] = 1;
  }
  
  if(!digitalRead(15)){
    btEtat[SELECT] = 1;
  }
  
  if(!digitalRead(16)){
    btEtat[XBOX] = 1;
  }
  
  if(!digitalRead(5)){
    btEtat[RBa] = 1;
  }
  
  if(!digitalRead(7)){
    btEtat[RBb] = 1;
  }

  
  /*les Bouton Additionnel Temporaire */
  if(btEtat[LBa] == 1){
    btEtat[BJOYL] = 1;
  }
  if(btEtat[RBa] == 1 ){
    //btEtat[BJOYR] = 1;
    //longCombo(btEtat[RBa] != btLastEtat[RBa]);
    if(Cycle(40)){
      btEtat[A] = 1;
    }
  }

  if(btEtat[BLB] == 1){
    btEtat[LBb] = 1;
  }
  if(btEtat[BRB] == 1){
    btEtat[RBb] = 1;
  }

  if(btEtat[AB] == 1){
    btEtat[CONFIG_BUTTON] = 1;
  }else if (btLastEtat[AB] == 1 && btEtat[XBOX] == 0){
    btEtat[CONFIG_BUTTON] = 0;
  }

  
  
  /************************************************************************/
  if (btEtat[A] != btLastEtat[A])     XInput.setButton(BUTTON_A, btEtat[A]);
  if (btEtat[B] != btLastEtat[B])     XInput.setButton(BUTTON_B, btEtat[B]);
  if (btEtat[X] != btLastEtat[X])     XInput.setButton(BUTTON_X, btEtat[X]);
  if (btEtat[Y] != btLastEtat[Y])     XInput.setButton(BUTTON_Y, btEtat[Y]);
  if (btEtat[LBb] != btLastEtat[LBb])   XInput.setButton(BUTTON_LB, btEtat[LBb]);
  if (btEtat[RBb] != btLastEtat[RBb])   XInput.setButton(BUTTON_RB, btEtat[RBb]);
  if (btEtat[START] != btLastEtat[START]) XInput.setButton(BUTTON_START, btEtat[START]);
  if (btEtat[SELECT] != btLastEtat[SELECT])XInput.setButton(BUTTON_BACK, btEtat[SELECT]);
  //if (btEtat[XBOX] != btLastEtat[XBOX])  XInput.setButton(BUTTON_LOGO, btEtat[XBOX]);
  if (btEtat[BJOYL] != btLastEtat[BJOYL]) XInput.setButton(BUTTON_L3, btEtat[BJOYL]);
  if (btEtat[BJOYR] != btLastEtat[BJOYR]) XInput.setButton(BUTTON_R3, btEtat[BJOYR]);
  
  int JoyL[4] ={90 ,840,135,840}; // XJoyLmin, XJoyLmax, YJoyLmin, YJoyLmax
  int JoyR[4] ={180,890,200,875}; // XJoyRmin, XJoyRmax, YJoyRmin, YJoyRmax

  btEtat[JOYLX] =  map( 1023-analogRead(A0)   , JoyL[0] , JoyL[1] , 0 , 1023 );
  btEtat[JOYLY] =  map(      analogRead(A1)   , JoyL[2] , JoyL[3] , 0 , 1023 );
  
  btEtat[JOYRX] =  map(        analogRead(A8) , JoyR[0] , JoyR[1] , 0 , 1023 );
  btEtat[JOYRY] =  map( 1023 - analogRead(A9) , JoyR[2] , JoyR[3] , 0 , 1023 );

  if (btEtat[JOYRX] != btLastEtat[JOYRX] || btEtat[JOYRY] != btLastEtat[JOYRY]) XInput.setJoystick(JOY_RIGHT, btEtat[JOYRX], btEtat[JOYRY]);
  
  if (btEtat[JOYLX] != btLastEtat[JOYLX] || btEtat[JOYLY] != btLastEtat[JOYLY]){
    if(btEtat[CONFIG_BUTTON] ==0){
      XInput.setJoystick(JOY_LEFT , btEtat[JOYLX], btEtat[JOYLY]);
    }else{
      XInput.setJoystick(JOY_LEFT , 511, 511);
      if(btEtat[CONFIG_BUTTON] == 1){
        btEtat[PADU] = btEtat[JOYLY]>824 ? 1 : 0;
        btEtat[PADD] = btEtat[JOYLY]<200 ? 1 : 0;
        btEtat[PADR] = btEtat[JOYLX]<200 ? 1 : 0;
        btEtat[PADL] = btEtat[JOYLX]>824 ? 1 : 0;
      }
      
    }
  }
  if(btEtat[PADU] != btLastEtat[PADU] ||
    btEtat[PADD] != btLastEtat[PADD] ||
    btEtat[PADR] != btLastEtat[PADR] ||
    btEtat[PADL] != btLastEtat[PADL]) XInput.setDpad(btEtat[PADU], btEtat[PADD], btEtat[PADR],btEtat[PADL]);

  
  btEtat[LT] = map((1023 - analogRead(A3)), 4,640,0,255);
  btEtat[RT] = map(1023 - analogRead(A6), 4,300,0,255);
  //Serial.println(1023 - analogRead(A3));
  //Serial.println(1023 - analogRead(A6));
  //Serial.println(" ");
  if (btEtat[RT] != btLastEtat[RT]) XInput.setTrigger(TRIGGER_RIGHT, btEtat[RT]);
  if (btEtat[LT] != btLastEtat[LT]) XInput.setTrigger(TRIGGER_LEFT, btEtat[LT]);

  for(int i = 0 ; i < NB_BUTTONS ;i++){
    //Serial.print(btEtat[i]);
    //Serial.print(" - ");
    if(btEtat[i] != btLastEtat[i]){
      btLastEtat[i] = btEtat[i] ;
    }
    if(i != CONFIG_BUTTON)
      btEtat[i] = 0;
  }
  //Serial.println(" ");
  //delay(5);
}






bool push = false;
bool pushTampon = false;

bool Cycle(int frequence){
  bool returnCycle = false;
  if(millis() % frequence > frequence/2)
    push = true;
  else 
    push = false;

  if(push != pushTampon)
  {
    returnCycle= true;
    pushTampon = push;
  }

  return returnCycle;
}


/*
int timeBegin = -1;
bool donne = false;
int lastetape = 0;
int etape = 0;
const int tempBase = 240;
//int tempCombo[16];
int tempCombo[16]={200,400,650,820,900,1100,1300,1500,1600,1800,1850,1900,1950,2250,2500,2700};
bool inito = false;
bool longCombo(bool reload){
  if(inito){
    for(int i = 0; i<10 ; i++){
      tempCombo[i] = tempBase*(i+1);
    }
    for(int i =9 ; i<12 ; i++){
      tempCombo[i] = (tempBase*10)+(tempBase/3*(i-8));
    }
    for(int i = 11; i<16 ; i++){
      tempCombo[i] = tempBase*(i+1);
    }
    inito = true;
  }
    
  if(reload){
    timeBegin = millis();
    donne = false;
    etape = 0;
    lastetape = -1;
    
  }
  
  int timeNow = millis()-timeBegin;
  //Serial.println(etape);
  //Serial.println(timeBegin);
    
    if(timeNow < tempCombo[0]) {etape = 1;}
    else if(timeNow < tempCombo[1]){etape = 2;}
    else if(timeNow < tempCombo[2]){etape = 3;}
    else if(timeNow < tempCombo[3]){etape = 4;}
    else if(timeNow < tempCombo[4]){etape = 5;}
    else if(timeNow < tempCombo[5]){etape = 6;}
    else if(timeNow < tempCombo[6]){etape = 7;}
    else if(timeNow < tempCombo[7]){etape = 8;}
    else if(timeNow < tempCombo[8]){etape = 9;}
    else if(timeNow < tempCombo[9]){etape = 10;}
    else if(timeNow < tempCombo[10]){etape = 11;}
    else if(timeNow < tempCombo[11]){etape = 12;}
    else if(timeNow < tempCombo[12]){etape = 13;}
    else if(timeNow < tempCombo[13]){etape = 14;}
    else if(timeNow < tempCombo[14]){etape = 15;}
    else if(timeNow < tempCombo[15]){etape = 16;}
  
  
  if(etape != lastetape){
    
    if(etape == 1 ) {btEtat[X] = 1; }
    else if(etape == 2 )  {btEtat[X] = 1; }
    else if(etape == 3 )  {btEtat[X] = 1; }
    else if(etape == 4 )  {btEtat[A] = 1; btEtat[PADD] = 1; }
    else if(etape == 5 )  {btEtat[X] = 1; btEtat[PADR] = 1; }
    else if(etape == 6 )  {btEtat[A] = 1; }
    else if(etape == 7 )  {btEtat[X] = 1; btEtat[PADD] = 1; }
    else if(etape == 8 )  {btEtat[A] = 1; btEtat[PADR] = 1; }
    else if(etape == 9 )  {btEtat[X] = 1; }
    else if(etape == 10 ) {btEtat[B] = 1; }
    else if(etape == 11 ) {btEtat[PADD] = 1; }
    else if(etape == 12 ) {btEtat[PADD] = 1; btEtat[PADL] = 1; }
    else if(etape == 13 ) {btEtat[B] = 1; btEtat[PADL] = 1; }
    else if(etape == 14 ) {btEtat[A] = 1; }
    else if(etape == 15 ) {btEtat[B] = 1; btEtat[PADR] = 1; }
    else if(etape == 16 ) {btEtat[X] = 1; btEtat[PADD] = 1; }
    
    if(timeNow > tempCombo[etape-2]+2)
      lastetape = etape ;
  }

  return timeNow == -1 ? 1 : 0;
}
*/
