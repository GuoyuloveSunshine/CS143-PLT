#ifndef BASE
#define BASE

#include "../utiles/stack.h"

#define TRUE 1
#define FALSE 0

CStack *s_donnee;
CStack *s_retour;
char *str;

//We define lac
#define longeur 500
int LAC[longeur]={0}; // symbol table
int pointer_LAC=1;

int VM[longeur]={0}; // VM table
int pointer_VM=0;

typedef void (*base)(void); // processor
base processeur[50];
int pointer_processeur=0;



void func_dup(){
    Elemtype copy = cstack_gettop(s_donnee);
    cstack_push(s_donnee,copy);
}

void func_drop(){
    Elemtype out;
    out = cstack_pop(s_donnee);
}

void func_swap(){
    Elemtype e1 = cstack_pop(s_donnee);
    Elemtype e2 = cstack_pop(s_donnee);
    cstack_push(s_donnee,e1);
    cstack_push(s_donnee,e2);
}

void func_point(){
    Elemtype e1 = cstack_pop(s_donnee);
    printf("%d\n",e1);
}

void func_count(){
    int iAddr=cstack_pop(s_donnee);
    int len = VM[iAddr];
    cstack_push(s_donnee,iAddr+1);
    cstack_push(s_donnee,len);


}
void func_type(){
    Elemtype len,iAddr;
    len = cstack_pop(s_donnee);
    iAddr = cstack_pop(s_donnee);
    for(int i=0;i<len;i++){
        printf("%c",VM[iAddr+i]);
    }
    printf(" ");
}
void func_equal(){
    Elemtype e1 = cstack_pop(s_donnee);
    Elemtype e2 = cstack_pop(s_donnee);
    if(e1==e2) cstack_push(s_donnee,1);
    else cstack_push(s_donnee,0);
}

void func_plus(){
    Elemtype e1 = cstack_pop(s_donnee);
    Elemtype e2 = cstack_pop(s_donnee);
    cstack_push(s_donnee,e2+e1);
}

void func_minus(){
    Elemtype e1 = cstack_pop(s_donnee);
    Elemtype e2 = cstack_pop(s_donnee);
    cstack_push(s_donnee,e2-e1);
}

void func_multiply(){
    Elemtype e1 = cstack_pop(s_donnee);
    Elemtype e2 = cstack_pop(s_donnee);
    cstack_push(s_donnee,e1*e2);
}


void func_lit(){
    Elemtype iAddr = cstack_pop(s_retour);
    iAddr+=1;
    cstack_push(s_donnee,VM[iAddr]);
    cstack_push(s_retour,iAddr);
}

void func_str(){ 
    Elemtype iAddr = cstack_pop(s_retour); 
    iAddr +=1;
    cstack_push(s_donnee,iAddr);
    cstack_push(s_retour,iAddr+VM[iAddr]);
}

void func_fin(){
    cstack_pop(s_retour);
}


void func_if(){
    int iCond=cstack_pop(s_donnee);
    int iAddr;
    if (iCond>0){
        iAddr=cstack_pop(s_retour);
        cstack_push(s_retour,iAddr+1);
    }
    else{
        iAddr=cstack_pop(s_retour);
        cstack_push(s_retour,VM[iAddr+1]);
    }
}

void func_else(){
    int iAddr = cstack_pop(s_retour);
    cstack_push(s_retour,VM[iAddr+1]);
}

void func_then(){
    ;
}

void func_recurse(){
    ;
}

void func_erreur(){
    printf("function pas defini\n");
}
#endif