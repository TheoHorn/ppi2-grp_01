#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "station_node_priority_queue.h"

# define DEBUG_MODE false
# define DEBUG_PRINT false
# define DEBUG_STATION_ID 6

// Create a root from the queue and return it
station_node_queue *create_queue(){
    station_node_queue *tmp = malloc(sizeof(station_node_queue));
    tmp->value = NULL;
    tmp->left = NULL;
    tmp->right = NULL;
    tmp->parent = NULL;
    tmp->color = BLACK;
    return tmp;
}


// Create a station_node with the given parameters and return it. The parent is null by default and the battery_after_charge is 0
station_node* create_station_node(station_t *station, double cost, double heuristic){
    station_node *node = malloc(sizeof(station_node));
    node->station = station;
    node->cost = cost;
    node->heuristic = heuristic;
    node->battery_after_charge = 0;
    node->parent = NULL;
    return node;
}

// Returns the parent of the node
station_node_queue *parent(station_node_queue *node){
    return node->parent;
}

// Returns the grandparent of the node
station_node_queue* grandparent(station_node_queue *node){
    if(node != NULL && node->parent != NULL){
        return node->parent->parent;
    }
    else{
        return NULL;
    }
}

// Returns the sibling of the node
station_node_queue* sibling(station_node_queue *node){
    if(node != NULL && node->parent != NULL){
        if(node == node->parent->left){
            return node->parent->right;
        }
        else{
            return node->parent->left;
        }
    }
    else{
        return NULL;
    }
}

// Returns the uncle of the node
station_node_queue* uncle(station_node_queue *node){
    if(node != NULL && node->parent != NULL){
        return sibling(node->parent);
    }
    else{
        return NULL;
    }
}

// Rotate the tree to the left
station_node_queue* left_rotate(station_node_queue *node){
    station_node_queue *x = node;
    station_node_queue *y = node->right;
    x->right = y->left;
    if(y->left != NULL && (y->left->left != NULL || y->left->right != NULL)){
        y->left->parent = x;
    }
    y->parent = x->parent;

    if(x->parent == NULL){
        x = y;
    }
    else if(x == x->parent->left){
        x->parent->left = y;
    }
    else{
        x->parent->right = y;
    }

    y->left = x;
    x->parent = y;
    return x;
}   

// Rotate the tree to the right
station_node_queue* right_rotate(station_node_queue *node){
    station_node_queue *x = node;
    station_node_queue *y = node->left;
    x->left = y->right;

    if(y->right != NULL && (y->right->left != NULL || y->right->right != NULL)){
        y->right->parent = x;
    }
 
    y->parent = x->parent;
    if (x->parent == NULL){
        x = y;
    }
    else if (x == x->parent->right){
        x->parent->right = y;
    }  
    else{
        x->parent->left = y;
    }
    
    y->right = x;
    x->parent = y;
    return x;
}

