﻿#include <stdio.h>
#include <stdlib.h>

#define TRUE 1
#define FALSE 0

#define COLOR_WHITE 0
#define COLOR_GRAY 1
#define COLOR_BLACK 2

#define NULL_ID -1
#define INFINITY 3892920

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
	int is_critical;
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
	int* index_array;
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
int read_problem();
void reset_crit_points(t_graph graph);

/*****************************************************************
 ***************************Algoritm.h****************************
 *****************************************************************/
void initialize_preflow(t_graph graph, t_node source);
void discharge(t_node vertex);
int relabel_to_front();
void push(t_node origin, t_edge edge);

void print_list();
void debug();
void reset_list();

/*****************************************************************
 ***************************Global Vars***************************
 *****************************************************************/

t_graph graph = NULL;
int list_head_index = 0;
int V, E;
int links_to_cut;
int crits_found = 0;
int checked_nodes = 0;

int main() {
	int n_problems, vertex_id, max_flow;
	t_node vertex;
	read_graph();

	initialize_preflow(graph, graph->vertexs[0]);

	scanf("%d", &n_problems);

	while (n_problems > 0) {
		links_to_cut = -1;
		reset_crit_points(graph);
		if (read_problem(graph) == 0) { /* menos de 2 pontos criticos */
			printf("0\n");
			--n_problems;
			continue;
		}
		for (vertex_id = 0; vertex_id < graph->max_vertex; ++vertex_id) {
			vertex = graph->vertexs[vertex_id];
			if (vertex->is_critical) {
				max_flow = relabel_to_front(graph, vertex);
				if (max_flow == 0) {
					links_to_cut = 0;
					break;
				} else if (max_flow < links_to_cut || links_to_cut < 0) {
					links_to_cut = max_flow;
				}
			}

		}
		--n_problems;
		printf("%d\n", links_to_cut);
	}

	/*n_problems = read the problem line count

	 for each line (problem)
	 reset critic points
	 read_problem(); <- set the critic points
	 for each V in critics
	 maxflow = relabel_to_front()
	 if maxflow == 0
	 links_to_cut < 2;
	 break;
	 else if maxflow < links_to_cut
	 links_to_cut = maxflow

	 print links_to_cute*/

	return 0;
}

void debug() {
	int vertex_id;
	t_node n;
	for (vertex_id = 0; vertex_id < graph->max_vertex; vertex_id++) {
		n = graph->vertexs[vertex_id];
		printf("Node id %d :: Exc %d :: Hei %d :: Crit %d \n", n->node_id, n->e,
				n->h, n->is_critical);
	}
	n = graph->vertexs[vertex_id];
	printf("SINK: Node id %d :: Exc %d :: Hei %d :: Crit %d \n", n->node_id,
			n->e, n->h, n->is_critical);
	printf("\n\n");
}

void print_list() {
	t_node vertex;
	vertex = graph->vertexs[list_head_index];
	while (vertex != NULL) {
		printf("Node id %d :: Prev %d :: Next %d\n", vertex->node_id,
				vertex->prev_id, vertex->next_id);
		if (vertex->next_id == NULL_ID) {
			vertex = NULL;
		} else {
			vertex = graph->vertexs[vertex->next_id];
		}
	}
}

void reset_list() {
	int vertex_id = 0;

	graph->vertexs[vertex_id]->prev_id = NULL_ID;
	graph->vertexs[vertex_id]->next_id = vertex_id + 1;

	for (vertex_id = 1; vertex_id < graph->max_vertex - 1; ++vertex_id) {
		graph->vertexs[vertex_id]->prev_id = vertex_id - 1;
		graph->vertexs[vertex_id]->next_id = vertex_id + 1;
	}
	graph->vertexs[vertex_id]->prev_id = vertex_id - 1;
	graph->vertexs[vertex_id]->next_id = NULL_ID;
	list_head_index = 0;

}

/*****************************************************************
 ***************************Algorithm*****************************
 *****************************************************************/
void initialize_preflow(t_graph graph, t_node source) {
	int i_vertex;
	t_edge edge;
	/*t_node adj_vertex;*/
	for (i_vertex = 0; i_vertex < graph->max_vertex; ++i_vertex) {
		graph->vertexs[i_vertex]->h = 0;
		graph->vertexs[i_vertex]->e = 0;
		edge = graph->vertexs[i_vertex]->edges;
		while (edge != NULL) {
			if (edge->dest_node_id == V) {
				edge->capacity = INFINITY;
				edge->anti_parallel = 0;
			} else {
				edge->capacity = 1;
			}
			edge = edge->next;
		}
	}
	source->h = V + 1; /* includes the super sink*/
	/* resets the supersink */
	graph->vertexs[V]->h = 0;
	graph->vertexs[V]->e = 0;

	edge = source->edges;
	while (edge != NULL) {
		edge->capacity = 0;
		graph->vertexs[edge->dest_node_id]->e = 1;
		source->e--;
		edge = edge->next;
	}
}

void discharge(t_node vertex) {
	t_edge edge = NULL;
	int min_height = V + 2;

	while (vertex->e > 0) {
		edge = vertex->current;
		if (edge == NULL) {
			min_height++;
			vertex->h = min_height;
			vertex->current = vertex->edges;
		} else if (edge->capacity > 0) {
			if (min_height > graph->vertexs[edge->dest_node_id]->h) {
				min_height = graph->vertexs[edge->dest_node_id]->h;
			}
			if (graph->vertexs[edge->dest_node_id]->h + 1 == vertex->h) {
				push(vertex, edge);
			} else {
				vertex->current = edge->next;
			}
		} else {
			vertex->current = edge->next;
		}
	}

	/*int i = 0;
	 printf("------------- DISCHARGED vertex %d -------------\n",
	 vertex->node_id);
	 for (i = 0; i < V + 1; i++) {
	 printf("Vertex: %d\t e: %d\t h: %d\n", i, graph->vertexs[i]->e,
	 graph->vertexs[i]->h);
	 }
	 */
}

