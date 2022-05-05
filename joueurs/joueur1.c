// TODO: Utiliser les optimisation à la compilation (-O3) et la tester
#include "../include/avalam.h"
#include "../include/moteur.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

// Utile pour simuler un coup
// void copierPlateau(T_Position currentPosition, T_Position currentPositionCopy)
// {
// 	currentPositionCopy.trait	= currentPosition.trait;
// 	currentPositionCopy.numCoup = currentPosition.numCoup;
// 	for(int i = 0; i < NBCASES; i++) { // TODO: Fix the copy, will maybe fix SPD
// 		currentPositionCopy.cols[i].nb		= currentPosition.cols[i].nb;
// 		currentPositionCopy.cols[i].couleur = currentPosition.cols[i].couleur;
// 	}
// 	currentPositionCopy.evolution.bonusJ = currentPosition.evolution.bonusJ;
// 	currentPositionCopy.evolution.bonusR = currentPosition.evolution.bonusR;
// 	currentPositionCopy.evolution.malusJ = currentPosition.evolution.malusJ;
// 	currentPositionCopy.evolution.malusR = currentPosition.evolution.malusR;
// 	return;
// }

// Recher l'index d'un coup avec un case origine et une case destiantion donnée
int rechercheCoup(T_ListeCoups listeCoups, octet origine, octet destination)
{
	printf("RC: o:%d d:%d\n", origine, destination);
	int moy	  = 0;
	int debut = 0;
	int size  = 8 * NBCASES;
	while(debut <= size) {
		moy = (debut + size) / 2;
		if(listeCoups.coups[moy].origine == 0 && moy != 0) {
			size = moy - 1;
		}
		else if(listeCoups.coups[moy].origine == origine) { //If origine in tab = origine wanted
			if(listeCoups.coups[moy].destination > destination) { //If the value of destination in tab > destination wanted
				while(listeCoups.coups[moy].origine == origine && listeCoups.coups[moy].destination > destination)
					moy--; //We go up the array to find the right destination (because the array is in sort ascending)
			}
			else if(listeCoups.coups[moy].destination < destination) { //If the value of destination in array < destination wanted
				while(listeCoups.coups[moy].origine == origine && listeCoups.coups[moy].destination < destination)
					moy++; //We go down the array to find the right destinaion
			}
			if(listeCoups.coups[moy].origine == origine && listeCoups.coups[moy].destination == destination) {
				//If the origine and destination are the one wanted
				return moy; //We return the index of the cell in the table
			}
			return -1; //Else return -1
		}
		//Dichotomous search
		else if(listeCoups.coups[moy].origine < origine) {
			debut = moy + 1;
		}
		else {
			size = moy - 1;
		}
	}
	return -1;
}

