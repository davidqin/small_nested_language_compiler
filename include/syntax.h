#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef enum { SNL_INTERGER, SNL_CHAR, SNL_ARRAY, SNL_RECORD, SNL_REVERSE_WORD, SNL_ID, SNL_SYMBOL } SNL_TYPE;

typedef enum { ProK, PheadK, TypeK, VarK, ProcDecK, StmLK, DecK, StmtK, ExpK } NodeKind;
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
  char op;
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
    char name[30];
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

extern FILE * fp;

extern SNL_TYPE tokenType;
extern char tokenValueBuffer[30];
extern int i_tokenValueBuffer;

extern char unReadTokenBuffer[30];
extern SNL_TYPE unReadTokenType;
extern int hasUnReadToken;

void printf_syntax_tree(TreeNode * root, int dep);

TreeNode * DeclarePart();
TreeNode * ProgramBody();

int is_reversed_word(const char * );
int is_not_reversed_word(const char * );
SNL_TYPE ReadToken();
void UnReadToken();

TreeNode * TypeDec();
TreeNode * typeDeclaration();
TreeNode * VarDec();
void TypeDef(TreeNode * t);

TreeNode * TypeDecList();
TreeNode * TypeDecMore();

TreeNode * VarDecList();
void VarIdList(TreeNode * t);

int is_reversed_word(const char * chs);
int is_not_reversed_word(const char * chs);

int is_symbol(const char * chs);
int is_not_symbol(const char * chs);

TreeNode * ProcDec();
void FormList(TreeNode * t);
TreeNode * ParamDecList();

TreeNode * StmList();
TreeNode * ActParamList();
TreeNode * Exp();

#define token_is_id ( tokenType == SNL_ID )
#define token_is_not_id ( !token_is_id )
#define token_is_eof ( strcmp("EOF", tokenValueBuffer) == 0 )
#define token_is_integer ( tokenType == SNL_INTERGER )
#define pct ( printf("ct = %s\n", tokenValueBuffer) )
