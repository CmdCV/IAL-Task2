/*
 * Binární vyhledávací strom — iterativní varianta
 *
 * S využitím datových typů ze souboru btree.h, zásobníku ze souboru stack.h
 * a připravených koster funkcí implementujte binární vyhledávací
 * strom bez použití rekurze.
 */

#include "../btree.h"

#include <stdio.h>
#include <stdlib.h>

#include "stack.h"

/*
 * Inicializace stromu.
 *
 * Uživatel musí zajistit, že inicializace se nebude opakovaně volat nad
 * inicializovaným stromem. V opačném případě může dojít k úniku paměti (memory
 * leak). Protože neinicializovaný ukazatel má nedefinovanou hodnotu, není
 * možné toto detekovat ve funkci.
 */
void bst_init(bst_node_t **tree) {
  *tree = NULL;
}

/*
 * Vyhledání uzlu v stromu.
 *
 * V případě úspěchu vrátí funkce hodnotu true a do proměnné value zapíše
 * ukazatel na obsah daného uzlu. V opačném případě funkce vrátí hodnotu false a proměnná
 * value zůstává nezměněná.
 *
 * Funkci implementujte iterativně bez použité vlastních pomocných funkcí.
 */
bool bst_search(bst_node_t *tree, char key, bst_node_content_t **value) {
    bst_node_t *item = tree;
    while(item != NULL){
      if(item->key == key){
        *value = item->value;
        return true;
      } else if(item->key > key){
        item = item->left;
      } else {
        item = item->right;
      }

    }
  return false;
}

/*
 * Vložení uzlu do stromu.
 *
 * Pokud uzel se zadaným klíče už ve stromu existuje, nahraďte jeho hodnotu.
 * Jinak vložte nový listový uzel.
 *
 * Výsledný strom musí splňovat podmínku vyhledávacího stromu — levý podstrom
 * uzlu obsahuje jenom menší klíče, pravý větší.
 *
 * Funkci implementujte iterativně bez použití vlastních pomocných funkcí.
 */
void bst_insert(bst_node_t **tree, char key, bst_node_content_t value) {
  while (*tree != NULL) {
    if (key == (*tree)->key) {
      (*tree)->content = value;
      return;
    } else if (key < (*tree)->key) {
      tree = &(*tree)->left;
    } else {
      tree = &(*tree)->right;
    }
  }
  *tree = (bst_node_t *)malloc(sizeof(bst_node_t));
  if (*tree != NULL) {
    (*tree)->key = key;
    (*tree)->content = value;
    (*tree)->left = NULL;
    (*tree)->right = NULL;
  }
}

/*
 * Pomocná funkce která nahradí uzel nejpravějším potomkem.
 *
 * Klíč a hodnota uzlu target budou nahrazené klíčem a hodnotou nejpravějšího
 * uzlu podstromu tree. Nejpravější potomek bude odstraněný. Funkce korektně
 * uvolní všechny alokované zdroje odstraněného uzlu.
 *
 * Funkce předpokládá, že hodnota tree není NULL.
 *
 * Tato pomocná funkce bude využita při implementaci funkce bst_delete.
 *
 * Funkci implementujte iterativně bez použití vlastních pomocných funkcí.
 */
void bst_replace_by_rightmost(bst_node_t *target, bst_node_t **tree) {
  bst_node_t *parent = NULL;
  while ((*tree)->right != NULL) {
    parent = *tree;
    tree = &(*tree)->right;
  }
  target->key = (*tree)->key;
  target->content = (*tree)->content;
  bst_node_t *temp = *tree;
  if (parent != NULL) {
    parent->right = (*tree)->left;
  } else {
    *tree = (*tree)->left;
  }
  free(temp);
}

/*
 * Odstranění uzlu ze stromu.
 *
 * Pokud uzel se zadaným klíčem neexistuje, funkce nic nedělá.
 * Pokud má odstraněný uzel jeden podstrom, zdědí ho rodič odstraněného uzlu.
 * Pokud má odstraněný uzel oba podstromy, je nahrazený nejpravějším uzlem
 * levého podstromu. Nejpravější uzel nemusí být listem.
 *
 * Funkce korektně uvolní všechny alokované zdroje odstraněného uzlu.
 *
 * Funkci implementujte iterativně pomocí bst_replace_by_rightmost a bez
 * použití vlastních pomocných funkcí.
 */
void bst_delete(bst_node_t **tree, char key) {
  bst_node_t **node = tree, *temp = NULL;
  while (*node != NULL && (*node)->key != key) {
    if (key < (*node)->key) {
      node = &(*node)->left;
    } else {
      node = &(*node)->right;
    }
  }
  if (*node == NULL)
    return;

  if ((*node)->left == NULL) {
    temp = *node;
    *node = (*node)->right;
    free(temp);
  } else if ((*node)->right == NULL) {
    temp = *node;
    *node = (*node)->left;
    free(temp);
  } else {
    bst_replace_by_rightmost(*node, &(*node)->left);
  }
}

/*
 * Zrušení celého stromu.
 *
 * Po zrušení se celý strom bude nacházet ve stejném stavu jako po
 * inicializaci. Funkce korektně uvolní všechny alokované zdroje rušených
 * uzlů.
 *
 * Funkci implementujte iterativně s pomocí zásobníku a bez použití
 * vlastních pomocných funkcí.
 */
