#include <stdio.h>	// Pour avoir les fonctions d'entrée/sortie standardes
#include <stdlib.h>	// Pour avoir la fonction exit
#include <string.h>	// Pour avoir la fonction strncpy
#include "../Projet1/analyse_lexicale.h"
#include "../utiles/queue.h"
#include "../utiles/stack.h"
#include "syntaxique.h"

int main()
{	
	char *Readbuffer = NULL;
    FILE *fp = fopen("test_projet2.lac", "r");
    int Flength = read_into_buffer(fp,&Readbuffer);

    CQueue queRes;
    cqueue_init(&queRes);
    find_lac(Readbuffer, Readbuffer + Flength, &queRes);
    print_out(Readbuffer, &queRes);
	CStack *S = cstack_init();
    calculateur(Readbuffer,&queRes,S);
    cstack_clear(S);
	fflush(stdout);
    cqueue_clear(&queRes);
	
	
	// // s est la chaîne à calculer, par exemple "(1-2)+(3-4)"
	// char s[50];
	// printf("Entrez l'expression à calculer : ");
	// scanf("%49[^\n]s", s);	// On saisit tous les caractères, et on protège la saisie...
	
	
	return(0);
}