/**
 * Read a list of numbers, sort it, and print it out.
 *
 * @author Quinn Morrissey
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

/** The name of this program. */
char *prog;

/**
 * A node in a doubly-linked list.
 *
 * The following diagram illustrates a single node.
 *
 *                    +------+
 *                    | -0.3 |
 *                    | next | ---> pointer to a
 * pointer to a  <--- | prev |      next node
 * previous node      +------+
 */
struct node {
    double value;       ///< A value
    struct node *next;  ///< Pointer to the next node
    struct node *prev;  ///< Pointer to the previous node
};

/**
 * A doubly-linked list.
 *
 * This doubly-linked list has dummy head and tail to simplify
 * insertion, deletion, and swapping.
 *
 * The following diagram illustrates a singleton list.
 *
 *           +------+      +------+      +------+
 *   list -> | dummy|      | -0.3 |      | dummy|
 *           | next | ---> | next | ---> | next | ---> NULL
 * NULL <--- | prev | <--- | prev | <--- | prev |
 *           +------+      +------+      +------+
 */
struct list {
    struct node head;  ///< Dummy head
    struct node tail;  ///< Dummy tail
};

struct list *list_new();
void list_add(struct list *, double);
void list_swap(struct node *);
void read(struct list *);
void sort(struct list *);
void print(struct list *);

int main(int argc, char *argv[]) {
    prog = argv[0];
    struct list *l = list_new();
    read(l);
    //sort(l);
    print(l);
}

/**
 * Create a new empty doubly-linked list.
 * @return a new empty list
 */
struct list *list_new() {
  struct list *newList = (struct list *)malloc(sizeof(struct list));
  struct node *start = (struct node *)malloc(sizeof(struct node));
  struct node *end = (struct node *)malloc(sizeof(struct node));
  start->next = end;
  end->prev = start;
  newList->head = *start;
  newList->tail = *end;
  return newList;
}

/**
 * Insert a value at the end of the doubly-linked list.
 * @param l  Pointer to a valid doubly-linked list
 * @param d  Value to add
 */
void list_add(struct list *l, double d) {
  struct node *tail = &l->tail;
  struct node *newNode = (struct node *)malloc(sizeof(struct node));
  newNode->value = d;
  newNode->next = tail;
  newNode->prev = tail->prev;
  tail->prev->next = newNode;
  tail->prev = newNode;
  free(newNode);
}

/**
 * Swap this node with the next.
 * @param n  the node to swap with its next neighbor
 */
void list_swap(struct node *n) {
    struct node *next = n->next;
    next->next = next;
    next->prev = n->prev;
    n->next = n->next->next;
    n->prev = n;
    n = n->next;
    next = n->prev;
    n->prev = next;
    free(next);
}

/**
 * Read values into a list.
 * @param l  Pointer to a valid doubly-linked list
 * Read values from standard input and insert them at the end of the
 * list.  Stop when EOF is encountered.
 */
void read(struct list *l) {
  struct node *tail = &l->tail;
  struct node *tailVal = tail->prev;
  
  double x;
  while (scanf("%lf", &x) != EOF) {
    struct node *next = (struct node *)malloc(sizeof(struct node));
    next->prev = tailVal;
    next->value = x;
    next->next = tail;
    tailVal->next = next;
    tail->prev = next;
  }
}

/**
 * Sort the given list in ascending order.
 * @param l  Pointer to a valid doubly-linked list to sort
 */
void sort(struct list *l) {
    struct node *last = &l->tail;
    while (1) {
        int inorder = 1;
        struct node *n = l->head.next;
        while (n != last) {
            struct node *next = n->next;
            if (next == last) break;
            if (n->value > next->value) {
                list_swap(n);
                inorder = 0;
            }
            n = n->next;
        }
        if (inorder) break;
    }
}

/**
 * Print each value of the list, one per line to standard output
 * @param l  Pointer to valid doubly-linked list to print
 */
void print(struct list *l) {
  struct node *start = (struct node *)malloc(sizeof(struct node));
  start = &l->head;
  bool end = false;
  while (!end) {
    printf("%lf\n",start->value);
    struct node *next = (struct node *)malloc(sizeof(struct node));
    next = start->next;
    start = next;
    if (start->value) end = true;
    
  }
}
