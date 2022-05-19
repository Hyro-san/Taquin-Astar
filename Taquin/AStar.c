#include "AStar.h"
#include <windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <conio.h>
#include "SDL.h"
#include "SDL_image.h"

Taquin* InitialTaquin(Taquin* _pTaquin)
{
	static Taquin* taq = NULL;
	if (taq)
		return taq;


	taq = calloc(1, sizeof(Taquin));
	if (!taq)
		return NULL;

	createTaquin(taq, _pTaquin->hauteur, _pTaquin->largeur);
	initTaquin(taq);

	return taq;
}

// fonction pour cr�er (allouer) un noeud de liste et l'initialiser avec le taquin pass� en param�tre
ptrListAStar createNodeList(Taquin* pTaquin, int gValue, int fValue, deplacement d, ptrListAStar pPrevPlay)
{
	ptrListAStar node = calloc(1, sizeof(ListAStar));
	if (!node)
		return NULL;

	copyTaquin(pTaquin, &(node->pTaquin));
	if (moveTaquin(&(node->pTaquin), d)) {
		freeTaquin(&(node->pTaquin));
		free(node);
		return NULL;
	}
	node->g = gValue;
	node->f = gValue + h(&(node->pTaquin));
	node->prev_d = d;
	node->prev_node = pPrevPlay;
	return node;
}

// Insertion dans la liste de fa�on tri�e ou non
// si on passe le param�tre tri � 0, on ins�re en t�te de liste
int insertList(ptrListAStar* ppHead, ptrListAStar pNode, int tri)
{
	if (!pNode || !ppHead)
		return 1;
	if (!tri || !(*ppHead)) {
		pNode->post_node = (*ppHead);
		(*ppHead) = pNode;
		return 0;
	}

	ptrListAStar cursor = (*ppHead);

	while (cursor->post_node != NULL && pNode->f > cursor->post_node->f)
		cursor = cursor->post_node;

	pNode->post_node = cursor->post_node;
	cursor->post_node = pNode;

	return 0;
}

// Fonction pour pr�lever le noeud en t�te de liste
// Retourne le noeud pr�lev�
ptrListAStar popList(ptrListAStar* ppHead)
{
	if (!ppHead || !(*ppHead))
		return NULL;
	ptrListAStar node = (*ppHead);
	(*ppHead) = node->post_node;

	return node;
}

// fonction qui retourne le noeud dans lequel on trouve le taquin pass� en param�tre (pointeur sur le pointeur dans la liste)
ptrListAStar isInList(ptrListAStar* ppHead, Taquin* pTaquin)
{
	if (!ppHead || !(*ppHead))
		return NULL;

	if (equalTaquin(pTaquin, &((*ppHead)->pTaquin)))
		return *ppHead;

	while ((*ppHead)->post_node)
	{
		
		if (equalTaquin(pTaquin, &((*ppHead)->pTaquin)))
			return *ppHead;

		ppHead = &((*ppHead)->post_node);

	}
	return NULL;
}

// fonction pour afficher une liste
// si on met displayFGH � 0 les valeur de F, G et H ne sont pas affich�es
int displayList(ptrListAStar pHead, int displayFGH)
{
	int i = 0;
	while (pHead != NULL)
	{
		if (displayFGH)
		{
			printf("F=%d, G=%d, H=\n", pHead->f, pHead->g);
		}
		printf("count = %d, Prev deplace: %d\n", i++, pHead->prev_d);
		displayTaquin(&(pHead->pTaquin), 0);
		pHead = pHead->post_node;

	}

	return 0;
}

// Fonction pour r�soudre le taquin en utilisant l'algorithme A*
// La fonction prend comme taquin initial le taquin sp�cifi� par pTaquin
// elle remplit 
//   - pTabDeplacement (un tableau contenant les d�placements � effectuer pour r�soudre le taquin)
//   - pNbDeplacements
//   - pNbTaquinsGeneres
//   - pTimeElapsed
// Si stepByStep est diff�rent de 0 on affiche dans la console toutes les �tapes de la r�solution
// pWindow
int solveTaquin(Taquin* pTaquin, deplacement** pTabDeplacement, unsigned long* pNbDeplacements, unsigned long* pNbTaquinsGeneres, unsigned long* pTimeElapsed, int stepByStep, SDL_Surface* pWindow)
{
	ptrListAStar closed= NULL, parent = NULL;
	ptrListAStar open = createNodeList(pTaquin, 0, h(pTaquin), AUCUN, NULL);
	ptrListAStar cursor = NULL;
	ptrListAStar cursorchild = NULL;
	ptrListAStar compare = NULL;
	int g = 0, nbcount = 0;
	int end = 0;
	int parcouredNoded = 0;
	while (!end) 
	{
		cursor = open;
		open = open->post_node;
		cursor->post_node = NULL;

		insertList(&closed, cursor, 0);

		for (int i = 0; i < 4; i++)
		{
			cursorchild = createNodeList(&(cursor->pTaquin), cursor->g + 1, 0, i + 1 , cursor);
			if (cursorchild)
			{
				if (equalTaquin(&(cursorchild->pTaquin), InitialTaquin(&(cursor->pTaquin))))
				{
					copyTaquin(&(cursorchild->pTaquin), pTaquin);

					printf("PRIVATE %d\n", parcouredNoded);
					displayTaquin(&(cursorchild->pTaquin), 0);
					int a = 0;
					parent = closed;
					while (parent) {
						++a;
						printf("Parent : %d", parent->g);
						displayTaquin(&(parent->pTaquin), 0);
						parent = parent->prev_node;
					}
					printf("%d", a);
					printf("g = %d, NodesPar=%d\nChemin : \n", g, parcouredNoded);
					freeList(&cursorchild);
					end = 1;
					break;
				}

				if (cursor->prev_node && equalTaquin(&(cursorchild->pTaquin), &(cursor->prev_node->pTaquin))) {
					freeList(&cursorchild);
					//cursor->post_node -> calculer ses enfants, enlever le mouvement nul et prendre celui qui est le meilleur parmi les restant si il y en a  
					continue;
				}
				
				compare = isInList(&closed, &(cursorchild->pTaquin));
				if (compare)
				{
					if (compare->f > cursorchild->f) 
					{
						compare->f = cursorchild->f;
						compare->g = cursorchild->g;
						compare->prev_node = cursorchild->prev_node;
					}
					freeList(&cursorchild);
					continue; //on abandonne l'enfant
				}

				insertList(&open, cursorchild, 1);
				parcouredNoded++;
			}
		}
	}
	freeList(&open);
	freeList(&closed);

	return 1;
}

// fonction d'�valuation pour la r�solution avec AStar
int h(Taquin* pTaquin)
{
	int tot = 0, index = 0;;
	for (int x = 0; x < pTaquin->hauteur; ++x)
		for (int y = 0; y < pTaquin->largeur; ++y) {
			tot += 1*(index == pTaquin->plateau[x][y]) + abs(x - pTaquin->plateau[x][y] % pTaquin->largeur) + abs(y - pTaquin->plateau[x][y] / pTaquin->largeur);
			++index;
		}
	return tot;
}

void freeList(ptrListAStar* ppHead)
{
	//ptrListAStar* iterator = ppHead;
	ptrListAStar tmp = NULL;

	while (*ppHead)
	{
		freeTaquin(&((*ppHead)->pTaquin));
		tmp = (*ppHead)->post_node;
		free((*ppHead));
		(*ppHead) = tmp;
	}
	
	return;
}