#include "../include/syntax.h"
static int a = 1;
TreeNode * DeclarePart(){

  TreeNode * typeP = (TreeNode *)malloc(sizeof(TreeNode));

  if( typeP ){
    typeP->child[0] = TypeDec();
  }

  if( typeP->child[0] ){
    typeP->nodeKind = TypeK;
    strcpy(typeP->nodeKindStr, "TypeK");
  } else {
    free(typeP);
    typeP = NULL;
  }

  TreeNode * varP = (TreeNode *)malloc(sizeof(TreeNode));

  if( varP ){
    varP->child[0] = VarDec();
  }

  if( varP->child[0] ){
    varP->nodeKind = VarK;
    strcpy(varP->nodeKindStr, "VarK");
  } else {
    free(varP);
    varP = NULL;
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
