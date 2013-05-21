#include "../include/syntax.h"

void BaseType(TreeNode * t){
  ReadToken();

  if( is_reversed_word("integer") ){
    t->kind.d = IntegerK;
    strcpy(t->kind.name, "IntegerK");
    return;
  }

  if( is_reversed_word("char") ){
    t->kind.d = CharK;
    strcpy(t->kind.name, "CharK");
    return;
  }
}

void ArrayType(TreeNode * t){

  ReadToken();
  if( is_not_symbol("[") ){
    fprintf(stderr, "array declaration '[' miss! \n");
    return;
  }

  if( ReadToken() != SNL_INTERGER){
    fprintf(stderr, "array declaration 'low' miss! \n");
    return;
  } else
    t->attr.a_attr.low = i_tokenValueBuffer;

  ReadToken();
  if( is_not_symbol("..") ){
    fprintf(stderr, "array declaration '..' miss! \n");
    return;
  }

  if( ReadToken() != SNL_INTERGER){
    fprintf(stderr, "array declaration 'up' miss! \n");
    return;
  } else
    t->attr.a_attr.up = i_tokenValueBuffer;

  ReadToken();
  if( is_not_symbol("]") ){
    fprintf(stderr, "array declaration ']' miss! \n");
    return;
  }

  ReadToken();
  if( is_not_reversed_word("of") ){
    fprintf(stderr, "array declaration 'of' miss! \n");
    return;
  }

  BaseType(t);

  t->attr.a_attr.childType = t->kind.d;
  t->kind.d = ArrayK;
  strcpy(t->kind.name, "ArrayK");

}

void RecType(TreeNode * t){
  // TreeNode * p = FieldDecList();
}

void StrutureType(TreeNode * t){
  if( is_reversed_word("array") ){
    ArrayType(t);
    return;
  }

  if( is_reversed_word("record") ){
    t->kind.d = RecordK;
    strcpy(t->kind.name, "RecordK");
    RecType(t);
    return;
  }
}

void TypeId(TreeNode * t){
  ReadToken();
  if( token_is_id )
    strcpy(t->name[t->idnum++], tokenValueBuffer);
  else{
    fprintf(stderr, "type declaration miss Id.\n");
    UnReadToken();
  }
}

void TypeDef(TreeNode * t){
  ReadToken();

  if( is_reversed_word("integer") || is_reversed_word("char") ){
    UnReadToken();
    BaseType(t);
    return;
  }

  if( is_reversed_word("array") || is_reversed_word("record") ){
    StrutureType(t);
    return;
  }

  if( tokenType == SNL_ID ){
    t->kind.d = IdK;
    strcpy(t->type_name, tokenValueBuffer);
  }
}

TreeNode * TypeDecMore(){
  TreeNode * t = NULL;

  ReadToken();

  if( token_is_id ){
    UnReadToken();
    t = TypeDecList();
  } else
    UnReadToken();

  return t;
}

TreeNode * TypeDecList(){
  TreeNode * t = (TreeNode *)malloc(sizeof(TreeNode));;

  if( t == NULL )
    return t;

  TypeId(t);

  ReadToken();
  if( is_not_symbol("=") )
    fprintf(stderr, "type declaration '=' miss! \n");

  TypeDef(t);

  ReadToken();
  if( is_not_symbol(";") )
    fprintf(stderr, "type declaration ';' miss! \n");

  t->nodeKind = DecK;
  strcpy(t->nodeKindStr, "DecK");

  TreeNode * p;
  p = TypeDecMore();

  if( p )
    t->Sibling = p;

  return t;
}

TreeNode * typeDeclaration(){

  TreeNode * t = TypeDecList();

  if( t == NULL )
    fprintf(stderr, "type declaration error!\n");

  return t;
}

TreeNode * TypeDec(){
  TreeNode * t = NULL;

  ReadToken();
  if( is_reversed_word("type") ){
    t = typeDeclaration();
  } else
    UnReadToken();

  return t;
}