void bst_dispose(bst_node_t **tree) {
  // Initialize the stack
  stack_bst_t bst_stack;
  stack_bst_init(&bst_stack);

  do {
    if (*tree != NULL) {                             // Root node pointer is NOT invalid
      if ((*tree)->right != NULL) {                  // Root node has a right subtree
        stack_bst_push(&bst_stack, (*tree)->right);  // Push the right subtree to the stack
      }

      // Helping variable
      bst_node_t *tmp_node = *tree;

      *tree = (*tree)->left;  // Move to the left subtree

      // Delete the correct node
      free(tmp_node);
      tmp_node = NULL;
    } else {                                // Root node pointer is invalid
      if (!stack_bst_empty(&bst_stack)) {   // The stack is not empty
        *tree = stack_bst_pop(&bst_stack);  // Pop the node from top of the stack
      }
    }
  } while ((*tree != NULL) || (!stack_bst_empty(&bst_stack)));  // Iterate until the stack gets emptied or the root pointer gets invalid
}

/*
 * Pomocná funkce pro iterativní preorder.
 *
 * Prochází po levé větvi k nejlevějšímu uzlu podstromu.
 * Nad zpracovanými uzly zavolá bst_add_node_to_items a uloží je do zásobníku uzlů.
 *
 * Funkci implementujte iterativně s pomocí zásobníku a bez použití
 * vlastních pomocných funkcí.
 */
void bst_leftmost_preorder(bst_node_t *tree, stack_bst_t *to_visit, bst_items_t *items) {
  while (tree != NULL) {
    stack_bst_push(to_visit, tree);
    bst_add_node_to_items(items, tree);
    tree = tree->left;
  }
}

/*
 * Preorder průchod stromem.
 *
 * Pro aktuálně zpracovávaný uzel zavolejte funkci bst_add_node_to_items.
 *
 * Funkci implementujte iterativně pomocí funkce bst_leftmost_preorder a
 * zásobníku uzlů a bez použití vlastních pomocných funkcí.
 */
void bst_preorder(bst_node_t *tree, bst_items_t *items) {
  stack_bst_t stack;
  stack_bst_init(&stack);
  bst_leftmost_preorder(tree, &stack, items);
  while (!stack_bst_empty(&stack)) {
    tree = stack_bst_pop(&stack);
    bst_leftmost_preorder(tree->right, &stack, items);
  }
}

/*
 * Pomocná funkce pro iterativní inorder.
 *
 * Prochází po levé větvi k nejlevějšímu uzlu podstromu a ukládá uzly do
 * zásobníku uzlů.
 *
 * Funkci implementujte iterativně s pomocí zásobníku a bez použití
 * vlastních pomocných funkcí.
 */
void bst_leftmost_inorder(bst_node_t *tree, stack_bst_t *to_visit) {
  while (tree != NULL) {
    stack_bst_push(to_visit, tree);
    tree = tree->left;
  }
}

/*
 * Inorder průchod stromem.
 *
 * Pro aktuálně zpracovávaný uzel zavolejte funkci bst_add_node_to_items.
 *
 * Funkci implementujte iterativně pomocí funkce bst_leftmost_inorder a
 * zásobníku uzlů a bez použití vlastních pomocných funkcí.
 */
void bst_inorder(bst_node_t *tree, bst_items_t *items) {
  stack_bst_t stack;
  stack_bst_init(&stack);
  bst_leftmost_inorder(tree, &stack);
  while (!stack_bst_empty(&stack)) {
    tree = stack_bst_pop(&stack);
    bst_add_node_to_items(items, tree);
    bst_leftmost_inorder(tree->right, &stack);
  }
}

/*
 * Pomocná funkce pro iterativní postorder.
 *
 * Prochází po levé větvi k nejlevějšímu uzlu podstromu a ukládá uzly do
 * zásobníku uzlů. Do zásobníku bool hodnot ukládá informaci, že uzel
 * byl navštíven poprvé.
 *
 * Funkci implementujte iterativně pomocí zásobníku uzlů a bool hodnot a bez použití
 * vlastních pomocných funkcí.
 */
void bst_leftmost_postorder(bst_node_t *tree, stack_bst_t *to_visit, stack_bool_t *first_visit) {
  while (tree != NULL) {
    stack_bst_push(to_visit, tree);
    stack_bool_push(first_visit, true);
    tree = tree->left;
  }
}

/*
 * Postorder průchod stromem.
 *
 * Pro aktuálně zpracovávaný uzel zavolejte funkci bst_add_node_to_items.
 *
 * Funkci implementujte iterativně pomocí funkce bst_leftmost_postorder a
 * zásobníku uzlů a bool hodnot a bez použití vlastních pomocných funkcí.
 */
void bst_postorder(bst_node_t *tree, bst_items_t *items) {
  stack_bst_t stack;
  stack_bool_t first_visit;
  stack_bst_init(&stack);
  stack_bool_init(&first_visit);
  bst_leftmost_postorder(tree, &stack, &first_visit);
  while (!stack_bst_empty(&stack)) {
    tree = stack_bst_pop(&stack);
    bool visited = stack_bool_pop(&first_visit);
    if (visited) {
      stack_bst_push(&stack, tree);
      stack_bool_push(&first_visit, false);
      bst_leftmost_postorder(tree->right, &stack, &first_visit);
    } else {
      bst_add_node_to_items(items, tree);
    }
  }
}
