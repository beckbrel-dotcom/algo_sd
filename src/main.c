#include "structures.h"
#include "donnees.h"



void menu(int* choix, int nb){
    while(choix[0] != 0){
        printf(">>>>>>>>>>>>>>> MENU MOBILITE MAP <<<<<<<<<<<<<<<\n");
        printf("[        TAILLE DU RESEAU : %i STATIONS        ]\n", nb);
        printf("1   -   Afficher les informations d'une station\n");
        printf("2   -   Lister les voisins d'une station\n");
        printf("3   -   Calculer un chemin minimal\n");
        printf("4   -   Afficher les stations triées par degré\n");
        printf("0   -   Quitter\n\n");
        printf("Votre choix : ");



        if (scanf("%i", choix)!=1){
            printf("\nERREUR LORS DE LA SAISIE DU CHOIX : VEUILLEZ ENTRER UN ENTIER ENTRE 0 ET 5\n") ;
            // Nettoyage du tampon (évite une boucle infinie si l'utilisateur tape des lettres)
            int c;
            while ((c = getchar()) != '\n' && c != EOF); 
            
            choix[0] = -1; // On réinitialise pour ne pas quitter la boucle
            continue;   // On réaffiche le menu
        }
        getchar();
        printf("Vous avez entré : %d\n", choix[0]) ;
    }
}

int main(int argc, char* argv[]){
    if(argc != 2){
        printf("Erreur\nFORMAT ATTENDU : ./metro [fichierReseau]\n") ;
        return -1;
    }
    char* reseau = argv[1] ;
    printf("FICHIER CONTENANT LE RESEAU : %s.\n",reseau) ;

    Graph *G = charger_donnees(reseau) ;
    if(G==NULL) {
        printf("ERREUR LORS DE LA CRÉATION DU GRAPHE\n"); 
        return -1;
    }
    int nb = G->nb_stations ;

    int* choix = malloc(sizeof(int)) ; //Varibable qui va stocker le choix de l’utilisateur
    choix[0] = -1 ;
    //Affichage du menu
    menu(choix, nb) ;



    free_graph(G) ;
    return 0;
}