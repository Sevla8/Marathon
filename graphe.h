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
		void ajouter_arete(const EA&, const A&, const ES&, const ES&);
		const S& obtenir_sommet(const ES&) const;
		void modifier_etat_arete(const EA&, const bool&);
		std::list<ES> lister_sommets() const;
		std::set<EA> lister_aretes() const;
		std::set<EA> lister_aretes_sortantes(const ES&) const;
		std::set<EA> lister_aretes_entrantes(const ES&) const;
		bool existe_arete_sortante(const ES&, const ES&) const;
		bool existe_arete_entrante(const ES&, const ES&) const;
		const long unsigned taille() const;

		// std::set<long> obtenir_sommets_inaccessibles() const;
		// void retirer_sommets_inaccessibles();

		void dijkstra_point_a_multipoints(const ES&, std::unordered_map<ES, A>&, std::unordered_map<ES, ES>&, const A&);
		void dijkstra_multipoints_a_point(const ES&, std::unordered_map<ES, A>&, std::unordered_map<ES, ES>&, const A&);
		void floyd_warshall();
		void a_etoile(const ES&, const ES&);

		// recouvrement minimal
	private:
		// std::list<std::set<long>> tarjan_parcours_profondeur() const;
		// void parcours_profondeur(const typename std::unordered_map<long, typename Graphe<S, ES, A, EA>::Sommet>::const_iterator&, long int&, std::stack<long>&, std::list<std::set<long>>&) const;
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
		std::unordered_map<ES, long unsigned> indices;
		std::vector<Sommet> sommets;
};

template <class S, class ES, class A, class EA>
Graphe<S, ES, A, EA>::Sommet::Sommet(const S& s, const ES& e) : objet(s), etiquette(e) {}

template <class S, class ES, class A, class EA>
Graphe<S, ES, A, EA>::Arete::Arete() : active(true) {}

template <class S, class ES, class A, class EA>
void Graphe<S, ES, A, EA>::ajouter_sommet(const S& s, const ES& e) {
	assert(this->indices.find(e) == this->indices.end());
	long unsigned indice = this->indices.size();
	this->indices[e] = indice;
	this->sommets.push_back(Sommet(s, e));
}

template <class S, class ES, class A, class EA>
void Graphe<S, ES, A, EA>::ajouter_arete(const EA& e, const A& p, const ES& e1, const ES& e2) {
	this->sommets[this->indices.at(e1)].aretesSortantes[this->indices.at(e2)].poids = p;
	this->sommets[this->indices.at(e1)].aretesSortantes[this->indices.at(e2)].etiquette = e;
	this->sommets[this->indices.at(e2)].aretesEntrantes[this->indices.at(e1)].poids = p;
	this->sommets[this->indices.at(e2)].aretesEntrantes[this->indices.at(e1)].etiquette = e;
}

template <class S, class ES, class A, class EA>
const S& Graphe<S, ES, A, EA>::obtenir_sommet(const ES& etiquette) const {
	return this->sommets.at(this->indices.at(etiquette)).objet;
}

template <class S, class ES, class A, class EA>
void Graphe<S, ES, A, EA>::modifier_etat_arete(const EA& e, const bool& act) {
	for (typename std::vector<Sommet>::iterator iter0 = this->sommets.begin(); iter0 != this->sommets.end(); ++iter0) {
		for (typename std::unordered_map<long unsigned, Arete>::iterator iter1 = (*iter0).aretesSortantes.begin(); iter1 != (*iter0).aretesSortantes.end(); ++iter1) {
			if (iter1->second.etiquette == e) iter1->second.active = act;
		}
		for (typename std::unordered_map<long unsigned, Arete>::iterator iter1 = (*iter0).aretesEntrantes.begin(); iter1 != (*iter0).aretesEntrantes.end(); ++iter1) {
			if (iter1->second.etiquette == e) iter1->second.active = act;
		}
	}
}

template <class S, class ES, class A, class EA>
std::list<ES> Graphe<S, ES, A, EA>::lister_sommets() const {
	std::list<ES> liste;
	for (typename std::vector<Sommet>::const_iterator iter = this->sommets.begin(); iter != this->sommets.end(); ++iter) {
		liste.push_back((*iter).etiquette);
	}
	return liste;
}

template <class S, class ES, class A, class EA>
std::set<EA> Graphe<S, ES, A, EA>::lister_aretes() const {
	std::set<EA> liste;
	for (typename std::vector<Sommet>::const_iterator iter0 = this->sommets.begin(); iter0 != this->sommets.end(); ++iter0) {
		for (typename std::unordered_map<long unsigned, Arete>::const_iterator iter1 = (*iter0).aretesSortantes.begin(); iter1 != (*iter0).aretesSortantes.end(); ++iter1) {
			liste.insert(iter1->second.etiquette);
		}
	}
	return liste;
}

template <class S, class ES, class A, class EA>
std::set<EA> Graphe<S, ES, A, EA>::lister_aretes_sortantes(const ES& etiquette) const {
	std::set<EA> liste;
	for (typename std::unordered_map<long unsigned, Arete>::const_iterator iter = this->sommets[this->indices.at(etiquette)].aretesSortantes.begin(); iter != this->sommets[this->indices.at(etiquette)].aretesSortantes.end(); ++iter) {
		liste.insert(iter->second.etiquette);
	}
	return liste;
}

