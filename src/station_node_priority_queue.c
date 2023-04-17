#include <stdlib.h>
#include "station_node_priority_queue.h"

// Create a queue as a binary tree with value, left and right null
station_node_queue* create_queue(){
    station_node_queue *queue = malloc(sizeof(station_node_queue));
    queue->value = NULL;
    queue->left = NULL;
    queue->right = NULL;
    return queue;
}

// Add a station_node to the queue considering the heuristic value for its ranking
station_node_queue* add_to_queue(station_node_queue *root, station_node *value){
    if(root == NULL){
        root = create_queue();
    }
    if(root->value == NULL){
        root->value = value;
        return root;
    }
    else{
        if(value->heuristic <= root->value->heuristic){
            root->left = add_to_queue(root->left, value);
        }
        else{
            root->right = add_to_queue(root->right, value);
        }
    }
    return root;
}

// Remove the first element of the queue (the one with the lowest heuristic value) and return it
station_node* unqueue(station_node_queue **root){
    if(*root == NULL || (*root)->value == NULL){
        return NULL;
    }
    else if((*root)->left == NULL){
        station_node *value = (*root)->value;
        (*root)->value = NULL;
        return value;
    }
    else{
        station_node *value = unqueue(&(*root)->left);
        if((*root)->left->value == NULL){
            station_node_queue *tmp = (*root)->left;
            (*root)->left = (*root)->left->right;
            free(tmp);
        }
        return value;
    }
}

// Check if the queue is empty
bool queue_is_empty(station_node_queue *root){
    return root->value == NULL;
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

// Free the queue memory
void free_queue(station_node_queue *root){
    if(root->left != NULL){
        free_queue(root->left);
    }
    if(root->right != NULL){
        free_queue(root->right);
    }
    free(root->value);
    free(root);
}
