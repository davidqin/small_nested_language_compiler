#include "../include/syntax.h"

TreeNode * DeclarePart(){
  ReadToken();

  TreeNode * typeP = (TreeNode *)malloc(sizeof(TreeNode));
  TreeNode * pp = (TreeNode *)malloc(sizeof(TreeNode));;

  if(typeP){
    typeP->child[0] = TypeDec();
  }

  TreeNode * varP = (TreeNode *)malloc(sizeof(TreeNode));

  if(varP){
    varP->child[0] = VarDec();
  }

  TreeNode * t = NULL;

  if( is_reversed_word("type") ){
    t = typeDeclaration();
    return t;
  }

  if( strcmp(tokenValueBuffer, "var") != 0 &&
      strcmp(tokenValueBuffer, "procedure") != 0 &&
      strcmp(tokenValueBuffer, "begin") != 0
    )
    ReadToken();

  return t;
}
