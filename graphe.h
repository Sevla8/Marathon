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
		Graphe();
		~Graphe();
		void ajouter_sommet(const S&, const ES&);
		void ajouter_arete(const EA&, const A&, const bool&, const ES&, const ES&);
		const S& obtenir_sommet(const ES&) const;
		std::list<ES> lister_sommets() const;
		bool contient_sommet(const ES&) const;
		void modifier_etat_arete(const EA&, const bool&);
		void dijkstra_point_a_multipoints(const ES&, std::unordered_map<ES, A>&, std::unordered_map<ES, ES>&, const A&) const;
		void dijkstra_multipoints_a_point(const ES&, std::unordered_map<ES, A>&, std::unordered_map<ES, ES>&, const A&) const;
		Graphe<S, ES, A, EA> sous_graphe(const ES&, const A&) const;
	private:
		struct Sommet {
			Sommet(const S&, const ES&);
			S objet;	// doit avoir une fonction: A S::distance(const S&) const;
			std::unordered_map<Sommet*, typename Graphe<S, ES, A, EA>::Arete*> aretesSortantes;
			std::unordered_map<Sommet*, typename Graphe<S, ES, A, EA>::Arete*> aretesEntrantes;
			ES etiquette;
		};
		struct Arete {
			Arete(const A&, const bool&, const EA&);
			A poids;
			bool active;
			EA etiquette;
		};
		std::unordered_map<ES, Sommet*> sommets;
		std::unordered_map<EA, std::list<Arete*>> aretes;
};

template <class S, class ES, class A, class EA>
Graphe<S, ES, A, EA>::Graphe() {}

template <class S, class ES, class A, class EA>
Graphe<S, ES, A, EA>::~Graphe() {
	for (typename std::unordered_map<EA, std::list<Arete*>>::iterator iter0 = this->aretes.begin(); iter0 != this->aretes.end(); ++iter0) {
		for (typename std::list<Arete*>::iterator iter1 = iter0->second.begin(); iter1 != iter0->second.end(); ++iter1) {
			delete *iter1;
		}
	}
	for (typename std::unordered_map<ES, Sommet*>::iterator iter = this->sommets.begin(); iter != this->sommets.end(); ++iter) {
		delete iter->second;
	}
}

template <class S, class ES, class A, class EA>
Graphe<S, ES, A, EA>::Sommet::Sommet(const S& s, const ES& e) : objet(s), etiquette(e) {}

template <class S, class ES, class A, class EA>
Graphe<S, ES, A, EA>::Arete::Arete(const A& p, const bool& a, const EA& e) : poids(p), active(a), etiquette(e) {}

template <class S, class ES, class A, class EA>
void Graphe<S, ES, A, EA>::ajouter_sommet(const S& s, const ES& e) {
	// assert(this->sommets.find(e) == this->sommets.end());
	Sommet* sommet = new Sommet(s, e);
	this->sommets[e] = sommet;
}

template <class S, class ES, class A, class EA>
void Graphe<S, ES, A, EA>::ajouter_arete(const EA& e, const A& p, const bool& a, const ES& e1, const ES& e2) {
	// assert(this->sommets.find(e1) != this->sommets.end());
	// assert(this->sommets.find(e2) != this->sommets.end());
	Arete* arete = new Arete(p, a, e);
	(*this->sommets.at(e1)).aretesSortantes[this->sommets.at(e2)] = arete;
	(*this->sommets.at(e2)).aretesEntrantes[this->sommets.at(e1)] = arete;
	this->aretes[e].push_back(arete);
}

template <class S, class ES, class A, class EA>
const S& Graphe<S, ES, A, EA>::obtenir_sommet(const ES& e) const {
	return this->sommets.at(e)->objet;
}

template <class S, class ES, class A, class EA>
std::list<ES> Graphe<S, ES, A, EA>::lister_sommets() const {
	std::list<ES> liste;
	for (typename std::unordered_map<ES, Sommet*>::const_iterator iter = this->sommets.begin(); iter != this->sommets.end(); ++iter) {
		liste.push_back(iter->first);
	}
	return liste;
}

template <class S, class ES, class A, class EA>
bool Graphe<S, ES, A, EA>::contient_sommet(const ES& e) const {
	return this->sommets.find(e) != this->sommets.end();
}

template <class S, class ES, class A, class EA>
void Graphe<S, ES, A, EA>::modifier_etat_arete(const EA& e, const bool& a) {
	for (typename std::list<Arete*>::const_iterator iter = this->aretes.at(e).begin(); iter != this->aretes.at(e).end(); ++iter) {
		(*iter)->active = a;
	}
}

