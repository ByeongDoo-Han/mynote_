#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <memory.h>

typedef struct _incidentEdges IncidentEdges;
typedef struct _vertices Vertices;
typedef struct _edges Edges;

struct _incidentEdges {
	int edgeindex;
	struct _incidentEdges* next;
};
struct _vertices {
	int num;
	IncidentEdges* head;
};
struct _edges {
	int weight;
	Vertices* endpoints[2];
};
typedef struct _graph {
	Vertices vertex[6];
	Edges edge[21];
}Graph;

Graph grp;
int edgenum = 7;

void removeEdge(int v1, int v2) {
	IncidentEdges* p, *prev, *removed;
	int index = 0;
	p = grp.vertex[v1].head->next;
	prev = grp.vertex[v1].head;

	// remove incident from v1
	while (p != NULL) {
		if (grp.edge[p->edgeindex].endpoints[0]->num == v1 + 1 && grp.edge[p->edgeindex].endpoints[1]->num == v2 + 1
			|| grp.edge[p->edgeindex].endpoints[0]->num == v2 + 1 && grp.edge[p->edgeindex].endpoints[1]->num == v1 + 1) {
			index = p->edgeindex;

			removed = p;
			prev->next = p->next;
			free(removed);
			break;
		}
		prev = p;
		p = p->next;
	}
	
	// remove incident from v2
	p = grp.vertex[v2].head->next;
	prev = grp.vertex[v2].head;
	while (p != NULL) {
		if (p->edgeindex == index) {
			removed = p;
			prev->next = p->next;
			free(removed);
			break;
		}
		prev = p;
		p = p->next;
	}

	grp.edge[index].endpoints[0] = NULL;
	grp.edge[index].endpoints[1] = NULL;
	grp.edge[index].weight = 0;
}
IncidentEdges* MakeIncident(void) {
	IncidentEdges* new_incident;
	new_incident = (IncidentEdges*)malloc(sizeof(IncidentEdges));

	if (new_incident != NULL) {
		new_incident->edgeindex = 0;
		new_incident->next = NULL;
	}

	return new_incident;
}
void setEdge(Vertices* v1, Vertices* v2, int edge_index, int weight) {
	IncidentEdges* p, * new_incident;
	int compare;

	new_incident = MakeIncident();
	new_incident->edgeindex = edge_index;
	

	if (grp.edge[edge_index].endpoints[0] == NULL && grp.edge[edge_index].endpoints[1] == NULL) {	// edge 처음생성
		grp.edge[edge_index].weight = weight;
		grp.edge[edge_index].endpoints[0] = v1;
		grp.edge[edge_index].endpoints[1] = v2;
	}

	p = v1->head;
	while (p->next != NULL) {
		compare = (grp.edge[p->edgeindex].endpoints[0]->num == v1->num) ? 
					grp.edge[p->edgeindex].endpoints[1]->num : grp.edge[p->edgeindex].endpoints[0]->num;
		if (v2->num < compare)
			break;

		p = p->next;
	}
	new_incident->next = p->next;
	p->next = new_incident;
	
	return;
}
void makeGraph() {
	for (int i = 0; i < 6; i++) {
		grp.vertex[i].num = i + 1;			// set vertext number

		// make incident lists header
		grp.vertex[i].head = MakeIncident();
	}

	// vertex 1
	setEdge(&grp.vertex[0], &grp.vertex[1], 0, 1);	// 1->2
	setEdge(&grp.vertex[0], &grp.vertex[2], 1, 1);	// 1->3
	setEdge(&grp.vertex[0], &grp.vertex[3], 2, 1);	// 1->4
	setEdge(&grp.vertex[0], &grp.vertex[5], 3, 2);	// 1->6

	// vertex 2
	setEdge(&grp.vertex[1], &grp.vertex[0], 0, 1);	// 2->1
	setEdge(&grp.vertex[1], &grp.vertex[2], 4, 1);	// 2->3

	// vertex 3
	setEdge(&grp.vertex[2], &grp.vertex[0], 1, 1);	// 3->1
	setEdge(&grp.vertex[2], &grp.vertex[1], 4, 1);	// 3->2
	setEdge(&grp.vertex[2], &grp.vertex[4], 5, 4);	// 3->5

	// vertex 4
	setEdge(&grp.vertex[3], &grp.vertex[0], 2, 1);	// 4->1

	// vertex 5
	setEdge(&grp.vertex[4], &grp.vertex[2], 5, 4);	// 5->3
	setEdge(&grp.vertex[4], &grp.vertex[4], 6, 4);	// 5->5
	setEdge(&grp.vertex[4], &grp.vertex[5], 7, 3);	// 5->6

	// vertex 6
	setEdge(&grp.vertex[5], &grp.vertex[0], 3, 2);	// 6->1
	setEdge(&grp.vertex[5], &grp.vertex[4], 7, 3);	// 6->5
}

