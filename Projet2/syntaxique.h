#ifndef SYNTAXIQUE
#define SYNTAXIQUE


#include "../utiles/stack.h"
// Définition des types de lexème
typedef enum lexeme_tsyn
{
	NOMBRE, 	// Naturel
	Par_ou,		// Parenthèse ouvrante
	Par_fe,		// Parenthèse fermante
	OP			// Opérateurs (+ ou -)
} lexeme_tsyn;

// Définition du type lexème
typedef struct lexeme
{
	lexeme_tsyn type;	// Son type
	char valeur[20];// Sa valeur, limitée à 20 caractères
} lexeme;


typedef struct llexeme 
{
    lexeme tab[50];	// Le résultat
    int N;			// Le nombre de lexèmes du tableau
} llexeme;

llexeme resultat;	// Le résultat de l'analyse lexicale

// Déclaration pour pouvoir faire la récursivité croisée
int somme(int d, int f);	// Utilisée par <facteur>

void add_divers(enum lexeme_tsyn type, char * s, int i, int * ref_j, lexeme * ref_lu);

// Définitions des fonctions de la BNF
int entier(int d)
{	
    return((resultat.tab[d].type == NOMBRE));
};


int facteur(int d, int f)
{	// On lit les lexèmes entre les indices d (début) et f (fin)
    return((entier(d) && (d == f)) || ((resultat.tab[f].type == Par_fe) && (resultat.tab[d].type == Par_ou) && (somme(d+1, f-1))));
};

int somme(int d, int f)
{	// On lit les lexèmes entre les indices d (début) et f (fin)
    int i = f-1, b = facteur(d, f);
    // Il faut chercher le bon opérateur, malheureusement, il peut être au milieu d'un facteur et provoquer ainsi une erreur de filtrage... C'est le principal défuat de cette méthode !
    while ((b==0) && (i>d))
    {
        b = (resultat.tab[i].type == OP) && facteur(i+1, f) && somme(d, i-1);
        i--;
    };
    return(b);
};

void add_nombre(char * s, int i, int j, lexeme * ref_lu)
{
    /* Input： j,s,i */
    /* Output: lu, resultat */
    if (j != 0)
    {
        ref_lu->type = NOMBRE;
        strncpy(ref_lu->valeur, s+i-j, j);
        ref_lu->valeur[j] = 0;
        resultat.tab[resultat.N] = *ref_lu;
        resultat.N += 1;
    }
}

void add_divers(enum lexeme_tsyn type, char * s, int i, int * ref_j, lexeme * ref_lu)
{
    /* Input： s,i */
    /* Output: j, lu, resultat */
    add_nombre(s, i, *ref_j, ref_lu);
    *ref_j = 0;
    ref_lu->type = type;
    strncpy(ref_lu->valeur, s+i, 1);
    ref_lu->valeur[1] = 0;
    resultat.tab[resultat.N] = * ref_lu;
    resultat.N += 1;
};

int synta(char *s){
    // Analyse lexicale en utilisant l'automate le plus simple. Il faudrait changer le switch pour le deuxième automate
	int i = 0, j = 0;	// i est l'indice de parcours de s, j est le nombre de chiffres lus
    resultat.N = 0;
    lexeme lu;	// Facile à comprendre, c'est le lexème lu !

	// On a trouvé un lexème de type NOMBRE


	// On a un autre lexème
		
	do 	// L'analyseur lexical proprement dit
		switch(s[i])
		{
			case '0': case '1': case '2': case '3': case '4': case '5': case '6': case '7': case '8': case '9': 
				j++;
				break;
            case '-': // if we find the negative number, we add dans le type number.
                if (i!=0 &&s[i-1]=='('){
                    int begin = i;
                    while(isdigit(s[i+1])){
                        j++;i++;
                        if (s[i+1]=='\0') break;
                    }
                    if(begin==i){
                        add_divers(OP, s, i, &j, & lu);
                        break;
                    }
                    else{
                        i++;
                        j+=1;
                        add_nombre(s, i, j, &lu);

                        i--;
                        j=0;
                        break;
                    }
                }
                else{
                    add_divers(OP, s, i, &j, & lu);
                    break;
                }
			case '+': case '*': case '/':
                add_divers(OP, s, i, &j, & lu);
				break;
			case '(':
                add_divers(Par_ou, s, i, &j, & lu);
                break;
			case ')':
                add_divers(Par_fe, s, i, &j, & lu);
				break;
			default: 
				printf("Erreur !");
				exit(1);
		}
	while (s[++i] != 0);
	
	// La ligne peut se terminer par un nombre qui n'a pas été encore pris en compte
    add_nombre(s, i, j, &lu);

    if (resultat.N>1 && strcmp(resultat.tab[0].valeur,"-")==0){
        printf("\n\nL'expression %s est syntaxiquement %s", s, somme(1, resultat.N-1) ? "correcte\n" : "incorrecte\n");
	    return somme(1, resultat.N-1);
    }
    else{
        printf("\n\nL'expression %s est syntaxiquement %s", s, somme(0, resultat.N-1) ? "correcte\n" : "incorrecte\n");
	    return somme(0, resultat.N-1);
    }

	
}

