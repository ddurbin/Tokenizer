
#include <stdio.h>
#include <string.h>
#include <stdlib.h>



struct TokenizerT_ { /* Struct which holds String Token and String separators */
    char *string;
    char * seps;
};
typedef struct TokenizerT_ TokenizerT;

int arrayIndex = 0; //keeps track of position in the string index

//function declarations
char *Concat(char *string, char letter);


TokenizerT *TKCreate(char *separators, char *ts) {
    TokenizerT *tokenizer = (TokenizerT*)malloc(sizeof(TokenizerT));
    tokenizer->string = ts;
    tokenizer->seps = separators;
    arrayIndex = 0;
    
    return tokenizer;
}



void TKDestroy(TokenizerT *tk) {
    free(tk->string);
    free(tk);
}


void trimStringLead(TokenizerT *tk)  /* Trims leading delimiter characters */
{
    int i;
    int j;
    int booleanNoMatch = 0;
    int count = 0;
    size_t sizeToken = strlen(tk->string);
    size_t sizeSep = strlen(tk->seps);
    char *newToken = malloc(sizeToken*sizeof(char));
    
    for(i=0;i<sizeToken; i++) /* Loop through token string */
    {
        for(j=0;j<sizeSep;j++) /* Loop through separator string */
        {
            if(tk->string[i] == tk->seps[j]) /* If token character and separator character match */
            {
                count++;
                break;
            }else if(j == sizeSep-1) /* Loop is on last character of separator string with no matches */
            {
                booleanNoMatch = 1;
            }
        }
        if(booleanNoMatch) /* If charactcter is not a separator break for loop */
        {
            break;
        }
    }
    
    if(count != 0) /* Separators exist */
    {
        j=0;
        for(i=count;i<sizeToken;i++) /* Copy over remaining characters leaving out separators */
        {
            newToken[j] = tk->string[i];
            j++;
        }
        newToken[sizeToken - count] = '\0'; /* Setting end null character */
        tk->string = newToken; /* Pointing struct to newToken */
    }
    return;
}

void trimStringTrail(TokenizerT *tk)  /* Trims trailing delimiter characters */
{
    int i;
    int j;
    int booleanNoMatch = 0;
    int count = 0;
    size_t sizeToken = strlen(tk->string);
    size_t sizeSep = strlen(tk->seps);
    char *newToken = malloc(sizeToken*sizeof(char));
    for(i=sizeToken-1; i >= 0; i--)  /* Loops through the string fromt he last character */
    {
        for(j=0;j<sizeSep;j++) /* Loops throught the separator string */
        {
            if(tk->string[i] == tk->seps[j]) /* Character is a separator */
            {
                count++;
                break;
            }
            else if(j == (sizeSep-1)) /* Character is not a sep and all sep characters have been checked */
            {
                booleanNoMatch = 1;
            }
        }
        if(booleanNoMatch) /* Break the Loop, character is not a sep */
        {
            break;
        }
    }
    for(i=0; i<sizeToken-(count);i++) /* Loop through and add all characters which are not seps to newToken */
    {
        newToken = Concat(newToken, tk->string[i]);
    }
    tk->string = newToken; /* Set string of struct to point to newToken */
    return;
}

void printToken(char* string) /* Prints out each token and checks for escape characters and manipulates as necessary */
{
    int i;
    char escape = '0';
    size_t strLen = strlen(string);
    char* token = malloc(strLen*sizeof(char));
    int indexPtr = 0;
    if(strLen == 1 && string[0] == '\\') /* If string is i character and the character is a '\' */
    {
        return;
    }
    for(i=0; i<strLen;i++) /* Loop through string */
    {
        
        if(string[i] == '\\') /* If character is a  '\' */
        {
            if(strlen(token) != 0) /* Print word in front of the '\' */
            {
                token[indexPtr] = '\0';
                printf("%s", token);
                indexPtr = 0;
                token[0] = '\0';
            }
            
            escape = string[i+1]; /* Get next character after the '\' */
            
            if(escape == 'n') /* If statements check the type of escape character */
            {
                escape = '\n';
                printf("[0x%.2x]",escape);
                i++;
            }
            else if(escape == 't')
            {
                escape = '\t';
                printf("[0x%.2x]",escape);
                i++;
            }
            else if(escape == 'v')
            {
                escape = '\v';
                printf("[0x%.2x]",escape);
                i++;
            }
            else if(escape == 'b')
            {
                escape = '\b';
                printf("[0x%.2x]",escape);
                i++;
            }
            else if(escape == 'r')
            {
                escape = '\r';
                printf("[0x%.2x]",escape);
                i++;
            }
            else if(escape == 'f')
            {
                escape = '\f';
                printf("[0x%.2x]",escape);
                i++;
            }
            else if(escape == '\\')
            {
                escape = '\\';
                printf("[0x%.2x]",escape);
                i++;
            }
            else if(escape == 'a')
            {
                escape = '\a';
                printf("[0x%.2x]",escape);
                i++;
            }
            else if(escape == '\"')
            {
                escape = '\"';
                printf("[0x%.2x]",escape);
                i++;
            }else                             /* Not a valid escape character */
            {
                token[indexPtr] = string[i+1];
                indexPtr++;
                i++;
            }
            
        }else   /* Character is not a '\' */
        {
            token[indexPtr] = string[i];
            indexPtr++;
        }
        
    }
    if(strlen(token) != 0)  /* Prints string if length not 0 */
    {
        printf("%s", token);
    }
    printf("\n"); /* Print new line */
    return;
}


