#include "network.h"
#include <random>
#include <algorithm>
#include <map>
#include <vector>
#include <iterator>
#include <iostream>

Network::Network()
{}

void Network::resize(const size_t &n)
{
	while(values.size() > n) 
	{
		size_t k = values.size();
		for(std::multimap <size_t, size_t> ::const_iterator i = links.lower_bound(k); i != links.upper_bound(k) ; ++i) //pour chaque valeur liée à k (i->second)
		{
			for(std::multimap <size_t, size_t> ::const_iterator j = links.lower_bound(i->first); j != links.upper_bound(i->first) ; ++j) //on parcourt tous les liens formés par i->first
			{
				if(j->second == k) links.erase(j);//quand on trouve le lien (i, k) on le supprime
			}
		}
		links.erase(links.lower_bound(k), links.upper_bound(k)); //efface tous les liens de k à autre chose	
		values.pop_back(); //si le vector est trop grand on le raccourcit en supprimant les liens faits par des nodes supprimés au préalable
	}
	while(values.size() < n)
	{
		values.push_back(0.0); //si le vector est trop petit on lui rajoute des cases
	}
	std::random_device rd;
	std::mt19937 rnd = std::mt19937(rd());
	std::normal_distribution<> d{0,1};
	for(size_t i(0); i < n ; ++i)
	{
		values[i] = d(rnd); //on réinitialise chaque valeur avec un nombre aléatoire de distribution normale
	}	
}

bool Network::add_link(const size_t& a, const size_t& b)
{
	bool condition(true);
	for(std::multimap <size_t, size_t> ::const_iterator i = links.lower_bound(a); i != links.upper_bound(a) ; ++i) 
	{
		if(i->second == b) //si il y a deja un lien a, b
		{
			condition = false;
		}
	}
	for(std::multimap <size_t, size_t> ::const_iterator i = links.lower_bound(b); i != links.upper_bound(b) ; ++i)
	{
		if(i->second == a) //si il y a deja un lien b, a
		{
			condition = false;
		}
	}
	if(condition and (a!=b) and (a >= 0 and a < values.size()) and (b >= 0 and b < values.size()))
	{
		links.insert(std::pair<size_t, size_t> (a,b));
		links.insert(std::pair<size_t, size_t> (b,a));
		return true;
	}
	else return false;
}
 
size_t Network::random_connect(const double& m)
{
	links.clear();
	std::random_device rd;
	std::mt19937 rnd = std::mt19937(rd());
	std::poisson_distribution<> poiss(m);
	size_t n = values.size();
	for(size_t i(0); i < n; ++i) //pour chaque noeud
	{
		int d = poiss(rnd); //nb de liens qu'on voudrait que le node ait
		if(d > n - 1) d = n - 1; //(on ne peut pas faire plus de liens qu'il y a de nodes
		size_t k(d - degree(n));
		std::uniform_int_distribution<> unif(0, n-1);
		for(int j(0); j < k ; ++j) //nb de liens à ajouter = d - degré actuel du noeud
		{ 
			size_t l(0);
			bool ok;
			do{
				int b = unif(rnd); //on choisit un autre noeud aleatoirement
				ok = add_link(i, b); //on lie le noeud actuel et l'autre noeud
				++l;
			}while(not ok and l < n-1); //on essaye de créer un nouveau lien jusqu'à ce qu'on y arrive ou alors jusqu'à ce qu'on ait essayé tous les noeuds sans succès (si aucun n'accepte la création du lien)
			/* int b = unif(rnd);
			add_link(i, b);*/ //ces 2 lignes sont la version où on fait 1 seul essai, peu importe qu'il réussisse ou pas
		}
	} 
	//on divise par deux la taille de links car les liens sont bidirectionnels : 1 lien bidirectionnel = 2 liens dans links (a, b et b, a)
	return links.size() / 2;
}


size_t Network::set_values(const std::vector<double>& newValues) 
{
	values.clear(); //on vide tous les nodes
	links.clear(); //on vide tous les links
	while(values.size() > newValues.size()) values.pop_back();
	while(values.size() < newValues.size()) values.push_back(0.0); //on ajuste la taille de values à la taille du nouveau vector
	for(size_t i(0); i < values.size(); ++i)
	{
		values[i] = newValues [i]; //et on remplit values avec les nouvelles valeurs
	}
	return values.size();
}

size_t Network::size() const
{
	return values.size();
}

size_t Network::degree(const size_t &_n) const
{
	return links.count(_n);
}

double Network::value(const size_t &_n) const
{
	return values[_n];
}

std::vector<double> Network::sorted_values() const 
{
	std::vector<double> sorted = values; //on copie values dans un autre vector pour ne pas modifier values
	std::sort(sorted.begin(), sorted.end()); //on trie le vector par ordre croissant
	std::reverse(sorted.begin(), sorted.end()); //puis on inverse l'ordre du vector
	return sorted; //retourne le vector trié par ordre décroissant des valeurs
}

std::vector<size_t> Network::neighbors(const size_t& a) const 
{
	std::vector<size_t> neighbors; //on va stocker les numeros des voisins dans ce vector
	for(std::multimap <size_t, size_t> ::const_iterator i = links.lower_bound(a); i != links.upper_bound(a) ; ++i) //on parcourt tous les voisins de a
	{
		neighbors.push_back(i -> second); //et on les ajoute au vector de voisins
	}
	return neighbors;
}
