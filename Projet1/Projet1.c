#include <stdio.h>
#include <stdlib.h>
#include "analyse_lexicale.h"
#include "../utiles/queue.h"

int main() {
    char *Readbuffer = NULL;
    FILE *fp = fopen("test.lac", "r");
    int Flength = read_into_buffer(fp,&Readbuffer);

    CQueue queRes;
    cqueue_init(&queRes);
    find_lac(Readbuffer, Readbuffer + Flength, &queRes);
    print_out(Readbuffer, &queRes);
    fflush(stdout);
    cqueue_clear(&queRes);
    return 0;
}