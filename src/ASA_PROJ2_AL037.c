#include <stdio.h>
#include <stdlib.h>

#define TRUE 1
#define FALSE 0

#ifndef NULL
#define NULL   ((void *) 0)
#endif

/*****************************************************************
 ****************************Edge.h*******************************
 *****************************************************************/
struct Edge {
	int dest_node_id;
	int used;
	struct Edge* next;
};

typedef struct Edge* t_edge;

t_edge create_edge(int dest_node_id);
void destroy_edge(t_edge edge);

/*****************************************************************
 ****************************Node.h*******************************
 *****************************************************************/
struct Node {
	int h;
	int e;
	int node_id;
	int critical_link;
	t_edge edges;
};

typedef struct Node* t_node;

t_node create_node(int node_id);
void destroy_node(t_node node);

/*****************************************************************
 ***************************Graph.h*******************************
 *****************************************************************/

struct Graph {
	int max_vertex;
	t_node* vertexs;
};

typedef struct Graph* t_graph;

t_graph create_graph(int num_vertexs);
void destroy_graph(t_graph graph);
void add_connection(t_graph graph, int orig_node_id, int dest_node_id);

/*****************************************************************
 ***************************Setup.h*******************************
 *****************************************************************/

void read_graph();
void print_output();

/*****************************************************************
 ***************************Algoritm.h****************************
 *****************************************************************/

int relabel_to_front(t_graph graph);

/*****************************************************************
 ***************************Global Vars***************************
 *****************************************************************/

t_graph graph = NULL;
int V, E;

/*
 * Estas variaveis serao actualizadas durante
 * a segunda DFS, e corresponderao ao
 * resultado a imprimir no final do programa
 */
int links_to_close = 0;

int main() {
	read_graph();

	// n_problems = read the problem line count
	// read_problem(); <- set the critic points

//  for each problem
	//	for each V in critics
	//	   maxflow = relabel_to_front()
	//	   if maxflow == 0
	//	       links_to_cut < 2;
	//	       break;
	//	   else if maxflow < links_to_cut
	//		   links_to_cut = maxflow

	print_output();

	return 0;
}

/*****************************************************************
 ***************************I/O***********************************
 *****************************************************************/

void read_graph() {
	int line;
	int orig_id, dest_id;

	scanf("%d %d", &V, &E);
	graph = create_graph(V);

	for (line = 0; line < E; line++) {
		scanf("%d %d", &orig_id, &dest_id);
		add_connection(graph, orig_id, dest_id);
		add_connection(graph, dest_id, orig_id);
	}
}

void print_output() {
	// print lista de outputs
}

/*****************************************************************
 ************************ Data structures ************************
 *****************************************************************/

t_node create_node(int node_id) {
	t_node new_node = malloc(sizeof(struct Node));
	new_node->node_id = node_id;
	new_node->edges = NULL;
	new_node->h = 0;
	new_node->e = 0;
	new_node->critical_link = FALSE;
	return new_node;
}

void destroy_node(t_node node) {
	/*TODO: Implement this*/
}

t_edge create_edge(int dest_node_id) {
	t_edge new_edge = malloc(sizeof(struct Edge));
	new_edge->dest_node_id = dest_node_id;
	new_edge->next = NULL;
	new_edge->used = FALSE;
	return new_edge;
}

void destroy_edge(t_edge edge) {
	/*TODO: Implement this*/
}

t_graph create_graph(int num_vertexs) {
	int v;
	t_graph graph = malloc(sizeof(struct Graph));
	graph->vertexs = malloc(sizeof(t_node) * num_vertexs);
	graph->max_vertex = num_vertexs;
	for (v = 0; v < num_vertexs; ++v) {
		graph->vertexs[v] = create_node(v);
	}
	return graph;
}

void destroy_graph(t_graph graph) {
	/*TODO: Implement this*/
}

void add_connection(t_graph graph, int orig_node_id, int dest_node_id) {

	t_edge target_list = graph->vertexs[orig_node_id]->edges;
	t_edge new_edge = create_edge(dest_node_id);
	new_edge->next = target_list;
	graph->vertexs[orig_node_id]->edges = new_edge;

}
