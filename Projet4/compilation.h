#ifndef COMPILATION
#define COMPILATION

#include <string.h>
#include <stdio.h>
#include "../utiles/queue.h"
#include "../utiles/stack.h"
#include "../Projet1/analyse_lexicale.h"
#include "../Projet3/base.h"


void declare_base(char nom[],void *function_nom){
    //add to LAC
    int PBegin = pointer_LAC;
    size_t len = strlen(nom);
    LAC[pointer_LAC++] = len;
    for(int i=0;i<len;i++){
        LAC[pointer_LAC++] = nom[i];
    }
    LAC[pointer_LAC++] = pointer_VM;
    LAC[pointer_LAC++] = PBegin;
    
    //add to VM
    VM[pointer_VM++]=0;
    VM[pointer_VM++]=pointer_processeur;
    processeur[pointer_processeur++]=function_nom;

}

// void declare_base(char nom[],void *function_nom){
//     // add to LAC
//     int PBegin = pointer_LAC;  // Nfa
//     size_t len = strlen(nom);  // length
//     printf("position: %d",pointer_LAC);
//     LAC[pointer_LAC++] = len;
//     printf(", value: %ld, signification: len\n",len);
//     for(int i=0;i<len;i++){   // add chaque character
//         printf("position: %d, value: %d, signification: %c\n",pointer_LAC,nom[i],nom[i]);
//         LAC[pointer_LAC++] = nom[i];
//     }
//     printf("position: %d, value: %d, signification: Cfa \n",pointer_LAC,pointer_VM);
//     LAC[pointer_LAC++] = pointer_VM;
//     printf("position: %d, value: %d, signification: Nfa \n",pointer_LAC,PBegin);
//     LAC[pointer_LAC++] = PBegin;
    
//     // add to VM
//     VM[pointer_VM++]=0;    // sindicate base function
//     VM[pointer_VM++]=pointer_processeur;
//     // add to processor
//     processeur[pointer_processeur++]=function_nom;

// }

int find_func(char *exp){
    int pointer_now=pointer_LAC-1;
    while(pointer_now>0){
        pointer_now = LAC[pointer_now];
        if (strlen(exp)==LAC[pointer_now]){
            int tmp = pointer_now+1;
            int flag=1;
            char *p=exp;
            while(*p!='\0'){
                if(*p!=LAC[tmp]){
                    flag=0;
                    break;
                }
                tmp++;
                p++;
            }
            if (flag==1){
                int VM_index=LAC[pointer_now+LAC[pointer_now]+1];
                return VM_index;
            }
        }
        pointer_now--;
    }
    return -1;

}

void declare_LAC(char *function[],int length){
    // add to LAC
    int PBegin = pointer_LAC;
    size_t len = strlen(function[0]);  //first one is the name of function
    //printf("length: %ld\n",len);
    //printf("position: %d",pointer_LAC);
    LAC[pointer_LAC++] = len;
    //printf(", value: %ld, signification: len\n",len);
    for(int i=0;i<len;i++){
        //printf("position: %d, value: %d, signification: %c\n",pointer_LAC,function[0][i],function[0][i]);
        LAC[pointer_LAC++] = function[0][i];
    }

    //printf("position: %d, value: %d, signification: Cfa \n",pointer_LAC,pointer_VM);
    LAC[pointer_LAC++] = pointer_VM;
    //printf("position: %d, value: %d, signification: Nfa \n",pointer_LAC,PBegin);
    LAC[pointer_LAC++] = PBegin;
    
    //add to VM
    int if_next_index;
    int else_next_index;
    int recurse_index;
    recurse_index=pointer_VM;
    VM[pointer_VM++]=1;
    for(int j=1;j<length;j++){
        int func=find_func(function[j]);
        //printf("function nom: %s, find func: %d\n",function[j],func);
        VM[pointer_VM++]=func;
        if(strcmp(function[j],"<lit>")==0){  // the element behind <lit> is always a number.
            j++;
            //printf("digit, function nom: %s\n",function[j]);
            VM[pointer_VM++]=atoi(function[j]);
        }
        else if(strcmp(function[j],"<str>")==0){ // the element behind <str> is always a string.
            j++;
            //printf("string, function nom: %s\n",function[j]);
            int str_l=strlen(function[j]);
            VM[pointer_VM++]=str_l;
            for (int m=0;m<str_l;m++){
                VM[pointer_VM++]=function[j][m];
            }
        }
        else if(strcmp(function[j],"if")==0){
            if_next_index=pointer_VM++;
        }
        else if(strcmp(function[j],"else")==0){
            else_next_index=pointer_VM++;
            VM[if_next_index]=else_next_index;
        }
        else if(strcmp(function[j],"then")==0){
            VM[else_next_index]=pointer_VM-1;
        }
        else if(strcmp(function[j],"recurse")==0){
            VM[pointer_VM-1]=recurse_index;
        }
    }  
}



