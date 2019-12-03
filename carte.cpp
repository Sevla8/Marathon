/*  TP3
    Écrivez vos noms et codes permanents ici.
*/

#include "carte.h"

void Carte::ajouter_noeud(long id, const PointST& p){
    // À compléter.
}

void Carte::ajouter_route(const std::string& nomroute, const std::list<long>& noeuds){
    // À compléter.
}

void Carte::changerDisponibilites(const std::list<std::string>& nomsroute, bool estdisponible){
    // À compléter.
}

long Carte::trouverDestIdeale(long noeudorigine, double distancecible, double& distancetrouve, std::list<long>& trajet){
    // À compléter.
    distancetrouve = 0;
    trajet.push_front(noeudorigine);
    return noeudorigine;
}
