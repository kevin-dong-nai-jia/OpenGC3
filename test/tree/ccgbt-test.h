#include "../../src/tree/ccgbt.h"

typedef ccgbt_struct(char) *tree_t; typedef struct CCGBT_ITER iter_t;
typedef ccgbt_trav_t trav_t;

void tree_init        (tree_t *tree) { ccgbt_init(*tree); }
void tree_free        (tree_t *tree) { ccgbt_free(*tree); }

void tree_iter_init   (iter_t *iter,
                       tree_t *tree) { ccgbt_iter_init(iter, *tree); }
void tree_iter_root   (iter_t *iter) { ccgbt_iter_root(iter); }
void tree_iter_left   (iter_t *iter) { ccgbt_iter_left(iter); }
void tree_iter_right  (iter_t *iter) { ccgbt_iter_right(iter); }
void tree_iter_parent (iter_t *iter) { ccgbt_iter_parent(iter); }
void tree_iter_inorder(iter_t *iter,
                       trav_t *stat) { ccgbt_iter_inorder(iter, stat); }

void tree_push_left   (iter_t *iter, char ch) { ccgbt_push_left(iter, ch); }
void tree_push_right  (iter_t *iter, char ch) { ccgbt_push_right(iter, ch); }
void tree_insert_left (iter_t *iter, char ch) { ccgbt_insert_left(iter, ch); }
void tree_insert_right(iter_t *iter, char ch) { ccgbt_insert_right(iter, ch); }