int placerBonus(T_Position currentPosition, T_ListeCoups listeCoups)
// TODO: Adapater technique de la mante religieuse, pas adaptée avec l'ordre de placement des bonus
{
	int	  coup;
	octet tempo;
	switch(currentPosition.numCoup) {
		case 3:; // Malus rouge
			coup = rechercheCoup(listeCoups, 2, 2);
			return coup;
			break;

		case 2:; // Malus jaune
			coup = rechercheCoup(listeCoups, 1, 1);
			return coup;
			break;

		case 1:; // Bonus rouge
			octet bonusJaune = currentPosition.evolution.bonusJ;
			for(int i = 0; i < 2; i++) {
				if(19 == bonusJaune || 28 == bonusJaune)
					tempo = bonusJaune;
			}
			if(tempo == 28) {
				coup = 22;
			}
			else if(tempo == 19) {
				coup = rechercheCoup(listeCoups, 25, 25);
			}
			else {
				coup = rechercheCoup(listeCoups, 1, 1);
			}
			return coup;
			break;

		case 0:; // Bonus jaune
			if(currentPosition.trait == JAU) {
				// Technique du cobra ancestral

				coup = rechercheCoup(listeCoups, 19, 19);
			}
			return coup;
			break;

		default:;
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
	if(zonesafe(currentPosition) == -1) {
		printf("Erreur zonesafe == -1 (?)\n");
		return -1;
	}

	if(currentPosition.trait == JAU) {
		// Technique du cobra ancestral

		if(currentPosition.evolution.bonusJ == 28) {
			switch(currentPosition.numCoup) {
				case 4:;
					return rechercheCoup(listeCoupsSoi, 21, 29);
					break;

				case 6:;
					return rechercheCoup(listeCoupsSoi, 29, 20);
					break;

				case 8:;
					return rechercheCoup(listeCoupsSoi, 20, 28);
					break;
				default:;
					break;
			}
		}

		else {
			switch(currentPosition.numCoup) {
				case 4:
					return rechercheCoup(listeCoupsSoi, 26, 18);
					break;

				case 6:;
					return rechercheCoup(listeCoupsSoi, 18, 27);
					break;

				case 8:;
					// TODO: C'est un coup inutile, car c'est la seule possibilité de l'ilot, à faire en fin de partie
					return rechercheCoup(listeCoupsSoi, 27, 19);
					break;
				default:
					break;
			}
		}
	}
	else if(currentPosition.trait == ROU) {
		// Technique de la mante religieuse

		if(currentPosition.evolution.bonusR == 22) {
			switch(currentPosition.numCoup) {
				case 5:;
					return rechercheCoup(listeCoupsSoi, 22, 29);
					break;

				case 7:;
					if(currentPosition.cols[21].nb == 1 && currentPosition.cols[20].couleur == ROU &&
					   estValide(currentPosition, 29, 28) == VRAI)
						return rechercheCoup(listeCoupsSoi, 29, 28);
					return rechercheCoup(listeCoupsSoi, 29, 20); // else implicite
					break;

				case 9:;
					return rechercheCoup(listeCoupsSoi, 20, 28);
					break;
				default:;
					break;
			}
		}
		else {
			switch(currentPosition.numCoup) {
				case 5:;
					return rechercheCoup(listeCoupsSoi, 25, 18);
					break;

				case 7:;
					if(currentPosition.cols[26].nb == 1 && currentPosition.cols[27].couleur == ROU &&
					   estValide(currentPosition, 18, 19) == VRAI)
						return rechercheCoup(listeCoupsSoi, 18, 19);
					return rechercheCoup(listeCoupsSoi, 29, 20); // else implicite
					break;

				case 9:;
					if(currentPosition.cols[19].nb == 3 && currentPosition.cols[19].couleur == ROU &&
					   estValide(currentPosition, 26, 35) == VRAI)
						return rechercheCoup(listeCoupsSoi, 26, 35);
					return rechercheCoup(listeCoupsSoi, 20, 28); // else implicite
					break;
				default:;
					break;
			}
		}
	}
	return -1;
}

// Valeur du plateau relativement à l'adversaire (le plus grand est le mieux), possiblement négatif -> avantage à l'adversaire
float evaluerScorePlateau(T_Position currentPosition)
{
	// TODO: SPD bugger a corriger; il est toujours positif
	float evaluation = 0;

	// Liste des paramètres
	int score_soi, score_adv, score5_soi, score5_adv;
	int score_soi_coeff	 = 1;
	int score_adv_coeff	 = 1;
	int score5_soi_coeff = 1000;
	int score5_adv_coeff = 1000;

	// On évalue le score
	T_Score score = evaluerScore(currentPosition);
	if(JAU == currentPosition.trait) {
		score_soi  = (int) score.nbJ;
		score_adv  = (int) score.nbR;
		score5_soi = (int) score.nbJ5;
		score5_adv = (int) score.nbR5;
	}
	else {
		score_soi  = (int) score.nbR;
		score_adv  = (int) score.nbJ;
		score5_soi = (int) score.nbR5;
		score5_adv = (int) score.nbJ5;
	}

	// TODO: déterminer coeff

	evaluation =
		score_soi * score_soi_coeff - score_adv * score_adv_coeff + score5_soi * score5_soi_coeff - score5_adv * score5_adv_coeff;

	printf(" soi:%d, soi5:%d, adv:%d, adv5:%d ", score_soi, score5_soi, score_adv, score5_adv);

	return evaluation;
}

float evaluerScoreCoup(T_Position currentPosition, int origine, int destination)
{
	float	  evaluation		= 0;
	octet	  traitPerso		= currentPosition.trait;
	T_Voisins voisinOrigine		= getVoisins(origine);
	T_Voisins voisinDestination = getVoisins(destination);
	int		  nb_evaluations	= 0; // Nombre de fois où l'évalaution a été modifée, pour le debug

	// Si la tour que l'on déplace est de la couleur adverse
	if(currentPosition.cols[origine].couleur != traitPerso) {
		// Tour adverse sur soi	-100
		if(currentPosition.cols[destination].couleur == traitPerso) {
			nb_evaluations++;
			evaluation = evaluation - 98;
			printf("a");
		}

		// Tour 5 adverse	-100
		if((currentPosition.cols[origine].nb + currentPosition.cols[destination].nb) == 5) {
			nb_evaluations++;
			evaluation = evaluation - 100;
			printf("z");
		}

		// isoler pion adverse - 100
		if(voisinDestination.nb - 1 == 0) {
			nb_evaluations++;
			evaluation = evaluation - 99;
			printf("e");
		}

		// Tour adverse à moins de voisin à la destination (donc en train d'être isolé)
		if(voisinOrigine.nb >
		   voisinDestination.nb - 1) { // TODO: raisonnement faux: 0 et 2 ne sont pas voisins, il ne faudrait donc pas faire -1
			nb_evaluations++;
			evaluation = evaluation - 50; // Score négatif car on isole une tour adverse // TODO: déterminer score
			printf("r");
		}

		// averse sur adverse	68
		if(currentPosition.cols[destination].couleur != traitPerso) {
			nb_evaluations++;
			evaluation = evaluation + 50;
			printf("t");
		}

		// Un bonus pour l'adversaire est en jeu
		if(origine == currentPosition.evolution.bonusJ || origine == currentPosition.evolution.bonusR ||
		   destination == currentPosition.evolution.bonusJ || destination == currentPosition.evolution.bonusR) {
			nb_evaluations++;
			evaluation = evaluation - 20;
			printf("f");
		}

		// Un malus contre l'adversaire est en jeu
		if(origine == currentPosition.evolution.malusJ || origine == currentPosition.evolution.malusR ||
		   destination == currentPosition.evolution.malusJ || destination == currentPosition.evolution.malusR) {
			nb_evaluations++;
			evaluation = evaluation + 20;
			printf("g");
		}
	}

	// Si la tour que l'on déplace est de notre couleur
	else {
		// soi sur adverse	58
		if(currentPosition.cols[destination].couleur != traitPerso) {
			nb_evaluations++;
			evaluation = evaluation + 50;
			printf("y");
		}
		else { // Soi sur soi
			nb_evaluations++;
			evaluation = evaluation - 30;
			printf("u");
		}

		// Tour alliée à moins de voisin à la destination (donc en train d'être isolé)
		if(voisinOrigine.nb >
		   voisinDestination.nb - 1) { // TODO: raisonnement faux: 0 et 2 ne sont pas voisins, il ne faudrait donc pas faire -1
			nb_evaluations++;
			evaluation = evaluation + 50; // Score positif car on isole une tour alliée // TODO: détermienr score
			printf("i");
		}

		// Tour de 5 sur son propre pion	90
		if((currentPosition.cols[origine].nb + currentPosition.cols[destination].nb) == 5 &&
		   currentPosition.cols[destination].couleur == traitPerso) {
			nb_evaluations++;
			evaluation = evaluation + 99;
			printf("o");
		}

		// Tour de 4 si aucune tour de 1 à côté de destination //?
		if((currentPosition.cols[origine].nb + currentPosition.cols[destination].nb) == 4 &&
		   currentPosition.cols[origine].couleur == traitPerso) {
			int i = 0;
			for(i = 0; i < voisinDestination.nb; i++) {
				if(currentPosition.cols[voisinDestination.cases[i]].nb == 1 && voisinDestination.cases[i] != origine) {
					break;
				}
			}
			if(currentPosition.cols[voisinDestination.cases[i]].nb != 1) {
				nb_evaluations++;
				evaluation = evaluation + 80;
				printf("p");
			}
		}

		// Isole tour alliée
		if(voisinDestination.nb - 1 == 0) {
			nb_evaluations++;
			evaluation = evaluation + 98; // peut-etre 100 car tour pleine  mais pas forcement de 5; // TODO: déterminer score
			printf("q");
		}

		// Tour 5 sur pion adverse	100
		if((currentPosition.cols[origine].nb + currentPosition.cols[destination].nb) == 5 &&
		   currentPosition.cols[destination].couleur != traitPerso) {
			nb_evaluations++;
			evaluation = evaluation + 100;
			printf("s");
		}

		// Un bonus en notre faveur est en jeu
		if(origine == currentPosition.evolution.bonusJ || origine == currentPosition.evolution.bonusR ||
		   destination == currentPosition.evolution.bonusJ || destination == currentPosition.evolution.bonusR) {
			nb_evaluations++;
			evaluation = evaluation + 20;
			printf("f");
		}

		// Un malus contre nous est en jeu
		if(origine == currentPosition.evolution.malusJ || origine == currentPosition.evolution.malusR ||
		   destination == currentPosition.evolution.malusJ || destination == currentPosition.evolution.malusR) {
			nb_evaluations++;
			evaluation = evaluation - 20;
			printf("g");
		}
	}

	// Tour de 3 si tour de 2 à côté de destination	-90
	if((currentPosition.cols[origine].nb + currentPosition.cols[destination].nb == 3)) {
		for(int i = 0; i < voisinDestination.nb; i++) {
			if(currentPosition.cols[voisinDestination.cases[i]].nb == 2 && voisinDestination.cases[i] != origine) {
				nb_evaluations++;
				evaluation = evaluation - 90;
				printf("d");
				break;
			}
		}
	}

	// Tour de 4 si tour de 1 à côté de destination	-90
	if((currentPosition.cols[origine].nb + currentPosition.cols[destination].nb == 4)) {
		for(int i = 0; i < voisinDestination.nb; i++) {
			if(currentPosition.cols[voisinDestination.cases[i]].nb == 1 && voisinDestination.cases[i] != origine) {
				nb_evaluations++;
				evaluation = evaluation - 90;
				printf("f");
				break;
			}
		}
	}

	// Isoler 4 voisins	50
	// contre isoler 3 voisins	55
	// Isoler 3 voisins	60
	// contre isoler 2 voisins	65
	// Isoler 2 voisins	70
	// contre isoler 1 voisin	75
	// Isoler 1 voisin	80

	// T_Voisins voisins = getVoisins(destination);
	// for(int i = 0; i < voisins.nb; i++) {
	// 	if(voisins.case[i] == 0 ||
	// 	   voisins.case[i] == origine) // Si le voisin est une case vide ou que c'est notre place de départ, on ignore
	// 	{
	// 		break;
	// 	}
	// }

	// TODO: ajouter gestion basique des ilots
	printf(" EC: nb:%d sc:%.0f", nb_evaluations, evaluation);
	return evaluation;
}

float evaluerScoreGen(T_Position currentPosition, T_Position nextPosition, int origine, int destination)
{
	float currentPositionScore = evaluerScorePlateau(currentPosition);
	float nextPositionScore	   = evaluerScorePlateau(nextPosition);
	float scorePlateau		   = fabs(currentPositionScore - nextPositionScore);
	// if (currentPositionScore > nextPositionScore) // Coup avantageux // Le score de score plateu est déjà positif
	// {

	// }
	if(currentPositionScore > nextPositionScore) { // Coup désavantageux // On le multiplie par -1 car on veut décourager ce coup
		scorePlateau = -1 * scorePlateau;
	}

	// On le ramène au num coup
	// scorePlateau = scorePlateau + 2* currentPosition.numCoup;

	// float scorePlateau = (evaluerScorePlateau(emptyPosition) - evaluerScorePlateau(currentPosition)) * 1; // TODO: Fix
	// // (currentPosition.numCoup / 3); // Score plateau final - initial. (gain net de points) Si positif, avantageux pour nous.
	// On
	// // tente de corrgier la diminution progressive des scores
	// printf(" SP1:%.0f SP2:%.0f", evaluerScorePlateau(currentPosition), evaluerScorePlateau(emptyPosition));
	printf(" | SPD: %.0f | ", scorePlateau);
	return scorePlateau + evaluerScoreCoup(currentPosition, origine, destination); // TODO: déterminer valeur
}

void choisirCoup(T_Position currentPosition, T_ListeCoups listeCoups)
{
	printf("\033[0;31m\n"); // On passe la couleur de la sortie interne du bot en rouge pour plus de lisibilité
	int		   result;
	float	   max_temp = 0;
	float	   score_temp;
	T_Position tempPlateau;

	// Pour le debug: Affiche tous les coups possibles
	for(int i = 0; i < listeCoups.nb; i++) {
		tempPlateau = jouerCoup(currentPosition, listeCoups.coups[i].origine, listeCoups.coups[i].destination);
		// On inverse le trait pour comparer les même résultats
		if(tempPlateau.trait == JAU)
			tempPlateau.trait = ROU;
		else
			tempPlateau.trait = JAU;

		printf("o:%d d:%d n:%d", listeCoups.coups[i].origine, listeCoups.coups[i].destination, i);
		score_temp = evaluerScoreGen(currentPosition, tempPlateau, listeCoups.coups[i].origine, listeCoups.coups[i].destination);
		printf(" | SCG: %.0f", score_temp);
		if(score_temp > max_temp) {
			result	 = i;
			max_temp = score_temp;
			printf(" (CF)");
		}
		printf("\n");
		// evaluerScoreCoup(currentPosition, listeCoups.coups[i].origine, listeCoups.coups[i].destination);
	}

	// Gestion des bonus/malus:
	if(4 > currentPosition.numCoup) // (bj, br, mj, mr)
	{
		result = placerBonus(currentPosition, listeCoups);

		if(result == -1)
			printf("IMPOSSIBLE DE PLACER LE BONUS/MALUS: -1 (numCoup: %d)\n", currentPosition.numCoup);
		printf("On place le le bonus, coup %d (o:%d, d:%d)\n", result, listeCoups.coups[result].origine,
			   listeCoups.coups[result].destination);
		ecrireIndexCoup(result);
		printf("\033[0m\n");
		return; // C'est la fin du programme pour cette action, évite un else pour les ouvertures
	}

	// Gestion des ouvertures
	if(currentPosition.numCoup < 10) // Valeur de fin des ouvertures à déterminer
	{
		result = ouverture(currentPosition, listeCoups);
		if(result == -1)
			printf("IMPOSSIBLE DE PLACER L'OUVERTURE: -1 (numCoup: %d)\n", currentPosition.numCoup);
		printf("On place une ouverture, coup %d (o:%d, d:%d)\n", result, listeCoups.coups[result].origine,
			   listeCoups.coups[result].destination);
		ecrireIndexCoup(result);
		if(result != -1) {
			printf("\033[0m\n");
			return; // C'est la fin du programme pour cette action, évite un else pour la partie jeu
		}
		printf("Coup non satisfaisant, situation possiblement problématique; tentative de placememnt de coup classique");
	}

	// Partie de jeu
	// result = 0;

	if(result == -1)
		printf("IMPOSSIBLE DE PLACER LE COUP: -1 (numCoup: %d)\n", currentPosition.numCoup);
	printf("On place le le coup %d (o:%d, d:%d)\n", result, listeCoups.coups[result].origine,
		   listeCoups.coups[result].destination);
	ecrireIndexCoup(result);

	printf("Etat des bonus/malus: bj: %d, mj: %d, br: %d, mr:%d. Trait: %d\n", currentPosition.evolution.bonusJ,
		   currentPosition.evolution.malusJ, currentPosition.evolution.bonusR, currentPosition.evolution.malusR,
		   currentPosition.trait);

	printf("\033[0m\n"); // On repasse en police par déaut pour les messages de gestion
	return;
}