void adjacentVertices(int node_number) {	// command = a
	IncidentEdges* p;
	int index;

	if (node_number >= 6 || node_number < 0) {
		printf("-1\n");
		return;
	}

	p = grp.vertex[node_number].head->next;
	while (p != NULL) {
		index = p->edgeindex;
		if (grp.edge[index].endpoints[0]->num == node_number + 1)
			printf(" %d %d", grp.edge[index].endpoints[1]->num, grp.edge[index].weight);
		else
			printf(" %d %d", grp.edge[index].endpoints[0]->num, grp.edge[index].weight);
		p = p->next;
	}
	printf("\n");

	return;
}

void modifyEdgeWeight(int v1, int v2, int change_weight) {	// command = m
	IncidentEdges* p;
	if (v1 >= 6 || v1 < 0 || v2 >= 6 || v2 < 0) {
		printf("-1\n");
		return;
	}

	if (change_weight == 0) {
		removeEdge(v1, v2);
	}
	else {
		p = grp.vertex[v1].head->next;
		while (p != NULL) {
			if (grp.edge[p->edgeindex].endpoints[0]->num == v1 + 1 && grp.edge[p->edgeindex].endpoints[1]->num == v2 + 1
				|| grp.edge[p->edgeindex].endpoints[0]->num == v2 + 1 && grp.edge[p->edgeindex].endpoints[1]->num == v1 + 1) {
				grp.edge[p->edgeindex].weight = change_weight;
				return;
			}
			p = p->next;
		}
		setEdge(&grp.vertex[v1], &grp.vertex[v2], ++edgenum, change_weight);
		setEdge(&grp.vertex[v2], &grp.vertex[v1], edgenum, change_weight);
	}

	return;
}
void deallocate(void) {
	IncidentEdges* p, * removed;
	for (int i = 0; i < 6; i++) {
		p = grp.vertex[i].head;
		while (p != NULL) {
			removed = p;
			p = p->next;
			free(removed);
		}
	}
}
void printGraph(void) {
	IncidentEdges* p;
	for (int i = 0; i < 6; i++){
		printf("%d :", grp.vertex[i].num);
		p = grp.vertex[i].head->next;
		while (p != NULL) {
			printf(" %d", p->edgeindex);
			p = p->next;
		}
		printf("\n");
	}
	
}
int main(void) {
	char com;
	int node_num;
	int v1, v2, change_weight;

	memset(&grp.vertex, 0, sizeof(Vertices));
	memset(&grp.edge, 0, sizeof(Edges));

	makeGraph();

	while (1) {
		scanf("%c", &com);

		switch (com) {
		case 'a':
			scanf("%d", &node_num);
			adjacentVertices(node_num - 1);
			break;
		case 'm':
			scanf("%d %d %d", &v1, &v2, &change_weight);
			modifyEdgeWeight(v1 - 1, v2 - 1, change_weight);
			break;
		case 'q':
			return 0;
		}

		while (getchar() != '\n') {}
	}

	deallocate();
}
