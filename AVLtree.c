#include "AVLtree.h"
#include <stdio.h>
#include <curses.h>
#include <stdlib.h>

int depth(struct AVL_node *tmp)
{
  if(tmp == NULL)  return 0;
  else  {
    int lDepth = depth(tmp -> left);
    int rDepth = depth(tmp -> right);
    if(lDepth > rDepth)
      return (lDepth + 1);
    else 
      return (rDepth + 1);
  }
}

void success() 
{
  attron(COLOR_PAIR(2));
  mvprintw(20, 10, "Press any key to continue......");
  attroff(COLOR_PAIR(2));
  getch();
}

void updateBalanceFactor(struct AVL_node *ptr)
{
  if(ptr != NULL)  {
    ptr -> b_f  =  (depth(ptr -> left) - depth(ptr -> right));
    updateBalanceFactor(ptr -> left);
    updateBalanceFactor(ptr -> right);
  }
}

void LL_rotation(struct AVL_node **root, struct AVL_node *p, struct AVL_node *p_parent)
{
  struct AVL_node *q, *hold;
  q = p->left;
  hold = q->right;
  q->right = p;
  p->left = hold;
  updateParent(root, p, p_parent, q);
}

void RR_rotation(struct AVL_node **root, struct AVL_node *p, struct AVL_node *p_parent)
{
  struct AVL_node *q, *hold;
  q = p->right;
  hold = q->left;
  q->left = p;
  p->right = hold;
  updateParent(root, p, p_parent, q);
}
 
void updateParent(struct AVL_node **root, struct AVL_node *p, struct AVL_node *p_parent, struct AVL_node *q)
{
  if(p_parent != NULL)  {
    if(p_parent -> right == p)
      p_parent->right = q;
    else 
      p_parent->left = q;
  }
  else 
    *root = q;
}
  
void LR_rotation(struct AVL_node **root, struct AVL_node *p, struct AVL_node *p_parent)
{
  RR_rotation(root, p->left, p);
  LL_rotation(root, p, p_parent);
}

void RL_rotation(struct AVL_node **root, struct AVL_node *p, struct AVL_node *p_parent)
{
  LL_rotation(root, p->right, p);
  RR_rotation(root, p, p_parent);
}

void checkImbalance(struct AVL_node *ptr, struct AVL_node *parent, struct AVL_location *tmp)
{
  if(ptr != NULL)  {
    if(IS_NOT_BALANCED(ptr->b_f))  {
      tmp->p = ptr;
      tmp->p_parent = parent;
    }
    checkImbalance(ptr->left, ptr, tmp);
    checkImbalance(ptr->right, ptr, tmp);
  }
}

void manageBalance(struct AVL_node **root, struct AVL_node *new, int last_selected)
{
  struct AVL_location tmp;
  tmp.p = NULL;
  tmp.p_parent = NULL;
  checkImbalance(*root, NULL, &tmp);
  move(40, 100);
  if(tmp.p != NULL)  {
    attron(COLOR_PAIR(5));
    addch(126 | A_ALTCHARSET);
    attroff(COLOR_PAIR(5));   
    attron(COLOR_PAIR(2));
    printw("  Not Balanced");
    frame(41, 45, 80, 131);
    mvprintw(42, 83, "(Node contaning info %d is reason for imbalance)", (tmp.p)->info);
    if(last_selected == 1) 
      apply_Post_Insertion_Rotations(root, new, tmp.p, tmp.p_parent);
    else if(last_selected == 2)
      apply_Post_Deletion_Rotations(root, tmp.p, tmp.p_parent);
    attroff(COLOR_PAIR(2));
  }
  else {
    attron(COLOR_PAIR(6));
    addch(126 | A_ALTCHARSET);
    attroff(COLOR_PAIR(6));   
    attron(COLOR_PAIR(2));
    printw("  Balanced");
    frame(41, 45, 80, 131);
    mvprintw(42, 83, "No Rotations needed ...");
    attroff(COLOR_PAIR(2));
  }
}     

