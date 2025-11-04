typedef struct ASTnode astnode;

enum node_type {
  LIST,
  SYMBOL,
  CONSTANT,
};

astnode* create_astnode(enum node_type type, char* value);
int add_child_node(astnode* parent, astnode* child);
char* eval_node(astnode* node);
