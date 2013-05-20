#include <stdio.h>
#include <string.h>
#include <ctype.h>

static int lineno;

static char reserved_words[][20] = {
  "main",
  "int",
  "if",
  "then",
  "else",
  "return",
  "while",
  "for",
  "begin",
  "writeln",
  "program",
  "end",
  "var",
  "char",
  "fi",
  "type",
  "integer",
  "write",
  "array",
  "of"
};

void letter_start_handler(FILE * fp){
  char ch, buffer[30];
  int i = 0;

  while( (ch = fgetc(fp) ) && isalnum(ch) )
    buffer[i++] = ch;

  ungetc(ch, fp);
  buffer[i] = '\0';

  for(i = 0; i < 20; ++i){
    if( strcmp(buffer, reserved_words[i]) == 0 ){
      printf("%d: reserved word: %s\n", lineno, buffer);
      return;
    }
  }
  printf("%d: ID, name = %s\n", lineno, buffer);
}

void digit_start_handler(FILE * fp){
  int i;
  fscanf(fp, "%d", &i);
  printf("%d: INTEGER, var = %d\n", lineno, i);
}

void print_sym(char ch){
  printf("%d: %c\n", lineno, ch);
}

void colon_start_handler(FILE * fp){
  char ch;
  ch = fgetc(fp);
  if( ch == '=' )
    printf("%d: :=\n", lineno);
  else {
    printf("(100,\"error\")\n"); //100 error
    ungetc(ch, fp);
  }
}

void dot_start_handler(FILE * fp){
  char ch;
  ch = fgetc(fp);
  if( ch == '.' )
    printf("%d: ..\n", lineno);
  else {
    printf("%d: .\n", lineno);
    ungetc(ch, fp);
  }
}

void string_handler(FILE * fp){
  char ch, buffer[100];
  int i = 0;
  while( (ch = fgetc(fp) ) != '"' ){
    buffer[i++] = ch;
    if(i > 99) fprintf(stderr, "String Size Over Buffer Size!\n");
  }

  buffer[i] = '\0';
  printf("(17,\"%s\")\n", buffer); // 17 string
}

void comment_handler(FILE * fp){
  while( fgetc(fp) != '}' )
    ;
}

void analyzer(FILE * fp){
  lineno = 1;
  char ch;
  while( !feof(fp) ){
    ch = fgetc(fp);
    // printf("%c", ch);
    if( isalpha(ch) ){
      ungetc(ch, fp);
      letter_start_handler(fp);
    } else if ( isdigit(ch) ){
      ungetc(ch, fp);
      digit_start_handler(fp);
    } else if ( ch == ':'){
      colon_start_handler(fp);
    } else if ( ch == '.'){
      dot_start_handler(fp);
    } else if ( ch == '"'){
      string_handler(fp);
    } else if ( ch == '{' ){
      comment_handler(fp);
    } else if ( ch == '\n') {
      lineno++;
      continue;
    }
    else if ( ch == ' ' ) continue;
    else if ( ch == '\t') continue;
    else if ( ch == '(' ) print_sym( '(');
    else if ( ch == ')' ) print_sym(')');
    else if ( ch == ';' ) print_sym(';');
    else if ( ch == '=' ) print_sym('=');
    else if ( ch == '+' ) print_sym('+');
    else if ( ch == '*' ) print_sym('*');
    else if ( ch == '-' ) print_sym('-');
    else if ( ch == '/' ) print_sym('/');
    else if ( ch == '>' ) print_sym('>');
    else if ( ch == '<' ) print_sym('<');
    else if ( ch == ',' ) print_sym(',');
    else if ( ch == '"' ) print_sym('"');
    else if ( ch == '[' ) print_sym('[');
    else if ( ch == ']' ) print_sym(']');
  }
  printf("%d: EOF\n", lineno);
}

int main(int argc, char * argv[]){
  char * file_name = *++argv;

  // FILE * fp = fopen(file_name, "r");
  FILE * fp = fopen("../snl", "r");
  freopen("../results/lexical_analysis.txt", "w", stdout);

  analyzer(fp);

  fclose(fp);
  return 0;
}
