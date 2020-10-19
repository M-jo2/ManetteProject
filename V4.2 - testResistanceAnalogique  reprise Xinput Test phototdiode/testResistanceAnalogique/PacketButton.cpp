
#include "PacketButton.h"

PacketButton::PacketButton(int diff, int tol){
  smallestDiff = diff;
  tolerence =  tol;
}

void PacketButton::fillVE(int tab[], int taille){
  for(int i = 0 ; i < taille ; i++) VE.push_back(tab[i]);
}

void PacketButton::positionVE(int valeur){
  if(abs(valeur - lastValue) > smallestDiff || true){
    //Retourne la position de la valeur qui se rapproche le plus de "valeur"
    int tampon;
    int tamponlast = abs(VE[0]-valeur);
    int retour = 0;
    
    for(int i = 0 ; i < VE.size() ; i ++){
      tampon= abs(VE[i]-valeur);
      if(tamponlast >= tampon){
        retour = i;
        tamponlast = tampon;
      }
    }
    if (valeur < 100){
      assign (0);
    }else  if(valeur < VE[retour]+(tolerence/2) && valeur > VE[retour]-(tolerence/2)){
      assign(retour);
      
    }
  }
  lastValue = valeur;
}

void PacketButton::assign(int valeur){

  Button1 = bitRead(valeur,0);
  Button2 = bitRead(valeur,1);
  Button3 = bitRead(valeur,2);
  Button4 = bitRead(valeur,3);
}