template <class S, class ES, class A, class EA>
void Graphe<S, ES, A, EA>::dijkstra_point_a_multipoints(const ES& depart, std::unordered_map<ES, A>& distances, std::unordered_map<ES, ES>& parents, const A& distMax) const{

	std::priority_queue<std::pair<A, ES>, std::vector<std::pair<A, ES>>, std::greater<std::pair<A, ES>>> Q;

	for (typename std::unordered_map<ES, Sommet*>::const_iterator iter = this->sommets.begin(); iter != this->sommets.end(); ++iter) {
		if (this->sommets.at(depart)->objet.distance(iter->second->objet) <= distMax) {	//
			distances[iter->first] = std::numeric_limits<double>::infinity();
			Q.push(std::pair<A, ES>(distances[iter->first], iter->first));
		}	//
	}
	distances[depart] = 0; // element neutre du + de A
	Q.push(std::pair<A, ES>(0, depart));

	while (!Q.empty()) {
		A dist = Q.top().first;
		ES v = Q.top().second;
		Q.pop();

		if (dist >= distMax) break;

		for (typename std::unordered_map<Graphe<S, ES, A, EA>::Sommet*, Graphe<S, ES, A, EA>::Arete*>::const_iterator iter = this->sommets.at(v)->aretesSortantes.begin(); iter != this->sommets.at(v)->aretesSortantes.end(); ++iter) {
			if (iter->second->active) {
				ES w = iter->first->etiquette;
				double d = dist + iter->second->poids;

				if (distances.find(w) != distances.end() && d < distances.at(w)) {
					parents[w] = v;
					distances[w] = d;
					Q.push(std::pair<A, ES>(d, w));
				}
			}
		}
	}
}

template <class S, class ES, class A, class EA>
void Graphe<S, ES, A, EA>::dijkstra_multipoints_a_point(const ES& depart, std::unordered_map<ES, A>& distances, std::unordered_map<ES, ES>& parents, const A& distMax) const {

	std::priority_queue<std::pair<A, ES>, std::vector<std::pair<A, ES>>, std::greater<std::pair<A, ES>>> Q;

	for (typename std::unordered_map<ES, Sommet*>::const_iterator iter = this->sommets.begin(); iter != this->sommets.end(); ++iter) {
		if (this->sommets.at(depart)->objet.distance(iter->second->objet) <= distMax) {	//
			distances[iter->first] = std::numeric_limits<double>::infinity();
			Q.push(std::pair<A, ES>(distances[iter->first], iter->first));
		}	//
	}
	distances[depart] = 0; // element neutre du + de A
	Q.push(std::pair<A, ES>(0, depart));

	while (!Q.empty()) {
		A dist = Q.top().first;
		ES v = Q.top().second;
		Q.pop();

		if (dist >= distMax) break;

		for (typename std::unordered_map<Graphe<S, ES, A, EA>::Sommet*, Graphe<S, ES, A, EA>::Arete*>::const_iterator iter = this->sommets.at(v)->aretesEntrantes.begin(); iter != this->sommets.at(v)->aretesEntrantes.end(); ++iter) {
			if (iter->second->active) {
				ES w = iter->first->etiquette;
				double d = dist + iter->second->poids;

				if (distances.find(w) != distances.end() &&d < distances.at(w)) {
					parents[w] = v;
					distances[w] = d;
					Q.push(std::pair<A, ES>(d, w));
				}
			}
		}
	}
}

template <class S, class ES, class A, class EA>
Graphe<S, ES, A, EA> Graphe<S, ES, A, EA>::sous_graphe(const ES& e, const A& d) const {
	Graphe<S, ES, A, EA> g;
	S objet = this->obtenir_sommet(e);

	for (typename std::unordered_map<ES, Sommet*>::const_iterator iter = this->sommets.begin(); iter != this->sommets.end(); ++iter) {
		if (iter->second->objet.distance(objet) < 2 * d) {
			g.ajouter_sommet(iter->second->objet, iter->second->etiquette);
		}
	}

	for (typename std::unordered_map<ES, Sommet*>::const_iterator iter0 = g.sommets.begin(); iter0 != g.sommets.end(); ++iter0) {
		for (typename std::unordered_map<Sommet*, typename Graphe<S, ES, A, EA>::Arete*>::const_iterator iter1 = this->sommets.at(iter0->first)->aretesSortantes.begin(); iter1 != this->sommets.at(iter0->first)->aretesSortantes.end(); ++iter1) {
			if (g.contient_sommet(iter1->first->etiquette)) {
				g.ajouter_arete(iter1->second->etiquette, iter1->second->poids, iter1->second->active, iter0->first, iter1->first->etiquette);
			}
		}
	}

	return g;
}

#endif
