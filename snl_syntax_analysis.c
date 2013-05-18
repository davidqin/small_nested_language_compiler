#include <stdio.h>
#include <string.h>
#include <stdlib.h>

static FILE * fp;

typedef enum { SNL_INTERGER, SNL_CHAR, SNL_ARRAY, SNL_RECORD, SNL_REVERSE_WORD, SNL_ID, SNL_SYMBOL } SNL_TYPE;

typedef enum { ProK, PheadK, TypeK, VarK, ProcDecK, StmLK, Deck, StmtK, ExpK } NodeKind;
typedef enum { ArrayK, CharK, IntegerK, RecordK, IdK } dec;
typedef enum { IfK, WhileK, AssignK, ReadK, WriteK, CallK, ReturnK } stmt;
typedef enum { OpK, ConstK, IdEK } exp;
typedef enum { valparamtype, varparamtype } ProcAttrType;
typedef enum { IdV, ArrayMembV, FieldMembV } varKind;
typedef enum { Void, Integer, Boolean } ExpType;

typedef struct array {
  int low;
  int up;
  dec childType;
} ArrayAttr;

typedef struct proc {
  ProcAttrType paramt;
} procAttr;

typedef struct expAttr {
  //op
  int val;
  varKind varkind;
  ExpType type;
} ExpAttr;

typedef struct node {
  struct node * child[3];
  struct node * Sibling;
  int Lineno;

  NodeKind nodeKind;
  char nodeKindStr[30];

  union k {
    dec d;
    stmt s;
    exp e;
  } kind;

  int idnum;
  char name[10][30];
  //table
  char type_name[30];

  union a {
    ArrayAttr a_attr;
    ExpAttr e_attr;
    procAttr p_attr;
  } attr;

} TreeNode;

static SNL_TYPE tokenType;
static char tokenValueBuffer[30];
static int i_tokenValueBuffer;

SNL_TYPE ReadToken(){
  int lineno, i;
  char type[30], chs[30];
  fscanf(fp, "%d: %s", &lineno, type);

  if( strcmp(type, "reserved") == 0 ){
    fscanf(fp, " word: %s", chs);
    // printf("%d %s\n", lineno, chs);

    tokenType = SNL_REVERSE_WORD;
    strcpy(tokenValueBuffer, chs);

  } else if ( strcmp(type, "ID,") == 0 ){
    fscanf(fp, " name = %s", chs);
    // printf("%d %s\n", lineno, chs);

    tokenType = SNL_ID;
    strcpy(tokenValueBuffer, chs);

  } else if ( strcmp(type, "INTEGER,") == 0 ){
    fscanf(fp, " var = %d", &i);
    // printf("%d %d\n", lineno, i);

    tokenType = SNL_INTERGER;
    i_tokenValueBuffer = i;

  } else if (type[0] == '\0'){
    ;
  } else {
    // printf("%d %s\n", lineno, type);
    tokenType = SNL_SYMBOL;
    strcpy(tokenValueBuffer, type);

  }

  type[0] = '\0';
  return tokenType;
}

TreeNode * ProgramHead(){

  if( (ReadToken() != SNL_REVERSE_WORD) || strcmp(tokenValueBuffer, "program") != 0){
    fprintf(stderr, "Bad Begin(No reserved word program)\n");
    return NULL;
  }

  if( ReadToken() != SNL_ID ){
    fprintf(stderr, "Bad program name\n");
    return NULL;
  }

  TreeNode * t = (TreeNode *)malloc(sizeof(TreeNode));

  t->nodeKind = PheadK;
  strcpy(t->nodeKindStr, "PheadK");
  strcpy(t->name[0], tokenValueBuffer);

  return t;
}

TreeNode * VarDec(){
  printf("%s", tokenValueBuffer);
  return NULL;
}

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

  return NULL;
}

TreeNode * ProgramBody(){

  return NULL;
}

TreeNode * Program(){
  TreeNode *t, *q, *s, *root;
  t = ProgramHead();
  q = DeclarePart();
  s = ProgramBody();

  root = (TreeNode *)malloc(sizeof(TreeNode));

  root->nodeKind = ProK;
  strcpy(root->nodeKindStr, "ProK");

  if( root == NULL)
    return NULL;

  if(t == NULL){
    fprintf(stderr, "No Program Head\n");
  }
  else
    root->child[0] = t;

  if(q != NULL)
    root->child[1] = q;

  if(s == NULL){
    fprintf(stderr, "No Program Body\n");
  }
  else
    root->child[2] = s;

  if( ReadToken() != SNL_SYMBOL || strcmp(tokenValueBuffer, ".") != 0 )
    fprintf(stderr, "No Program End Symbol(Dot) \n");;

  return root;
}

TreeNode * parse(){
  int i = 0;

  TreeNode * t;
  t = Program();

  if( ReadToken() && strcmp(tokenValueBuffer, "EOF") != 0){
    fprintf(stderr, "Bad END\n");
    return NULL;
  }

  return t;
}

static int dep = 0;

void printf_syntax_tree(TreeNode * root){
  int i;
  for(i = 0; i < dep * 4; ++i)putchar(' ');

  // printf("%s", root->nodeKindStr);
  // if(root->type_name)printf(" %s", root->type_name);
  // printf("\n");

  for(i = 0; i < 3; ++i){
    if(root->child[i]){
      dep++;
      printf_syntax_tree(root->child[i]);
      dep--;
    }
  }

}

int main(){
  fp = fopen("lexical_analysis.txt", "r");
  // freopen("lexical_syntax.txt", "w", stdout);

  printf_syntax_tree(parse());

  fclose(fp);
  return 0;
}
