#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct nd {
    int v;
    char c;
    bool sentinel;
    struct nd *l, *r, *p;
} node, *node_ptr;

node_ptr RBT_newNode(int v) {
    node_ptr n = (node_ptr)malloc(sizeof(node));
    n->l = n->r = NULL;
    n->c = 0;
    n->v = v;
    n->sentinel = false;
    return n;
}

node_ptr RBT_init(int v) {
    node_ptr n = RBT_newNode(v);
    n->c = 1;
    node_ptr sentinel = RBT_newNode(-1);
    n->l = sentinel;
    n->r = sentinel;
    sentinel->c = 1;
    sentinel->sentinel = true;
    return n;
    //node_ptr n = (node_ptr)malloc(sizeof(node));
}

node_ptr rotateRight(node_ptr rbt, node_ptr node) {
    if (node->l == NULL)
        return rbt;
    if (node != rbt) {
        if (node->p->l == node)
            node->p->l = node->l;
        else
            node->p->r = node->l;
        node->l->p = node->p;
    }
    else {
        rbt = node->l;
        rbt->p = NULL;
    }
    node->p = node->l;
    if (node->l->r != NULL) {
        node->l = node->l->r;
        node->l->p = node;
    }
    else
        node->l = NULL;
    node->p->r = node;
    return rbt;
}

node_ptr rotateLeft(node_ptr rbt, node_ptr node) {
    if (node->r == NULL)
        return rbt;
    if (node != rbt) {
        if (node->p->l == node)
            node->p->l = node->r;
        else
            node->p->r = node->r;
        node->r->p = node->p;
    }
    else {
        rbt = node->r;
        rbt->p = NULL;
    }
    node->p = node->r;
    if (node->r->l != NULL) {
        node->r = node->r->l;
        node->r->p = node;
    }
    else
        node->r = NULL;
    node->p->l = node;
    return rbt;
}

node_ptr RBT_add(node_ptr rbt, int v) {
    node_ptr iter = rbt, prev = rbt;
    while (iter != NULL && !iter->sentinel) {
        prev = iter;
        if (v < iter->v)
            iter = iter->l;
        else
            iter = iter->r;
    }
    if (iter == NULL || iter->sentinel) {
        node_ptr sentinel = v < prev->v ? prev->l : prev->r;
        iter = RBT_newNode(v);
        if (v < prev->v)
            prev->l = iter;
        else
            prev->r = iter;
        iter->p = prev;
        iter->l = sentinel;
        iter->r = sentinel;
    }
    while (iter != rbt && iter->p->c != 1) {
        if (iter->p->p->r->c == 0 && iter->p->p->l->c == 0) {
            iter->p->p->r->c = 1;
            iter->p->p->l->c = 1;
            iter->p->p->c = iter->p->p == rbt ? 1 : 0;
            iter = iter->p->p;
        }
        if (iter->p == NULL) continue;
        if (iter->p->p == NULL) continue;
        if (iter->p->p->r->c == 1 && iter->p->r == iter) {
            rbt = rotateLeft(rbt, iter->p);
            iter = iter->l;
        }
        if (iter->p->p->l->c == 1 && iter->p->l == iter) {
            rbt = rotateRight(rbt, iter->p);
            iter = iter->r;
        }
        if (iter->p->p->r->c == 1 && iter->p->l == iter) {
            rbt = rotateRight(rbt, iter->p->p);
            iter->p->c = 1;
            iter->p->r->c = 0;
        }
        else if (iter->p->p->l->c == 1 && iter->p->r == iter) {
            rbt = rotateLeft(rbt, iter->p->p);
            iter->p->c = 1;
            iter->p->l->c = 0;
        }
    }
    return rbt;
}

int RBT_maxDepth(node_ptr rbt) {
    if (rbt == NULL)
        return 0;
    if (rbt->l == NULL && rbt->r == NULL)
        return 1;
    int leftDepth = RBT_maxDepth(rbt->l) + 1;
    int rightDepth = RBT_maxDepth(rbt->r) + 1;
    return leftDepth > rightDepth ? leftDepth : rightDepth;
}

int RBT_minDepth(node_ptr rbt) {
    if (rbt == NULL)
        return 0;
    if (rbt->l == NULL && rbt->r == NULL)
        return 1;
    int leftDepth = RBT_minDepth(rbt->l) + 1;
    int rightDepth = RBT_minDepth(rbt->r) + 1;
    return leftDepth < rightDepth ? leftDepth : rightDepth;
}

