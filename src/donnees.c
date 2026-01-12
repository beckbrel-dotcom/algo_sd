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
        // Ignorer commentaires et lignes vides
        if (ligne[0] == '#' || ligne[0] == '\n' || ligne[0] == '\r' || ligne[0] == ' ') continue;

        int id1, id2;
        char nom_temp[128];

        // Lecture des STATIONS
        if (sscanf(ligne, "STATION %d \"%[^\"]\"", &id1, nom_temp) == 2) {
            if (id1 >= 0 && id1 < nb_stations) {
                g->stations[id1].id = id1;
                g->stations[id1].name = strdup(nom_temp);
            }
        }
        // Lecture des EDGES (arêtes)
        else if (sscanf(ligne, "EDGE %d %d", &id1, &id2) == 2) {
            // Sécurité : ID valides ?
            if (id1 < 0 || id1 >= nb_stations || id2 < 0 || id2 >= nb_stations) {
                fprintf(stderr, "Erreur : Arête invalide (%d -> %d)\n", id1, id2);
                continue;
            }

            // Vérification des doublons (parcours de la liste Node)
            int existe = 0;
            Node *curr = g->stations[id1].adj_list;
            while (curr) {
                if (curr->dest == id2) { 
                    existe = 1; 
                    break; 
                }
                curr = curr->next;
            }

            if (!existe) {
                // Création d'un Node (on utilise tes structures)
                Node *nouveau = malloc(sizeof(Node));
                nouveau->dest = id2;
                nouveau->weight = 1; // Poids par défaut (1 min entre stations)
                nouveau->next = g->stations[id1].adj_list;
                
                // Mise à jour de la station
                g->stations[id1].adj_list = nouveau;
                g->stations[id1].degree++; // Indispensable pour ton tri par degré !
            }
        }
    }

    fclose(file);
    return g;
}