#include "../include/syntax.h"

TreeNode * DeclarePart(){

  TreeNode * typeP = (TreeNode *)malloc(sizeof(TreeNode));
  TreeNode * pp = (TreeNode *)malloc(sizeof(TreeNode));;

  if(typeP){
    typeP->child[0] = TypeDec();
  }

  TreeNode * varP = (TreeNode *)malloc(sizeof(TreeNode));

  if(varP){
    varP->child[0] = VarDec();
  }
  // ReadToken();
  // printf("token = %s\n", tokenValueBuffer);

  return pp;
}
