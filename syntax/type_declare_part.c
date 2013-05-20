#include "../include/syntax.h"

void BaseType(TreeNode * t){

  if( strcmp(tokenValueBuffer, "integer") == 0 ){
    t->kind.d = IntegerK;
    return;
  }

  if( strcmp(tokenValueBuffer, "char") == 0 ){
    t->kind.d = CharK;
    return;
  }
}

void ArrayType(TreeNode * t){
  if( ReadToken() != SNL_SYMBOL || strcmp(tokenValueBuffer, "[") != 0){
    fprintf(stderr, "array declaration '[' miss! \n");
    return;
  }

  if( ReadToken() != SNL_INTERGER){
    fprintf(stderr, "array declaration 'low' miss! \n");
    return;
  } else
    t->attr.a_attr.low = i_tokenValueBuffer;

  if( ReadToken() != SNL_SYMBOL || strcmp(tokenValueBuffer, "..") != 0){
    fprintf(stderr, "array declaration '..' miss! \n");
    return;
  }

  if( ReadToken() != SNL_INTERGER){
    fprintf(stderr, "array declaration 'up' miss! \n");
    return;
  } else
    t->attr.a_attr.up = i_tokenValueBuffer;

  if( ReadToken() != SNL_SYMBOL || strcmp(tokenValueBuffer, "]") != 0){
    fprintf(stderr, "array declaration ']' miss! \n");
    return;
  }

  if( ReadToken() != SNL_REVERSE_WORD || strcmp(tokenValueBuffer, "of") != 0){
    fprintf(stderr, "array declaration 'of' miss! \n");
    return;
  }

  ReadToken();
  BaseType(t);

  t->attr.a_attr.childType = t->kind.d;
  t->kind.d = ArrayK;

}

void RecType(TreeNode * t){
  // TreeNode * p = FieldDecList();
}

void StrutureType(TreeNode * t){
  if( strcmp(tokenValueBuffer, "array") == 0 ){
    ArrayType(t);
    return;
  }

  if( strcmp(tokenValueBuffer, "record") == 0 ){
    t->kind.d = RecordK;
    RecType(t);
    return;
  }
}

void TypeId(TreeNode * t){
  if( tokenType == SNL_ID )
    strcpy(t->name[t->idnum++], tokenValueBuffer);
}

void TypeDef(TreeNode * t){
  ReadToken();

  if( tokenType == SNL_REVERSE_WORD &&
      ( strcmp(tokenValueBuffer, "integer") == 0 ||
        strcmp(tokenValueBuffer, "char") == 0
      )
    ){
    BaseType(t);
    return;
  }

  if( tokenType == SNL_REVERSE_WORD &&
      ( strcmp(tokenValueBuffer, "array") == 0 ||
        strcmp(tokenValueBuffer, "record") == 0
      )
    ){
    StrutureType(t);
    return;
  }

  if( tokenType == SNL_ID ){
    t->kind.d = IdK;
    strcpy(t->type_name, tokenValueBuffer);
  }
}

TreeNode * TypeDecList();
TreeNode * TypeDecMore(){
  TreeNode * t = NULL;

  if( ReadToken() == SNL_REVERSE_WORD &&
      ( strcmp(tokenValueBuffer, "var") == 0 ||
        strcmp(tokenValueBuffer, "procedure") == 0 ||
        strcmp(tokenValueBuffer, "begin") == 0
      )
    )
    return t;

  if( tokenType == SNL_ID )
    t = TypeDecList();

  return t;
}

TreeNode * TypeDecList(){
  TreeNode * t = (TreeNode *)malloc(sizeof(TreeNode));;

  if( t == NULL )
    return t;

  TypeId(t);

  if( ReadToken() != SNL_SYMBOL || strcmp(tokenValueBuffer, "=") != 0 )
    fprintf(stderr, "type declaration '=' miss! \n");

  TypeDef(t);

  if( ReadToken() != SNL_SYMBOL || strcmp(tokenValueBuffer, ";") != 0 )
    fprintf(stderr, "type declaration ';' miss! \n");

  TreeNode * p;
  p = TypeDecMore();

  if( p )
    t->Sibling = p;

  return t;
}

TreeNode * typeDeclaration(){

  ReadToken();
  TreeNode * t = TypeDecList();

  if( t == NULL )
    fprintf(stderr, "type declaration error!\n");

  return t;
}

TreeNode * TypeDec(){
  TreeNode * t = NULL;

  if(tokenType == SNL_REVERSE_WORD && strcmp(tokenValueBuffer, "type") == 0 ){
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