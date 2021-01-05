#ifndef ANALYSE_LEXICALE
#define ANALYSE_LEXICALE

#include <stdlib.h>
#include <string.h>
#include <regex.h>
#include "../utiles/queue.h"

#define TRUE 1
#define FALSE 0
#define REGEX_COMMENT "(^|[\n| ])\\( [^\\)]*?\\)|(^|[\n| ])\\\\[^\n]*"
#define REGEX_STRING "(^|[\n| ])\" [^\"]*?\""
#define REGEX_IDENTIFIER    "[^ \n\"\\\\\\(\\)]+"
#define REGEX_INTEGER   "(\\-|\\+)?[0-9]+(\\.[0-9]+)?[ |\n]"

;

typedef struct lexeme_t {
    char *type;
    long lStart;
    long lEnd;
} lexeme_t;


char * copy_mid(char *dst,const char *src, int n,int m) // n is longeur, m is position
{
    const char *p = src;
    char *q = dst;
    int len = strlen(src);
    if(n>len) n = len-m;    // from m to the last one
    if(m<0) m=0;    // from the first one
    if(m>len) return NULL;
    p += m;
    while(n--) *(q++) = *(p++);
    *(q++)='\0';
    return dst;
}

// Verifier if it is integer


int isInteger(const char *pBegin, const char *pFin, regex_t *MyInteger) {
    int res;
    regmatch_t pMatch[10];
    int iStart;
    int iEnd;

    res = regexec(MyInteger, pBegin, 1, pMatch, 0);
    if (res == REG_NOMATCH) {
        return -1;
    } 
    else {
        iStart = pMatch[0].rm_so;
        iEnd = pMatch[0].rm_eo;
        //out of range
        if (pBegin + iEnd - 1 > pFin) {
            return -1;
        } else {
            return 1;
        }
    }
}

int find_iden(const char *Readbuffer, const char *pIdenBegin, const char *pIdenFin,
             regex_t *MyIdentifier, regex_t *MyInteger,CQueue *FinalQue) {
    int res;
    int offset;
    regmatch_t pMatch[10];
    int MyIdenStart;
    int MyidenEnd;

    lexeme_t solu;
    solu.type = (char*)"word";

    while (1) {
        res = regexec(MyIdentifier, pIdenBegin, 1, pMatch, 0);
        if (res == REG_NOMATCH) { // No match
            break;
        } else {
            MyIdenStart = pMatch[0].rm_so;
            MyidenEnd = pMatch[0].rm_eo;

            // out of range, break
            if (pIdenBegin + MyidenEnd > pIdenFin) {
                break;
            }

            offset = pIdenBegin - Readbuffer;
            
            solu.lStart = MyIdenStart + offset;
            solu.lEnd = MyidenEnd + offset;

            char mot[100];
            char* p = mot;
            copy_mid(mot,Readbuffer,solu.lEnd-solu.lStart,solu.lStart);
            printf("Iden, mot:%s\n",mot);
            
            // identifier number
            int IdenN=isInteger(Readbuffer + solu.lStart, Readbuffer + solu.lEnd, MyInteger);
            if (IdenN==1) {
                solu.type = (char*)"number";
            } 
            else {
                solu.type = (char*)"word";
            }

            // put in the queue
            cqueue_push_back(FinalQue, (void *) &solu, sizeof(solu));
            pIdenBegin += MyidenEnd;

        }
    }
    return 1;
}

int find_string(const char *Readbuffer, const char *pStringBegin, const char *pStringFin,
                regex_t *MyString, regex_t *MyIdentifier, regex_t *MyInteger, CQueue *FinalQue) {
    int res;
    int offset;
    regmatch_t pMatch[10];
    int MyStringStart;
    int MyStringEnd;
    const char *pIdenBegin = NULL;
    const char *pIdenFin = NULL;

    lexeme_t solu;
    solu.type = (char*)"string";

    while (1) {
        res = regexec(MyString, pStringBegin, 1, pMatch, 0);
        if (res == REG_NOMATCH) {
            // last string is matched, try match identifier in the rest of characters
            pIdenBegin = pStringBegin;
            pIdenFin = (char *) pStringFin;
            find_iden(Readbuffer, pIdenBegin, pIdenFin, MyIdentifier, MyInteger, FinalQue);
            break;
        } 
        else {
            MyStringStart = pMatch[0].rm_so;
            MyStringEnd = pMatch[0].rm_eo;
            // out of boundary, try match identifier
            if ((pStringBegin + MyStringEnd) > pStringFin) {
                pIdenBegin = pStringBegin;
                pIdenFin = (char *) pStringFin;
                find_iden(Readbuffer, pIdenBegin, pIdenFin, MyIdentifier, MyInteger, FinalQue);
                break;
            }
            // set limit
            pIdenBegin = pStringBegin;
            pIdenFin = pStringBegin + MyStringStart;

            offset = pStringBegin - Readbuffer;
            //printf("string, offset:%d\n",offset);
            
            if(offset==0){
                char comp[10];
                copy_mid(comp,Readbuffer,1,MyStringStart);
                //printf("%s",comp);
                if(strcmp(comp,"\"")==0){
                    solu.lStart = MyStringStart + offset + 2; // remove "
                    solu.lEnd = MyStringEnd + offset - 1; // remove "
                }
                else{
                    solu.lStart = MyStringStart + offset + 3; // remove "
                    solu.lEnd = MyStringEnd + offset - 1; // remove "
                }

            }
            else{
                solu.lStart = MyStringStart + offset + 3; // remove "
                solu.lEnd = MyStringEnd + offset - 1; // remove "
            }

            // match identifier
            find_iden(Readbuffer, pIdenBegin, pIdenFin, MyIdentifier, MyInteger, FinalQue);

            // put into queue
            cqueue_push_back(FinalQue, (void *) &solu, sizeof(solu));
            pStringBegin += MyStringEnd;
        }
    }
    return 1;
}

