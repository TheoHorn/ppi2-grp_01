#include <stdlib.h>
#include "station_node_priority_queue.h"


station_node_queue* create_queue(){
    station_node_queue *queue = malloc(sizeof(station_node_queue));
    queue->value = NULL;
    queue->left = NULL;
    queue->right = NULL;
    return queue;
}

station_node_queue* add_to_queue(station_node_queue *root, station_node *value){
    if(root == NULL){
        root = create_queue();
    }
    if(root->value == NULL){
        root->value = value;
        return root;
    }
    else{
        if(value->heuristic < root->value->heuristic){
            root->left = add_to_queue(root->left, value);
        }
        else{
            root->right = add_to_queue(root->right, value);
        }
    }
    return root;
}


station_node* unqueue(station_node_queue *root){
    if(root->left == NULL){
        station_node *value = root->value;
        root->value = NULL;
        return value;
    }
    else{
        return unqueue(root->left);
    }
}

bool queue_is_empty(station_node_queue *root){
    return root->value == NULL;
}


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

// Peut-être supprimer de la file si cout plus grand
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
}


station_node* create_station_node(station_t *station, double cost, double heuristic){
    station_node *node = malloc(sizeof(station_node));
    node->station = station;
    node->cost = cost;
    node->heuristic = heuristic;
    return node;
}

void free_queue(station_node_queue *root){
    if(root->left != NULL){
        free_queue(root->left);
    }
    if(root->right != NULL){
        free_queue(root->right);
    }
    free(root);
}
