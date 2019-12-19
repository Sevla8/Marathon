#ifndef _CARTE__H_
#define _CARTE__H_

#define CST 7000
// Dans dijkstra_calcul on cherche à réduire l'ensemble de recherche.
// On garde les noeuds qui sont à une certaine dstance de départ.
// Seulement on cherche une distance supérieure à distancecible.
// On pourrait, je pense, pour chaque requête calculer la valeur de CST
// Mais je ne pense pas que ce serait efficace.
// La valeur distancecible + 7000 semble convenir à tout les tests.

#include <unordered_map>
#include <istream>
#include <limits>
#include <string>
#include <list>
#include <set>
#include "pointst.h"
#include "graphe.h"

class Carte {
	public:
		void ajouter_noeud(long, const PointST&);
		void ajouter_route(const std::string&, const std::list<long>&);
		void changerDisponibilites(const std::list<std::string>&, bool);
		long trouverDestIdeale(long, double, double&, std::list<long>&);
	private:
		Graphe<PointST, long, double, std::string> graphe;
	friend std::istream& operator>>(std::istream&, Carte&);
};

#endif