int find_lac(char *Readbuffer, char *ReadbufferFin, CQueue *FinalQue) {
    // Definition of 4 regex parser
    regex_t MyComment;
    regex_t MyString;
    regex_t MyIdentifier;
    regex_t MyInteger;
    if(regcomp(&MyComment, REGEX_COMMENT, REG_EXTENDED)) exit(2); 
    if(regcomp(&MyString, REGEX_STRING, REG_EXTENDED)) exit(2); 
    if(regcomp(&MyIdentifier, REGEX_IDENTIFIER, REG_EXTENDED)) exit(2); 
    if(regcomp(&MyInteger, REGEX_INTEGER, REG_EXTENDED)) exit(2); 
    // if failed, get out.
    
    int res = 0;
    regmatch_t pMatch[10]; // match results
    int MyCommentStart;
    int MyCommentEnd;
    char *pCommentBegin = Readbuffer;
    char *pStringBegin = NULL;
    char *pStringFin = NULL;

    while (1) {
        // match comments
        res = regexec(&MyComment, pCommentBegin, 1, pMatch, 0);
        if (res == REG_NOMATCH) { // the last comment is extracted
            pStringBegin = pCommentBegin;
            pStringFin = ReadbufferFin;
            // match string from the rest
            find_string(Readbuffer, pStringBegin, pStringFin, &MyString, &MyIdentifier, &MyInteger, FinalQue);
            break;
        } else {
            MyCommentStart = pMatch[0].rm_so;
            MyCommentEnd = pMatch[0].rm_eo;
            pStringBegin = pCommentBegin;
            pStringFin = pCommentBegin + MyCommentStart;
            
            char mot[500];
            char* p = mot;
            copy_mid(mot,Readbuffer,MyCommentEnd-MyCommentStart,MyCommentStart);
            //printf("Comment, mot:%s\n",mot);

            // match string in between
            find_string(Readbuffer, pStringBegin, pStringFin, &MyString, &MyIdentifier, &MyInteger, FinalQue);

            // Move to the next comment
            pCommentBegin += MyCommentEnd;
        }

    }
    // free 
    regfree(&MyComment);
    regfree(&MyString);
    regfree(&MyIdentifier);

    return 1;
}


int read_into_buffer(FILE * fp, char ** ReadBuffer) {
    int Bufferlen;
    int lSize;

    // Get the size of file
    fseek (fp , 0 , SEEK_END);
    lSize = ftell (fp);
    rewind (fp);

    *ReadBuffer = (char*) malloc (sizeof(char)*lSize);
    Bufferlen = fread(*ReadBuffer, 1, lSize, fp);
    return (int)Bufferlen;
}


void print_out(char * psReadBuffer, CQueue * pqueRes){
    QueueNode * tmp = pqueRes->pHead;
    lexeme_t res;
    int flag = FALSE;
    while (TRUE) {
        res =  *(lexeme_t *)tmp->pData;
        if (strcmp(res.type,"word")==0){
            printf("M(\"%.*s\")->", (int)(res.lEnd - res.lStart), psReadBuffer + res.lStart);
        }
        else if(strcmp(res.type,"string")==0){
            printf("S(\"%.*s\")->", (int)(res.lEnd - res.lStart), psReadBuffer + res.lStart);
        }
        else if(strcmp(res.type,"number")==0){
            printf("N(\"%.*s\")->", (int)(res.lEnd - res.lStart), psReadBuffer + res.lStart);
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
