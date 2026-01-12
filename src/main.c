#include "structures.h"
#include "donnees.h"


int main(int argc, char* argv[]){
    if(argc != 2){
        printf("Erreur\nFormat attendu : ./metro [reseau.txt]\n") ;
        return -1;
    }
    char* reseau = argv[1] ;
    printf("Fichier contenant le réseaux à charger : %s.\n",reseau) ;

    Graph *G = charger_donnees(reseau) ;
    printf("Chargement du réseau réussi ! nombre de stations : %i.\n", G->nb_stations) ;



    free_graph(G) ;
    return 0;
}