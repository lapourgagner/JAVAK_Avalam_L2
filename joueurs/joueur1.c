// TODO: Utiliser les optimisation à la compilation (-O3) et la tester
#include "../include/avalam.h"
#include "../include/moteur.h"

#include <stdio.h>
#include <stdlib.h>

// Utile pour simuler un coup
void copierPlateau(T_Position currentPosition, T_Position currentPositionCopy)
{
	currentPositionCopy.trait	= currentPosition.trait;
	currentPositionCopy.numCoup = currentPosition.numCoup;
	for(int i = 0; i < NBCASES; i++)
		currentPositionCopy.cols[i] = currentPosition.cols[i];
	currentPositionCopy.evolution.bonusJ = currentPosition.evolution.bonusJ;
	currentPositionCopy.evolution.bonusR = currentPosition.evolution.bonusR;
	currentPositionCopy.evolution.malusJ = currentPosition.evolution.malusJ;
	currentPositionCopy.evolution.malusR = currentPosition.evolution.malusR;
	return;
}

// Recher l'index d'un coup avec un case origine et une case destiantion donnée
int rechercheCoup(T_ListeCoups listeCoups, int origine, int destination)
{
	int result_1;

	// result = bsearch();

	if(NULL == result_1) {
		return -1;
	}
	return result_1;
}

int placerBonus(T_Position currentPosition, T_ListeCoups listeCoups)
// TODO: Adapater technique de la mante religieuse, pas adaptée avec l'ordre de placement des bonus
{
	int coup;
	switch(currentPosition.numCoup) {
		case 4: // Malus rouge
			//
			return coup;
			break;
		case 3: // Malus jaune
			//
			return coup;
			break;
		case 2: // Bonus rouge
			//
			return coup;
			break;
		case 1: // Bonus jaune
			//
			return coup;
			break;
		default:

			return -1;
			break;
	}
}

float evaluerPlateau(T_Position currentPosition, T_ListeCoups listeCoupsSoi)
{
	float evaluation;

	// Liste des paramètres
	int nb_coups_soi = 0;
	int nb_coups_adv, nb_pions_soi, nb_pion_adv, score_soi, score_adv, score5_soi, score5_adv;

	// On évalue le score
	T_Score score = evaluerScore(currentPosition);
	if(JAU == currentPosition.trait) {
		score_soi  = score.nbJ;
		score_adv  = score.nbR;
		score5_soi = score.nbJ5;
		score5_adv = score.nbR5;
	}
	else {
		score_soi  = score.nbR;
		score_adv  = score.nbJ;
		score5_soi = score.nbR5;
		score5_adv = score.nbJ5;
	}

	// On inverse le trait pour lister les coups adverses
	if(JAU == currentPosition.trait)
		currentPosition.trait = ROU;
	else
		currentPosition.trait = JAU;

	// On liste les coups adverses
	T_ListeCoups listeCoupsAdversaire = getCoupsLegaux(currentPosition);
	// TODO: Parcourir tous les coups possibles et le compter

	// On replace le trait correctement
	if(JAU == currentPosition.trait)
		currentPosition.trait = ROU;
	else
		currentPosition.trait = JAU;

	// TODO: Stopped here
	// On compte les coups:
	while(0 == listeCoupsSoi.coups[nb_coups_soi].origine && 0 == listeCoupsSoi.coups[nb_coups_soi].destination) {
		nb_coups_soi++;
	}

	// TODO: mutiplier toutes lezs valeurs que l'on a obtenu par des coeffecicient à defeinir pour avoir un score final du plateau, à comparer aux auutres mo ments 
	// score = ;

	printf1("Evaluation: %f\n", evaluation);
	return evaluation;
}

void choisirCoup(T_Position currentPosition, T_ListeCoups listeCoups)
{
	int result;	


	// Pour le debug: Affiche tous les coups possibles, et même plus, ceux qui n'exitent pas !
	for(int i = 0; i < NBCASES * 8; i++) {
		printf("orig: %d, dest: %d, num: %d\n", listeCoups.coups[i].origine, listeCoups.coups[i].destination, i);
	}

	// Gestion des bonus/malus:
	if(5 > currentPosition.numCoup) // (bj, br, mj, mr)
	{
		result = placerBonus(currentPosition, listeCoups);

		if(result != -1)
			printf("Erreur lors du placememnt des bonus (numCoup: %d)\n", currentPosition.numCoup);

		ecrireIndexCoup(result);
	}

	printf("Etat des bonus/malus: bj: %d, mj: %d, br: %d, mr:%d. Trait: %d\n", currentPosition.evolution.bonusJ,
		   currentPosition.evolution.malusJ, currentPosition.evolution.bonusR, currentPosition.evolution.malusR,
		   currentPosition.trait);

	ecrireIndexCoup(0);
}