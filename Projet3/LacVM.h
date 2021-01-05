#ifndef LACVM
#define LACVM

#include <string.h>
#include <stdio.h>

#include "../Projet1/analyse_lexicale.h"
#include "../utiles/queue.h"
#include "../utiles/stack.h"
#include "base.h"




void declare_base(char nom[],void *function_nom){
    // add to LAC
    int PBegin = pointer_LAC;  // Nfa
    size_t len = strlen(nom);  // length
    printf("position: %d",pointer_LAC);
    LAC[pointer_LAC++] = len;
    printf(", value: %ld, signification: len\n",len);
    for(int i=0;i<len;i++){   // add chaque character
        printf("position: %d, value: %d, signification: %c\n",pointer_LAC,nom[i],nom[i]);
        LAC[pointer_LAC++] = nom[i];
    }
    printf("position: %d, value: %d, signification: Cfa \n",pointer_LAC,pointer_VM);
    LAC[pointer_LAC++] = pointer_VM;
    printf("position: %d, value: %d, signification: Nfa \n",pointer_LAC,PBegin);
    LAC[pointer_LAC++] = PBegin;
    
    // add to VM
    VM[pointer_VM++]=0;    // sindicate base function
    VM[pointer_VM++]=pointer_processeur;
    // add to processor
    processeur[pointer_processeur++]=function_nom;

}


int find_func(char *exp){
    int pointer_now=pointer_LAC-1;  // every index of position Nfa
    while(pointer_now>0){   // while not the first one
        pointer_now = LAC[pointer_now];
        if (strlen(exp)==LAC[pointer_now]){ // compare the string length
            int tmp = pointer_now+1;
            int flag=1;
            char *p=exp;
            while(*p!='\0'){  // compare each character
                if(*p!=LAC[tmp]){
                    flag=0;
                    break;
                }
                tmp++;
                p++;
            }
            if (flag==1){  // successfully find, return the Cfa
                int VM_value=LAC[pointer_now+LAC[pointer_now]+1];
                return VM_value;
            }
        }
        pointer_now--;
    }
    return -1;
}

void execution(char * psReadBuffer, CQueue * pqueRes, CStack *stack){
    QueueNode * tmp = pqueRes->pHead;
    lexeme_t res;
    int flag = FALSE;
    while (TRUE) {
        res =  *(lexeme_t *)tmp->pData;
        char ele[50]; // copy into a char.
        copy_mid(ele,psReadBuffer,(int)(res.lEnd-res.lStart),res.lStart);

        if (strcmp(res.type,"number")==0){  // if number, push in the statistic stack
            cstack_push(stack,atoi(ele));
        }
        else if(strcmp(res.type,"word")==0){ //if word, find if it exists.
            int VM_value=find_func(ele);
            if(VM_value==-1) {  // not exist, exit.
                printf("Function not exist\n");
                exit(1);
            }
            else{
                if(VM[VM_value]==0){    // exist, execute.
                    processeur[VM[VM_value+1]]();
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