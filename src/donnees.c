#include "structures.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// On passe le nom du fichier en argument pour respecter : ./metro metro.txt [cite: 55]
Graph* charger_donnees(const char *nom_fichier) {
    FILE *file = fopen(nom_fichier, "r");
    if (!file) {
        // Message d'erreur clair si le fichier est introuvable [cite: 62]
        fprintf(stderr, "Erreur : Impossible d'ouvrir le fichier %s\n", nom_fichier);
        return NULL;
    }

    char ligne[256];
    int nb_stations = 0;

    // Premier passage : déterminer le nombre de stations [cite: 56]
    while (fgets(ligne, sizeof(ligne), file)) {
        if (strncmp(ligne, "STATION", 7) == 0) nb_stations++;
    }

    if (nb_stations == 0) {
        fprintf(stderr, "Erreur : Format invalide, aucune station trouvée.\n");
        fclose(file);
        return NULL;
    }

    // Allocation du graphe et du tableau de stations [cite: 15, 57]
    Graph *g = malloc(sizeof(Graph));
    g->nb_stations = nb_stations;
    g->stations = malloc(sizeof(Station) * nb_stations);

    // Initialisation par défaut 
    for(int i = 0; i < nb_stations; i++) {
        g->stations[i].adj_list = NULL;
        g->stations[i].name = NULL;
    }

    rewind(file);

    // Deuxième passage : remplissage et création des arcs [cite: 59, 60]
    while (fgets(ligne, sizeof(ligne), file)) {
        // Ignorer proprement les lignes vides ou de commentaire [cite: 39, 40]
        if (ligne[0] == '#' || ligne[0] == '\n' || ligne[0] == '\r' || ligne[0] == ' ') continue;

        int id1, id2;
        char nom_temp[128];

        // Lecture des STATIONS
        if (sscanf(ligne, "STATION %d \"%[^\"]\"", &id1, nom_temp) == 2) {
            if (id1 >= 0 && id1 < nb_stations) {
                g->stations[id1].id = id1;
                g->stations[id1].name = strdup(nom_temp); // Initialise le nom 
            }
        }
        // Lecture des EDGES (arêtes)
        else if (sscanf(ligne, "EDGE %d %d", &id1, &id2) == 2) {
            // Vérification : stations inexistantes [cite: 42, 61]
            if (id1 < 0 || id1 >= nb_stations || id2 < 0 || id2 >= nb_stations) {
                fprintf(stderr, "Erreur : Arête entre stations inexistantes (%d -> %d)\n", id1, id2);
                continue;
            }

            // Vérification : doublons [cite: 43, 61]
            int existe = 0;
            Edge *curr = g->stations[id1].adj_list;
            while (curr) {
                if (curr->target_id == id2) { existe = 1; break; }
                curr = curr->next;
            }

            if (!existe) {
                Edge *nouveau = malloc(sizeof(Edge));
                nouveau->target_id = id2;
                nouveau->next = g->stations[id1].adj_list;
                g->stations[id1].adj_list = nouveau;
            }
        }
    }

    fclose(file);
    //printf(" ===== MENU RESEAU DE TRANSPORT ===== \n 1 - Afficher les informations d'une station \n 2 - Lister les voisins d'une station \n 3 - Calculer un chemin minimal \n 4 - Afficher les stations triées par degré \n 0 - Quitter \n Votre choix :" );
    return g;
}