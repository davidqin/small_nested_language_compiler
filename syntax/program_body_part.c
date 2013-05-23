#include "../include/syntax.h"

static char temp_name[30];

TreeNode * StmMore(){
  TreeNode * t = NULL;
  ReadToken();

  if( is_reversed_word("end") || is_reversed_word("endwh") ){
    UnReadToken();
    return NULL;
  }

  if( is_symbol(";") )
    return StmList();

  return t;
}

void FieldVarMore(TreeNode * t){

}

TreeNode * FieldVar(){
  TreeNode * t = NULL;
  ReadToken();
  if( t )
    FieldVarMore(t); //TODO

  return t;
}

void variMore(TreeNode * t){
  ReadToken();

  if( is_symbol("[") ){
    t->child[0] = Exp();
    ReadToken();

    if( is_not_symbol("]") ){
      fprintf(stderr, "VarMore ']' miss\n");
      UnReadToken();
    }

    return;
  }

  UnReadToken();

  // if( is_symbol(".") ){
    // t->child[0] = FieldVar();
    // t->attr.e_attr.varkind = FieldMembV;
    // t->child[0]->attr.e_attr.varkind = IdV;
  // }
}

TreeNode * Variable(){
  TreeNode * t = (TreeNode *)malloc(sizeof(TreeNode));
  ReadToken();
  if( t && token_is_id ){

    t->nodeKind = ExpK;
    sprintf(t->nodeKindStr, "ExpK");

    strcpy(t->name[t->idnum++], tokenValueBuffer);
    variMore(t);
  }

  return t;
}

TreeNode * IntegerExpk(){
  TreeNode * t = (TreeNode *)malloc(sizeof(TreeNode));

  t->nodeKind = ExpK;
  sprintf(t->nodeKindStr, "ExpK %d", i_tokenValueBuffer);

  return t;
}

TreeNode * Factor(){

  TreeNode * t = NULL;
  ReadToken();

  if( token_is_id ){
    UnReadToken();
    return Variable();
  }

  if( token_is_integer ){
    return IntegerExpk();
  }

  if( is_symbol("(") ){
    t = Exp();
    ReadToken();
    if( is_not_symbol(")") )
      fprintf(stderr, "Factor miss ')'\n");
  }

  return t;
}

TreeNode * Term(){

  TreeNode * t = Factor();

  TreeNode * p = NULL;

  while(1){
    ReadToken();

    if( is_not_symbol("/") && is_not_symbol("*") ){
      UnReadToken();
      return t;
    }

    p = (TreeNode *)malloc(sizeof(TreeNode));
    if( p ){
      p->child[0] = t;
      p->attr.e_attr.op = tokenValueBuffer[0];

      p->nodeKind = ExpK;
      sprintf(p->nodeKindStr,"ExpK Op %s", tokenValueBuffer);

      t = p;
    }
    t->child[1] = Factor();
  }

  return t;
}

TreeNode * SimpleExp(){
  TreeNode * t = Term();
  TreeNode * p = NULL;

  while(1){
    ReadToken();
    if( is_not_symbol("+") && is_not_symbol("-") ){
      UnReadToken();
      return t;
    }

    p = (TreeNode *)malloc(sizeof(TreeNode));
    if( p ){
      p->child[0] = t;
      p->attr.e_attr.op = tokenValueBuffer[0];

      p->nodeKind = ExpK;
      sprintf(p->nodeKindStr,"ExpK Op %s", tokenValueBuffer);

      t = p;
    }
    t->child[1] = Term();
  }
  return NULL;
}

TreeNode * Exp(){
  TreeNode * t = SimpleExp();

  ReadToken();
  if( is_not_symbol("<") && is_not_symbol(":=") && is_not_symbol(">") ){
    UnReadToken();
    return t;
  }

  TreeNode * p = (TreeNode *)malloc(sizeof(TreeNode));

  if( p ){
    p->child[0] = t;
    p->attr.e_attr.op = tokenValueBuffer[0];

    p->nodeKind = ExpK;
    sprintf(p->nodeKindStr,"ExpK Op %s", tokenValueBuffer);

    t = p;
  }

  if( t )
    t->child[1] = SimpleExp();

  return t;
}

TreeNode * ActParamMore(){
  ReadToken();

  if( is_symbol(")") )
    return NULL;

  if( is_symbol(",") )
    return ActParamList();

  return NULL;
}

TreeNode * ActParamList(){
  if( is_symbol(")") )
    return NULL;

  TreeNode * t = Exp();

  if( t )
    t->Sibling = ActParamMore();

  return t;
}