void apply_Post_Insertion_Rotations(struct AVL_node **root, struct AVL_node *new, struct AVL_node *ptr, struct AVL_node *ptr_parent)
{
  int rotation = 0;
  struct AVL_node *R, *L;
  R = ptr->right;
  L = ptr->left;
  if(R != NULL)  {
    if(R->left != NULL)  {
      if(AVL_simple_search(R->left, new->info))  
	rotation = RL_ROTATION;
    }
    if(R->right != NULL)  {
      if(AVL_simple_search(R->right, new->info))
	rotation = RR_ROTATION;
    }
  }
  if(L != NULL)  {
    if(L->left != NULL)  {
      if(AVL_simple_search(L->left, new->info))
        rotation = LL_ROTATION;
    }
    if(L->right != NULL)  {
      if(AVL_simple_search(L->right, new->info)) 
	rotation = LR_ROTATION;
    }
  }

  switch(rotation)  {
  case LL_ROTATION:
    mvprintw(43, 83, "LL Rotation is needed around node contaning %d", ptr->info);
    mvprintw(44, 83, "Press any key to apply rotation");
    getch();
    LL_rotation(root, ptr, ptr_parent);
    AVL_drawTree(root, new, 6, 100, 0); 
    break;
  
  case RR_ROTATION:
    mvprintw(43, 83, "RR Rotation is needed around node containing %d", ptr->info);
    mvprintw(44, 83, "Press any key to apply rotation");
    getch();
    RR_rotation(root, ptr, ptr_parent);
    AVL_drawTree(root, new, 6, 100, 0);     
    break;

  case LR_ROTATION:
    mvprintw(43, 83, "LR Rotation is needed around node containing %d", ptr->info);
    mvprintw(44, 83, "Press any key to apply rotation");
    getch();
    LR_rotation(root, ptr, ptr_parent);
    AVL_drawTree(root, new, 6, 100, 0);
    break;

  case RL_ROTATION:
    mvprintw(43, 83, "RL Rotation is needed around node containing %d", ptr->info);
    mvprintw(44, 83, "Press any key to apply rotation");
    getch();
    RL_rotation(root, ptr, ptr_parent);
    AVL_drawTree(root, new, 6, 100, 0);
    break;
  
  default:
    mvprintw(43, 83, "Control should never reach here");
    break;
  }
}
        
void apply_Post_Deletion_Rotations(struct AVL_node **root, struct AVL_node *ptr, struct AVL_node *ptr_parent)
{
  if(ptr->b_f == 2)  {   /* R0, R1 or R-1 Rotations  */
    struct AVL_node *left_child = ptr->left;
    switch(left_child->b_f)  {
    case 0:             /* R0 Rotation  */
      mvprintw(43, 83, "R0 Rotation is needed around node containing %d", ptr->info);
      mvprintw(44, 83, "Press any key to proceed ...");
      getch();
      LL_rotation(root, ptr, ptr_parent);
      AVL_drawTree(root, NULL, 6, 100, 0);
      break;
       
    case -1:           /* R-1 Rotation  */
      mvprintw(43, 83, "R-1 Rotation is needed around node containing %d", ptr->info);
      mvprintw(44, 83, "Press any key to proceed ...");
      getch();
      LR_rotation(root, ptr, ptr_parent);
      AVL_drawTree(root, NULL, 6, 100, 0);
      break;
 
    case 1:           /*R1 Rotation  */
      mvprintw(43, 83, "R1 Rotation is needed around node containing %d", ptr->info);
      mvprintw(44, 83, "Press any key to proceed ...");
      getch();
      LL_rotation(root, ptr, ptr_parent);
      AVL_drawTree(root, NULL, 6, 100, 0);
      break;
    }
  }
  else if(ptr->b_f == -2)  {  /* L0, L1 or L-1 Rotations  */
    struct AVL_node *right_child = ptr->right;
    if(ptr->right == NULL)
      mvprintw(43, 20, "ptr->right is NULL");
    switch(right_child->b_f)  {
    case 0:               /* L0 rotation  */ 
      mvprintw(43, 83, "L0 Rotation is needed around node containing %d", ptr->info);
      mvprintw(44, 83, "Press any key to proceed ...");
      getch();
      RR_rotation(root, ptr, ptr_parent);
      AVL_drawTree(root, NULL, 6, 100, 0);
      break;
      
    case 1:               /* L1 Rotations  */
      mvprintw(43, 83, "L1 Rotation is needed around node containing %d", ptr->info);
      mvprintw(44, 83, "Press any key to proceed ...");
      getch();
      RR_rotation(root, ptr, ptr_parent);
      AVL_drawTree(root, NULL, 6, 100, 0);
      break;
   
    case -1:   /* L-1 Rotations  */
      mvprintw(43, 83, "L-1 Rotations is neede around node containing %d", ptr->info);
      mvprintw(44, 83, "Press any key to proceed ...");
      getch();
      RR_rotation(root, ptr, ptr_parent);
      AVL_drawTree(root, NULL, 6, 100, 0);
      break;
    }
  }
}

