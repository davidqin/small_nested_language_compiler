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

void printf_syntax_tree(TreeNode * root, int dep){
  int i;
  for(i = 0; i < dep * 4; ++i)putchar(' ');

  printf("%s", root->nodeKindStr);
  printf(" %s", root->kind.name);

  // printf("((%d))", root->idnum);
  for(i = 0; i < root->idnum; ++i)
    printf(" %s", root->name[i]);

  printf("\n");


  for(i = 0; i < 3; ++i){
    if( root->child[i] ){
      printf_syntax_tree(root->child[i], dep + 1);
    }
  }

  if( root->Sibling ){
    printf_syntax_tree(root->Sibling, dep);
  }

}
