#include "../include/syntax.h"
static int a = 1;
TreeNode * DeclarePart(){
  TreeNode * tmp = NULL;
  TreeNode * typeP = NULL;
  tmp = TypeDec();

  if( tmp ){
    typeP = (TreeNode *)malloc(sizeof(TreeNode));
  }

  if( typeP ){
    typeP->child[0] = tmp;
    typeP->nodeKind = TypeK;
    strcpy(typeP->nodeKindStr, "TypeK");
  }

  tmp = NULL;

  tmp = VarDec();
  TreeNode * varP = NULL;

  if( tmp ){
    varP = (TreeNode *)malloc(sizeof(TreeNode));
  }

  if( varP ){
    varP->child[0] = tmp;
    varP->nodeKind = VarK;
    strcpy(varP->nodeKindStr, "VarK");
  }

  TreeNode * pp = NULL;

  if( pp == NULL && typeP )
    pp = typeP;

  if( pp == NULL && varP )
    pp = varP;
  else if ( pp && varP )
    pp->Sibling = varP;

  TreeNode * s = ProcDec();

  if( pp == NULL && s)
    pp = s;
  else if( pp && pp->Sibling && s)
    pp->Sibling->Sibling = s;
  else if( pp && !pp->Sibling && s)
    pp->Sibling = s;

  return pp;
}
