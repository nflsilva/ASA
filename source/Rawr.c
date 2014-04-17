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
void dfs2(t_graph graph);
void dfs_visit2(t_graph graph, t_node node, int* time, int* depth);

void do_voo_doo_magic();


/*****************************************************************
 ***************************Golbal Vars***************************
 *****************************************************************/

t_graph graph_original = NULL;
t_graph graph_transverse = NULL;

/*
 * Estas variaveis serao actualizadas durante
 * a funcao tarjan, e corresponderao ao
 * resultado a imprimir no final do programa
 */
int found;
int totalSCCs = 0;
int largerSCCSize = 0;
int closedForReceptionSCCs = 0;
int V, E;
int open_nodes;

int inverted_closed_nodes = 0;
int* ordered_by_scc;

int main() {
    read_input();

    dfs(graph_original);
    dfs2(graph_transverse); /* here, visit some edges in the original order to detect cross-edges connecting two
                        different SCCs -> IF edge is cross edge & vortex.colour == WHITE, then finish the DF tree and increment closedForReceptionSCCs*/

    /*
    int i;
    t_node vertex;
    for(i=0;i<V;++i){

    	vertex=graph_transverse->vertexs[i];
    	printf("Node id: %d, SCC: %d\n", vertex->node_id, vertex->scc_number);
    }*/

    closedForReceptionSCCs = totalSCCs;

    do_voo_doo_magic();

    print_output();

    return 0;
}

void do_voo_doo_magic(){

	int index, id;
	int current_scc = 0;
	int skip_scc = 0;
	t_node vertex;
	t_edge edges;

	for(index=0; index<V;++index){
		id=ordered_by_scc[index]-1;
		vertex = graph_transverse->vertexs[id];

		if(skip_scc && vertex->scc_number == current_scc-1){
			/*printf("Skiped one node in scc number %d \n", current_scc-1);*/
			continue;
		} else {
			/*printf("Stop skiping\n");*/
			skip_scc = 0;
		}

		current_scc = vertex->scc_number;

		edges = graph_original->vertexs[vertex->node_id-1]->edges;

	    	while(edges!= NULL){
	    		/*printf("Checking node: %d, edge: %d->%d\n",vertex->node_id, vertex->node_id, edges->dest_node_id);*/

	    		if((graph_transverse->vertexs[edges->dest_node_id-1])->scc_number != current_scc){
	    			--closedForReceptionSCCs;
	    			++current_scc;
	    			skip_scc = 1;
	    			/*printf("Skiping all the scc\n");*/
	    		}
	    		if(skip_scc)
	    			break;
	    		else
	    			edges=edges->next;
	    	}
	}

}


/*****************************************************************
 ***************************Setup.c*******************************
 *****************************************************************/

void read_input() {
    int line;
    int orig_id, dest_id;

    scanf("%d %d", &V, &E);
    graph_original = create_graph(V);
    graph_transverse = create_graph(V);

    ordered_by_scc = malloc(sizeof(int) * V);

    for(line = 0 ; line < E ; line++) {
        scanf("%d %d", &orig_id, &dest_id);
        add_connection(graph_original, orig_id, dest_id);
        add_connection(graph_transverse, dest_id, orig_id);
    }
}

void print_output() {
    printf("%d\n%d\n%d", totalSCCs, largerSCCSize, closedForReceptionSCCs);
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

    graph_transverse->index_array[open_nodes] = vertex->node_id-1;
    open_nodes--;
}

void dfs2(t_graph graph) {
    int vertex_id;
    int time = 1;
    int depth = 0;
    t_node vertex;
    for(vertex_id=0; vertex_id< graph->max_vertex; ++vertex_id){
	    found = 0;
        vertex = graph->vertexs[graph->index_array[vertex_id]];
        if(vertex->color==COLOR_WHITE){
            dfs_visit2(graph, vertex, &time, &depth);
            totalSCCs++;
            if(largerSCCSize < depth){
            	largerSCCSize = depth;
            }
        }
		if(found == 1) {
		    closedForReceptionSCCs++;
	    }
		    depth = 0;
    }
}
/*
 * TODO: create comment
 */
void dfs_visit2(t_graph graph, t_node vertex, int* time, int* depth) {
		vertex->color = COLOR_GRAY;
		vertex->d = *time;
		(*time)++;
		t_edge edges = vertex->edges;
		t_node adj_vertex=NULL;

		while(edges != NULL){
			adj_vertex = graph->vertexs[edges->dest_node_id-1];
	        if(adj_vertex->color == COLOR_WHITE){
				dfs_visit2(graph, adj_vertex, time, depth);
	        } else if (found == 0 && adj_vertex->color == COLOR_BLACK && adj_vertex->d < vertex->d) {
			    found = 1;
			}
			edges=edges->next;
		}
	    vertex->color = COLOR_BLACK;
	    vertex->f = *time;
	    vertex->scc_number = totalSCCs;
	    ordered_by_scc[inverted_closed_nodes] = vertex->node_id;
	    inverted_closed_nodes++;
	    (*time)++;
	    (*depth)++;
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
	open_nodes = num_vertexs-1;
	return graph;
}

void destroy_graph(t_graph graph){
	/*TODO: THIS*/
}

void add_connection(t_graph graph, int orig_node_id, int dest_node_id){

	t_edge target_list = graph->vertexs[orig_node_id-1]->edges;
	t_edge new_edge = create_edge(dest_node_id);
	new_edge->next = target_list;
	graph->vertexs[orig_node_id-1]->edges = new_edge;

}

void remove_connection(t_graph graph){
	/*TODO: Running away from problems*/
}
