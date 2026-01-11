#include "structures.h"



int main(int argc, char* argv[]){
    if(argc != 2){
        printf("Erreur\nFormat attendu : ./metro [reseau.txt]\n") ;
        return -1;
    }
    char* reseau = argv[1] ;
    printf("Fichier contenant le réseaux à charger : %s.\n",reseau) ;
    return 0;
}