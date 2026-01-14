
#include "menus.h"


//Fonction d'affichage du menu principal
void main_menu(){

    printf(">>>>>>>>>>>>>>> MENU MOBILITE MAP <<<<<<<<<<<<<<<\n");
    printf("[        TAILLE DU RESEAU : %i STATIONS        ]\n", nb);
    printf("1   -   Afficher les informations d'une station\n");
    printf("2   -   Lister les voisines d'une station\n");
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
        return;   // On réaffiche le menu
    }
    getchar();

}

void menu1() {
    int sous_choix = -1; // Variable locale
    
    while (sous_choix != 0) {
        printf("\n>>>>>>>>>>>>>>>>> MENU STATIONS <<<<<<<<<<<<<<<<\n");
        printf("1   -   Afficher par ID\n");
        printf("2   -   Afficher par NOM\n");
        printf("0   -   RETOUR AU MENU PRINCIPAL\n\n");
        printf("Votre choix : ");

        if (scanf("%d", &sous_choix) != 1) {
            printf("\n[!] ERREUR : Saisie invalide. Veuillez entrer un chiffre.\n");
            
            // Nettoyage du tampon (important pour ne pas boucler à l'infini)
            int c;
            while ((c = getchar()) != '\n' && c != EOF); 
            
            sous_choix = -1; // Force le retour au début de la boucle du sous-menu
        }
        getchar(); // Consomme le \n

        if (sous_choix == 1) {
            printf("Entrez l'ID : ");
            if (scanf("%d", &id) == 1) {
                if (id >= 0 && id < nb) display_station();
                else printf("ID invalide.\n");
            }
            getchar(); // Consomme le \n
            printf("\n(Entrée pour continuer)"); getchar(); 
        } 
        else if (sous_choix == 2) {
            printf("Entrez le NOM : ");
            scanf(" %[^\n]", name);
            get_id_by_name();
            if (id != -1) display_station();
            printf("\n(Entrée pour continuer)"); getchar();
        }
        else if (sous_choix < 0 || sous_choix > 2) {
            printf("\n[!] ERREUR : %d n'est pas une option valide (0, 1 ou 2).\n", sous_choix);
            sous_choix = -1;
        }
    }
    // Quand on sort de ce while, on retourne au main, 
    // et choix[0] vaut toujours 1, donc le menu principal s'affichera.
}


int menu2(){
    return 0 ;
}

int menu3(){
    return 0 ;
}

int menu4(){
    return 0 ;
}
