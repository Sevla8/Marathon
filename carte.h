/*  TP3
    Écrivez vos noms et codes permanents ici.
*/

#if !defined(_CARTE__H_)
#define _CARTE__H_
#include <istream>
#include <list>
#include <set>
#include <string>
#include "pointst.h"

class Carte{
  public:
    void ajouter_noeud(long id, const PointST& p);
    void ajouter_route(const std::string& nom, const std::list<long>& noeuds);
    
    void changerDisponibilites(const std::list<std::string>& nomsroute, bool estdisponible);
    long trouverDestIdeale(long noeudorigine, double distancecible, double& distancetrouve, std::list<long>& trajet);

    // À compléter (vous pouvez tout changer).

  private:
    // À compléter.

};

// Déclaration opérateur (fonction) global
std::istream& operator >> (std::istream& is, Carte& carte);

#endif
