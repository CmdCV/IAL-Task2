/*
 * Použití binárních vyhledávacích stromů.
 *
 * S využitím Vámi implementovaného binárního vyhledávacího stromu (soubory ../iter/btree.c a ../rec/btree.c)
 * implementujte triviální funkci letter_count. Všimněte si, že výstupní strom může být značně degradovaný
 * (až na úroveň lineárního seznamu). Jako typ hodnoty v uzlu stromu využijte 'INTEGER'.
 *
 */

#include <stdio.h>
#include <stdlib.h>

#include "../btree.h"

char _tolower(char ch) {
  if (ch >= 'A' && ch <= 'Z') {
    return ch + ('a' - 'A');
  }
  return ch;
}

/**
 * Vypočítání frekvence výskytů znaků ve vstupním řetězci.
 *
 * Funkce inicilializuje strom a následně zjistí počet výskytů znaků a-z (case insensitive), znaku
 * mezery ' ', a ostatních znaků (ve stromu reprezentováno znakem podtržítka '_'). Výstup je v
 * uložen ve stromu.
 *
 * Například pro vstupní řetězec: "abBccc_ 123 *" bude strom po běhu funkce obsahovat:
 *
 * key | value
 * 'a'     1
 * 'b'     2
 * 'c'     3
 * ' '     2
 * '_'     5
 *
 * Pro implementaci si můžete v tomto souboru nadefinovat vlastní pomocné funkce.
 */
void letter_count(bst_node_t **tree, char *input) {
  bst_init(tree);

  for (int i = 0; input[i] != '\0'; i++) {
    char key;
    if (_tolower(input[i]) >= 'a' && _tolower(input[i]) <= 'z') {
      key = _tolower(input[i]);
    } else if (input[i] == ' ') {
      key = ' ';
    } else {
      key = '_';
    }

    bst_node_content_t *value;
    int *count;
    if (bst_search(*tree, key, &value)) {
      count = (int *)(value->value);
      (*count)++;
    } else {
      count = malloc(sizeof(int));
      *count = 1;
      bst_node_content_t value = {count, INTEGER};
      bst_insert(tree, key, value);
    }
  }
}
