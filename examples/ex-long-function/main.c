#include <stdio.h>
#include <regex.h>
#include <stdlib.h>
#include <string.h>
#define BLOCKSIZE 10

int main(){
    char* regexString = "(\\w+)@[a-zA-Z0-9_-]+: ?~ ?# (.+)";
    size_t maxGroups = 3;
    
    regex_t regexCompiled;
    regmatch_t groupArray[maxGroups];

    if(regcomp(&regexCompiled, regexString, REG_EXTENDED)){
        printf("Comp error");
        return 0;
    }

    //freopen("text.txt", "r", stdin);

    char *s = (char*)malloc(BLOCKSIZE*sizeof(char));
    char **ss = (char**)malloc(BLOCKSIZE*sizeof(char*));

    int scapacity = BLOCKSIZE;
    int sscapacity = BLOCKSIZE;
    int susedcap = 0;
    int ssusedcap = 0;

    while(1){
        if(susedcap == scapacity-2){
            s = realloc(s, scapacity+BLOCKSIZE);
            scapacity+=BLOCKSIZE;
        }

        if(ssusedcap == sscapacity-2){
            ss = realloc(ss, (sscapacity+BLOCKSIZE)*sizeof(char*));
            sscapacity+=BLOCKSIZE;
        }

        char c = getchar();
        if(c == '\n' || c == EOF){
            s[susedcap] = '\0';
            if(strcmp(s, "Fin.") == 0){
                break;
            }
            ss[ssusedcap++] = strdup(s);
            free(s);
            s = (char*)malloc(BLOCKSIZE*sizeof(char));
            susedcap = 0;
            scapacity = BLOCKSIZE;
            continue;
        }
        s[susedcap++] = c;
    }

    for(int i = 0; i < ssusedcap; i++){
        if (regexec(&regexCompiled, ss[i], maxGroups, groupArray, 0) == 0) {
            ss[i][groupArray[1].rm_eo] = '\0';
            ss[i][groupArray[2].rm_eo] = '\0';
            printf("%s - %s\n", ss[i] + groupArray[1].rm_so, ss[i] + groupArray[2].rm_so);
        }
    }
    return 0;
}
