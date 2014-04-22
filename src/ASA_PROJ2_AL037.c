#include <stdio.h>
#include <stdlib.h>

#define TRUE 1
#define FALSE 0

#define NULL_ID -1;
#define INFINITY 2

#ifndef NULL
#define NULL   ((void *) 0)
#endif

/*****************************************************************
 ****************************Edge.h*******************************
 *****************************************************************/
struct Edge {
	int dest_node_id;
	int capacity;
	struct Edge* anti_parallel;
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
	int prev_id;
	int next_id;
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
void switch_to_front(t_graph graph, t_node vertex);

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
void push(t_node origin, t_edge edge);

/*****************************************************************
 ***************************Global Vars***************************
 *****************************************************************/

t_graph graph = NULL;
int list_head_index = 0;
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
	for (i_vertex = 0; i_vertex < graph->max_vertex; ++i_vertex) {
		graph->vertexs[i_vertex]->h = 0;
		graph->vertexs[i_vertex]->e = 0;
		edge = graph->vertexs[i_vertex]->edges;
		while (edge != NULL) {
			edge->capacity = 1;
			edge = edge->next;
		}
	}
	source->h = V + 1; /* includes the super sink*/
	edge = graph->vertexs[source->node_id]->edges;
	while (edge != NULL) {
		edge->capacity = 0;
		graph->vertexs[edge->dest_node_id]->e = 1;
		source->e--;
		edge = edge->next;
	}
}

void discharge(t_node vertex) {
	t_edge edge = NULL;
	int min_height = vertex->h;

	while (vertex->e > 0) {
		edge = vertex->current;
		if (edge == NULL) {
			vertex->h = min_height++;
			vertex->current = vertex->edges;
		} else if (edge->capacity > 0
				&& graph->vertexs[edge->dest_node_id]->h < vertex->h) { /*FIXME:*/
			push(vertex, edge);
			if (min_height > graph->vertexs[edge->dest_node_id]->h) {
				min_height = graph->vertexs[edge->dest_node_id]->h;
			}
		} else {
			vertex->current = vertex->current->next;
			/* FIXME: check if there's need to update min_height*/
		}
	}
}

int relabel_to_front(t_graph graph, t_node source) {
	initialize_preflow(graph, source);

	int i_vertex, old_height;
	t_node vertex = graph->vertexs[list_head_index];
	while (vertex != NULL) {
		if (vertex->node_id != source->node_id) {
			vertex->current = vertex->edges;
		}
	}
	vertex = graph->vertexs[list_head_index];

	while (vertex != NULL) {
		old_height = vertex->h;
		discharge(vertex);
		if (vertex->h > old_height) {
			switch_to_front(graph, vertex);
		}
		vertex = graph->vertexs[vertex->current->next->dest_node_id];

	}

	return -source->e;
}

void push(t_node origin, t_edge edge) {
	if (edge->capacity != INFINITY) {
		edge->capacity = 0;
	}
	edge->anti_parallel->capacity++;
	origin->e--;
	graph->vertexs[edge->dest_node_id]->e++;
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
		graph->vertexs[orig_id]->edges->anti_parallel =
				graph->vertexs[dest_id]->edges->anti_parallel;
		graph->vertexs[dest_id]->edges->anti_parallel =
				graph->vertexs[orig_id]->edges->anti_parallel;

	}
}

void print_output() {
	// TOD0:
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

	if (node_id == 0) {
		new_node->prev_id = NULL_ID
		;
	} else {
		new_node->prev_id = node_id - 1;
	}

	if (node_id == V - 1) {
		new_node->next_id = NULL_ID
		;
	} else {
		new_node->next_id = node_id + 1;
	}

	new_node->current = NULL;
	return new_node;
}

void switch_to_front(t_graph graph, t_node node) {
	graph->vertexs[node->prev_id]->next_id = node->next_id;
	graph->vertexs[node->next_id]->prev_id = node->prev_id;
	node->prev_id = NULL_ID
	;
	node->next_id = list_head_index;
	list_head_index = node->node_id;
}

void destroy_node(t_node node) {
	/*TOD0: Implement this*/
}

t_edge create_edge(int dest_node_id) {
	t_edge new_edge = malloc(sizeof(struct Edge));
	new_edge->dest_node_id = dest_node_id;
	new_edge->anti_parallel = NULL;
	new_edge->next = NULL;
	new_edge->capacity = FALSE;
	if (dest_node_id == V) {
		new_edge->capacity = INFINITY;
	} else {
		new_edge->capacity = 1;
	}
	return new_edge;
}

void destroy_edge(t_edge edge) {
	/*TOD0: Implement this*/
}

t_graph create_graph(int num_vertexs) {
	int v;
	t_graph graph = malloc(sizeof(struct Graph));
	graph->vertexs = malloc(sizeof(t_node) * (num_vertexs + 1));
	graph->max_vertex = num_vertexs;
	for (v = 0; v < num_vertexs; ++v) {
		graph->vertexs[v] = create_node(v);
	}
	graph->vertexs[v] = create_node(v); // supersink
	return graph;
}

void destroy_graph(t_graph graph) {
	/*TOD0: Implement this*/
}

void add_connection(t_graph graph, int orig_node_id, int dest_node_id) {

	t_edge target_list = graph->vertexs[orig_node_id]->edges;
	t_edge new_edge = create_edge(dest_node_id);
	new_edge->next = target_list;
	graph->vertexs[orig_node_id]->edges = new_edge;

}
