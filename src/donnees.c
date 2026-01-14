#include "donnees.h"


// Nettoie la ligne : ignore les espaces au début et détecte les commentaires
char* clean_line(char *ligne) {
    char *ptr = ligne;
    while (*ptr == ' ' || *ptr == '\t') ptr++;
    if (*ptr == '#' || *ptr == '\n' || *ptr == '\r' || *ptr == '\0') return NULL;
    return ptr;
}


// --- FONCTION PRINCIPALE ---

Graph* charger_donnees(const char *nom_fichier) {
    FILE *file = fopen(nom_fichier, "r");
    if (!file) {
        fprintf(stderr, "Erreur : Impossible d'ouvrir %s\n", nom_fichier);
        return NULL;
    }

    char ligne[256];
    int nb_theo = 0;

    // 1. Premier passage : Compter grossièrement pour allouer
    while (fgets(ligne, sizeof(ligne), file)) {
        if (strstr(ligne, "STATION;")) nb_theo++;
    }

    Graph *g = malloc(sizeof(Graph));
    g->nb_stations = nb_theo;
    g->stations = malloc(sizeof(Station) * nb_theo);

    // Initialisation
    for(int i = 0; i < nb_theo; i++) {
        g->stations[i].id = -1;
        g->stations[i].name = NULL;
        g->stations[i].degree = 0;
        g->stations[i].adj_list = NULL;
    }

    // 2. Deuxième passage : Charger les STATIONS (en filtrant les erreurs)
    rewind(file);
    int station_count = 0;
    while (fgets(ligne, sizeof(ligne), file)) {
        char *ptr = clean_line(ligne);
        if (!ptr || strncmp(ptr, "STATION", 7) != 0) continue;

        int id_f;
        char nom_temp[128];
        // On vérifie que sscanf lit bien un entier ET un nom
        if (sscanf(ptr, "STATION;%d;%[^\n\r]", &id_f, nom_temp) == 2) {
            if (trouver_index_par_id(g, id_f) == -1) { // Pas de doublon d'ID
                g->stations[station_count].id = id_f;
                g->stations[station_count].name = strdup(nom_temp);
                station_count++;
            }
        }
    }
    g->nb_stations = station_count; // Ajustement au nombre réel de stations valides

    // 3. Troisième passage : Charger les EDGES
    rewind(file);
    while (fgets(ligne, sizeof(ligne), file)) {
        char *ptr = clean_line(ligne);
        if (!ptr || strncmp(ptr, "EDGE", 4) != 0) continue;

        int id_f1, id_f2, temps;
        int res = sscanf(ptr, "EDGE;%d;%d;%d", &id_f1, &id_f2, &temps);
        
        if (res >= 2) {
            if (res == 2) temps = 1; // Temps par défaut si manquant (ex: EDGE;76;78;)

            int idx1 = trouver_index_par_id(g, id_f1);
            int idx2 = trouver_index_par_id(g, id_f2);

            // Vérification : Station référencée mais non définie
            if (idx1 == -1 || idx2 == -1) {
                fprintf(stderr, "[ERREUR] Arête ignorée : ID %d ou %d inconnu\n", id_f1, id_f2);
                continue;
            }

            // Vérification : Doublons d'arêtes identiques
            if (!edge_exists(g, idx1, idx2)) {
                // Aller
                Node *n1 = malloc(sizeof(Node));
                n1->dest = idx2;
                n1->weight = temps;
                n1->next = g->stations[idx1].adj_list;
                g->stations[idx1].adj_list = n1;
                g->stations[idx1].degree++;

                // Retour
                Node *n2 = malloc(sizeof(Node));
                n2->dest = idx1;
                n2->weight = temps;
                n2->next = g->stations[idx2].adj_list;
                g->stations[idx2].adj_list = n2;
                g->stations[idx2].degree++;
            }
        }
    }

    fclose(file);
    printf("Chargement réussi : %d stations chargées.\n", g->nb_stations);
    return g;
}