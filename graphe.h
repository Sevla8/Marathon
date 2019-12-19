#ifndef _GRAPHE__H_
#define _GRAPHE__H_

#include <unordered_map>
#include <istream>
#include <fstream>
#include <cstdlib>
#include <cassert>
#include <utility>
#include <limits>
#include <string>
#include <vector>
#include <queue>
#include <stack>
#include <list>
#include <set>

template <class S, class ES, class A, class EA>
class Graphe {
	public:
		void ajouter_sommet(const S&, const ES&);
		void ajouter_arete(const EA&, const ES&, const ES&);
		std::list<ES> lister_sommets() const;
		void modifier_etat_arete(const EA&, const bool&);
		void dijkstra_point_a_multipoints_calcul(const ES&, std::unordered_map<ES, A>&, std::unordered_map<ES, ES>&, const A&) const;
		void dijkstra_multipoints_a_point_calcul(const ES&, std::unordered_map<ES, A>&, std::unordered_map<ES, ES>&, const A&) const;
	private:
		struct Sommet {
			Sommet(const S&, const ES&);
			S objet;
			std::unordered_map<long unsigned, typename Graphe<S, ES, A, EA>::Arete> aretesSortantes;
			std::unordered_map<long unsigned, typename Graphe<S, ES, A, EA>::Arete> aretesEntrantes;
			ES etiquette;
		};
		struct Arete {
			Arete();
			A poids;
			bool active;
			EA etiquette;
		};
		std::unordered_map<ES, long unsigned> index;
		std::vector<Sommet> sommets;
};

template <class S, class ES, class A, class EA>
Graphe<S, ES, A, EA>::Sommet::Sommet(const S& s, const ES& e) : objet(s), etiquette(e) {}

template <class S, class ES, class A, class EA>
Graphe<S, ES, A, EA>::Arete::Arete() : active(true) {}

template <class S, class ES, class A, class EA>
void Graphe<S, ES, A, EA>::ajouter_sommet(const S& s, const ES& e) {
	// assert(this->index.find(e) == this->index.end());
	long unsigned indice = this->index.size();
	this->index[e] = indice;
	this->sommets.push_back(Sommet(s, e));
}

template <class S, class ES, class A, class EA>
void Graphe<S, ES, A, EA>::ajouter_arete(const EA& e, const ES& e1, const ES& e2) {
	this->sommets[this->index.at(e1)].aretesSortantes[this->index.at(e2)].poids = this->sommets.at(this->index.at(e1)).objet.distance(this->sommets.at(this->index.at(e2)).objet);
	this->sommets[this->index.at(e1)].aretesSortantes[this->index.at(e2)].etiquette = e;
	this->sommets[this->index.at(e2)].aretesEntrantes[this->index.at(e1)].poids = this->sommets.at(this->index.at(e1)).objet.distance(this->sommets.at(this->index.at(e2)).objet);
	this->sommets[this->index.at(e2)].aretesEntrantes[this->index.at(e1)].etiquette = e;
}

template <class S, class ES, class A, class EA>
std::list<ES> Graphe<S, ES, A, EA>::lister_sommets() const {
	std::list<ES> liste;
	for (typename std::vector<Sommet>::const_iterator iter = this->sommets.begin(); iter != this->sommets.end(); ++iter) {
		liste.push_back(iter->etiquette);
	}
	return liste;
}

template <class S, class ES, class A, class EA>
void Graphe<S, ES, A, EA>::modifier_etat_arete(const EA& e, const bool& a) {
	for (typename std::vector<Sommet>::iterator iter0 = this->sommets.begin(); iter0 != this->sommets.end(); ++iter0) {
		for (typename std::unordered_map<long unsigned, Arete>::iterator iter1 = iter0->aretesSortantes.begin(); iter1 != iter0->aretesSortantes.end(); ++iter1) {
			if (iter1->second.etiquette == e) iter1->second.active = a;
		}
		for (typename std::unordered_map<long unsigned, Arete>::iterator iter1 = iter0->aretesEntrantes.begin(); iter1 != iter0->aretesEntrantes.end(); ++iter1) {
			if (iter1->second.etiquette == e) iter1->second.active = a;
		}
	}
}

