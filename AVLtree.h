#define IS_NOT_BALANCED(x)  (x == -2 || x == 2)

#define LL_ROTATION 1
#define RR_ROTATION 2
#define LR_ROTATION 3
#define RL_ROTATION 4
#define STATUS_ROW 15
struct AVL_node  {
  int info;
  struct AVL_node *right;
  struct AVL_node *left;
  int b_f;
};

struct AVL_location  {
  struct AVL_node *p;
  struct AVL_node *p_parent;
};

int Gcol;
int selected_row ;
int depth(struct AVL_node *);
void updateBalanceFactor(struct AVL_node *);
void LL_rotation(struct AVL_node **, struct AVL_node *, struct AVL_node *);
void RR_rotation(struct AVL_node **, struct AVL_node *, struct AVL_node *);
void updateParent(struct AVL_node **, struct AVL_node *, struct AVL_node *, struct AVL_node*);
void LR_rotation(struct AVL_node **, struct AVL_node *, struct AVL_node *);
void RL_rotation(struct AVL_node **, struct AVL_node *, struct AVL_node *);
void checkImbalance(struct AVL_node *, struct AVL_node *, struct AVL_location *tmp);
struct AVL_node  *AVL_insert(struct AVL_node **);
void AVL_connect(struct AVL_node *, struct AVL_node *, int);
void AVL_print_inorder(struct AVL_node *, struct AVL_node *, int, int, int);
void AVL_print_postorder(struct AVL_node *, struct AVL_node *, int, int, int);
void AVL_print_preorder(struct AVL_node *, struct AVL_node *, int, int, int);
struct AVL_node *AVL_search_tree(struct AVL_node *, int, int, int, int);
struct AVL_node *AVL_simple_search(struct AVL_node *, int);
void AVL_display(struct AVL_node *, int, int, int);
void rectangle(int, int, int, int ,char);
int give_Main_option(char **);
struct AVL_location* AVL_search_with_parent(struct AVL_node *, struct AVL_node *, struct AVL_location *, int );
void AVL_delete(struct AVL_node **);
void AVL_delete_one_child(struct AVL_node **root, struct AVL_node *, struct AVL_node *);
void AVL_delete_two_child(struct AVL_node **, struct AVL_node *, struct AVL_node *);
void manageBalance(struct AVL_node **root, struct AVL_node *new, int last_selected);
void AVL_drawTree(struct AVL_node **, struct AVL_node *, int ,int, int);
void apply_Post_Insertion_Rotations(struct AVL_node **, struct AVL_node *, struct AVL_node *, struct AVL_node *);
void apply_Post_Deletion_Rotations(struct AVL_node **, struct AVL_node *, struct AVL_node *);
void frame(int, int, int ,int);
