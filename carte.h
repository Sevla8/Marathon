#ifndef _CARTE__H_
#define _CARTE__H_

#include <istream>
#include <list>
#include <set>
#include <string>
#include "pointst.h"
#include "graphe.h"

class Carte {
	public:
		void ajouter_noeud(long id, const PointST& p);
		void ajouter_route(const std::string& nom, const std::list<long>& noeuds);
		void changerDisponibilites(const std::list<std::string>& nomsroute, bool estdisponible);
		long trouverDestIdeale(long noeudorigine, double distancecible, double& distancetrouve, std::list<long>& trajet);
		//connexe
		// À compléter (vous pouvez tout changer).

	private:
		Graphe<PointST, std::string, long> graphe;

	friend std::istream& operator>>(std::istream&, Carte&);
};

#endif