char *TKGetNextToken(TokenizerT *tk) {
    char *seps = tk->seps;
    size_t sepsSize = strlen(tk->seps);
    char *string = tk->string;
    size_t stringSize = strlen(string);
    int i;
    int x = 0;
    int boolean, addBoolean, iStore;
    char *token = (char*)malloc(sizeof(char));
    
    token = "";
    if(arrayIndex >= stringSize){  /* All tokens returned */
        return NULL;
    }
    for(i = arrayIndex; i < stringSize; i++) /* Loop through string staring at arrayIndex from previous returned string */
    {
        boolean = 0;    //boolean used to tell if a char should be added to the token or not
        addBoolean = 0; //boolean used for adding/not adding a "\(something)"
        iStore = 0;
        for(x = 0; x < sepsSize; x++) /* Loop through seps string */
        {
            if(string[i] == seps[x]) /* Character of token is a separator */
            {
                //testing a backslash and its following character
                if(string[i] == '\\'){  
                    if(string[i+1] == seps[x+1] && strlen(token) != 0) /* Token is not empty */
                    {
                        token[strlen(token)] = '\0';    /*add null terminator */
                        arrayIndex += 2;
                        return token;
                    } else if(string[i+1] == seps[x+1]){    // at special character separator and token is empty
                        //i++;
                        //arrayIndex++;
                        addBoolean = 0;
                        break;
                    } else if(string[i+1] != seps[x+1]){
                        addBoolean = 1;
                        iStore = i; // temporarily stores the i value
                    }
                }
                else if(strlen(token) != 0) /* Token is not empty */
                {
                    token[strlen(token)] = '\0';    /*add null terminator */
                    arrayIndex++;
                    return token;
                } else {
                    boolean = 1;
                    break;
                }
            }
        }
        if(addBoolean == 1){
            token = Concat(token, string[iStore]);   //adds the '\\' to the token
            token = Concat(token, string[iStore+1]); //adds the character after the '\\' to the token
            i++;    //increment i  and arrayIndex an extra time since two chars were checked
            arrayIndex++;
            boolean = 1;
        }
        if(boolean == 0)
        {
            token = Concat(token, string[i]);   //adds next character to the token
        }
        if(arrayIndex == stringSize-1) /* Last token */
        {
            arrayIndex++;
            token[strlen(token)] = '\0';
            return token;
        }
        arrayIndex++; /* Increment arrayIndex for future reference */
    }
    return token;
}

char *Concat(char *string, char letter)
{
    char *str = string;
    char let = letter;
    size_t len = strlen(str);
    char *result = (char*) malloc((len + 2) * sizeof(char));    //add 2 to make room for added char and null terminator
    strcpy(result, str);
    result[len] = let;
    //printf("%s\n", result);
    return result;
}


int main(int argc, char **argv) {
    if(argc != 3){
        fprintf(stderr,"****Incorrect number of CMD line arguments****\n");  /* Error check command line arguments */
        return 0;
    }
    
    char* seperator = argv[1];   /* Storing command Arguments */
    char* string = argv[2];
    
    TokenizerT *tokenizer = TKCreate(seperator, string);
    trimStringLead(tokenizer);      /* Trim leading and trailing seps */
    trimStringTrail(tokenizer);
    char* newToken = TKGetNextToken(tokenizer);
    while(newToken != NULL )                 /* print each token individually */
    {
        printToken(newToken);
        newToken = TKGetNextToken(tokenizer);
    }
    free(newToken);
    TKDestroy(tokenizer);
    return 0;
}
