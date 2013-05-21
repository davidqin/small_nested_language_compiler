#include "../include/syntax.h"

TreeNode * ProcBody(){
  TreeNode * t = ProgramBody();
  if( t == NULL )
    fprintf(stderr, "no procedure body error\n");

  return t;
}

TreeNode * ProcDecPart(){
  return DeclarePart();
}

void FidMore(TreeNode * t){
  ReadToken();

  if( is_symbol(";") || is_symbol(")") )
    return;

  if( is_symbol(",") ){
    FormList(t);
    return;
  }

  fprintf(stderr, "FormList Error\n");
}

void FormList(TreeNode * t){
  ReadToken();
  if( token_is_id )
    strcpy(t->name[t->idnum++], tokenValueBuffer);

  FidMore(t);
}

TreeNode * Param(){
  TreeNode * t = (TreeNode *)malloc(sizeof(TreeNode));
  if( t == NULL )
    return NULL;

  if(
      is_reversed_word("integer") ||
      is_reversed_word("char") ||
      is_reversed_word("record") ||
      is_reversed_word("array") ||
      token_is_id
    ){
    t->attr.p_attr.paramt = varparamtype;
    UnReadToken();
    TypeDef(t);
    FormList(t);
  } else if( is_reversed_word("var") ){

    t->attr.p_attr.paramt = varparamtype;
    TypeDef(t);
    FormList(t);
  }

  return t;
}

TreeNode * ParamMore(){

  if( is_symbol(")") ){
    UnReadToken();
    return NULL;
  }

  TreeNode * t = NULL;

  if( is_symbol(";") ){
    ReadToken();
    t = ParamDecList();
  }

  if( t == NULL )
    fprintf(stderr,"Param More Error\n");

  return t;
}

TreeNode * ParamDecList(){
  TreeNode * t = Param();
  TreeNode * p = ParamMore();

  t->nodeKind = DecK;
  strcpy(t->nodeKindStr, "DecK");

  if(p)
    t->Sibling = p;

  return t;
}

void ParamList(TreeNode * t){
  ReadToken();

  if(
      is_reversed_word("integer") ||
      is_reversed_word("char") ||
      is_reversed_word("record") ||
      is_reversed_word("array") ||
      is_reversed_word("var") ||
      token_is_id
    );
    t->child[0] = ParamDecList();
}

TreeNode * ProcDecLaration(){
  TreeNode * t = NULL;
  t = (TreeNode *)malloc(sizeof(TreeNode));

  if( t == NULL )
    return t;

  t->nodeKind = ProcDecK;
  strcpy(t->nodeKindStr, "ProcDecK");

  ReadToken();

  if( token_is_id )
    strcpy(t->name[t->idnum++], tokenValueBuffer);

  ReadToken();

  if( is_not_symbol("(") )
    fprintf(stderr, "Procedure declare miss '('\n");

  ParamList(t);

  ReadToken();

  if( is_not_symbol(")") )
    fprintf(stderr, "Procedure declare miss ')'\n");

  ReadToken();

  if( is_not_symbol(";") )
    fprintf(stderr, "Procedure declare miss ';'\n");

  t->child[1] = ProcDecPart();
  t->child[2] = ProcBody();

  return t;
}

TreeNode * ProcDec(){
  ReadToken();

  if( is_not_reversed_word("procedure") ){
    UnReadToken();
    return NULL;
  }

  return ProcDecLaration();
}
