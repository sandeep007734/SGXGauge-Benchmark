#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include <limits.h>
#include <unistd.h>
#include <vector>
#include <fcntl.h> 
#include <sys/stat.h>

#include "common_rand.h"
#include "common.h"
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
    unsigned long dest;
    unsigned weight;
};

void InitializeGraph(Node**, bool**, bool**, bool**, int**, int**, int, char*, char*);

void Usage(char**argv) {
    fprintf(stdout,"Usage: %s <num_nodes>\n", argv[0]);
}

struct graph_links {
    long src;
    long dest;
    long weight;
};


void LoadGraph(
    Node **h_graph_nodes,
    bool **h_graph_mask,
    bool **h_updating_graph_mask,
    bool **h_graph_visited,
    int  **h_graph_edges,
    int  **h_cost,
    int numNodes,
    struct graph_links  *graph_data,
    long long graph_size){
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

    long long totalEdges = 0;
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

    long k = 0;
    for ( long long i = 0; i < numNodes; i++ ) {
        for ( long j = 0; j < graph[i].size(); j++ ) {
            (*h_graph_edges)[k] = graph[i][j].dest;
            ++k;
        }
    }

    for(int i = 0; i < numNodes; ++i) {
        (*h_cost)[i] = -1;
    }
    (*h_cost)[source] = 0;

}

void reset_graph(
    bool **h_graph_mask,
    bool **h_updating_graph_mask,
    bool **h_graph_visited,
    int  **h_cost,
    int numNodes){
    int source = 0;

    for (int i = 0; i < numNodes; ++i) {
        (*h_graph_mask)[i] = false;
        (*h_updating_graph_mask)[i] = false;
        (*h_graph_visited)[i] = false;
    }

    (*h_graph_mask)[source] = true;
    (*h_graph_visited)[source] = true;

    for(int i = 0; i < numNodes; ++i) {
        (*h_cost)[i] = -1;
    }
    
    (*h_cost)[source] = 0;
}

int main( int argc, char** argv) {
    char *vertice_file_path = NULL;
    char *edge_file_path = NULL;
    char *cost_file_path = NULL;
    unsigned int expected_no_of_nodes = 3000000;
    unsigned long int expected_total_cost = 26321966;
    vertice_file_path = GRAPH_FILE_PATH;

    Node *h_graph_nodes;
    bool *h_graph_mask;
    bool *h_updating_graph_mask;
    bool *h_graph_visited;
    int  *h_graph_edges;
    int  *h_cost;
    stopwatch sw1, sw2;

    struct stat buf;
    unsigned long total_cost = 0;

    struct timeval stop_time, start;
    gettimeofday(&start, NULL);

    int fd=open(vertice_file_path,O_RDONLY);

    FILE *fp = fopen(vertice_file_path, "r");

    fstat(fd, &buf);

    long graph_size = buf.st_size / sizeof(struct graph_links);

    struct graph_links* graph_data = (struct graph_links*) malloc(graph_size * sizeof(struct graph_links));

    struct graph_links input;
    long i = 0;
    while(fread(&input, sizeof(struct graph_links), 1, fp))
    {
        graph_data[i].src = input.src ;
        graph_data[i].dest = input.dest ;
        graph_data[i].weight = input.weight ;
        i++;
    }

    stopwatch_start(&sw2);
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
    stopwatch_stop(&sw2);

    for (int iter=0; iter<5000; iter++)
    {
        //int count = 0;
        stopwatch_start(&sw1);
        bool stop;
        total_cost = 0;
        do
        {
            //if no thread changes this value then the loop stops
            stop=false;

            for(int tid = 0; tid < NUM_NODES; tid++)
            {
                if (h_graph_mask[tid] == true)
                {
                    h_graph_mask[tid]=false;
                    for(int i=h_graph_nodes[tid].starting; i<(h_graph_nodes[tid].no_of_edges + h_graph_nodes[tid].starting); i++)
                    {
                        int id = h_graph_edges[i];
                        if(!h_graph_visited[id])
                        {
                            h_cost[id]=h_cost[tid]+1;
                            h_updating_graph_mask[id]=true;
                            //count++;
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
        stopwatch_stop(&sw1);

        //printf("The count is:%d \n", count);

        for(int i=0;i<NUM_NODES;i++) 
        {
            total_cost += h_cost[i];
        }
        reset_graph(
            &h_graph_mask,
            &h_updating_graph_mask,
            &h_graph_visited,
            &h_cost,
            NUM_NODES);
    }

    free(h_graph_nodes);
    free(h_graph_edges);
    free(h_graph_mask);
    free(h_updating_graph_mask);
    free(h_graph_visited);
    free(h_cost);
    fprintf(stdout, "{\"options\": \"%d\", \"time\": %f, \"status\": %d, \"output\": %lu }\n", NUM_NODES, get_interval_by_sec(&sw1), 1, total_cost);

    gettimeofday(&stop_time, NULL);
    fprintf(stderr, "SECUREFS_TIME %lu us\n",
        (stop_time.tv_sec - start.tv_sec) * 1000000 + stop_time.tv_usec - start.tv_usec);
    return 0;
}


