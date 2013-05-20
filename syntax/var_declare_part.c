#include "../include/syntax.h"

TreeNode * VarDecList();

TreeNode * VarDecMore(){
  TreeNode * t = NULL;

  if(
      ( tokenType == SNL_REVERSE_WORD &&
        ( strcmp(tokenValueBuffer, "integer") == 0 ||
          strcmp(tokenValueBuffer, "char")    == 0 ||
          strcmp(tokenValueBuffer, "array")   == 0 ||
          strcmp(tokenValueBuffer, "record")  == 0
        )
      ) ||
      (
        tokenType == SNL_ID
      )
    )
    VarDecList();

  return t;
}

void VarIdList(TreeNode * t);

void VarIdMore(TreeNode * t){
  if(ReadToken() == SNL_SYMBOL && (strcmp(tokenValueBuffer, ";") == 0 ) )
    return;
  else if(strcmp(tokenValueBuffer, ",") == 0)
    VarIdList(t);
}

void VarIdList(TreeNode * t){
  if(ReadToken() == SNL_ID)
    strcpy(t->name[t->idnum++], tokenValueBuffer);

  VarIdMore(t);
}

TreeNode * VarDecList(){
  TreeNode * t;
  t = (TreeNode *)malloc(sizeof(TreeNode));

  if(t == NULL)
    return t;

  TypeDef(t);
  VarIdList(t);

  t->Sibling = VarDecMore(); // To DO Write A Un Read Token

  return t;
}

TreeNode * VarDeclaration(){
  TreeNode * t = VarDecList();

  if( t == NULL )
    fprintf(stderr, "Error var dec");

  return t;
}

TreeNode * VarDec(){
  TreeNode * t = NULL;

  if(tokenType != SNL_REVERSE_WORD || strcmp(tokenValueBuffer, "var") != 0 )
    return t;

  VarDeclaration();

  ReadToken();

  return t;
}