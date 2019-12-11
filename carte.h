#ifndef _CARTE__H_
#define _CARTE__H_

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
		// void retirer_noeuds_inaccessibles();
		void changerDisponibilites(const std::list<std::string>&, bool);
		long trouverDestIdeale(long, double, double&, std::list<long>&);
	private:
		Graphe<PointST, long, double, std::string> graphe;
	friend std::istream& operator>>(std::istream&, Carte&);
};

#endif