// Remove a node from the queue
void remove_from_queue(station_node_queue **root, station_node *value){
    // From https://www.irif.fr/~carton/Enseignement/Algorithmique/Programmation/RedBlackTree/

    # if DEBUG_PRINT
    printf("--- Open list : ---\n");
    printf("Node to remove %d\n", value->station->id);
    # endif

    if(*root == NULL || (*root)->value == NULL){
        return;
    }
    // Research of the node to remove
    station_node_queue *node = *root;
    while(node->value != value){
        if(value->heuristic < node->value->heuristic){
            if(node->left == NULL){
                return;
            }
            node = node->left;
        }
        else{
            if(node->right == NULL){
                return;
            }
            node = node->right;
        }
    }

    # if DEBUG_MODE
    assert(node->value == value);
    # endif


    if(node->left == NULL && node->right == NULL){
        // If the node has no child, we remove it from the queue
        if(node->parent == NULL){
            // If the node is the root, we set the root to NULL
            # if DEBUG_PRINT
                if(node->value->station->id == DEBUG_STATION_ID){
                        printf("\naa\n"); // TODO delete
                }
            # endif
            *root = NULL;
        }
        else if(node->parent->left == node){
            // If the node is the left child of its parent, we set the left child of its parent to NULL
            # if DEBUG_PRINT
                if(node->value->station->id == DEBUG_STATION_ID){
                        printf("\na\n"); // TODO delete
                }
            # endif
            node->parent->left = NULL;
        }
        else{
            // If the node is the right child of its parent, we set the right child of its parent to NULL
            # if DEBUG_PRINT
                if(node->value->station->id == DEBUG_STATION_ID){
                        printf("\nb\n"); // TODO delete
                }
            # endif
            node->parent->right = NULL;
        }
        free(node);
    }
    else if((node->left == NULL) != (node->right == NULL)){
        // If the node has one child, we replace it by its child
        # if DEBUG_PRINT
            if(node->value->station->id == DEBUG_STATION_ID){
                    printf("\nc\n"); // TODO delete
            }
        # endif
        if(node->left != NULL){
            // If the node has a left child, we replace it by its left child
            # if DEBUG_PRINT
                if(node->value->station->id == DEBUG_STATION_ID){
                    printf("\nca\n"); // TODO delete
                }
            # endif

            node->left->parent = node->parent;
            if(node->parent == NULL){
                // If the node is the root and has a left child, we set the root to the left child
                # if DEBUG_PRINT
                    if(node->value->station->id == DEBUG_STATION_ID){
                      printf("\ncb\n"); // TODO delete
                    }
                # endif
                node->value = node->left->value;
                if(node->left->left != NULL){
                    node->left = node->left->left;
                }
                if(node->left->right != NULL){
                    node->right = node->left->right;
                }
            }
            else if(node->parent->left == node){
                // If the node is the left child of its parent, we set the left child of its parent to the left child of the node
                # if DEBUG_PRINT
                    if(node->value->station->id == DEBUG_STATION_ID){
                        printf("\ncd\n"); // TODO delete
                    }
                # endif
                node->parent->left = node->left;
            }
            else{
                // If the node is the right child of its parent, we set the right child of its parent to the left child of the node
                # if DEBUG_PRINT
                    if(node->value->station->id == DEBUG_STATION_ID){
                        printf("\nce\n"); // TODO delete
                    }
                # endif
                node->parent->right = node->left;
            }
        }
        else{
           //node->right->parent = node->parent;
            if(node->parent == NULL){
                // If the node is the root and has a right child
                # if DEBUG_PRINT
                    if(node->value->station->id == DEBUG_STATION_ID){
                        printf("\ncf\n"); // TODO delete
                    }
                # endif
                node->value = node->right->value;
                station_node_queue *tmp = node->right;
                node->right = NULL;
                free(tmp); // error here
            }
            else if(node->parent->left == node){
                // If the node is the left child of its parent and has a right child
                # if DEBUG_PRINT
                    if(node->value->station->id == DEBUG_STATION_ID){
                       printf("\ncg\n"); // TODO delete
                    }
                # endif
                node->right->parent = node->parent;
                node->parent->left = node->left;
                
            }
            else if(node->parent->right == node){
                // If the node is the right child of its parent and has a right child
                # if DEBUG_PRINT
                    if(node->value->station->id == DEBUG_STATION_ID){
                        printf("\nch\n"); // TODO delete
                        printf("parent : %d  - parent right : %d\n", node->parent->value->station->id , node->parent->right->value->station->id);
                    }
                # endif

                node->parent->right = node->right;
                node->right->parent = node->parent;
                //free(node);
            }
            else{
                printf("Error\n");
                exit(1);
            }
        }
    }
    else{
        // If the node has two children :
        # if DEBUG_PRINT
            if(node->value->station->id == DEBUG_STATION_ID){
                printf("\n%d : gauche : %d - droite : %d", node->value->station->id, node->left->value->station->id, node->right->value->station->id);
                printf("\nd\n"); // TODO delete
            }
        # endif

        if((node->left->left != NULL || node->left->right != NULL) && (node->right->left != NULL || node->right->right != NULL)){
            // If the node has no children as a leaf, we replace its value by the minimum node of the right subtree 
            // or the maximum node of the left subtree
            station_node_queue *max_left_subtree = node->left;
            while(max_left_subtree->right != NULL){
                max_left_subtree = max_left_subtree->right;
            }
            station_node *tmp = max_left_subtree->value;
            remove_from_queue(root, max_left_subtree->value);
            node->value = tmp;  
        }
        else {
            station_node_queue *child;
            if(node->left->left != NULL || node->left->right != NULL){
                // If the right child is a leaf and not the left child
                child = node->left;
            }
            else{
                // If the left child is a leaf and not the right child
                child = node->right;
            }

            if(node->color == RED){
                // If the node is red, we can remove it without any problem
                node->value = child->value;
                node->left = child->left;
                node->right = child->right;
                free(child);
            }
            else if(node->color == BLACK && child->color == RED){
                // If the node is black and its child is red, we can remove it and color its child in black
                node->value = child->value;
                node->left = child->left;
                node->right = child->right;
                free(child);
            }
            else{
                // if the node is black and its child is black, there are multiple cases :

                // If the node is the root, there is nothing to do
                if(node->parent == NULL){
                    node->value = child->value;
                    node->left = child->left;
                    node->right = child->right;
                    free(child);
                }
                else if(sibling(node)->color == BLACK){
                    // If the sibling of the node is black, there are multiple cases :
                    station_node_queue *sib = sibling(child);
                    if(sib->left->color == BLACK && sib->right->color == BLACK){
                        // If both childrens of the sibling are black, we color the sibling in red and the node in black
                        sib->color = RED;
                        node->color = BLACK;
                    }
                    else if(sib->right->color == RED){
                        // If the right child of the sibling is red, the sibling takes the color of its parent 
                        // and the parent, the node and the right children of the sibling are colored in black.
                        // We then do a left rotation on the parent of the node
                        sib->color = node->parent->color;
                        node->parent->color = BLACK;
                        sib->right->color = BLACK;
                        node->color = BLACK;
                        left_rotate(node->parent);
                    }
                }
                else if(sibling(node)->color == RED){
                    // If the sibling of the node is red, we do a left rotation on the parent of the node
                    node->parent->color = RED;
                    left_rotate(node->parent);
                    node->parent->parent->color = BLACK;
                    // we can call the case where the sibling of the node is black 
                }
                else{
                    // throw an error
                    printf("ERROR in remove_from_queue");
                    exit(1);
                }
                //TODO maybe delete
                remove_from_queue(root, node->value);
            }
        }
    }
    
    # if DEBUG_PRINT
    print_queue(*root);
    printf("--- End of open list ---\n");
    # endif
    # if DEBUG_MODE
    assert(!is_in_queue(*root, value->station));
    # endif
}