template <class S, class ES, class A, class EA>
void Graphe<S, ES, A, EA>::dijkstra_point_a_multipoints_calcul(const ES& depart, std::unordered_map<ES, A>& distances, std::unordered_map<ES, ES>& parents, const A& distMax) const {

	std::priority_queue<std::pair<A, ES>, std::vector<std::pair<A, ES>>, std::greater<std::pair<A, ES>>> Q;

	for (typename std::vector<Sommet>::const_iterator iter = this->sommets.begin(); iter != this->sommets.end(); ++iter) {
		if (this->sommets.at(this->index.at(depart)).objet.distance(iter->objet) <= distMax) {
			distances[iter->etiquette] = std::numeric_limits<A>::infinity();
			Q.push(std::pair<A, long unsigned>(distances.at(iter->etiquette), this->index.at(iter->etiquette)));
		}
	}
	distances[depart] = 0; // ¬ 0, element neutre du + de A
	Q.push(std::pair<A, long unsigned>(0, this->index.at(depart)));

	while (!Q.empty()) {
		const A dist = Q.top().first;
		const long unsigned v = Q.top().second;
		Q.pop();

		if (dist >= distMax) break;

		for (typename std::unordered_map<long unsigned, Arete>::const_iterator iter = this->sommets.at(v).aretesSortantes.begin(); iter != this->sommets.at(v).aretesSortantes.end(); ++iter) {
			if (iter->second.active) {
				const long unsigned& w = iter->first;
				double d = dist + iter->second.poids;

				if (distances.find(this->sommets.at(w).etiquette) != distances.end() && d < distances.at(this->sommets.at(w).etiquette)) {
					parents[this->sommets.at(w).etiquette] = this->sommets.at(v).etiquette;
					distances[this->sommets.at(w).etiquette] = d;
					Q.push(std::pair<A, long unsigned>(d, w));
				}
			}
		}
	}
}

template <class S, class ES, class A, class EA>
void Graphe<S, ES, A, EA>::dijkstra_multipoints_a_point_calcul(const ES& depart, std::unordered_map<ES, A>& distances, std::unordered_map<ES, ES>& parents, const A& distMax) const {

	std::priority_queue<std::pair<A, ES>, std::vector<std::pair<A, ES>>, std::greater<std::pair<A, ES>>> Q;

	for (typename std::vector<Sommet>::const_iterator iter = this->sommets.begin(); iter != this->sommets.end(); ++iter) {
		if (this->sommets.at(this->index.at(depart)).objet.distance(iter->objet) <= distMax) {
			distances[iter->etiquette] = std::numeric_limits<A>::infinity();
			Q.push(std::pair<A, long unsigned>(distances.at(iter->etiquette), this->index.at(iter->etiquette)));
		}
	}
	distances[depart] = 0; // ¬ 0, element neutre du + de A
	Q.push(std::pair<A, long unsigned>(0, this->index.at(depart)));

	while (!Q.empty()) {
		const A dist = Q.top().first;
		const long unsigned v = Q.top().second;
		Q.pop();

		if (dist >= distMax) break;

		for (typename std::unordered_map<long unsigned, Arete>::const_iterator iter = this->sommets.at(v).aretesEntrantes.begin(); iter != this->sommets.at(v).aretesEntrantes.end(); ++iter) {
			if (iter->second.active) {
				const long unsigned& w = iter->first;
				double d = dist + iter->second.poids;

				if (distances.find(this->sommets.at(w).etiquette) != distances.end() && d < distances.at(this->sommets.at(w).etiquette)) {
					parents[this->sommets.at(w).etiquette] = this->sommets.at(v).etiquette;
					distances[this->sommets.at(w).etiquette] = d;
					Q.push(std::pair<A, long unsigned>(d, w));
				}
			}
		}
	}
}

#endif