int RBT_countRedNodes(node_ptr rbt) {
    if (rbt == NULL)
        return 0;
    if (rbt->l == NULL && rbt->r == NULL) {
        if (rbt->c == 0)
            return 1;
        else
            return 0;
    }
    int all = RBT_countRedNodes(rbt->l) + RBT_countRedNodes(rbt->r);
    if (rbt->c == 0) all++;
    return all;
}

void RBT_print(node_ptr rbt) {
    int maxDepth = RBT_maxDepth(rbt), depth = 1, mov = -1;
    node_ptr iter = rbt;

    for (int i = 0; i < maxDepth; i++)
        printf("--------");
    putchar('\n');

    if ((rbt->r == NULL || rbt->r->sentinel) && (rbt->l != NULL && !rbt->l->sentinel)) {
        if (iter->c == 0) printf("-%d-", iter->v);
        else printf("|%d|", iter->v);
        putchar('\n');
    }

    while (iter != rbt || mov != 3) {
        if (iter == rbt && (rbt->r == NULL || rbt->r->sentinel) && (rbt->l == NULL || rbt->l->sentinel)) {
            if (iter->c == 0) printf("-%d-", iter->v);
            else printf("|%d|", iter->v);
            putchar('\n');
            break;
        }
        if (iter->r != NULL && !iter->r->sentinel && mov < 2) {
            iter = iter->r;
            mov = 0;
            depth++;
        }

        else if (iter->l != NULL && !iter->l->sentinel && mov < 3) {
            iter = iter->l;
            mov = 1;
            depth++;
        }

        else if (iter->p != NULL) {
            if (iter == iter->p->r)
                mov = 2;
            else
                mov = 3;
            iter = iter->p;
            depth--;
        }

        if (mov == 2  || ((mov == 1 || mov == 0) && (iter->r == NULL || iter->r->sentinel))) {
            for (int i = 0; i < depth - 2; i++)
                printf("        ");
            if (iter != rbt) printf("     ");
            if (iter->p != NULL) {
                if (iter->p->r == iter)
                    printf("/   ");
                else
                    printf("\\   ");
            }
            if (iter->c == 0) printf("-%d-", iter->v);
            else printf("|%d|", iter->v);
            putchar('\n');

            if (iter == rbt && !(rbt->r != NULL && rbt->l != NULL))
                break;
        }
    }
    for (int i = 0; i < maxDepth; i++)
        printf("--------");
    putchar('\n');
}

void RBT_delete(node_ptr rbt) {
    if (rbt->l != NULL)
        RBT_delete(rbt->l);
    if (rbt->r != NULL)
        RBT_delete(rbt->r);
    free(rbt);
}

int main() {
    node_ptr testRBT = RBT_init(18);
    RBT_print(testRBT);
    testRBT = RBT_add(testRBT, 11);
    RBT_print(testRBT);
    testRBT = RBT_add(testRBT, 6);
    RBT_print(testRBT);
    testRBT = RBT_add(testRBT, 30);
    RBT_print(testRBT);
    testRBT = RBT_add(testRBT, 21);
    RBT_print(testRBT);
    testRBT = RBT_add(testRBT, 19);
    RBT_print(testRBT);
    testRBT = RBT_add(testRBT, 8);
    RBT_print(testRBT);
    testRBT = RBT_add(testRBT, 22);
    RBT_print(testRBT);
    testRBT = RBT_add(testRBT, 23);
    RBT_print(testRBT);
    testRBT = RBT_add(testRBT, 5);
    RBT_print(testRBT);
    testRBT = RBT_add(testRBT, 20);
    RBT_print(testRBT);
    testRBT = RBT_add(testRBT, 26);
    RBT_print(testRBT);
    testRBT = RBT_add(testRBT, 17);
    RBT_print(testRBT);
    testRBT = RBT_add(testRBT, 27);
    RBT_print(testRBT);
    printf("min: %d\nmax: %d\nred: %d\n", RBT_minDepth(testRBT), RBT_maxDepth(testRBT), RBT_countRedNodes(testRBT));
    RBT_delete(testRBT);
    testRBT = RBT_init(1);
    for (int i = 2; i <= 1000; i++)
        testRBT = RBT_add(testRBT, i);
    printf("min: %d\nmax: %d\nred: %d\n", RBT_minDepth(testRBT), RBT_maxDepth(testRBT), RBT_countRedNodes(testRBT));
    return 0;
}
