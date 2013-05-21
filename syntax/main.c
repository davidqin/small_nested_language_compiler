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

  ReadToken();
  if( is_not_symbol(".") )
    fprintf(stderr, "No Program End Symbol(Dot) \n");;

  return root;
}

TreeNode * parse(){
  int i = 0;

  TreeNode * t;
  t = Program();

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
  // printf("%s", root->nodeKindStr);
  printf_syntax_tree(root, 0);

  // printf("%s\n",root->nodeKindStr);
  // printf("----%s\n",root->child[0]->nodeKindStr);
  // printf("----%s\n",root->child[1]->nodeKindStr);
  // printf("--------%s %s %s\n",root->child[1]->child[0]->nodeKindStr, root->child[1]->child[0]->kind.name, root->child[1]->child[0]->name[0]);

  // printf("----%s\n",root->child[1]->Sibling->child[1]->nodeKindStr);
  // printf("----%s\n",root->child[1]->Sibling->Sibling->child[1]->nodeKindStr);
  // printf("----%s\n",root->child[1]->Sibling->Sibling->child[1]->Sibling->nodeKindStr);
  // printf("----%s\n",root->child[1]->Sibling->Sibling->child[2]->nodeKindStr);

  // printf("%d",root->child[1]->child[0]->idnum);

  fclose(fp);
  return 0;
}
