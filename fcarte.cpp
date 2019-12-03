/*  INF3105 - Structures de données et algorithmes
    UQAM | Faculté des sciences | Département d'informatique
    Lab12 + TP3
    
    Fichier fcarte.cpp © Éric Beaudry (beaudry.eric@uqam.ca) 2019.
    http://ericbeaudry.ca/INF3105/lab12/ 

    Ce fichier source contient uniquement l'opérateur >> pour lire un objet Carte.
    Cette séparation permet de mieux isoler le code que vous écrirez dans carte.cpp.
*/
#include <cassert>
#include <cstdlib>
#include "carte.h"

/* Lire une carte. 
   Normalement, vous n'aurez pas à modifier ceci.
*/
std::istream& operator >> (std::istream& is, Carte& carte)
{
    int countnoeuds=0, countroutes=0, countrefs=0;
    // Lire les lieux
    while(is){
        std::string nomlieu;
        long osmid;
        is >> nomlieu;
        if(nomlieu == "---")
            break;
        assert(nomlieu[0]=='n');
        osmid = atol(nomlieu.c_str()+1);
        PointST point;
        char pv = 0;
        is >> point >> pv;
        assert(pv==';');
        carte.ajouter_noeud(osmid, point);
        countnoeuds++;
    }

    // Lire les routes
    while(is){
        std::string nomroute;
        is >> nomroute;
        if(nomroute.empty() || nomroute == "---")
            break;
        char deuxpoints=0;
        is >> deuxpoints;
        assert(deuxpoints == ':');
        
        std::list<long> listenomslieux;
        std::string nomlieu;
        while(is){
            is>>nomlieu;
            if(nomlieu==";")
                break;
            assert(nomlieu!=":"); // pour robustesse
            assert(nomlieu.find(";")==std::string::npos); // pour robustesse
            long osmid;
            assert(nomlieu[0]=='n');
            osmid = atol(nomlieu.c_str()+1);
            listenomslieux.push_back(osmid);
            countrefs++;
        }
        assert(nomlieu==";");
        carte.ajouter_route(nomroute, listenomslieux);
        countroutes++;
    }
    //std::cerr << "Carte chargée: " << countnoeuds << " noeuds; " << countroutes << " routes; " << countrefs << " références." << std::endl;

    return is;
}

