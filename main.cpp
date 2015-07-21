#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

typedef void *(*method)(void *);

typedef struct Node {
    char *key;
    method val;

    Node *left, *right;
    char color;

} Node;

int isRed(const Node *const n) {
    if (n == NULL) return 0;
    return 'R' == n->color;
}

int isEmpty(const Node *const n) {
    return NULL == n;
}

method get(const Node *const root, char const *const key) {
    const Node *n = root;
    while (NULL != n) {
        int cmp = strcmp(key, n->key);
        if (cmp < 0) {
            n = n->left;
        } else if (cmp > 0) {
            n = n->right;
        } else {
            return n->val;
        }
    }
    return NULL;
}

Node *rotateRight(Node *const h) {
    Node *x = h->left;
    h->left = x->right;
    x->right = h;
    x->color = x->right->color;
    x->right->color = 'R';

    return x;
}

Node *rotateLeft(Node *const h) {
    Node *x = h->right;
    h->right = x->left;
    x->left = h;
    x->color = x->left->color;
    x->left->color = 'R';

    return x;
}

void flipColor(Node *const h) {
    h->color = 'R' == h->color ? 'B' : 'R';
    h->left->color = 'R' == h->left->color ? 'B' : 'R';
    h->right->color = 'R' == h->right->color ? 'B' : 'R';
}

Node *_put(Node *h, char const *const key, const method val) {
    if (NULL == h) {
        Node *node = (Node *) malloc(sizeof(Node));
        node->left = node->right = NULL;
        node->key = (char *) malloc(strlen(key) + 1);
        strcpy(node->key, key);
        node->val = val;
        node->color = 'R';

        return node;
    }

    int cmp = strcmp(key, h->key);
    if (cmp < 0) {
        h->left = _put(h->left, key, val);
    } else if (cmp > 0) {
        h->right = _put(h->right, key, val);
    } else {
        h->val = val;
    }

    if (isRed(h->right) && !isRed(h->left)) h = rotateLeft(h);
    if (isRed(h->left) && isRed(h->left->left)) h = rotateRight(h);
    if (isRed(h->left) && isRed(h->right)) flipColor(h);

    return h;
}

void put(Node **h, char const *const key, const method val) {
    Node *root = _put(*h, key, val);
    root->color = 'B';
    *h = root;
}

void testPrintKeys(const Node *const n) {
    if (NULL == n) return;
    testPrintKeys(n->left);
    printf("%s\n", n->key);
    testPrintKeys(n->right);
}

void *testFuncA(void *s) {
    printf("A -> %s\n", (char *) s);
    return s;
}

void *testFuncB(void *s) {
    printf("B -> %s\n", (char *) s);
    return s;
}

void *testFuncC(void *s) {
    printf("C -> %s\n", (char *) s);
    return s;
}

int main() {
    Node *root = NULL;
    char *testMessage = "Test Message";

    put(&root, "Test1A", &testFuncA);
    put(&root, "Test1C", &testFuncC);
    put(&root, "Test1B", &testFuncB);

    (*get(root, "Test1A"))(testMessage);
    (*get(root, "Test1B"))(testMessage);
    (*get(root, "Test1C"))(testMessage);

    put(&root, "Test1A", &testFuncC);
    (*get(root, "Test1A"))(testMessage);

    return 0;
}