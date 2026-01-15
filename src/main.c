
#include "../include/donnees.h"
#include "../include/menus.h"


int* choix = NULL ;
int nb = 0;
int id = -1 ;
Graph* G = NULL ;
char *name = NULL ;



int main(int argc, char* argv[]){

    choix = malloc(sizeof(int)) ; //Varibable qui va stocker le choix de l’utilisateur
    name = malloc(40) ;

    if (choix == NULL || name == NULL) return 1;
    if(argc != 2){
        printf("Erreur\nFORMAT ATTENDU : ./metro [fichierReseau]\n") ;
        return -1;
    }
    char* reseau = argv[1] ;
    printf("FICHIER CONTENANT LE RESEAU : %s.\n",reseau) ;

    G = charger_donnees(reseau) ;
    if(G==NULL) {
        printf("ERREUR LORS DE LA CRÉATION DU GRAPHE\n"); 
        return -1;
    }
    nb = G->nb_stations ;

    choix[0] = -1 ;
    //Affichage du menu
    while(choix[0] != 0){
        main_menu() ;

        if(choix[0]==0){
            continue;
        }
        if(choix[0]==1){
            menu1();
            choix[0]=-1;
        }
        else if(choix[0]==2){
            menu2();
            choix[0]=-1;
            continue;
        }
        else if(choix[0]==3){
            menu3();
            choix[0]=-1;
            continue;
        }
        else if(choix[0]==4){
            menu4();
            choix[0]=-1;
            continue;
        }
        else{
            printf("ERREUR DE SAISIE : ENTREZ UN NOMBRE ENTRE 0 ET 4\n");
            continue;
        }
    }
    
    free(choix) ;
    free(name);
    free_graph(G) ;
    return 0;
}