// Remove the minimum node from the queue and return it
station_node *unqueue(station_node_queue **root){
    station_node_queue *node = *root;
    while(node->left != NULL){
        node = node->left;
    }
    station_node *value = node->value;
    remove_from_queue(root, node->value);
    return value;
}

// Insert a node in the queue
station_node_queue* insert_recursif(station_node_queue **root, station_node *value){
    # if DEBUG_PRINT
        printf("insert - a\n");
    # endif
    if(*root != NULL && (*root)->value != NULL && value->heuristic < (*root)->value->heuristic){
        // If the queue is not empty and the value is smaller than the root, we insert it in the left subtree
        # if DEBUG_PRINT
            printf("insert - b\n");
        # endif
        if((*root)->left != NULL || (*root)->right != NULL){
            // If the node is not a leaf, we insert the value in the left subtree
            # if DEBUG_PRINT
                printf("insert - ba\n");
            # endif
            insert_recursif(&(*root)->left, value);
        }
        else{
            // If the node is a leaf, we create a new node and insert it in the left subtree
            # if DEBUG_PRINT
                printf("insert - bb\n");
            # endif
            station_node_queue *node = create_queue();
            node->value = value;
            node->parent = (*root);
            node->color = RED;
            (*root)->left = node;
        }
    }
    else if(*root != NULL && (*root)->value != NULL){
        // If the queue is not empty and the value is greater or equal than the root, we insert it in the right subtree
        # if DEBUG_PRINT
            printf("insert - c\n");
        # endif
        if((*root)->left != NULL || (*root)->right != NULL){
            // If the node is not a leaf, we insert the value in the right subtree
            # if DEBUG_PRINT
                printf("insert - ca\n");
            # endif
            insert_recursif(&(*root)->right, value);
        }
        else{
            // If the node is a leaf, we create a new node and insert it in the right subtree
            # if DEBUG_PRINT
                printf("insert - cc\n");
            # endif
            station_node_queue *node = create_queue();
            node->value = value;
            node->parent = (*root);
            node->color = RED;
            (*root)->right = node;
        }
    }
    else{
        // If the queue is empty, we create a new node and insert it in the root
        # if DEBUG_PRINT
            printf("insert - d\n");
        # endif
        free(*root);
        *root = create_queue();
        (*root)->value = value;
        (*root)->color = BLACK;
    }

    #if DEBUG_PRINT
    printf("Insert : %d\n", value->station->id);
    #endif
    #if DEBUG_MODE
    if(root != NULL){
        assert(is_in_queue(*root, value->station));
    }
    if((*root)->left != NULL){
        assert((*root)->left != *root);
    }
    if((*root)->right != NULL){
        assert((*root)->right != *root);
    }
    #endif

    return (*root);
}

