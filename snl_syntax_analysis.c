#include <stdio.h>
#include <string.h>
#include <stdlib.h>

static FILE * fp;

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
  char childType[30];
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

  NodeKind nokeKind;
  char nokeKindStr[30];

  union kink{
    dec d;
    stmt s;
    exp e;
  };

  int idnum;
  char name[30];
  //table
  //type_name

  union attr {
    ArrayAttr a_attr;
    ExpAttr e_attr;
    procAttr p_attr;
  };

} TreeNode;

static char tokenTypeBuffer[30];
static char tokenValueBuffer[30];
static int i_tokenValueBuffer;

char * ReadToken(){
  int lineno, i;
  char type[30], chs[30];
  fscanf(fp, "%d: %s", &lineno, type);

  if( strcmp(type, "reserved") == 0 ){
    fscanf(fp, " word: %s", chs);
    // printf("%d %s\n", lineno, chs);

    strcpy(tokenTypeBuffer, "reserved_word");
    strcpy(tokenValueBuffer, chs);

  } else if ( strcmp(type, "ID,") == 0 ){
    fscanf(fp, " name = %s", chs);
    // printf("%d %s\n", lineno, chs);

    strcpy(tokenTypeBuffer, "ID");
    strcpy(tokenValueBuffer, chs);

  } else if ( strcmp(type, "INTC,") == 0 ){
    fscanf(fp, " var = %d", &i);
    // printf("%d %d\n", lineno, i);

    strcpy(tokenTypeBuffer, "INTC");
    i_tokenValueBuffer = i;

  } else if (type[0] == '\0'){
    ;
  } else {
    // printf("%d %s\n", lineno, type);
    strcpy(tokenTypeBuffer, "SYMBOL");
    strcpy(tokenValueBuffer, type);

  }

  type[0] = '\0';
  return tokenTypeBuffer;
}

void setNodeKind(TreeNode * node, NodeKind nk, char * str){
  node->nokeKind = nk;
  strcpy(node->nokeKindStr, str);
}

TreeNode * ProgramHead(){

  if( ReadToken() && strcmp(tokenValueBuffer, "program") != 0){
    fprintf(stderr, "Bad Begin(No reserved word program)\n");
    return NULL;
  }

  if( strcmp(ReadToken(), "ID") != 0){
    fprintf(stderr, "Bad program name\n");
    return NULL;
  }

  TreeNode * t = (TreeNode *)malloc(sizeof(TreeNode));
  setNodeKind(t, PheadK, "PheadK");
  strcpy(t->name, tokenValueBuffer);

  return t;
}

TreeNode * DeclarePart(){

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

  setNodeKind(root, ProK, "Prok");

  if( root == NULL)
    ;

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

  if( ReadToken() && strcmp(tokenValueBuffer, ".") != 0 )
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

}

int main(){
  fp = fopen("lexical_analysis.txt", "r");
  freopen("lexical_syntax.txt", "w", stdout);

  printf_syntax_tree(parse());

  fclose(fp);
  return 0;
}
