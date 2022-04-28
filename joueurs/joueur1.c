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
			coup = 2;
			return coup;
			break;

		case 3: // Malus jaune
			//
			coup = 1;
			return coup;
			break;

		case 2: // Bonus rouge
			//

			octet tab[] = {19,28};
			octet tempo;
			octet bonusJaune = currentPosition.evolution.bonusJ ;
			for(int i=0 ;i<2 ;i++)
			{
				if(tab[i] == bonusJaune)
					tempo = bonusJaune;
			}
			if( tempo == 28 )
			{
				currentPosition.evolution.bonusR = 22;
				coup = 22;
			}
			else if( tempo == 19)
			{
				currentPosition.evolution.bonusR = 25;
				coup = 25;
			}
			else
			{
				currentPosition.evolution.bonusR = 1;
				coup = 1;
			}
			return coup;
			break;



		case 1: // Bonus jaune
			//

			if (currentPosition.trait == JAU)
			{
				// Technique du cobra ancestral

				octet tab[] = {11,17,18,19,25,26};
				octet tempo;
				octet malusRouge = currentPosition.evolution.malusR ;
				for(int i=0 ;i<8 ;i++)
				{
					if(tab[i] == malusRouge)
						tempo = malusRouge;
				}
				if(currentPosition.evolution.malusR == tempo)
				{
					currentPosition.evolution.bonusJ = 19;
					coup = 19;
				}
				else
				{
					currentPosition.evolution.bonusJ = 28;
					coup = 28;
				}
			}


			return coup;
			break;



		default:

			return -1;
			break;
	}
}

int zonesafe(T_Position currentPosition)
{
	octet bonusJaune = currentPosition.evolution.bonusJ;
	if(currentPosition.cols[bonusJaune].couleur == ROU)
		return -1;
	else if(bonusJaune != 28 && bonusJaune != 20 && bonusJaune != 19 && bonusJaune != 27)
		return -1;
	return 1;
}

int ouverture(T_Position currentPosition, T_ListeCoups listeCoupsSoi)
{
	if (currentPosition.trait == JAU)
	{
		// Technique du cobra ancestral

		if (zonesafe(currentPosition) == -1 )
			return -1;

		if(currentPosition.evolution.bonusJ = 28)
		{
			switch(currentPosition.numCoup) 
			{
				case 5:
					return rechercheCoup(listeCoupsSoi,21,29);
					break;

				case 7: 
					if(estValide(currentPosition,29,20) == VRAI)
						return rechercheCoup(listeCoupsSoi,29,20);
					break;
				
				case 9:
					if(estValide(currentPosition,20,28) == VRAI)
						return rechercheCoup(listeCoupsSoi,20,28);
			}
				
		}
		
		else
		{
			
			switch(currentPosition.numCoup) 
			{
				case 5:
					return rechercheCoup(listeCoupsSoi,26,18);
					break;

				case 7: 
					if(estValide(currentPosition,18,27) == VRAI)
						return rechercheCoup(listeCoupsSoi,18,27);
					break;
				
				case 9:
					if(estValide(currentPosition,27,19) == VRAI)
						return rechercheCoup(listeCoupsSoi,27,19);
					break;
			}
				
		}


	}
	else if (currentPosition.trait ==  ROU)
	{
		// Technique de la mante religieuse

		if (zonesafe(currentPosition) == -1 )
			return -1;
								
		if(currentPosition.evolution.bonusR = 22)
		{
			switch(currentPosition.numCoup) 
			{
				case 6:
					if(estValide(currentPosition,22,29) == VRAI)
						return rechercheCoup(listeCoupsSoi,22,29);
					break;

				case 8: 
					if(currentPosition.cols[21].nb == 1 && currentPosition.cols[20].couleur == ROU && estValide(currentPosition,29,28) == VRAI)
						return rechercheCoup(listeCoupsSoi,29,28);
					else if(estValide(currentPosition,29,20) == VRAI)
						return rechercheCoup(listeCoupsSoi,29,20);
					break;
				
				case 10:
					if(estValide(currentPosition,20,28) == VRAI)
						return rechercheCoup(listeCoupsSoi,20,28);
			}
				
		}
		else
		{
			switch(currentPosition.numCoup) 
			{
				case 6:
					if(estValide(currentPosition,25,18) == VRAI)
						return rechercheCoup(listeCoupsSoi,25,18);
					break;

				case 8: 
					if(currentPosition.cols[26].nb == 1 && currentPosition.cols[27].couleur == ROU && estValide(currentPosition,18,19) == VRAI)
						return rechercheCoup(listeCoupsSoi,18,19);
					else if(estValide(currentPosition,29,20) == VRAI)
						return rechercheCoup(listeCoupsSoi,29,20);
					break;
				
				case 10:
					if(currentPosition.cols[19].nb == 3 && currentPosition.cols[19].couleur == ROU && estValide(currentPosition,26,35) == VRAI)
						return rechercheCoup(listeCoupsSoi,26,35);
					else if(estValide(currentPosition,20,28) == VRAI)
						return rechercheCoup(listeCoupsSoi,20,28);
			}
				
		}		
	}
	return -1;
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
	else if(currentPosition.numCoup >= 5 && currentPosition.numCoup <=10) 
	{
		result = ouverture(currentPosition,listeCoups);

		if(result == -1)
		{
			// USE MINI MAX
		}
		ecrireIndexCoup(result);
	}

	printf("Etat des bonus/malus: bj: %d, mj: %d, br: %d, mr:%d. Trait: %d\n", currentPosition.evolution.bonusJ,
		   currentPosition.evolution.malusJ, currentPosition.evolution.bonusR, currentPosition.evolution.malusR,
		   currentPosition.trait);

	ecrireIndexCoup(0);
}