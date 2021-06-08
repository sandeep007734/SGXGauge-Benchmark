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


#define MIN_NODES      20
#define MAX_NODES      ULONG_MAX
#define MIN_EDGES      2
#define MAX_INIT_EDGES 4
#define MIN_WEIGHT     1
#define MAX_WEIGHT     10

#define NUM_NODES      132768

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
    long long graph_size)
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

/*
void InitializeGraph(
    Node **h_graph_nodes,
    bool **h_graph_mask,
    bool **h_updating_graph_mask,
    bool **h_graph_visited,
    int  **h_graph_edges,
    int  **h_cost,
    int numNodes,
    char  *vertice_file_path,
    char  *edge_file_path) {

    node *graph = new node[numNodes];
    int source = 0;
    unsigned numEdges;
    unsigned long nodeID;
    unsigned weight;

    *h_graph_nodes = (Node*) malloc(sizeof(Node)*numNodes);
    *h_graph_mask = (bool*) malloc(sizeof(bool)*numNodes);
    *h_updating_graph_mask = (bool*) malloc(sizeof(bool)*numNodes);
    *h_graph_visited = (bool*) malloc(sizeof(bool)*numNodes);
    *h_cost = (int*) malloc(sizeof(int)*numNodes);


    for (int i = 0; i < numNodes; ++i) {
        numEdges = abs(common_rand() % ( MAX_INIT_EDGES - MIN_EDGES + 1 )) + MIN_EDGES;
        for ( unsigned j = 0; j < numEdges; j++ ) {
            nodeID = abs(common_rand() % numNodes);
            weight = abs(common_rand() % ( MAX_WEIGHT - MIN_WEIGHT + 1 )) + MIN_WEIGHT;
            graph[i].push_back( edge() );
            graph[i].back().dest = nodeID;
            graph[i].back().weight = weight;
            graph[nodeID].push_back( edge() );
            graph[nodeID].back().dest = i;
            graph[nodeID].back().weight = weight;
        }
    }

    unsigned long totalEdges = 0;
    for (int i = 0; i < numNodes; ++i) {
        unsigned long numEdges = graph[i].size();
        (*h_graph_nodes)[i].starting = totalEdges;
        (*h_graph_nodes)[i].no_of_edges = numEdges;
        (*h_graph_mask)[i] = false;
        (*h_updating_graph_mask)[i] = false;
        (*h_graph_visited)[i] = false;

        totalEdges += numEdges;
    }

    *h_graph_edges = (int*) malloc(sizeof(int)*totalEdges);

    struct graph_links* graph_data = (struct graph_links*) malloc(totalEdges * sizeof(struct graph_links));

    int graph_size = 0;
    for(int i=0; i < numNodes; i++)
    {
        for(int j=0; j < graph[i].size(); j++)
        {
            graph_data[graph_size].src = i;
            graph_data[graph_size].dest = graph[i][j].dest;
            graph_data[graph_size].weight = graph[i][j].weight;
            graph_size++;
        }
    }



    (*h_graph_mask)[source] = true;
    (*h_graph_visited)[source] = true;

    unsigned k = 0;
    for ( unsigned long i = 0; i < numNodes; i++ ) {
        for ( unsigned j = 0; j < graph[i].size(); j++ ) {
            (*h_graph_edges)[k] = graph[i][j].dest;
            ++k;
        }
    }

    for(int i = 0; i < numNodes; ++i) {
        (*h_cost)[i] = -1;
    }
    (*h_cost)[source] = 0;


    if (vertice_file_path != NULL && edge_file_path != NULL) {
		FILE *fp = fopen(vertice_file_path, "w");
		for (int i=0; i<graph_size; ++i) {
            fwrite (&graph_data[i], sizeof(struct graph_links), 1, fp);
		}
		fclose(fp);

        printf("%d\n", graph_size);
        printf("%ld\n", totalEdges);

        int graph_load = 0;

        fp = fopen(vertice_file_path, "r");
        struct graph_links input;
        while(fread(&input, sizeof(struct graph_links), 1, fp))
        {
            graph_load++;
        }
        printf("%d \n", graph_load);

		fp = fopen(edge_file_path, "w");
		for (int i=0; i<totalEdges; ++i) {
			fprintf(fp, "%d\n", (*h_graph_edges)[i]);
		}
		fclose(fp);
    }


} */


int main( int argc, char** argv) {
    char *vertice_file_path = NULL;
    char *edge_file_path = NULL;
    char *cost_file_path = NULL;
    unsigned int expected_no_of_nodes = 3000000;
    unsigned long int expected_total_cost = 26321966;
    vertice_file_path = "/tmp/vertex.dat";

    Node *h_graph_nodes;
    bool *h_graph_mask;
    bool *h_updating_graph_mask;
    bool *h_graph_visited;
    int  *h_graph_edges;
    int  *h_cost;
    stopwatch sw1, sw2;

    struct stat buf;

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
    /*InitializeGraph(
        &h_graph_nodes,
        &h_graph_mask,
        &h_updating_graph_mask,
        &h_graph_visited,
        &h_graph_edges,
        &h_cost,
        no_of_nodes,
        vertice_file_path,
        edge_file_path);*/

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

    stopwatch_start(&sw1);
    bool stop;
    do
    {
//if no thread changes this value then the loop stops
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
    stopwatch_stop(&sw1);

    unsigned long total_cost = 0;
    for(int i=0;i<NUM_NODES;i++) {
        total_cost += h_cost[i];
    }

    fprintf(stderr, "// Init time     : %f s\n", get_interval_by_sec(&sw2));

    if (cost_file_path != NULL) {
        FILE *fp = fopen(cost_file_path, "w");
        for (int i=0; i<NUM_NODES; ++i) {
            fprintf(fp, "%d\n", h_cost[i]);
        }
        fclose(fp);
    }

    free(h_graph_nodes);
    free(h_graph_edges);
    free(h_graph_mask);
    free(h_updating_graph_mask);
    free(h_graph_visited);
    free(h_cost);

    fprintf(stdout, "{\"options\": \"%d\", \"time\": %f, \"status\": %d, \"output\": %lu }\n", NUM_NODES, get_interval_by_sec(&sw1), 1, total_cost);
    return 0;
}


