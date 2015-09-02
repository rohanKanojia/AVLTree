#include <stdio.h>
#include <curses.h>
#include "AVLtree.h"

void curses_initializations()
{
  initscr();
  clear();
  start_color();
  curs_set(0);
  keypad(stdscr, true);
  init_pair(1, COLOR_WHITE, COLOR_GREEN);
  init_pair(2, COLOR_BLACK, COLOR_WHITE);
  init_pair(3, COLOR_WHITE, COLOR_RED);
  init_pair(4, COLOR_BLACK, COLOR_CYAN);
  init_pair(5, COLOR_RED, COLOR_WHITE);
  init_pair(6, COLOR_GREEN, COLOR_WHITE);
  refresh();
}

int main()
{
  char *menu[] = { "          Insertion              ",
                   "          Deletion               ",
                   "       Traverse Inorder          ",
                   "       Traverse Postorder        ",
                   "       Traverse Preorder         ",
                   "          Search                 ",
                   "           Exit                  ",
                   0
               };
  int r, c, opt, item, last_selected_option = 0;
  struct AVL_node *ptr, *root = NULL, *new = NULL;
  curses_initializations();
  attron(COLOR_PAIR(1) | A_BOLD);
  rectangle(0, 1, 0, 187, ' ');
  mvprintw(0, 70, "AVL Search Tree Application");
  attroff(COLOR_PAIR(2) | A_BOLD);
  attron(COLOR_PAIR(2));
  rectangle(1, 48, 0, 187, ' ');
  attroff(COLOR_PAIR(2));
  selected_row = 4;
  while(1)  {
    AVL_drawTree(&root, new, 6, 100, last_selected_option);
    switch(opt = give_Main_option(menu))  {
    case 1:   
      new = AVL_insert(&root);
      last_selected_option = 1;
      break;
    case 6:
      move(STATUS_ROW, c);
      attron(COLOR_PAIR(2));
      mvprintw(STATUS_ROW, 10, "Enter item to search : ");
      refresh();
      scanw("%d", &item);
      ptr = AVL_search_tree(root, item, 6, 100, 6);
      move(STATUS_ROW + 1, 10);
      attron(COLOR_PAIR(2));
      if(ptr != NULL)
	printw("Item found, it is at %p location", ptr);
      else 
	printw("Item not found ...");
      last_selected_option = 6;
      success();
      attroff(COLOR_PAIR(2));
      break;
    case 3:
      move(STATUS_ROW, 10);
      attron(COLOR_PAIR(2));
      printw("Inorder traversal : ");
      attroff(COLOR_PAIR(2));
      Gcol = 33;
      AVL_print_inorder(root, root, 6, 100, 6);
      attroff(COLOR_PAIR(2));
      success();
      last_selected_option = 3;
      break;
    case 4:
      move(STATUS_ROW, 10);
      attron(COLOR_PAIR(2));
      printw("Postorder traversal : ");
      attron(COLOR_PAIR(2));
      Gcol = 33;
      AVL_print_preorder(root, root, 6, 100, 6);
      last_selected_option = 4;
      success();
      break;
    case 5:
      move(STATUS_ROW, 10);
      attron(COLOR_PAIR(2));
      printw("Preorder traversal : ");
      attroff(COLOR_PAIR(2));
      Gcol = 33;
      AVL_print_preorder(root, root, 6, 100, 6);
      success();
      last_selected_option = 5;
      break;
    case 2:
      AVL_delete(&root);
      last_selected_option = 2;
      break;
    case 7:
      endwin();
      last_selected_option = 7;
      return 0;
    } 
  }
 
}
       
