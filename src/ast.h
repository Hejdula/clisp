typedef struct ASTnode astnode;

enum Ntype {
  LIST,
  SYMBOL,
  CONSTANT,
};

astnode* create_astnode(enum Ntype type, char* value);
int add_child_node(astnode* parent, astnode* child);
char* eval_node(astnode* node);
