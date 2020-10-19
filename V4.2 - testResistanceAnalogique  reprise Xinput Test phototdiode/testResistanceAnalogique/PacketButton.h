#ifndef PACKETBUTTON_H
#define PACKETBUTTON_H

#include <Array.h>
#include <math.h>

#define TAILLEMAXVE 32

class PacketButton{
  private:
  int lastValue;//sauvegarde de valeur
  int smallestDiff;//plus petite différence entre toute les valeurs
  int tolerence; // Pourcentage d erreur toléré pour éviter les confusion
  
  
  public:
  Array<int,TAILLEMAXVE> VE;  // Stockage de tout les pallier de tension
  Array<int,10> ValeurTemps; 
  
  int Button1;
  int Button2;
  int Button3;
  int Button4;


  private:
  int vraiValeur(int valeur);
  void assign(int valeur);
  
  public:
  PacketButton(int diff,int toll);
  void fillVE(int tab[], int taille);
  void positionVE(int valeur);
  
  
};

#endif
