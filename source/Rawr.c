#include <stdio.h>
#include <stdlib.h>

#ifndef NULL
#define NULL   ((void *) 0)
#endif

#define COLOR_WHITE 0
#define COLOR_GRAY 1
#define COLOR_BLACK 2

/*****************************************************************
 ****************************Edge.h*******************************
 *****************************************************************/
struct Edge {
	int dest_node_id;
	struct Edge* next;
};

typedef struct Edge* t_edge;

t_edge create_edge(int dest_node_id);
void destroy_edge(t_edge edge);


/*****************************************************************
 ****************************Node.h*******************************
 *****************************************************************/
struct Node {
	int color;
	int f;
	int d;
	int low;
	int onStack;
	int node_id;
	t_edge edges;

	int scc_number;
};

typedef struct Node* t_node;

t_node create_node(int node_id);
void destroy_node(t_node node);

/*****************************************************************
 ***************************Graph.h*******************************
 *****************************************************************/

struct Graph{

	int max_vertex;
	int* index_array;
	t_node* vertexs;
};

typedef struct Graph* t_graph;

t_graph create_graph(int num_vertexs);
void destroy_graph(t_graph graph);
void add_connection(t_graph graph, int orig_node_id, int dest_node_id);
void remove_connection(t_graph graph);

/*****************************************************************
 ***************************Setup.h*******************************
 *****************************************************************/

void read_input();
void print_output();

/*****************************************************************
 ***************************Algoritm.h****************************
 *****************************************************************/

void dfs(t_graph graph);
void dfs_visit(t_graph graph, t_node node, int* time);


/*****************************************************************
 ***************************Golbal Vars***************************
 *****************************************************************/

t_graph graph_original = NULL;

int V, E;

int inverted_closed_nodes = 0;
int* ordered_by_scc;

int main() {
    read_input();

    print_output();

    return 0;
}

/*****************************************************************
 ***************************Setup.c*******************************
 *****************************************************************/

void read_input() {
    int line;
    int orig_id, dest_id;

    scanf("%d %d", &V, &E);
    graph_original = create_graph(V);

    ordered_by_scc = malloc(sizeof(int) * V);

    for(line = 0 ; line < E ; line++) {
        scanf("%d %d", &orig_id, &dest_id);
        add_connection(graph_original, orig_id, dest_id);

    }


}

void print_output() {
    printf("Rawr");
}


/*****************************************************************
 ***************************Algoritm.c****************************
 *****************************************************************/

void dfs(t_graph graph) {
    int vertex_id;
    int time = 1;
    t_node vertex;
    for(vertex_id=0; vertex_id< graph->max_vertex; ++vertex_id){
        vertex = graph->vertexs[graph->index_array[vertex_id]];
        if(vertex->color==COLOR_WHITE){
            dfs_visit(graph, vertex, &time);
        }
    }
}

/*
 * TODO: create comment
 */
void dfs_visit(t_graph graph, t_node vertex, int* time) {
	vertex->color = COLOR_GRAY;
	vertex->d = *time;
	(*time)++;
	t_edge edges = vertex->edges;
	t_node adj_vertex=NULL;

	while(edges != NULL){
		adj_vertex = graph->vertexs[edges->dest_node_id-1];
        if(adj_vertex->color == COLOR_WHITE){
			dfs_visit(graph, adj_vertex, time);
        }
		edges=edges->next;
	}
    vertex->color = COLOR_BLACK;
    vertex->f = *time;
    (*time)++;


    //Old functions, should we delete these two lines?
    //graph_transverse->index_array[open_nodes] = vertex->node_id-1;
    //open_nodes--;
}


/*****************************************************************
 ****************************Node.c*******************************
 *****************************************************************/

t_node create_node(int node_id){
	t_node new_node = malloc(sizeof(struct Node));
	new_node->node_id = node_id;
	new_node->edges = NULL;
	new_node->color=COLOR_WHITE;
	new_node->f=0;
	new_node->d=0;
	return new_node;
}

void destroy_node(t_node node){

}

/*****************************************************************
 ****************************Edge.c*******************************
 *****************************************************************/

t_edge create_edge(int dest_node_id){
	t_edge new_edge = malloc(sizeof(struct Edge));
	new_edge->dest_node_id = dest_node_id;
	new_edge->next = NULL;
	return new_edge;
}

void destroy_edge(t_edge edge){
	/*TODO: Implement this;*/
}

/*****************************************************************
 ***************************Graph.c*******************************
 *****************************************************************/

t_graph create_graph(int num_vertexs){
	int v;
	t_graph graph = malloc(sizeof(struct Graph));
	graph->vertexs = malloc(sizeof(t_node) * num_vertexs);
	graph->index_array = malloc(sizeof(int) * num_vertexs);
	graph->max_vertex = num_vertexs;
	for(v=0; v<num_vertexs;++v){
		graph->vertexs[v]= create_node(v+1);
		graph->index_array[v] = v;
	}
	return graph;
}

void destroy_graph(t_graph graph){
	/*TODO: Implement this;*/
}

void add_connection(t_graph graph, int orig_node_id, int dest_node_id){

	t_edge target_list = graph->vertexs[orig_node_id-1]->edges;
	t_edge new_edge = create_edge(dest_node_id);
	new_edge->next = target_list;
	graph->vertexs[orig_node_id-1]->edges = new_edge;

}

void remove_connection(t_graph graph){
	/*TODO: Implement this;*/
}
