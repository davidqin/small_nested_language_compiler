#include "../include/syntax.h"

SNL_TYPE tokenType;
char tokenValueBuffer[30];
int i_tokenValueBuffer;

char unReadTokenBuffer[30];
SNL_TYPE unReadTokenType;

void UnReadToken(){

}

SNL_TYPE ReadToken(){
  int lineno, i;
  char type[30], chs[30];
  fscanf(fp, "%d: %s", &lineno, type);

  if( strcmp(type, "reserved") == 0 ){
    fscanf(fp, " word: %s", chs);
    // printf("%d %s\n", lineno, chs);

    tokenType = SNL_REVERSE_WORD;
    strcpy(tokenValueBuffer, chs);

  } else if ( strcmp(type, "ID,") == 0 ){
    fscanf(fp, " name = %s", chs);
    // printf("%d %s\n", lineno, chs);

    tokenType = SNL_ID;
    strcpy(tokenValueBuffer, chs);

  } else if ( strcmp(type, "INTEGER,") == 0 ){
    fscanf(fp, " var = %d", &i);
    // printf("%d %d\n", lineno, i);

    tokenType = SNL_INTERGER;
    i_tokenValueBuffer = i;

  } else if (type[0] == '\0'){
    ;
  } else {
    // printf("%d %s\n", lineno, type);
    tokenType = SNL_SYMBOL;
    strcpy(tokenValueBuffer, type);

  }

  type[0] = '\0';
  return tokenType;
}