int relabel_to_front(t_graph graph, t_node source) {
	int i_vertex, old_height;
	t_node vertex = NULL;
	t_edge source_tmp_edge = NULL;

	source_tmp_edge = source->edges;
	source->edges = source_tmp_edge->next;

	initialize_preflow(graph, source);

	int destiny = TRUE;
	for (i_vertex = 0; i_vertex < graph->max_vertex; ++i_vertex) {
		if (i_vertex != source->node_id) {
			vertex = graph->vertexs[i_vertex];
			vertex->current = vertex->edges;
			/* FIXME: Hack */
			if (destiny && graph->vertexs[i_vertex]->is_critical) {
				destiny = FALSE;
			} else if (graph->vertexs[i_vertex]->is_critical) {
				graph->vertexs[i_vertex]->edges->capacity = 0; /* cut capacity to supersink */
			}
		}
	}
	/*debug(); FIXME: rm me*/
	vertex = graph->vertexs[list_head_index];

	while (vertex != NULL) {
		if (vertex->node_id == source->node_id) {
			if (vertex->next_id == NULL_ID) {
				break;
			} else {
				vertex = graph->vertexs[vertex->next_id];
				continue;
			}
		}

		old_height = vertex->h;
		discharge(vertex);
		if (vertex->h > old_height) {
			switch_to_front(graph, vertex);
		}

		if (vertex->next_id == NULL_ID) {
			break;
		} else {
			vertex = graph->vertexs[vertex->next_id];
		}
	}

	source->edges = source_tmp_edge;
	/*debug(); FIXME: rm me*/

	return -source->e;
}

void push(t_node origin, t_edge edge) {
	if (edge->capacity != INFINITY) {
		edge->capacity = 0;
		graph->vertexs[edge->dest_node_id]->e++;
		origin->e--;
		edge->anti_parallel->capacity = 1;
	} else {
		graph->vertexs[edge->dest_node_id]->e += origin->e;
		origin->e = 0;
	}
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
				graph->vertexs[dest_id]->edges;
		graph->vertexs[dest_id]->edges->anti_parallel =
				graph->vertexs[orig_id]->edges;

	}
}

int read_problem(t_graph graph) {
	int n_crit_nodes, crit_node_id;

	scanf("%d", &n_crit_nodes);

	if (n_crit_nodes < 2) {
		return 0;
	}

	while (n_crit_nodes > 0) {
		scanf("%d", &crit_node_id);
		graph->vertexs[crit_node_id]->is_critical = TRUE;
		add_connection(graph, crit_node_id, V);
		add_connection(graph, V, crit_node_id);
		graph->vertexs[crit_node_id]->edges->anti_parallel =
				graph->vertexs[V]->edges;
		graph->vertexs[V]->edges->anti_parallel =
				graph->vertexs[crit_node_id]->edges;
		n_crit_nodes--;
	}

	return -1;
}

/*****************************************************************
 ************************ Data structures ************************
 *****************************************************************/
void reset_crit_points(t_graph graph) {
	int vertex_id;
	for (vertex_id = 0; vertex_id < graph->max_vertex; ++vertex_id) {
		if (graph->vertexs[vertex_id]->is_critical == TRUE) {
			graph->vertexs[vertex_id]->is_critical = FALSE;
			graph->vertexs[vertex_id]->edges =
					graph->vertexs[vertex_id]->edges->next; /*FIXME*/
		}
	}
}

t_node create_node(int node_id) {
	t_node new_node = malloc(sizeof(struct Node));
	new_node->node_id = node_id;
	new_node->edges = NULL;
	new_node->h = 0;
	new_node->e = 0;
	new_node->is_critical = FALSE;

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
	if (node->node_id == list_head_index) {
		return;
	}

	/*t_node current = graph->vertexs[list_head_index];

	 printf("BEFORE RELABEL TO FRONT:\n");
	 int i = 0;
	 while (current != NULL) {
	 printf("THIS: %d , PREV: %d, NEXT: %d\n", current->node_id,
	 current->prev_id, current->next_id);
	 if (current->next_id != NULL_ID) {
	 current = graph->vertexs[current->next_id];
	 } else {
	 current = NULL;
	 }
	 i++;
	 if (i == 6)
	 break;
	 }*/

	graph->vertexs[node->prev_id]->next_id = node->next_id;
	if (node->next_id != NULL_ID) {
		graph->vertexs[node->next_id]->prev_id = node->prev_id;
	}
	node->prev_id = NULL_ID
	;
	node->next_id = list_head_index;

	graph->vertexs[list_head_index]->prev_id = node->node_id;
	list_head_index = node->node_id;

	/*current = graph->vertexs[list_head_index];

	 printf("AFTER RELABEL TO FRONT:\n");
	 i=0;
	 while (current != NULL) {
	 printf("THIS: %d , PREV: %d, NEXT: %d\n", current->node_id,
	 current->prev_id, current->next_id);
	 if (current->next_id != NULL_ID) {
	 current = graph->vertexs[current->next_id];
	 } else {
	 current = NULL;
	 }
	 i++;
	 if (i == 6)
	 break;
	 }*/
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
	graph->index_array = malloc(sizeof(int) * num_vertexs);
	graph->max_vertex = num_vertexs;
	for (v = 0; v < num_vertexs; ++v) {
		graph->vertexs[v] = create_node(v);
		graph->index_array[v] = v;
	}
	graph->vertexs[v] = create_node(v); /* supersink*/
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
