#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include <limits.h>
#include <unistd.h>
#include <vector>

#include "Enclave.h"
#include "Enclave_t.h"
#include "config.h"

using namespace std;

//Structure to hold a node information
struct Node
{
    int starting;
    int no_of_edges;
};


struct edge; // forward declaration
typedef vector<edge> node;
struct edge {
    long dest;
    long weight;
};

void LoadGraph(Node**, bool**, bool**, bool**, int**, int**, int, struct graph_link*, uint64_t);

void LoadGraph(
    Node **h_graph_nodes,
    bool **h_graph_mask,
    bool **h_updating_graph_mask,
    bool **h_graph_visited,
    int  **h_graph_edges,
    int  **h_cost,
    int numNodes,
    struct graph_link  *graph_data,
    uint64_t graph_size)
{
    node *graph = new node[numNodes];
    int source = 0;

    *h_graph_nodes = (Node*) malloc(sizeof(Node)*numNodes);
    *h_graph_mask = (bool*) malloc(sizeof(bool)*numNodes);
    *h_updating_graph_mask = (bool*) malloc(sizeof(bool)*numNodes);
    *h_graph_visited = (bool*) malloc(sizeof(bool)*numNodes);
    *h_cost = (int*) malloc(sizeof(int)*numNodes);

    for (long long i = 0; i < graph_size; ++i) 
    {
        graph[graph_data[i].src].push_back( edge() );
        graph[graph_data[i].src].back().dest = graph_data[i].dest;
        graph[graph_data[i].src].back().weight = graph_data[i].weight;
        graph[graph_data[i].dest].push_back( edge() );
        graph[graph_data[i].dest].back().dest = graph_data[i].src;
        graph[graph_data[i].dest].back().weight = graph_data[i].weight;
    }

    uint64_t totalEdges = 0;
    for (int i = 0; i < numNodes; ++i) {
        long long numEdges = graph[i].size();
        (*h_graph_nodes)[i].starting = totalEdges;
        (*h_graph_nodes)[i].no_of_edges = numEdges;
        (*h_graph_mask)[i] = false;
        (*h_updating_graph_mask)[i] = false;
        (*h_graph_visited)[i] = false;

        totalEdges += numEdges;
    }

    *h_graph_edges = (int*) malloc(sizeof(int)*totalEdges);

    (*h_graph_mask)[source] = true;
    (*h_graph_visited)[source] = true;

    uint64_t k = 0;
    for ( uint64_t i = 0; i < numNodes; i++ ) {
        for ( uint64_t j = 0; j < graph[i].size(); j++ ) {
            (*h_graph_edges)[k] = graph[i][j].dest;
            ++k;
        }
    }

    for(uint64_t i = 0; i < numNodes; ++i) {
        (*h_cost)[i] = -1;
    }
    (*h_cost)[source] = 0;

}

int ecall_real_main() {
    printf("Number of nodes from config file %d\n", NUM_NODES);

    Node *h_graph_nodes;
    bool *h_graph_mask;
    bool *h_updating_graph_mask;
    bool *h_graph_visited;
    int  *h_graph_edges;
    int  *h_cost;

    uint64_t graph_size = 0;
    ocall_get_edge_count(GRAPH_FILE_PATH, &graph_size);
    ocall_load_graph(GRAPH_FILE_PATH, graph_size);

    struct graph_link* graph_data = (struct graph_link*) malloc(graph_size * sizeof(struct graph_link));

    printf("Starting Location of graph_data: %u \n", graph_data);
    printf("Size of Memory to be allocated in Bytes: %lld \n", graph_size * sizeof(struct graph_link));
    printf("Graph Size: %lld \n", graph_size);

    uint64_t pos;

    for(uint64_t i = 0; i< graph_size; i+=170)
    {
        pos = ((i+170) < graph_size) ? 170 : (graph_size - i);
        ocall_get_graph_data((graph_data + i), pos, i);
    }

    LoadGraph(
        &h_graph_nodes,
        &h_graph_mask,
        &h_updating_graph_mask,
        &h_graph_visited,
        &h_graph_edges,
        &h_cost,
        NUM_NODES,
        graph_data,
        graph_size);

    bool stop;
    do
    {

        stop=false;

        for(int tid = 0; tid < NUM_NODES; tid++ )
        {
            if (h_graph_mask[tid] == true){
                h_graph_mask[tid]=false;
                for(int i=h_graph_nodes[tid].starting; i<(h_graph_nodes[tid].no_of_edges + h_graph_nodes[tid].starting); i++)
                {
                    int id = h_graph_edges[i];
                    if(!h_graph_visited[id])
                    {
                        h_cost[id]=h_cost[tid]+1;
                        h_updating_graph_mask[id]=true;
                    }
                }
            }
        }

        for(int tid=0; tid< NUM_NODES ; tid++ )
        {
            if (h_updating_graph_mask[tid] == true){
                h_graph_mask[tid]=true;
                h_graph_visited[tid]=true;
                stop=true;
                h_updating_graph_mask[tid]=false;
            }
        }
    }
    while(stop);
    
    long long total_cost = 0;
    for(int i=0;i<NUM_NODES;i++) {
        total_cost += h_cost[i];
    }

    free(h_graph_nodes);
    free(h_graph_edges);
    free(h_graph_mask);
    free(h_updating_graph_mask);
    free(h_graph_visited);
    free(h_cost);

    printf("{\"options\": \"%d\", \"status\": %d, \"output\": %lu }\n", NUM_NODES, 1, total_cost);
    return 0;
}


