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

extern FILE * fp;

extern SNL_TYPE tokenType;
extern char tokenValueBuffer[30];
extern int i_tokenValueBuffer;

extern char unReadTokenBuffer[30];
extern SNL_TYPE unReadTokenType;

int is_reversed_word(const char * );
int is_not_reversed_word(const char * );
SNL_TYPE ReadToken();
void UnReadToken();