template <class S, class ES, class A, class EA>
std::set<EA> Graphe<S, ES, A, EA>::lister_aretes_entrantes(const ES& etiquette) const {
	std::set<EA> liste;
	for (typename std::unordered_map<long unsigned, Arete>::const_iterator iter = this->sommets[this->indices.at(etiquette)].aretesEntrantes.begin(); iter != this->sommets[this->indices.at(etiquette)].aretesEntrantes.end(); ++iter) {
		liste.insert(iter->second.etiquette);
	}
	return liste;
}

template <class S, class ES, class A, class EA>
bool Graphe<S, ES, A, EA>::existe_arete_sortante(const ES& e1, const ES& e2) const {
	return this->sommets[this->indices.at(e1)].aretesSortantes.find(this->sommets[this->indices.at(e2)]) != this->sommets[this->indices.at(e1)].aretesSortantes.end();
}

template <class S, class ES, class A, class EA>
bool Graphe<S, ES, A, EA>::existe_arete_entrante(const ES& e1, const ES& e2) const {
	return this->sommets[this->indices.at(e1)].aretesEntrantes.find(this->sommets[this->indices.at(e2)]) != this->sommets[this->indices.at(e1)].aretesEntrantes.end();
}

template <class S, class ES, class A, class EA>
const long unsigned Graphe<S, ES, A, EA>::taille() const {
	return this->sommets.size();
}

template <class S, class ES, class A, class EA>
void Graphe<S, ES, A, EA>::dijkstra_point_a_multipoints(const ES& s, std::unordered_map<ES, A>& distances, std::unordered_map<ES, ES>& parents, const A& distMax) {

	std::list<ES> liste = this->lister_sommets();
	for (typename std::list<ES>::const_iterator iter = liste.begin(); iter != liste.end(); ++iter) {
		distances[*iter] = std::numeric_limits<double>::infinity();
	}
	distances[s] = 0; // element neutre du + de A

	std::priority_queue<std::pair<A, long unsigned>, std::vector<std::pair<A, long unsigned>>, std::greater<std::pair<A, long unsigned>>> Q;
	for (typename std::list<ES>::const_iterator iter = liste.begin(); iter != liste.end(); ++iter) {
		Q.push(std::pair<A, long unsigned>(distances.at(*iter), this->indices.at(*iter)));
	}

	while (!Q.empty()) {
		A dist = Q.top().first;
		long unsigned v = Q.top().second;
		Q.pop();

		if (dist >= distMax) break;

		for (typename std::unordered_map<long unsigned, Arete>::const_iterator iter = this->sommets.at(v).aretesSortantes.begin(); iter != this->sommets.at(v).aretesSortantes.end(); ++iter) {
			if (iter->second.active) {
				long unsigned w = iter->first;
				double d = dist + iter->second.poids;

				if (d < distances.at(this->sommets.at(w).etiquette)) {
					parents[this->sommets.at(w).etiquette] = this->sommets.at(v).etiquette;
					distances[this->sommets.at(w).etiquette] = d;
					Q.push(std::pair<A, long unsigned>(d, w));
				}
			}
		}
	}
}

template <class S, class ES, class A, class EA>
void Graphe<S, ES, A, EA>::dijkstra_multipoints_a_point(const ES& s, std::unordered_map<ES, A>& distances, std::unordered_map<ES, ES>& parents, const A& distMax) {

	std::list<ES> liste = this->lister_sommets();
	for (typename std::list<ES>::const_iterator iter = liste.begin(); iter != liste.end(); ++iter) {
		distances[*iter] = std::numeric_limits<double>::infinity();
	}
	distances[s] = 0; // element neutre du + de A

	std::priority_queue<std::pair<A, long unsigned>, std::vector<std::pair<A, long unsigned>>, std::greater<std::pair<A, long unsigned>>> Q;
	for (typename std::list<ES>::const_iterator iter = liste.begin(); iter != liste.end(); ++iter) {
		Q.push(std::pair<A, long unsigned>(distances.at(*iter), this->indices.at(*iter)));
	}

	while (!Q.empty()) {
		A dist = Q.top().first;
		long unsigned v = Q.top().second;
		Q.pop();

		if (dist >= distMax) break;

		for (typename std::unordered_map<long unsigned, Arete>::const_iterator iter = this->sommets.at(v).aretesEntrantes.begin(); iter != this->sommets.at(v).aretesEntrantes.end(); ++iter) {
			if (iter->second.active) {
				long unsigned w = iter->first;
				double d = dist + iter->second.poids;

				if (d < distances.at(this->sommets.at(w).etiquette)) {
					parents[this->sommets.at(w).etiquette] = this->sommets.at(v).etiquette;
					distances[this->sommets.at(w).etiquette] = d;
					Q.push(std::pair<A, long unsigned>(d, w));
				}
			}
		}
	}
}

#endif