// convert in-fix expression to post-fix
char *exp_to_postfix(llexeme resultat){
    
    CStack *s = cstack_init();
    char e;
    char *res=malloc(100*sizeof(char));
    char *p = res;
    int len_res = 0;

    if (strcmp(resultat.tab[0].valeur,"-")==0){ // if the first element '-', we add 0 in the begin
        *(p++) = '0';
        len_res++;
        *(p++) = ' ';
        len_res++;

    }
    for(int i=0;i<resultat.N;i++){ // add number to the array
        if(resultat.tab[i].type==NOMBRE){
            int num_len=strlen(resultat.tab[i].valeur);
            for(int j=0;j<num_len;j++){
                *(p++)=resultat.tab[i].valeur[j];
                len_res++;
            }
            *(p++) = ' ';
            len_res++;
        }
        else if (strcmp(resultat.tab[i].valeur,")")==0){
            e = cstack_pop(s);
            while( '(' != e ){
                *(p++) = e;
                len_res++;
                *(p++) = ' ';
                len_res++;
                e = cstack_pop(s);
            }
        }
        else if ( strcmp(resultat.tab[i].valeur,"+")==0 || strcmp(resultat.tab[i].valeur,"-")==0){
            if ( !s->size ){ // if the stack is vide, we push the operator into stack.
                cstack_push(s,resultat.tab[i].valeur[0]);
            }
            else{
                do{ // if the stack is not vide
                    e = cstack_pop(s);
                    if ( '('==e ){ // if the first element is '(', we do nothing.
                        cstack_push(s,e);
                    }
                    else{ // otherwise, we add the element to the array
                        *(p++) = e;
                        len_res++;
                        *(p++) = ' ';
                        len_res++;
                    }
                }while( s->size && '('!=e );
                cstack_push(s,resultat.tab[i].valeur[0]);
            }
        }
        else if ( strcmp(resultat.tab[i].valeur,"*")==0 || strcmp(resultat.tab[i].valeur,"/")==0 || strcmp(resultat.tab[i].valeur,"(")==0 ){
            cstack_push(s,resultat.tab[i].valeur[0]); // push operator to the stack
        }

    }
    while( s->size){ // add the rest of operand to the array
        e = cstack_pop(s);
        *(p++) = e;
        len_res++;
        *(p++) = ' ';
        len_res++;
    }
    res[len_res] = '\0';
    return res;
}


void exp_calcul(char * psReadBuffer, CQueue * pqueRes, CStack *stack){
    QueueNode * tmp = pqueRes->pHead;
    lexeme_t res;
    int flag = FALSE;
    while (TRUE) {
        res =  *(lexeme_t *)tmp->pData;
        char ele[50];
        copy_mid(ele,psReadBuffer,(int)(res.lEnd-res.lStart),res.lStart);
        if (strcmp(res.type,"number")==0){
            cstack_push(stack, atoi(ele));
        }
        else if(strcmp(res.type,"word")==0){
            Elemtype e1 = cstack_pop(stack);
            Elemtype e2 = cstack_pop(stack);
            if(strcmp(ele,"+")==0){
                cstack_push(stack,e2+e1);
            }
            if(strcmp(ele,"-")==0){
                cstack_push(stack,e2-e1);
            }
            if(strcmp(ele,"*")==0){
                cstack_push(stack,e2*e1);
            }
            if(strcmp(ele,"/")==0){
                cstack_push(stack,e2/e1);
            }
        }
        tmp = tmp->pNext;
        if (flag == TRUE) break;
        if (tmp == tmp->pNext) {
            flag = TRUE;
        }
    }
    printf("\n");
}


void calculateur(char * psReadBuffer, CQueue * pqueRes, CStack *stack){
    QueueNode * tmp = pqueRes->pHead;
    lexeme_t res;
    int flag = FALSE;
    while (TRUE) {
        res =  *(lexeme_t *)tmp->pData;
        if (strcmp(res.type,"word")==0){
            char cal[50];
            copy_mid(cal,psReadBuffer,(int)(res.lEnd-res.lStart),res.lStart);
            if(strcmp(cal,"calculate")==0){
                QueueNode *now=tmp->pPrev;
                res=*(lexeme_t *)now->pData;
                char expr[50];
                copy_mid(expr,psReadBuffer,(int)(res.lEnd-res.lStart),res.lStart);
                if(synta(expr)){
                    char *exp_post = exp_to_postfix(resultat);
                    int len=strlen(exp_post);
                    printf("exp_post: %s\n",exp_post);
                    CQueue queNum;
                    cqueue_init(&queNum);
                    find_lac(exp_post, exp_post + len, &queNum);
                    exp_calcul(exp_post,&queNum,stack);
                }
            }
            if(strcmp(cal,".")==0){
	            while(!(cstack_isempty(stack))){
                    Elemtype resul = cstack_pop(stack);
                    printf("%d\n",resul);
                }

            }
        }
        tmp = tmp->pNext;
        if (flag == TRUE) break;
        if (tmp == tmp->pNext) {
            flag = TRUE;
        }
    }
    printf("\n");
}

#endif