struct AVL_node *AVL_insert(struct AVL_node **root) 
{
  int item, flag, r = STATUS_ROW, c = 10;
  struct AVL_node *new = NULL;
 GET:
  move(r, c);
  attron(COLOR_PAIR(2));
  printw("Enter an item :");
  flag = scanw("%d",&item);
  if(flag == 0) {
    r++;
    move(r, c);
    printw("Error : Enter integers only");
    r++;
    goto GET;
  }
  if(*root == NULL) {
    *root=(struct AVL_node *) malloc(sizeof(struct AVL_node ));
    (*root)->info=item;
    (*root)->left=NULL;
    (*root)->right=NULL;
    (*root)->b_f = 0;
    success();
    return (*root);
  }
  else {
    if(AVL_search_tree(*root,item, 6, 100, 6)) {
      attron(COLOR_PAIR(2));
      move(16,10);
      printw("ERROR :item already in list");
      success();
      attroff(COLOR_PAIR(2));
      return NULL;
    }
    else {
      new=(struct AVL_node *) malloc(sizeof(struct AVL_node));
      new->info=item;
      new->left=NULL;
      new->right=NULL;
      new->b_f = 0;
      AVL_connect(*root,new,item);
      move(16,10);
      printw("Action successful");
      success();
      return new;
    }
  }
  attroff(COLOR_PAIR(2));
}

void AVL_connect(struct AVL_node *temp, struct AVL_node *new, int item) {
  if(item > temp->info)
    if(temp->right == NULL )               temp->right = new;
    else                                   AVL_connect(temp->right,new,item);
  else
    if(temp->left == NULL)                  temp->left = new;
    else                                    AVL_connect(temp->left,new,item);
}

void AVL_print_inorder(struct AVL_node *root, struct AVL_node *ptr, int row, int col, int length) {
  if(ptr != NULL) {
    AVL_print_inorder(root, ptr->left, row+length, col-length, length-1);
    attron(COLOR_PAIR(4));
    AVL_display(root, 6, 100, 6);
    mvprintw(row, col-1, " %d ",ptr->info);
    attroff(COLOR_PAIR(4));
    refresh();
    usleep(300*1000);
    attron(COLOR_PAIR(2));
    mvprintw(STATUS_ROW, Gcol, " %d", ptr->info);
    attroff(COLOR_PAIR(2));
    Gcol += 4;
    refresh();
    usleep(300*1000);
    AVL_print_inorder(root, ptr->right, row+length, col+length, length-1);
  }
}

void AVL_print_postorder(struct AVL_node *root, struct AVL_node *ptr, int row, int col, int length) {
  if(ptr != NULL) {
    AVL_print_postorder(root, ptr->left, row+length, col-length, length-1);
    AVL_print_postorder(root, ptr->right, row+length, col+length, length-1);
    attron(COLOR_PAIR(4));
    AVL_display(root, 6, 100, 6);
    mvprintw(row, col-1, " %d ",ptr->info);
    attroff(COLOR_PAIR(4));
    attron(COLOR_PAIR(2));
    mvprintw(STATUS_ROW, Gcol, " %d", ptr->info);
    attroff(COLOR_PAIR(2));
    Gcol += 4;
    refresh();
    usleep(600*1000);
  }
}

