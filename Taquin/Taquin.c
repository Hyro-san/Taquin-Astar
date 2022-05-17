// Inclusion des biblioth�ques utiles
#include "taquin.h"
#include "AStar.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>
#include <assert.h>



// Fonction pour copier un plateau de taquin pSrc vers pDest
int copyTaquin(Taquin * _pSrc, Taquin * _pDest)
{
	// TODO: copyTaquin en cours...
	memcpy(pDest, pSrc, sizeof(Taquin));

	_pDest->plateau = (unsigned char**)calloc(_pDest->largeur, sizeof(unsigned char*));
	if(!_pDest->plateau)
		return 1;

	for (int i = 0; i < _pDest->largeur; i++)
	{
		(_pDest->plateau)[i] = (unsigned char*)calloc(_pDest->hauteur, sizeof(unsigned char));
		if (!(_pDest->plateau)[i])
			return 1;
	}

	return 0;
}

// fonction qui renvoie 1 si les 2 taquins sont identiques
// 0 sinon
// -1 si il y a une erreur au passage des param�tres
int equalTaquin(Taquin * _pTaquin1, Taquin * _pTaquin2)
{
	if (_pTaquin1->hauteur != _pTaquin2->hauteur || _pTaquin1->largeur != _pTaquin2->largeur)
		return -1;

	for (int x = 0; x < _pTaquin1->largeur; x++)
		for (int y = 0; y < _pTaquin1->hauteur; y++)
			if (_pTaquin1->plateau[x][y] != _pTaquin1->plateau[x][y])
				return -1;

	return 0;
}

// Fonction qui cr�e un plateau de taquin 
// ATTENTION le plateau doit �tre NULL avant l'appel � cette fonction 
// Si le plateau n'est pas NULL la fonction essayera de lib�rer la zone m�moire occup�e par le plateau et cela peut donc aboutir � un crash si le plateau n'a pas �t� initialis�
int createTaquin(Taquin * _pTaquin, unsigned char _hauteur, unsigned char _largeur)
{
	// Test pour v�rifier que les donn�es pass�es ne sont pas corrompues
	if(!pTaquin) return 0;
	
	// Tests pour v�rifier les param�tres de taille de taquin
	if(_hauteur<SIZE_MINI) _hauteur = SIZE_MINI;
	if(_hauteur>SIZE_MAXI) _hauteur = SIZE_MAXI;
	if(_largeur<SIZE_MINI) _largeur = SIZE_MINI;
	if(_largeur>SIZE_MAXI) _largeur = SIZE_MAXI;
	
	{
		int i;

		// On v�rifie que le plateau n'existe pas
		// S'il existe on lib�re la m�moire avant de recr�er le plateau
		freeTaquin(pTaquin);
		pTaquin = calloc(1, sizeof(Taquin));
		if (!pTaquin)
			return 1;

	_pTaquin->hauteur = _hauteur;
	_pTaquin->largeur = _largeur;

	// on alloue la zone m�moire pour stocker les adresses des lignes du tableau
	_pTaquin->plateau = (unsigned char**) calloc(_hauteur, sizeof(unsigned char*));
		
	// si on a pas r�ussi � allouer la zone m�moire on retourne 0
	if(!_pTaquin->plateau) return 0;

	for(int i=0; i < _hauteur; i++)
	{
		// On alloue la zone m�moire pour contenir la ligne i
		_pTaquin->plateau[i] = (unsigned char*) malloc(sizeof(unsigned char)*_largeur);
		// S'il y a eu un souci � l'allocation on lib�re tout ce qui a d�j�� �t� allou� et on retourne 0
		if(!_pTaquin->plateau[i])
		{
			freeTaquin(_pTaquin);
			return 1;
		}
	}
	// On initialise le taquin
	if(!initTaquin(_pTaquin)) return 0;

	return 0;
}


// Fonction qui initialise le taquin
int initTaquin(Taquin * _pTaquin)
{
	for (int x = 0; x < _pTaquin->largeur; x++)
		for (int y = 0; y < _pTaquin->hauteur; y++)
			_pTaquin->plateau[x][y] = x + y * _pTaquin->hauteur;


	return 1;
}

// Fonction qui m�lange le taquin en effectuant entre minRandom et maxRandom coups al�atoires
int mixTaquin(Taquin * _pTaquin, int _minRandom, int _maxRandom)
{
	// TODO: mixTaquin

	return 1;
}

// Fonction qui permet de bouger une pi�ce du taquin (en bougeant la case vide)
int moveTaquin(Taquin * _pTaquin, deplacement _d)
{
	// TODO: moveTaquin

	return 1;
}

// Fonction qui renvoie le d�placement � effectuer pour annuler le d�placement donn� en param�tre
deplacement cancelMove(deplacement _d)
{
	// TODO: cancelMove

	return AUCUN;
}

// Fonction qui renvoie 1 si le taquin es r�solu, 0 sinon
int endTaquin(Taquin * _pTaquin)
{
	// TODO: endTaquin

	return 0;
}

// fonction d'affichage du taquin
int displayTaquin(Taquin * _pTaquin, int _offset)
{
	// TODO: displayTaquin 

	return 1;
}

// Fonction pour lib�rer les zones m�moires occup�es par un taquin
int freeTaquin(Taquin * _pTaquin)
{
	if (_pTaquin->plateau)
	{
		// On lib�re le plateau ligne par ligne
		for (int i = 0; i < _pTaquin->hauteur; i++) if (_pTaquin->plateau[i]) free(_pTaquin->plateau[i]);
		// On lib�re le tableau qui stockait les lignes
		free(_pTaquin->plateau);
		_pTaquin->plateau = NULL;
	}
	free(_pTaquin);
	_pTaquin = NULL;
	return 1;
}
// Boucle de jeu 
int gameLoop(int hauteur,int largeur,int minRandom, int maxRandom)
{

	// BOUCLE DE JEU ! A DEVELOPPER
	while(!_kbhit())
	{
		printf(".");
	}
	return 1;
}

