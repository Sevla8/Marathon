#include "carte.h"

void Carte::ajouter_noeud(long id, const PointST& p) {
	this->graphe.ajouter_sommet(p, id);
}

void Carte::ajouter_route(const std::string& nomroute, const std::list<long>& noeuds) {
	for (std::list<long>::const_iterator iter = noeuds.begin(); iter != noeuds.end(); ) {
		long tmp1 = *iter;
		++iter;
		if (iter == noeuds.end()) break;
		long tmp2 = *iter;
		this->graphe.ajouter_arete(nomroute, tmp1, tmp2);
	}
}

void Carte::changerDisponibilites(const std::list<std::string>& nomsroute, bool estdisponible) {
	for (std::list<std::string>::const_iterator iter = nomsroute.begin(); iter != nomsroute.end(); ++iter) {
		this->graphe.modifier_etat_arete(*iter, estdisponible);
	}
}

long Carte::trouverDestIdeale(long noeudorigine, double distancecible, double& distancetrouve, std::list<long>& trajet) {

	long destinationIdeale = -1;
	distancetrouve = std::numeric_limits<double>::infinity();

	std::unordered_map<long, double> distances0;
	std::unordered_map<long, long> parents0;
	this->graphe.dijkstra_point_a_multipoints_calcul(noeudorigine, distances0, parents0, distancecible+CST);

	std::unordered_map<long, double> distances1;
	std::unordered_map<long, long> parents1;
	this->graphe.dijkstra_multipoints_a_point_calcul(noeudorigine, distances1, parents1, distancecible+CST);

	std::unordered_map<long, double>::const_iterator iter0 = distances0.begin();
	std::unordered_map<long, double>::const_iterator iter1 = distances1.begin();
	while (iter0 != distances0.end() && iter1 != distances1.end()) { // même taille normalement
		double x = iter0->second + iter1->second;

		if (x >= distancecible && x < distancetrouve) {
			destinationIdeale = iter0->first;
			distancetrouve = x;
		}

		++iter0;
		++iter1;
	}

	if (destinationIdeale != -1) {
		for (long noeud = destinationIdeale; noeud != noeudorigine; noeud = parents0.at(noeud)) {
			trajet.push_front(noeud);
		}
		trajet.push_front(noeudorigine);
		if (parents1.find(destinationIdeale) != parents1.end()) {	// cas où distancecible = 0
			for (long noeud = parents1.at(destinationIdeale); noeud != noeudorigine; noeud = parents1.at(noeud)) {
				trajet.push_back(noeud);
			}
			trajet.push_back(noeudorigine);
		}
	}

	return destinationIdeale;
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