void AVL_print_preorder(struct AVL_node *root, struct AVL_node *ptr, int row, int col, int length) {
  if(ptr != NULL) {
    attron(COLOR_PAIR(4));
    AVL_display(root, 6, 100, 6);
    mvprintw(row, col-1, " %d ",ptr->info);
    attroff(COLOR_PAIR(4));
    attron(COLOR_PAIR(2));
    mvprintw(STATUS_ROW, Gcol, " %d", ptr->info);
    attroff(COLOR_PAIR(2));
    Gcol += 4;
    refresh();
    usleep(600*1000);
    AVL_print_preorder(root, ptr->left, row + length, col - length, length-1);
    AVL_print_preorder(root,ptr->right, row + length, col + length, length-1);
  }
}
struct AVL_node *AVL_search_tree(struct AVL_node *ptr, int item, int row, int col, int length) {
  if(ptr == NULL)  return ptr;
  if(ptr->info == item) {
    attron(COLOR_PAIR(4));
    mvprintw(row, col-1, " %d ",ptr->info);
    attroff(COLOR_PAIR(4));
    return ptr;
  }
  if(ptr->info > item)    return AVL_search_tree(ptr->left, item, row + length, col - length , length-1);
  if(ptr->info < item)    return AVL_search_tree(ptr->right,item, row + length, col + length, length-1);
}


struct AVL_node *AVL_simple_search(struct AVL_node *ptr, int item)
{
  if(ptr == NULL)  return ptr;
  if(ptr->info == item)
   return ptr;
  if(ptr->info > item)  return AVL_simple_search(ptr->left, item);
  if(ptr->info < item)  return AVL_simple_search(ptr->right, item);
}

void AVL_display(struct AVL_node *ptr, int row, int col, int length) 
{
  attron(COLOR_PAIR(2));
  if(ptr != NULL) {
    int i,j;
    move(row,col-1);
    attron(COLOR_PAIR(3));
    printw(" %d ",ptr->info);
    attroff(COLOR_PAIR(3));
    attron(COLOR_PAIR(2));
    printw( "(%d)", ptr->b_f);
    if(ptr->left != NULL) {
      for(i=row+1,j=col-2 ; i < row+length ; i++,j--) {
	move(i,j);
	printw(".");
      }
    }
    attroff(COLOR_PAIR(2));
    AVL_display(ptr->left, row+(length), col-(length), length-1);
    AVL_display(ptr->right, row+(length), col+(length), length-1);   
    attron(COLOR_PAIR(2));
    if(ptr->right != NULL) {
      for(i=row+1,j=col+2 ; i < row+length ; i++,j++) {
	move(i,j);
	printw(".");
      }
    }
    attroff(COLOR_PAIR(2));
  }
}              

void rectangle(int row, int max_row, int col, int max_col,char ch)
{
     int r, c;
     for(r = row; r < max_row; r++)
       for(c = col; c < max_col; c++)
           mvprintw(r, c, "%c", ch);
}

int give_Main_option(char **menu)
{
  int row = 4, col = 10, index = 0, row_max = 0, selected = 0, button = 0;
  attron(COLOR_PAIR(2) | A_BOLD | A_UNDERLINE);
  mvprintw( row - 3, col,  "Choose one of the following :::>>>>");
  attroff(A_BOLD | A_UNDERLINE);
  for(row = 4, col = 10; menu[index]; row ++, index++) 
    mvprintw(row, col, "%s ", menu[index]);
  row_max = row - 1;
  attron(A_STANDOUT);
  mvprintw(selected_row, col, "%s ", menu[selected_row - 4]);
  attroff(A_STANDOUT);
  button = getch();
  switch(button) {
  case KEY_UP:
    if(selected_row == 4)       selected_row = row_max;
    else                        selected_row --;
    break;
  case KEY_DOWN:
    if(selected_row == row_max) selected_row = 4;
    else                        selected_row ++;
    break;
  case 10:
    selected = (selected_row - 4) + 1;
  }
  return selected;
}

struct AVL_location *AVL_search_with_parent(struct AVL_node *root, struct AVL_node *ptr, struct AVL_location *loc,int item)
{
  if(ptr == NULL) 
    return loc;
  if(ptr->info == item) {
    if(ptr == root) {
      loc->p_parent = NULL;
      loc->p = ptr;
    }
    return loc;
  }
  if(ptr->info > item) {
    loc->p_parent = ptr;
    loc->p = ptr->left;
    return AVL_search_with_parent(root, ptr->left, loc, item);
  }
  if(ptr->info < item) {
    loc->p_parent = ptr;
    loc->p = ptr->right;
    return AVL_search_with_parent(root, ptr->right, loc, item);
  }
}         

