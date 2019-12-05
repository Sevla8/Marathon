#include "carte.h"

void Carte::ajouter_noeud(long id, const PointST& p) {
	// À compléter.
}

void Carte::ajouter_route(const std::string& nomroute, const std::list<long>& noeuds) {
	// À compléter.
}

void Carte::changerDisponibilites(const std::list<std::string>& nomsroute, bool estdisponible) {
	// À compléter.
}

long Carte::trouverDestIdeale(long noeudorigine, double distancecible, double& distancetrouve, std::list<long>& trajet) {
	// À compléter.
	distancetrouve = 0;
	trajet.push_front(noeudorigine);
	return noeudorigine;
}

std::istream& operator>>(std::istream& is, Carte& carte) {
	int countnoeuds=0, countroutes=0, countrefs=0;
	// Lire les lieux
	while (is) {
		std::string nomlieu;
		long osmid;
		is >> nomlieu;
		if (nomlieu == "---") break;
		assert(nomlieu[0] == 'n');
		osmid = atol(nomlieu.c_str() + 1);
		PointST point;
		char pv;
		is >> point >> pv;
		assert(pv == ';');
		carte.ajouter_noeud(osmid, point);
		countnoeuds++;
	}

	// Lire les routes
	while (is) {
		std::string nomroute;
		is >> nomroute;
		if (nomroute.empty() || nomroute == "---") break;
		char deuxpoints;
		is >> deuxpoints;
		assert(deuxpoints == ':');

		std::list<long> listenomslieux;
		std::string nomlieu;
		while (is) {
			is >> nomlieu;
			if (nomlieu == ";") break;
			assert(nomlieu != ":"); // pour robustesse
			assert(nomlieu.find(";") == std::string::npos); // pour robustesse
			long osmid;
			assert(nomlieu[0] == 'n');
			osmid = atol(nomlieu.c_str() + 1);
			listenomslieux.push_back(osmid);
			countrefs++;
		}
		assert(nomlieu == ";");
		carte.ajouter_route(nomroute, listenomslieux);
		countroutes++;
	}
	//std::cerr << "Carte chargée: " << countnoeuds << " noeuds; " << countroutes << " routes; " << countrefs << " références." << std::endl;

	return is;
}
