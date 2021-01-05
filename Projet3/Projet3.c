#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "base.h"
#include "LacVM.h"

int main(){
    
    //establish pile
    s_donnee=cstack_init();
    s_retour=cstack_init();
    char *nom[10]={"+","*","-","dup","drop","swap","count",".","type","="};
    void *function_list[10] = {func_plus,func_multiply,func_minus,func_dup,func_drop,
                                    func_swap,func_count,func_point,func_type,func_equal};

    printf("-------------------LAC---------------------\n");
    for(int i=0;i<10;i++){
        declare_base(nom[i],function_list[i]);
    }

    printf("--------------------VM---------------------\n");
    for (int i=0;i<pointer_VM;i++){
        if (i%2==0){
            printf("position: %d, value: %d, signification: fonction de base\n",i,VM[i]);
        }
        else{
            int a=i/2;
            printf("position: %d, value: %d, signification: %s\n",i,VM[i],nom[a]);
        }
    }


    //read the sentence
    char *Readbuffer = NULL;
    FILE *fp = fopen("test_projet3.lac", "r");
    int Flength = read_into_buffer(fp,&Readbuffer);

    CQueue queRes;
    cqueue_init(&queRes);
    find_lac(Readbuffer, Readbuffer + Flength, &queRes);
    print_out(Readbuffer, &queRes);
    execution(Readbuffer,&queRes,s_donnee);

    fflush(stdout);
    cqueue_clear(&queRes);
    return 0;


}