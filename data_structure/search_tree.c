#include <stdio.h>
#include <stdlib.h>
typedef int key_t;
typedef struct tr_n_t {
    key_t          key;
    struct tr_n_t* left;
    struct tr_n_t* right;
    /* possibly additional information */
} tree_node_t;

tree_node_t* get_node(void) { return calloc(1, sizeof(tree_node_t)); }
tree_node_t* create_tree(void)
{
    tree_node_t* tmp_node;
    tmp_node       = get_node();
    tmp_node->left = NULL;
    return tmp_node;
}

void left_rotation(tree_node_t* n)
{
    tree_node_t* tmp_node;
    key_t        tmp_key;
    tmp_node       = n->left;
    tmp_key        = n->key;
    n->left        = n->right;
    n->key         = n->right->key;
    n->right       = n->left->right;
    n->left->right = n->left->left;
    n->left->left  = tmp_node;
    n->left->key   = tmp_key;
}

void right_rotation(tree_node_t* n)
{
    tree_node_t* tmp_node;
    key_t        tmp_key;
    tmp_node        = n->right;
    tmp_key         = n->key;
    n->right        = n->left;
    n->key          = n->left->key;
    n->left         = n->right->left;
    n->right->left  = n->right->right;
    n->right->right = tmp_node;
    n->right->key   = tmp_key;
}

/* non-recurvie */
void* find(tree_node_t* tree, key_t query_key)
{
    tree_node_t* tmp_node;
    if (tree->left == NULL) return NULL;

    tmp_node = tree;
    while (tmp_node->right) {
        if (query_key < tmp_node->key) {
            tmp_node = tmp_node->left;
        } else {
            tmp_node = tmp_node->right;
        }
    }
    if (tmp_node->key == query_key) {
        return tmp_node->left;
    } else {
        return NULL;
    }
}

/* recurve */
void* rfind(tree_node_t* tree, key_t query_key)
{
    if (tree->left == NULL || (tree->right == NULL && tree->key != query_key)) {
        return NULL;
    } else if (tree->right == NULL && tree->key == query_key) {
        return tree->left;
    } else {
        if (query_key < tree->key) {
            return rfind(tree->left, query_key);
        } else {
            return rfind(tree->right, query_key);
        }
    }
}

int insert(tree_node_t* tree, key_t new_key, void* new_object)
{
    tree_node_t* tmp_node;
    if (tree->left == NULL) {
        tree->left  = (tree_node_t*)new_object;
        tree->key   = new_key;
        tree->right = NULL;
    }
    tmp_node = tree;
    while (tmp_node->right) {
        if (new_key < tmp_node->key)
            tmp_node = tmp_node->left;
        else
            tmp_node = tmp_node->right;
    }
    if (tmp_node->key == new_key) {
        return -1;
    }

    /* key is distinct, now perform the insert */
    {
        tree_node_t *old_leaf, *new_leaf;
        old_leaf        = get_node();
        old_leaf->left  = tmp_node->left;
        old_leaf->key   = tmp_node->key;
        old_leaf->right = NULL;
        new_leaf        = get_node();
        new_leaf->left  = new_object;
        new_leaf->key   = new_key;
        new_leaf->right = NULL;
        if (tmp_node->key < new_key) {
            tmp_node->left  = old_leaf;
            tmp_node->right = new_leaf;
            tmp_node->key   = new_key;
        } else {
            tmp_node->left  = new_leaf;
            tmp_node->right = old_leaf;
        }
    }
    return 0;
}

void* remove(tree_node_t* tree, key_t delete_key)
{
    tree_node_t* tmp_node;
    void*        deleted_object;
    tree_node_t* upper_node = NULL;
    tree_node_t* other_node = NULL;
    if (tree->left == NULL) {
        return NULL;
    } else if (tree->right == NULL) {
        if (tree->key == delete_key) {
            deleted_object = tree->left;
            tree->left     = NULL;
            return deleted_object;
        } else
            return NULL;
    } else {
        tmp_node = tree;
        while (tmp_node->right != NULL) {
            upper_node = tmp_node;
            if (delete_key < tmp_node->key) {
                tmp_node   = upper_node->left;
                other_node = upper_node->right;
            } else {
                tmp_node   = upper_node->right;
                other_node = upper_node->left;
            }
        }

        if (tmp_node->key != delete_key) {
            return NULL;
        } else {
            upper_node->key   = other_node->key;
            upper_node->left  = other_node->left;
            upper_node->right = other_node->right;
            deleted_object    = tmp_node->left;
            return deleted_object;
        }
    }
}

tree_node_t* interval_find(tree_node_t* tree, key_t a, key_t b)
{
    tree_node_t* tr_node;
    tree_node_t *result_list, *tmp;
    result_list = NULL;
    create_stack();
    push(tree);
    while (!stack_empty()) {
        tr_node = pop();
        if (tr_node->right == NULL) {
            if (a <= tr_node->key && tr_node->key < b) {
                tmp         = get_node();
                tmp->key    = tr_node->key;
                tmp->left   = tr_node->left;
                tmp->right  = result_list;
                result_list = tmp;
            }
        } else if (b <= tr_node->key) {
            push(tr_node->left);
        } else if (tr_node->key <= a) {
            push(tr_node->right);
        } else {
            push(tr_node->left);
            push(tr_node->right);
        }
    }
    remove_stack();
    return result_list;
}

tree_node_t* make_tree(tree_node_t* list)
{
    tree_node_t *end, *root;
    if (list == NULL) {
        root       = get_node();
        root->left = root->right = NULL;
        return root;
    } else if (list->right) {
        /* code */
    }
}

int main()
{
    tree_node_t* tree = create_tree();
    if (tree == NULL) {
        printf("error in creating tree");
        return 1;
    }
    insert(tree, 1, "1");
    insert(tree, 2, "2");
    insert(tree, 3, "3");
    insert(tree, 4, "4");
    insert(tree, 5, "5");
    insert(tree, 6, "6");
    insert(tree, 7, "7");
    insert(tree, 8, "8");

    return 0;
}