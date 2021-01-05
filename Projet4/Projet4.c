#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "../Projet3/base.h"
#include "compilation.h"

int main(){
    char *Readbuffer = NULL;
    FILE *fp = fopen("test_projet4.lac", "r");
    int Flength = read_into_buffer(fp,&Readbuffer);     //read the sentence
    CQueue queRes;
    cqueue_init(&queRes);
    find_lac(Readbuffer, Readbuffer + Flength, &queRes);
    print_out(Readbuffer, &queRes);  //print out the results of lexical analyse of document.


    //establish stack
    s_donnee = cstack_init();
    s_retour = cstack_init();
    char *nom[4]={"+",".","<lit>","<fin>"};
    void *function_list[4] = {func_plus,func_point,func_lit,func_fin};
    
    //printf("-------------------LAC---------------------\n");
    //printf("position: %d, value: %d, signification: fini \n",0,0);
    for(int i=0;i<4;i++){
        declare_base(nom[i],function_list[i]);
    }
    compilation(Readbuffer,&queRes);
    fflush(stdout);
    cqueue_clear(&queRes);
    return 0;


}