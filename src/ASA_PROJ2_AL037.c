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
	int flow;
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
	t_edge current;
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

void initialize_preflow(t_graph graph, t_node source);
void discharge(t_node vertex);
int relabel_to_front();

/*****************************************************************
 ***************************Global Vars***************************
 *****************************************************************/

t_graph graph = NULL;
t_node* list = NULL;
int V, E;
int links_to_close = 0;

int main() {
	read_graph();

	initialize_preflow(graph, graph->vertexs[0]);
	// n_problems = read the problem line count

//  for each line (problem)
	//  reset critic points
	//  read_problem(); <- set the critic points
	//	for each V in critics
	//	   maxflow = relabel_to_front()
	//	   if maxflow == 0
	//	       links_to_cut < 2;
	//	       break;
	//	   else if maxflow < links_to_cut
	//		   links_to_cut = maxflow
//  print links_to_cute

	return 0;
}

/*****************************************************************
 ***************************Algorithm*****************************
 *****************************************************************/
void initialize_preflow(t_graph graph, t_node source) {
	int i_vertex;
	t_edge edge;
	t_node adj_vertex;
	for(i_vertex = 0; i_vertex < graph->max_vertex; ++ i_vertex){
		graph->vertexs[i_vertex]->h = 0;
		graph->vertexs[i_vertex]->e = 0;
		edge = graph->vertexs[i_vertex]->edges;
		while(edge!=NULL){
			edge->flow = 0;
			edge = edge->next;
		}
	}
	source->h = V;
	edge = graph->vertexs[source->node_id]->edges;
	while(edge!=NULL){
		edge->flow = 1;
		graph->vertexs[edge->dest_node_id]->e = 1;
		source->e--;
		edge = edge->next;
	}

	// for each vertex v
	//    v.h = 0;
	//    v.e = 0;
	// for each edge e
	//    e.flow = 0
	// source.h = |V|
	// for each v in Adj(source)
	//    edge.flow = 1;
	//    v.e = 1;
	//    source.e--;
}

void discharge(t_node vertex) {
	t_edge v = vertex->current;
	while(vertex->e > 0) {
		if(v == NULL){
			//TODO:
			//RELABEL(vertex);
			vertex->current = vertex->edges;
		} else if(v->flow == 1){
			//TODO:
			//PUSH(vertex, v);
		} else {
			vertex->current = vertex->current->next;
		}



	}
//	while vertex.e > 0
//	   v = vertex.current
//	   if v == NULL
//	       RELABEL(vertex)
//	       vertex.current = first in vertex edges;
//	   else if (vertex, v).flow == 1
//		   PUSH(vertex ,v)
//	   else vertex.current = vertex.current.next;
}

int relabel_to_front(t_graph graph, t_node source) {
	initialize_preflow(graph, source);

	int i_vertex, old_height;
	t_node vertex = NULL;

	for(i_vertex=0; i_vertex < graph->max_vertex; ++i_vertex){

	}

	while(vertex != NULL){
		old_height = vertex->h;
		discharge(vertex);
		if(vertex->h > old_height){

		}

	}
//	L = vertexes except source
//	for each vertex u in L
//	    u.current = first vertex in u.edges
//	u = L.head
//	while u == NULL
//	    old-height = u.h
//	    DISCHARGE(u)
//	    if u.h > old-height
//	        move u to the front of list L
//	    u = u.current.next
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
	// TODO:
	// probably won't be implemented here... will be done in main()
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
	new_node->current = NULL;
	return new_node;
}

void destroy_node(t_node node) {
	/*TODO: Implement this*/
}

t_edge create_edge(int dest_node_id) {
	t_edge new_edge = malloc(sizeof(struct Edge));
	new_edge->dest_node_id = dest_node_id;
	new_edge->next = NULL;
	new_edge->flow = FALSE;
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
