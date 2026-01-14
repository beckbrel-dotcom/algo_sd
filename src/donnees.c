#include "structures.h"

Graph* charger_donnees(const char *nom_fichier) {
    FILE *file = fopen(nom_fichier, "r");
    if (!file) {
        fprintf(stderr, "Erreur : Impossible d'ouvrir le fichier %s\n", nom_fichier);
        return NULL;
    }

    char ligne[256];
    int nb_stations = 0;

    // 1. Premier passage : Compter les stations
    while (fgets(ligne, sizeof(ligne), file)) {
        if (strncmp(ligne, "STATION", 7) == 0) nb_stations++;
    }

    if (nb_stations == 0) {
        fprintf(stderr, "Erreur : Format invalide, aucune station trouvée.\n");
        fclose(file);
        return NULL;
    }

    // 2. Allocation du Graphe
    Graph *g = malloc(sizeof(Graph));
    g->nb_stations = nb_stations;
    g->stations = malloc(sizeof(Station) * nb_stations);

    // Initialisation par défaut pour éviter les pointeurs fous
    for(int i = 0; i < nb_stations; i++) {
        g->stations[i].id = -1;
        g->stations[i].name = NULL;
        g->stations[i].degree = 0;
        g->stations[i].adj_list = NULL;
    }

    rewind(file);

    // 3. Deuxième passage : Remplissage
    while (fgets(ligne, sizeof(ligne), file)) {
        // Nettoyer les retours à la ligne parasites en fin de chaîne
        ligne[strcspn(ligne, "\r\n")] = 0;

        if (ligne[0] == '#' || ligne[0] == '\0') continue;

        int id1, id2, temps;
        char nom_temp[128];

        // Format : STATION;id;nom
        if (sscanf(ligne, "STATION;%d;%[^\n]", &id1, nom_temp) == 2) {
            if (id1 >= 0 && id1 < nb_stations) {
                g->stations[id1].id = id1;
                g->stations[id1].name = strdup(nom_temp);
                // printf("Chargé : %s\n", g->stations[id1].name); // Pour vérifier
            }
        }
        // Format : EDGE;id_dep;id_arr;temps
        else if (sscanf(ligne, "EDGE;%d;%d;%d", &id1, &id2, &temps) == 3) {
            if (id1 >= 0 && id1 < nb_stations && id2 >= 0 && id2 < nb_stations) {
                Node *nouveau = malloc(sizeof(Node));
                nouveau->dest = id2;
                nouveau->weight = temps; // On utilise le vrai temps du fichier !
                nouveau->next = g->stations[id1].adj_list;
                g->stations[id1].adj_list = nouveau;
                g->stations[id1].degree++;
            }
        }
    }
    fclose(file);
    return g;
}