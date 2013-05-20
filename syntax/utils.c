#include "../include/syntax.h"

int is_reversed_word(const char * chs){
  return (tokenType == SNL_REVERSE_WORD && strcmp(chs, tokenValueBuffer) == 0) ? 1 : 0;
}

int is_not_reversed_word(const char * chs){
  return is_reversed_word(chs) ? 0 : 1;
}

int is_symbol(const char * chs){
  return (tokenType == SNL_SYMBOL && strcmp(chs, tokenValueBuffer) == 0) ? 1 : 0;
}

int is_not_symbol(const char * chs){
  return is_symbol(chs) ? 0 : 1;
}

int is_eof(){
  return strcmp("EOF", tokenValueBuffer) == 0 ? 1 : 0;
}

static int dep = 0;

void printf_syntax_tree(TreeNode * root){
  int i;
  for(i = 0; i < dep * 4; ++i)putchar(' ');

  // printf("%s", root->nodeKindStr);
  // if(root->type_name)printf(" %s", root->type_name);
  // printf("\n");

  for(i = 0; i < 3; ++i){
    if(root->child[i]){
      dep++;
      printf_syntax_tree(root->child[i]);
      dep--;
    }
  }
}
