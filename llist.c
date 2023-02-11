#include "llist.h"
#include <stdlib.h>
#include <assert.h>
#include <printf.h>
#include <string.h>

#ifndef DEBUG
#define DEBUG 0
#endif

int tests();
int rubric_tests();

struct node *node_alloc(int value) {
    struct node *allocated_node = (struct node *) malloc(sizeof(struct node));
    allocated_node->value = value;
    allocated_node->next = NULL;
    return allocated_node;
}

void node_free(struct node *n) {
    n->next = NULL;
    n->value = '\0';
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
        printf("[empty]\n");
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
    // NULL checking
    if (!*head) {
        return NULL;
    }
    struct node *old_head = *head;
    *head = old_head->next;

    return old_head;
}

void llist_insert_tail(struct node **head, struct node *n) {
    if (!*head) {
        *head = n;
        return;
    }

    struct node *tail = *head;
    while (tail->next) {
        tail = tail->next;
    }

    tail->next = n;
}

void llist_free(struct node **head) {
    if (!(*head)) {
        return;
    }
    struct node *next = (*head)->next;

    while (next) {
        node_free(*head);
        *head = NULL;
        *head = next;
        next = (*head)->next;
    }
    node_free(*head);
    *head = NULL;
}

int main(int argc, char *argv[]) {

    if (DEBUG) tests();
    rubric_tests();

    if (argc < 2){
        perror("use more arguments");
        exit(EXIT_FAILURE);
    }
    struct node *list_head = NULL;


    argv++;

    while(*argv){
        if (!strcmp(*argv,"ih")) {
            argv++;
            llist_insert_head(&list_head, node_alloc(atoi(*argv)));
        } else if (!strcmp(*argv,"it")) {
            argv++;
            llist_insert_tail(&list_head, node_alloc(atoi(*argv)));
        } else if (!strcmp(*argv, "dh")) {
            if(list_head) llist_delete_head(&list_head);
        }
        else if (!strcmp(*argv, "f")) {
            llist_free(&list_head);
        }
        else if (!strcmp(*argv,"p")){
            llist_print(list_head);
        }
        argv++;
    }
}

int tests() {

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

    llist_free(&link_list2);
    llist_print(link_list2);



    return 0;
}

int rubric_tests() {

    //llist_insert_head functions on empty list
    {
        struct node *empty_linked_list = NULL;
        struct node *test_node = node_alloc(1);
        llist_insert_head(&empty_linked_list, test_node);
        assert(empty_linked_list == test_node);
    }

    //llist_insert_head functions on non-empty list
    {
        struct node *non_empty_linked_list = node_alloc(1);
        struct node *test_node = node_alloc(2);
        llist_insert_head(&non_empty_linked_list, test_node);
        assert(non_empty_linked_list == test_node);
    }

    //llist_delete_head returns NULL on empty list
    {
        struct node *empty_linked_list = NULL;
        assert(llist_delete_head(&empty_linked_list) == NULL);
    }

    //llist_delete_head functions on non-empty list
    {
        struct node *chopable_head_node = node_alloc(1);
        struct node *non_empty_linked_list = chopable_head_node;

        struct node *incumbent_head_node= node_alloc(2);
        llist_insert_tail(&non_empty_linked_list, incumbent_head_node);

        assert((llist_delete_head(&non_empty_linked_list) == chopable_head_node) &&
            non_empty_linked_list == incumbent_head_node);
    }

    //llist_insert_tail functions on empty list
    {
        struct node *empty_linked_list = NULL;
        struct node *test_node = node_alloc(1);

        llist_insert_tail(&empty_linked_list, test_node);

        assert(empty_linked_list == test_node);
    }

    //llist_insert_tail functions on non-empty list
    {
        struct node *head = node_alloc(1);
        struct node *non_empty_linked_list = head;

        struct node *insertable_tail = node_alloc(9);
        llist_insert_tail(&non_empty_linked_list, insertable_tail);

        assert(non_empty_linked_list->next == insertable_tail);
    }

    //llist_free frees all nodes
    {
        struct node *head_node = node_alloc(1);
        struct node *body_node = node_alloc(2);
        struct node *tail_node = node_alloc(3);

        struct node *non_empty_linked_list = head_node;
        non_empty_linked_list->next = body_node;
        non_empty_linked_list->next->next = tail_node;

        llist_free(&non_empty_linked_list);

        //I couldn't get this to work. I think I would have to change the way the functions are defined though.
        //assert(head_node == NULL &&
        //    body_node == NULL &&
        //    tail_node == NULL);
    }

    //llist_free sets head to NULL
    {
        struct node *head_node = node_alloc(1);

        struct node *non_empty_linked_list = head_node;

        llist_free(&non_empty_linked_list);
        assert(non_empty_linked_list == NULL);

        //This test made me refactor my code. +1 for TDD.
    }

    //llist_free works on empty list
    {
        struct node *empty_linked_list = NULL;
        llist_free(&empty_linked_list);

        assert(empty_linked_list == NULL);
    }

    //llist_print prints correctly
    {
        struct node *linked_list = node_alloc(1);
        linked_list->next = node_alloc(2);

        //I think I need to fork a process and redirect std out to a string I can test
        //That might be too much.
    }

    //node_alloc properly allocates and initializes a node
    {
        struct node *test_node = node_alloc(1);
        assert(test_node->next == NULL && test_node->value == 1);
    }

    //node_free properly frees the node
    {
        struct node *the_gordon_freeman_of_nodes = node_alloc(3);
        node_free(the_gordon_freeman_of_nodes);

        //How do I test this?
    }

    //command line 'ih' command functions properly &&
    //command line 'ih' command functions properly &&
    //command line 'ih' command functions properly &&
    //command line 'ih' command functions properly &&
    //command line 'ih' command functions properly
    // How do I test command line arguments from within C?
    return 0;
}

