
#include "menus.h"


//Fonction d'affichage du menu principal
void main_menu(){

    printf(">>>>>>>>>>>>>>> MENU MOBILITE MAP <<<<<<<<<<<<<<<\n");
    printf("[        TAILLE DU RESEAU : %i STATIONS        ]\n", nb);
    printf("1   -   Afficher les informations d'une station\n");
    printf("2   -   Lister les voisines d'une station\n");
    printf("3   -   Calculer un chemin minimal\n");
    printf("4   -   Afficher les stations triées par degré\n");
    printf("0   -   QUITTER\n\n");
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
                else printf("ERREUR : ID doit être compris entre 0 et %d.\n", nb-1);
            }
            getchar(); // Consomme le \n
            printf("\n(Entrée pour continuer)"); getchar(); //Laisse le temps de lire les infos
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


void menu2(){
    int sous_choix = -1; // Variable locale
    
    while (sous_choix != 0) {
        printf("\n>>>>>>>>>>>>>>>>> INFORMATIONS VOISINS <<<<<<<<<<<<<<<<\n");
        printf("1   -   Selectionnez station par ID\n");
        printf("2   -   Selectionnez station par NOM\n");
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
                if (id >= 0 && id < nb) display_neighbors() ;
                else printf("ERREUR : ID doit être compris entre 0 et %d.\n", nb-1);
            }
            getchar(); // Consomme le \n
        } 
        else if (sous_choix == 2) {
            printf("Entrez le NOM : ");
            scanf(" %[^\n]", name);
            get_id_by_name();
            if (id != -1) display_neighbors() ;
        }
        else if (sous_choix < 0 || sous_choix > 2) {
            printf("\n[!] ERREUR : %d n'est pas une option valide (0, 1 ou 2).\n", sous_choix);
            sous_choix = -1;
        }
    }
    // Quand on sort de ce while, on retourne au main, 
    // et choix[0] vaut toujours 1, donc le menu principal s'affichera.
}

void menu3() {
    int id_dep, id_arr;
    printf("ID Station de départ : ");
    scanf("%d", &id_dep);
    printf("ID Station d'arrivée : ");
    scanf("%d", &id_arr);

    int idx_dep = trouver_index_par_id(G, id_dep);
    int idx_arr = trouver_index_par_id(G, id_arr);

    if (idx_dep != -1 && idx_arr != -1) {
        dijkstra(idx_dep, idx_arr);
    } else {
        printf("Erreur : Une des stations n'existe pas.\n");
    }
}

void menu4() {
    int sous_choix = -1;
    StatsTri stats = {0, 0};

    // 1. Préparation du tableau de Tuples (ID, Degré)
    Tuple *tab = malloc(nb * sizeof(Tuple));
    if (tab == NULL) return;

    for (int i = 0; i < nb; i++) {
        tab[i].id = G->stations[i].id;
        tab[i].degree = G->stations[i].degree;
    }

    while (sous_choix != 0) {
        printf("\n>>>>>>>>>>>>>> MENU TRI (DEGRÉS) <<<<<<<<<<<<<<<<\n");
        printf("1 - Tri par SÉLECTION\n");
        printf("2 - Tri par INSERTION\n");
        printf("3 - Tri RAPIDE (Itératif)\n");
        printf("3 - Tri RAPIDE (Récursif)\n");
        printf("0 - RETOUR AU MENU PRINCIPAL\n");
        printf("--------------------------------------------------\n");
        printf("Votre choix : ");

        if (scanf("%d", &sous_choix) != 1) {
            while (getchar() != '\n'); // Nettoyage buffer
            continue;
        }

        if (sous_choix == 0) break;

        // On réinitialise le tableau à chaque test pour comparer équitablement
        for (int i = 0; i < nb; i++) {
            tab[i].id = G->stations[i].id;
            tab[i].degree = G->stations[i].degree;
        }

        // 2. Exécution du tri choisi
        switch (sous_choix) {
            case 1:
                stats = tri_selection(tab, nb);
                printf("\n[Exécution : Tri par Sélection]\n");
                break;
            case 2:
                stats = tri_insertion(tab, nb);
                printf("\n[Exécution : Tri par Insertion]\n");
                break;
            case 3:
                stats = tri_rapide_iter(tab, nb);
                printf("\n[Exécution : Tri Rapide Itératif]\n");
                break;
            case 4:
                stats = tri_rapide_rec(tab, nb);
                printf("\n[Exécution : Tri Rapide Itératif]\n");
                break;
            default:
                printf("Choix invalide.\n");
                continue;
        }

        // 3. Affichage des Statistiques
        printf(">> STATISTIQUES :\n");
        printf("   - Comparaisons : %ld\n", stats.nb_comparaisons);
        printf("   - Permutations : %ld\n\n", stats.nb_permutations);

        // 4. Affichage des stations
        printf(">> CLASSEMENT DES PLUS GRANDES STATIONS :\n");
        printf("     %-6s %-30s %s\n", "[ ID ]", "[ NOM ]", "[ DEGRÉ ]");
        
        for (int i = 0; i < nb; i++) {
            int s_id = tab[i].id;
            printf("%2d - %-6d %-30s %d\n", 
                    i + 1, s_id, G->stations[s_id].name, tab[i].degree);
        }

        printf("\nAppuyez sur Entrée pour continuer...");
        getchar(); getchar(); // Pause
    }

    free(tab);
}