TreeNode * AssCall(){
  TreeNode * t = (TreeNode * )malloc(sizeof(TreeNode));
  TreeNode * p = (TreeNode * )malloc(sizeof(TreeNode));

  if( p ){
    p->nodeKind = ExpK;
    strcpy(p->nodeKindStr, "ExpK");
    strcpy(p->name[p->idnum++], tokenValueBuffer);

    variMore(p);
  }

  ReadToken();
  if( is_symbol(":=") && t ){
    t->child[0] = p;
    t->child[1] = Exp();

    t->nodeKind = ExpK;
    strcpy(t->nodeKindStr,"ExpK Op :=");

    return t;
  }

  if( is_symbol("(") && t ){
    t->child[0] = p;
    t->child[1] = ActParamList();

    t->nodeKind = ExpK;
    strcpy(t->nodeKindStr,"ExpK Call");

    if( is_not_symbol(")") )
      fprintf(stderr, "Call method miss ')'\n");
    return t;
  }

  fprintf(stderr, "AssCall error\n");
  return NULL;
}

TreeNode * ReturnStm(){
  return NULL;
}

TreeNode * InputStm(){
  TreeNode * t = (TreeNode * )malloc(sizeof(TreeNode));

  ReadToken();
  if( is_not_symbol("(") )
    fprintf(stderr, "InputStm miss '('\n");

  if( t )
    t->child[0] = Variable();

  t->nodeKind = StmtK;
  strcpy(t->nodeKindStr, "StmtK Input");

  ReadToken();
  if( is_not_symbol(")") )
    fprintf(stderr, "InputStm miss ')'\n");

  return t;
}

TreeNode * OutputStm(){
  TreeNode * t = (TreeNode * )malloc(sizeof(TreeNode));

  ReadToken();
  if( is_not_symbol("(") )
    fprintf(stderr, "OutputStm miss '('\n");

  if( t )
    t->child[0] = Exp();

  t->nodeKind = StmtK;
  strcpy(t->nodeKindStr, "StmtK Write");

  ReadToken();
  if( is_not_symbol(")") )
    fprintf(stderr, "OutputStm miss ')'\n");

  return t;
}


TreeNode * LoopStm(){
  TreeNode * t = (TreeNode * )malloc(sizeof(TreeNode));
  if( t == NULL )
    return t;

  t->nodeKind = StmtK;
  strcpy(t->nodeKindStr, "StmtK While");

  t->child[0] = Exp();

  ReadToken();
  if( is_not_reversed_word("do") )
    fprintf(stderr, "LoopStm miss 'do'\n");

  t->child[1] = StmList();

  ReadToken();
  if( is_not_reversed_word("endwh") )
    fprintf(stderr, "LoopStm miss 'endwh'\n");

  return t;
}

TreeNode * ConditionalStm(){
  TreeNode * t = (TreeNode * )malloc(sizeof(TreeNode));

  t->nodeKind = StmtK;
  strcpy(t->nodeKindStr, "StmtK If");

  if( t )
    t->child[0] = Exp();

  ReadToken();
  if( is_reversed_word("then") && t)
    t->child[1] = Exp();
  else
    fprintf(stderr, "Conditional Stm miss 'then' \n");

  ReadToken();
  if( is_reversed_word("else") && t)
    t->child[2] = Exp();

  ReadToken();
  if( is_not_reversed_word("fi") )
    fprintf(stderr, "Conditional Stm miss 'fi' \n");

  return t;
}

TreeNode * Stm(){
  ReadToken();

  if( is_reversed_word("if") )
    return ConditionalStm();

  if( is_reversed_word("while") )
    return LoopStm();

  if( is_reversed_word("return") )
    return ReturnStm();

  if( is_reversed_word("write") )
    return OutputStm();

  if( is_reversed_word("read") )
    return InputStm();

  if( token_is_id ) {
    strcpy(temp_name, tokenValueBuffer);
    return AssCall();
  }

  UnReadToken();

  return NULL;
}

TreeNode * StmList(){
  TreeNode * t = NULL, * p = NULL;

  t = Stm();
  p = StmMore();

  if(p)
    t->Sibling = p;

  return t;
}

TreeNode * ProgramBody(){

  TreeNode * t = NULL;
  ReadToken();

  if( is_not_reversed_word("begin") ){
    fprintf(stderr, "Program body miss reversed word 'begin' \n");
    UnReadToken();
    return NULL;
  }

  t = (TreeNode * )malloc(sizeof(TreeNode));

  t->nodeKind = StmLK;
  strcpy(t->nodeKindStr, "StmLK");

  if(t)
    t->child[0] = StmList();

  ReadToken();

  if( is_not_reversed_word("end") ){
    fprintf(stderr, "Program body miss reversed word 'end' \n");
    UnReadToken();
    return NULL;
  }

  return t;
}
