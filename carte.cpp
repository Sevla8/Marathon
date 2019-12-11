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
		double poids = this->graphe.obtenir_sommet(tmp1).distance(this->graphe.obtenir_sommet(tmp2));
		this->graphe.ajouter_arete(nomroute, poids, tmp1, tmp2);
	}
}

// void Carte::retirer_noeuds_inaccessibles() {
// 	this->graphe.retirer_sommets_inaccessibles();
// }

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

	this->graphe.dijkstra_point_a_multipoints(noeudorigine, distances0, parents0);

	std::unordered_map<long, double> distances1;
	std::unordered_map<long, long> parents1;

	std::list<long> liste = this->graphe.lister_sommets();

	for (std::list<long>::const_iterator iter = liste.begin(); iter != liste.end(); ++iter) {

		std::unordered_map<long, double> distances1_tmp;
		std::unordered_map<long, long> parents1_tpm;

		this->graphe.dijkstra_point_a_multipoints(*iter, distances1_tmp, parents1_tpm);

		double x = distances0[*iter] + distances1_tmp[noeudorigine];

		if (x >= distancecible && x < distancetrouve) {
			destinationIdeale = *iter;
			distancetrouve = x;
			distances1 = distances1_tmp;
			parents1 = parents1_tpm;
		}
	}

	if (destinationIdeale != -1) {
		for (long noeud = noeudorigine; noeud != destinationIdeale; noeud = parents1[noeud]) {
			trajet.push_front(noeud);
		}
		for (long noeud = destinationIdeale; noeud != noeudorigine; noeud = parents0[noeud]) {
			trajet.push_front(noeud);
		}
		trajet.push_front(noeudorigine);
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
