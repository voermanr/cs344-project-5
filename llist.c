#include "llist.h"
#include <stdlib.h>
#include <assert.h>
#include <printf.h>

struct node *node_alloc(int value) {
    struct node *allocated_node = (struct node *) malloc(sizeof(struct node));
    allocated_node->value = value;
    allocated_node->next = NULL;
    return allocated_node;
}

void node_free(struct node *n) {
    free(n);
}

void llist_print(struct node *head) {
    if(head) {
        printf("%i",head->value);

        struct node *next = head->next;
        while (next) {
            printf(" -> %i", next->value);
            next = next->next;
        }
        printf("\n");
    }
    else
        printf("[empty]");
}

void llist_insert_head(struct node **head, struct node *n) {
    //Error checking
    if (n->value) {
        struct node *old_head = *head;

        *head = n;
        n->next = old_head;
    }
}

struct node *llist_delete_head(struct node **head) {
    struct node *old_head = *head;
    *head = old_head->next;

    return old_head;
}

void llist_insert_tail(struct node **head, struct node *n) {
    struct node *tail = *head;
    while (tail->next) {
        tail = tail->next;
    }

    tail->next = n;
}

int main() {

    //Structure Tests
    assert(node_alloc(69)->value == 69);
    assert(node_alloc(1)->next == NULL);

    struct node *test_node = node_alloc(69);
    node_free(test_node);

    struct node *link_list = node_alloc(69);
    link_list->next = node_alloc(420);
    link_list[1].next = node_alloc(1);

    llist_print(link_list);

    struct node *link_list2 = NULL;

    llist_insert_head(&link_list2, node_alloc(69));
    llist_insert_head(&link_list2, node_alloc(420));
    llist_print(link_list2);

    node_free(llist_delete_head(&link_list2));
    llist_print(link_list2);
    llist_insert_tail(&link_list2, node_alloc(10));
    llist_print(link_list2);
}
