#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "../Projet3/base.h"
#include "../Projet4/compilation.h"

int main(){
    
    //establish pile
    s_donnee = cstack_init();
    s_retour = cstack_init();
    //int base_len = 15;
    char *nom[18] = {"dup","drop","swap","count",".","type","=","+",
                        "*","-","<lit>","<str>","<fin>","if","else","then","recurse","erreur"};
    void *function_list[18] = {func_dup,func_drop,func_swap,func_count,func_point,func_type,func_equal,func_plus,
                                func_multiply,func_minus,func_lit,func_str,func_fin,func_if,func_else,func_then,func_recurse,func_erreur};

    for(int i=0;i<18;i++){
        declare_base(nom[i],function_list[i]);
    }

    //read the sentence
    char *Readbuffer = NULL;
    CQueue queRes;

    printf("le document: test1.lac----------------------------------------------\n");
    FILE *fp = fopen("test1.lac", "r");
    int Flength = read_into_buffer(fp,&Readbuffer);
    cqueue_init(&queRes);
    find_lac(Readbuffer, Readbuffer + Flength, &queRes);
    print_out(Readbuffer, &queRes);
    compilation(Readbuffer,&queRes);
    fflush(stdout);
    cqueue_clear(&queRes);

    printf("le document: test2.lac-----------------------------------------------\n");
    fp = fopen("test2.lac", "r");
    Flength = read_into_buffer(fp,&Readbuffer);
    cqueue_init(&queRes);
    find_lac(Readbuffer, Readbuffer + Flength, &queRes);
    print_out(Readbuffer, &queRes);
    compilation(Readbuffer,&queRes);
    fflush(stdout);
    cqueue_clear(&queRes);
    printf("\n");
    
    printf("le document: factorielle.lac------------------------------------------\n");
    fp = fopen("factorielle.lac", "r");
    Flength = read_into_buffer(fp,&Readbuffer);
    cqueue_init(&queRes);
    find_lac(Readbuffer, Readbuffer + Flength, &queRes);
    print_out(Readbuffer, &queRes);
    compilation(Readbuffer,&queRes);
    fflush(stdout);
    cqueue_clear(&queRes);
    
    printf("le document: function_u.lac-------------------------------------------\n");
    fp = fopen("function_u.lac", "r");
    Flength = read_into_buffer(fp,&Readbuffer);
    cqueue_init(&queRes);
    find_lac(Readbuffer, Readbuffer + Flength, &queRes);
    print_out(Readbuffer, &queRes);
    compilation(Readbuffer,&queRes);
    fflush(stdout);
    cqueue_clear(&queRes);
    
    return 0;


}