#include "../include/syntax.h"

FILE * fp;

TreeNode * ProgramHead(){

  ReadToken();

  if( is_not_reversed_word("program") ){
    fprintf(stderr, "Bad Begin(No reserved word program)\n");
  }

  if( ReadToken() != SNL_ID ){
    fprintf(stderr, "Bad program name\n");
  }

  TreeNode * t = (TreeNode *)malloc(sizeof(TreeNode));

  t->nodeKind = PheadK;
  strcpy(t->nodeKindStr, "PheadK");
  strcpy(t->name[t->idnum++], tokenValueBuffer);

  return t;
}

TreeNode * Program(){
  TreeNode *t = NULL, *q = NULL, *s = NULL, *root = NULL;
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
  else
    root->child[1] = NULL;

  if(s == NULL){
    fprintf(stderr, "No Program Body\n");
  }
  else
    root->child[2] = s;

  ReadToken();

  if( is_not_symbol(".") )
    fprintf(stderr, "No Program End Symbol(Dot) \n");;

  return root;
}

TreeNode * parse(){
  int i = 0;

  TreeNode * t = Program();

  ReadToken();
  if( !token_is_eof ){
    fprintf(stderr, "Bad END\n");
    return NULL;
  }

  return t;
}

int main(){
  fp = fopen("results/lexical_analysis.txt", "r");
  // freopen("../results/lexical_syntax.txt", "w", stdout);
  TreeNode * root = parse();

  printf_syntax_tree(root, 0);

  fclose(fp);
  return 0;
}