void execution(int vm_index){
    if (vm_index==-1){
        printf("Function not exist");
        exit(1);
    }
    if(VM[vm_index]==0){
        processeur[VM[vm_index+1]]();
    }
    else if(VM[vm_index]==1){
        cstack_push(s_retour,vm_index+1);
        execution(VM[vm_index+1]);
        while(s_retour->size!=0){
            int z = cstack_pop(s_retour);
            cstack_push(s_retour,z+1);
            execution(VM[z+1]);        
        }
    }
}


void compilation(char * psReadBuffer, CQueue * pqueRes){
    QueueNode * tmp = pqueRes->pHead;
    lexeme_t res;
    int flag = TRUE;
    while (flag) {
        res =  *(lexeme_t *)tmp->pData;
        lexeme_t function_LAC;
        char ele[50];                   // copy into a char.
        copy_mid(ele,psReadBuffer,(int)(res.lEnd-res.lStart),res.lStart);
        if (strcmp(res.type,"word")==0){
            //printf("word:%s\n",ele);
            if(strcmp(ele,"defer")==0){
                char *function[10];
                int len=0;
                tmp = tmp->pNext;
                function_LAC = *(lexeme_t *)tmp->pData;
                char mot[50];
                copy_mid(mot,psReadBuffer,(int)(function_LAC.lEnd-function_LAC.lStart),function_LAC.lStart);
                function[len]=malloc(strlen(mot)*sizeof(char));
                strcpy(function[len++],mot);
                function[len++] = "erreur";
                function[len++] = "<fin>";
                declare_LAC(function,len);
            }
            else if(strcmp(ele,"'")==0){
                char *function[10];
                int len=0;
                tmp = tmp->pNext;
                function_LAC = *(lexeme_t *)tmp->pData;
                char first[50];
                copy_mid(first,psReadBuffer,(int)(function_LAC.lEnd-function_LAC.lStart),function_LAC.lStart);
                int Cfa_first=find_func(first);

                tmp = tmp->pNext;
                function_LAC = *(lexeme_t *)tmp->pData;
                char is[50];
                copy_mid(is,psReadBuffer,(int)(function_LAC.lEnd-function_LAC.lStart),function_LAC.lStart);
                
                if (strcmp(is,"is")==0){
                    tmp = tmp->pNext;
                    function_LAC = *(lexeme_t *)tmp->pData;
                    char second[50];
                    copy_mid(second,psReadBuffer,(int)(function_LAC.lEnd-function_LAC.lStart),function_LAC.lStart);
                    int Cfa_second=find_func(second);
                    VM[Cfa_second+1]=Cfa_first;
                }
            }
            else if(strcmp(ele,":")==0){  // When we see :, we begin to define a function.
                int flag2 = TRUE;
                char *function[200];
                int len=0;
                while(flag2){
                    tmp = tmp->pNext;
                    function_LAC = *(lexeme_t *)tmp->pData;
                    char mot[50];
                    copy_mid(mot,psReadBuffer,(int)(function_LAC.lEnd-function_LAC.lStart),function_LAC.lStart);
                    if (strcmp(function_LAC.type,"number")==0){
                        function[len++] = "<lit>"; // See number add <lit> before
                        function[len]=malloc(strlen(mot)*sizeof(char));
                        strcpy(function[len++],mot);
                    }
                    else if(strcmp(function_LAC.type,"string")==0){
                        function[len++] = "<str>";
                        function[len]=malloc(strlen(mot)*sizeof(char));
                        strcpy(function[len++],mot);                     
                    }
                    else if(strcmp(function_LAC.type,"word")==0){
                        if (strcmp(mot,";")==0){
                            function[len++] = "<fin>";   //When we see ;, the function terninate.
                            flag2 = FALSE;
                        }
                        else{
                            function[len]=malloc(strlen(mot)*sizeof(char));
                            strcpy(function[len++],mot);
                        }
                    }
                }
                declare_LAC(function,len);  // add function LAC into LAC and VM

            }
            else{
                int VM_index=find_func(ele);
                execution(VM_index);
            }    
        }
        else if(strcmp(res.type,"number")==0){  // if number, push in the statistic stack
            cstack_push(s_donnee,atoi(ele));
        }
        if (tmp == tmp->pNext) {
            flag = FALSE;
        }
        else tmp = tmp->pNext;
    }
    printf("\n");
}

#endif