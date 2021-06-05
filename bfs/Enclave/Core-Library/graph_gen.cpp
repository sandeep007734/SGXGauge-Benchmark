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

void GenerateGraph(int, char*);

struct graph_links {
    long src;
    long dest;
    long weight;
};

void GenerateGraph(
    int numNodes,
    char  *graph_file_path) 
{

    node *graph = new node[numNodes];
    unsigned long nodeID, numEdges;
    unsigned weight;
    unsigned long totalEdges = 0;


    for (int i = 0; i < numNodes; ++i) 
    {
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
            totalEdges +=2;
        }
    }

    printf("Location of graph_data: %lld \n", totalEdges * sizeof(struct graph_links));

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


    FILE *fp = fopen(graph_file_path, "w");
	for (int i=0; i<graph_size; ++i) {
        fwrite (&graph_data[i], sizeof(struct graph_links), 1, fp);
	}
	fclose(fp);

    printf("Number of Edges: %d\n", graph_size);
    printf("Number of Vertices: %d\n", numNodes);
    printf("The file is generated at %s \n", graph_file_path);
}


int main( int argc, char** argv) {

    GenerateGraph(
        NUM_NODES,
        GRAPH_FILE_PATH);

    return 0;
}