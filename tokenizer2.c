/*
 * tokenizer.c
 */
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/*
 * Tokenizer type.  You need to fill in the type as part of your implementation.
 */

struct TokenizerT_ {
    char *delims;
    char *ptr;
    char *fixed;
};

typedef struct TokenizerT_ TokenizerT;

/*
 * TKCreate creates a new TokenizerT object for a given set of separator
 * characters (given as a string) and a token stream (given as a string).
 *
 * TKCreate should copy the two arguments so that it is not dependent on
 * them staying immutable after returning.  (In the future, this may change
 * to increase efficiency.)
 *
 * If the function succeeds, it returns a non-NULL TokenizerT.
 * Else it returns NULL.
 *
 * You need to fill in this function as part of your implementation.
 */
char isSpec(char check){
    switch(check){
        case 'n':{
            return '\n';
        }
        case 'v':{
            return '\v';
        }
        case 't':{
            return '\t';
        }
        case 'b':{
            return '\b';
        }
        case 'r':{
            return '\r';
        }
        case 'f':{
            return '\t';
        }
        case 'a':{
            return '\a';
        }
        case '\"':{
            return '\"';
        }
        case '\\':{
            return '\\';
        }
        default :return 0;
    }
}

TokenizerT *TKCreate(char *separators, char *ts) {
    TokenizerT *ret = calloc(1,sizeof(TokenizerT));
    ret->delims = calloc(128,sizeof(char));
    //ret->fixed = (char *) malloc(sizeof(char)*(strlen(ts)+1));
    ret->fixed = (char *) malloc(sizeof(char)*(256));
    ret->ptr = ret->fixed;
    int i;
    char spec;

    /*this section of code build my lookup table*/
    for(i=0; i <= strlen(separators);i++){
        if(separators[i]=='\\'){
            if((spec = isSpec(separators[i+1]))!=0){
                ret->delims[(int)spec]=1;
                i++;
            }else{
                ret->delims[(int)separators[i+1]]=1;
                i++;
            }
        }else{
            ret->delims[(int)separators[i]]=1;
        }
    }
    ret->delims[0]=1;
    /**********************************************/
    /*builds the fixed string*/
    char *p = ret->fixed;
    for(i =0; i<=strlen(ts);i++){
        if(ts[i]!='\\'){
            *p = ts[i];
            p++;
        }else{
            if((spec= isSpec(ts[i+1]))!=0){
                *p= spec;
                p++;
                i++;
            }else{
                *p=ts[i+1];
                p++;
                i++;
            }
        }
    }
    *(p)='\0';
    /**********************************************/
  return ret;
}

/*
 * TKDestroy destroys a TokenizerT object.  It should free all dynamically
 * allocated memory that is part of the object being destroyed.
 *
 * You need to fill in this function as part of your implementation.
 */

void TKDestroy(TokenizerT *tk) {
    free(tk->delims);
    free(tk->fixed);
    free(tk);
}

/*
 * TKGetNextToken returns the next token from the token stream as a
 * character string.  Space for the returned token should be dynamically
 * allocated.  The caller is responsible for freeing the space once it is
 * no longer needed.
 *
 * If the function succeeds, it returns a C string (delimited by '\0')
 * containing the token.  Else it returns 0.
 *
 * You need to fill in this function as part of your implementation.
 */

char *TKGetNextToken(TokenizerT *tk) {
    int end=0;
    int start =end;
    int i;

    /* no more tokens */
    if(strlen(tk->ptr) == 0){
        return 0;
    }

    /*Find end of token*/
    for (end=0;tk->delims[tk->ptr[end]]!=1;end++){}


    /*findrst char is a delim */
    if (start == end){
        tk->ptr++;
        return TKGetNextToken(tk);
    }

    /*this is to check if the frist character is a delim*/
    char *ret = malloc((end+1)*sizeof(char));

    int stop =end;

    /*copy into token string*/
    for(end=0; end<stop;end++){
        ret[end] = tk->ptr[end];
    }

    ret[stop]='\0';
    tk->ptr= tk->ptr+stop+1;
    return ret;
}

/*
 * main will have two string arguments (in argv[1] and argv[2]).
 * The first string contains the separator characters.
 * The second string contains the tokens.
 * Print out the tokens in the second string in left-to-right order.
 * Each token should be printed on a separate line.
 */

int main(int argc, char **argv) {
    char *token;
    char spec;
    char *freeable;
    if(argc != 3){
        printf("fix ya shit\n");
        return 0;
    }
    int i;
    char *shortBus = "\n\t\v\b\r\f\a\\\'\"";
    TokenizerT *tok = TKCreate(argv[1], argv[2]);

    while(token = TKGetNextToken(tok), token){
        freeable = token;
        //strcpy(freeable, token);
        while(*token!='\0'){
            for(i=0;i<=strlen(shortBus);i++){
                if(*token == shortBus[i]){
                    printf("[0x%.2x]",shortBus[i]);
                    token++;
                    break;
                }
            }
            printf("%c",*token);
            token++;
        }
    printf("\n");
    free(freeable);
    }
    TKDestroy(tok);
  return 0;
}