// Insert a node in the queue and repair the tree to balance it
station_node_queue* insert_repair(station_node_queue **n){
    station_node_queue *node = *n;
    if(parent(node) == NULL){
        node->color = BLACK;
    }
    else if(node->parent->color == BLACK){
        
    }
    else if(uncle(node) != NULL && uncle(node)->color == RED){
        parent(node)->color = BLACK;
        uncle(node)->color = BLACK;
        station_node_queue *gp = grandparent(node);
        gp->color = RED;
        node = insert_repair(&gp);
    }
    else{
        station_node_queue *p = parent(node);
        station_node_queue *gp = grandparent(node);

        if (node == gp->left->right) {
            left_rotate(p);
            node = node->left;
        } 
        else if (node == gp->right->left) {
            right_rotate(p);
            node = node->right; 
        }

        if(node == p->left){
            right_rotate(gp);
        }
        else{
            left_rotate(gp);
        }
        p->color = BLACK;
        gp->color = RED;
    }
    return node;
}

// Find the root of the tree and return it
station_node_queue* find_root(station_node_queue *node){
    if(node->parent != NULL){
        return find_root(node->parent);
    }
    else{
        return node;
    }
}

station_node * get_from_queue(station_node_queue *root, station_t *station){
        if(root == NULL || root->value == NULL){
        return NULL;
    }
    else if(root->value->station->id == station->id){
        return root->value;
    }
    else{
        station_node *tmp = get_from_queue(root->left, station);
        if(tmp == NULL){
            get_from_queue(root->right, station);
        }
        return tmp;
    }
}

// Add a node to the queue
void add_to_queue(station_node_queue **root, station_node *value){
    station_node *tmp = get_from_queue(*root, value->station);
    if(tmp != NULL){
        remove_from_queue(root, tmp);
    }
    *root = insert_recursif(root, value);
    *root = insert_repair(root);
    *root = find_root(*root);
}

// Free the queue memory
void free_queue(station_node_queue **root){
    if(*root == NULL){
        return;
    }
    station_node_queue * left = (*root)->left;
    station_node_queue * right = (*root)->right;
    station_node_queue * parent = (*root)->parent;
    free((*root)->value);
    free((*root));

    if(left != NULL){
        free_queue(&left);
    }
    if(right != NULL){
        free_queue(&right);
    }

    # if DEBUG_MODE
    //assert(parent == NULL);
    assert(left == NULL);
    assert(right == NULL);
    assert((*root) == NULL);
    # endif

}

// Check if a station is in the queue
bool is_in_queue(station_node_queue *root, station_t *station){
    if(root == NULL || root->value == NULL){
        return false;
    }
    else if(root->value->station->id == station->id){
        return true;
    }
    else{
        return is_in_queue(root->left, station) || is_in_queue(root->right, station);
    }

}


// Check if a station is in the queue with a lower cost
bool is_in_queue_with_lower_cost(station_node_queue *root, station_node *station){
    if(root == NULL || root->value == NULL){
        return false;
    }
    else if(root->value->station->id == station->station->id){
        if(root->value->cost <= station->cost){
            return true;
        }
    }
    else{
        if(is_in_queue_with_lower_cost(root->left, station))
            return true;
        else return is_in_queue_with_lower_cost(root->right, station);
    }
    return false;
}

// Check if the queue is empty
bool queue_is_empty(station_node_queue *root){
    return root == NULL || root->value == NULL;
}

int size_queue(station_node_queue *root){
    if(root == NULL || root->value == NULL){
        return 0;
    }
    else{
        return 1 + size_queue(root->left) + size_queue(root->right);
    }
}

// Print the queue
void print_queue(station_node_queue *root){
    if(root != NULL && root->value != NULL){
        print_queue(root->left);
        printf("%d : %f\n", root->value->station->id, root->value->heuristic);
        print_queue(root->right);
    }
}
