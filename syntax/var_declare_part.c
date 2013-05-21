#include "../include/syntax.h"

TreeNode * VarDecMore(){
  TreeNode * t = NULL;
  ReadToken();

  if(
      is_reversed_word("integer") ||
      is_reversed_word("char")    ||
      is_reversed_word("array")   ||
      is_reversed_word("record")  ||
      token_is_id
    )
  {
    UnReadToken();
    t = VarDecList();
  } else
    UnReadToken();

  return t;
}


void VarIdMore(TreeNode * t){
  ReadToken();

  if( is_symbol(";") )
    return;
  else if( is_symbol(",") )
    VarIdList(t);
}

void VarIdList(TreeNode * t){
  ReadToken();

  if( token_is_id )
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

  t->nodeKind = DecK;
  strcpy(t->nodeKindStr, "DecK");

  t->Sibling = VarDecMore();

  return t;
}

TreeNode * VarDeclaration(){
  TreeNode * t = VarDecList();

  if( t == NULL )
    fprintf(stderr, "Error var dec");

  return t;
}

TreeNode * VarDec(){
  ReadToken();

  if( is_reversed_word("var") ){
    return VarDeclaration();}
  else
    UnReadToken();

  return NULL;
}
