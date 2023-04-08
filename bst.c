#include <stdio.h>
#include <stdlib.h>

typedef struct nd {
    int v, count ;
    struct nd *l, *r, *p;
} node, *node_ptr;

node_ptr BST_newNode(int v) {
    node_ptr n = (node_ptr)malloc(sizeof(node));
    n->l = n->r = NULL;
    n->count = 1;
    n->v = v;
    return n;
}

node_ptr BST_init(int v) {
    return BST_newNode(v);
}

void BST_add(node_ptr bst, int v) {
    node_ptr iter = bst, prev = bst;
    while (iter != NULL) {
        prev = iter;
        if (v < iter->v)
            iter = iter->l;
        else if (v > iter->v)
            iter = iter->r;
        else {
            iter->count++;
            break;
        }
    }

    if (iter == NULL) {
        iter = BST_newNode(v);
        if (v < prev->v)
            prev->l = iter;
        else
            prev->r = iter;
        iter->p = prev;
    }
}

int BST_depth(node_ptr bst) {
    int depth = 1, maxDepth = 1, mov=-1;
    node_ptr iter = bst;
    while (iter->r != NULL || iter->l != NULL)
        if (iter->r != NULL)
            iter = iter->r;
        else
            iter = iter->l;
    node_ptr r_end = iter;
    iter = bst;
    while (iter != r_end) {
        if (iter->l != NULL && mov < 2) {
            iter = iter->l;
            mov = 0;
            depth++;
        }
        else if (iter->r != NULL && mov < 3) {
            iter = iter->r;
            mov = 1;
            depth++;
        }
        else if (iter->p != NULL) {
            if (iter == iter->p->l)
                mov = 2;
            else
                mov = 3;
            iter = iter->p;
            depth--;
        }
        if (depth > maxDepth)
            maxDepth = depth;
    }
    return maxDepth;
}

void BST_print(node_ptr bst) {
    int maxDepth = BST_depth(bst), depth = 1, mov = -1;
    node_ptr iter = bst;

    for (int i = 0; i < maxDepth - 1; i++)
        printf("--------");
    printf("---\n");

    if (bst->l == NULL && bst->r != NULL) {
        for (int i = 0; i < maxDepth - depth; i++)
            printf("        ");

        if (iter->count > 1)
            printf("%d: %d\n", iter->v, iter->count);
        else
            printf("%d\n", iter->v);
    }

    while (iter != bst || mov != 3) {
        if (iter->l != NULL && mov < 2) {
            iter = iter->l;
            mov = 0;
            depth++;
        }

        else if (iter->r != NULL && mov < 3) {
            iter = iter->r;
            mov = 1;
            depth++;
        }

        else if (iter->p != NULL) {
            if (iter == iter->p->l)
                mov = 2;
            else
                mov = 3;
            iter = iter->p;
            depth--;
        }

        if (mov == 2  || ((mov == 1 || mov == 0) && iter->l == NULL)) {
            for (int i = 0; i < maxDepth - depth; i++)
                printf("        ");

            if (iter->count > 1)
                printf("%d: %d", iter->v, iter->count);
            else
                printf("%d", iter->v);

            if (iter->p != NULL) {
                if (iter->p->l == iter)
                    printf("   \\\n");
                else
                    printf("   /\n");
            }
            else
                putchar('\n');

            if (iter == bst && !(bst->l != NULL && bst->r != NULL))
                break;
        }
    }
    for (int i = 0; i < maxDepth - 1; i++)
        printf("--------");
    printf("---\n");
}

node_ptr BST_search(node_ptr bst, int v) {
    if (bst == NULL)
        return NULL;
    if (bst->v == v)
        return bst;
    if (v < bst->v)
        return BST_search(bst->l, v);
    else
        return BST_search(bst->r, v);
}

node_ptr BST_remove(node_ptr bst, int v, int all) {
    node_ptr del = BST_search(bst, v);
    if (del == NULL)
        return bst;

    if (del->count > 1 && !all) {
        del->count--;
        return bst;
    }

    if (del == bst) {
        if (bst->l == NULL && bst->r == NULL) {
            free(del);
            return NULL;
        }
        if (bst->l != NULL && bst->r == NULL) {
            node_ptr temp = bst->l;
            temp->p = NULL;
            free(bst);
            return temp;
        }
        if (bst->l == NULL && bst->r != NULL) {
            node_ptr temp = bst->r;
            temp->p = NULL;
            free(bst);
            return temp;
        }
    }

    if (del->l == NULL && del->r == NULL) {
        if (del->p->l == del)
            del->p->l = NULL;
        else
            del->p->r = NULL;
        free(del);
    }

    if (del->l != NULL && del->r == NULL) {
        if (del->p->l == del)
            del->p->l = del->l;
        else
            del->p->r = del->l;
        del->l->p = del->p;
        free(del);
    }

    if (del->l == NULL && del->r != NULL) {
        if (del->p->l == del)
            del->p->l = del->r;
        else
            del->p->r = del->r;
        del->r->p = del->p;
        free(del);
    }

    if (del->l != NULL && del->r != NULL) {
        node_ptr iter = del->l;
        while (iter->r != NULL)
            iter = iter->r;
        del->count = iter->count;
        int temp = iter->v;
       bst =  BST_remove(bst, iter->v, 1);
       del->v = temp;


    }
    return bst;
}

void BST_delete(node_ptr bst) {
    if (bst->l != NULL)
        BST_delete(bst->l);
    if (bst->r != NULL)
        BST_delete(bst->r);
    free(bst);
}

int main() {
    node_ptr testBST = BST_init(18);
    BST_add(testBST, 11);
    BST_add(testBST, 6);
    BST_add(testBST, 30);
    BST_add(testBST, 21);
    BST_add(testBST, 19);
    BST_add(testBST, 8);
    BST_add(testBST, 22);
    BST_add(testBST, 23);
    BST_add(testBST, 5);
    BST_add(testBST, 20);
    BST_add(testBST, 26);
    BST_add(testBST, 17);
    BST_add(testBST, 26);
    BST_add(testBST, 30);
    BST_add(testBST, 17);
    BST_print(testBST);

    printf("Szukamy 19: %d\n", BST_search(testBST, 19)->v);
    BST_search(testBST, 111);
    testBST = BST_remove(testBST, 111, 0);

    printf("usuwamy 8:\n");
    testBST = BST_remove(testBST, 8, 0);
    BST_print(testBST);

    printf("usuwamy 30:\n");
    testBST = BST_remove(testBST, 30, 0);
    BST_print(testBST);

    printf("usuwamy 30:\n");
    testBST = BST_remove(testBST, 30, 0);
    BST_print(testBST);

    printf("usuwamy 18:\n");
    testBST = BST_remove(testBST, 18, 0);
    BST_print(testBST);

    printf("usuwamy 11:\n");
    testBST = BST_remove(testBST, 11, 0);
    BST_print(testBST);

    BST_delete(testBST);
    return 0;
}