void AVL_delete(struct AVL_node **root)
{
  struct AVL_location LOC, *loc;
  int item, flag, r = STATUS_ROW;
  attron(COLOR_PAIR(2));
  if(*root == NULL) {
    mvprintw(STATUS_ROW, 10, "Binary tree is empty [ Nothing to delete ]");
    success();
    return;
  }
  GOGET:
  mvprintw(r , 10, "Enter item to delete :");
  flag = scanw("%d", &item);
  if(flag == 0) {
    r++;
    move(r, 10);
    printw("Error : Enter integers only");
    r++;
    goto GOGET;
  }
  loc = AVL_search_with_parent(*root, *root, &LOC, item);
  if(loc->p == NULL) {
    mvprintw(STATUS_ROW + 1, 10, "ERROR : Item not in the tree");
    success();
    return;
  }
  if((loc->p)->right != NULL && (loc->p)->left != NULL)
    AVL_delete_two_child(root, loc->p, loc->p_parent);
  else
    AVL_delete_one_child(root, loc->p, loc->p_parent);
  free(loc->p);
  attroff(COLOR_PAIR(2));
  success();
}

void AVL_delete_one_child(struct AVL_node **root, struct AVL_node *LOC, struct AVL_node *PAR)
{
  struct AVL_node *CHILD;
  //Determinig the child ----------------------------------------------
  if(LOC->left == NULL && LOC->right == NULL)
    CHILD = NULL;
  else if(LOC->left != NULL)
    CHILD = LOC->left;
  else
    CHILD = LOC->right;
  //Replacing node by its child ---------------------------------------
  if(PAR != NULL) {
    if(LOC == PAR->left)
      PAR->left = CHILD;
    else 
      PAR->right = CHILD;
  }
  else
    *root = CHILD;
}
void AVL_delete_two_child(struct AVL_node **root, struct AVL_node *LOC, struct AVL_node *PAR)
{
    struct AVL_node *SUC, *PARSUC;
    SUC = LOC->right;
    PARSUC = LOC;
    while( SUC->left != NULL) {
         PARSUC = SUC;
         SUC = SUC->left;
    }
    // Delete inorder successor , Using delete_one_child()  method
    AVL_delete_one_child(root, SUC, PARSUC);
    // Replace node by its inorder successor
    if(PAR != NULL) {
         if(LOC == PAR->left)
             PAR->left = SUC;
         else
             PAR->right = SUC;
    }
    else
        *root = SUC;
    //Assign successor the children of node to be deleted
    SUC->left = LOC->left;
    SUC->right = LOC->right;
}

void AVL_drawTree(struct AVL_node **root, struct AVL_node *new, int startRow, int startCol, int last_selected)
{
   int c;
   attron(COLOR_PAIR(2));
   rectangle(3, 46, 50, 150, ' ');
   rectangle(15, 25, 9, 79, ' ');
   attron(A_UNDERLINE );
   mvprintw(3, 95, "AVL tree status");
   attroff(A_UNDERLINE);
   mvprintw(6, 133, "Depth : %d", depth(*root) - 1);
   mvprintw(7, 133, "Worst Search Time complexity : O(%d)  ", depth(*root) - 1);
   frame(4, 38, 80, 131);
   attroff(COLOR_PAIR(2));
   updateBalanceFactor(*root);
   AVL_display(*root, startRow, startCol, 6); 
   manageBalance(root, new, last_selected);
   attron(COLOR_PAIR(1) | A_BOLD);
   for(c = 0; c < 170; c++)  mvprintw(47, c, " ");
   move(47, 10);
   addch(ACS_UARROW);
   addch(ACS_DARROW);
   printw( " Navigate");
   move(47, 28);
   addch('<');
   addch(ACS_LRCORNER);
   mvprintw(47, 30, "Select");  
   attroff(COLOR_PAIR(1) | A_BOLD);
}

void frame(int start_row, int max_row, int start_col, int max_col)
{
    int row, col;
    for(row = start_row ; row <= max_row; row++) {
        if(row == start_row || row == max_row) {
           move(row, start_col);
           if(row == start_row)    addch(ACS_ULCORNER);
           else                    addch(ACS_LLCORNER);
           for(col = start_col + 1 ; col < max_col; col++) {
              move(row, col);
              addch(ACS_HLINE);
           }
           move(row, max_col);
           if(row == start_row)    addch(ACS_URCORNER);
           else                    addch(ACS_LRCORNER);
       }
       else {
           move(row, start_col);
           addch(ACS_VLINE);
           move(row, max_col);
           addch(ACS_VLINE);
       }
    }
}  
