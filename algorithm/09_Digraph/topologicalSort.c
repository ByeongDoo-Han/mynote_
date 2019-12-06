// 위상정렬
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <memory.h>


typedef struct _vertices Vertices;
typedef struct _edges Edges;
typedef struct _incidentList IncidentList;

struct _incidentList {
	int edgeindex;
	struct _incidentList* next;
};
struct _vertices {
	char name;
	int inDegree;
	IncidentList* inEdges;
	IncidentList* outEdges;
	int v_label;
};
struct _edges {
	int origin;
	int dest;
	int e_label;
};
typedef struct _digraph {
	Vertices vertex[100];
	Edges edges[1000];
}digraph;

digraph digrp;

int n;	// number of vertex
int m;	// number of direct edge
int* topOrder = NULL;

// Q
int* Q;
int front = 0, rear = 0;

// queue method-------------------
bool isFull(void) {
	if ((rear + 1) % n == front)
		return true;
	return false;
}
bool isEmpty(void) {
	if (rear == front)
		return true;
	return false;
}
void enqueue(int e) {
	if (isFull()) {
		printf("overflow\n");
		exit(1);
	}
	rear = (++rear) % n;
	Q[rear] = e;
}
int dequeue() {
	int tmp;

	if (isEmpty()) {
		printf("underflow\n");
		exit(1);
	}
	front = (++front) % n;
	tmp = Q[front];
	Q[front] = 0;

	return tmp;
}
//--------------------------------------

void init() {
	topOrder = calloc(n + 1, sizeof(int));

	memset(&digrp.vertex, 0, sizeof(Vertices) * n);
	memset(&digrp.edges, 0, sizeof(Edges) * m);
}
int index(char vName) {
	for (int i = 0; i < n; i++) {
		if (digrp.vertex[i].name == vName)
			return i;
	}
	return -1;
}
IncidentList* makeIncident(int i) {
	IncidentList* new_incident;
	new_incident = (IncidentList*)malloc(sizeof(IncidentList));
	new_incident->edgeindex = i;
	new_incident->next = NULL;

	return new_incident;
}
void addFirst(IncidentList* list, int i) {
	IncidentList* new_incident = makeIncident(i);

	new_incident->next = list->next;
	list->next = new_incident;
	
	return;
}

void insertVertex(char vName, int index) {
	digrp.vertex[index].name = vName;
	digrp.vertex[index].inDegree = 0;
	digrp.vertex[index].inEdges = makeIncident(0);	// make header inEdge
	digrp.vertex[index].outEdges = makeIncident(0);	// make header outEdge

	return;
}
void insertDirectedEdge(char uName, char wName, int i) {
	int u, w;

	u = index(uName);
	w = index(wName);

	digrp.edges[i].origin = u;
	digrp.edges[i].dest = w;
	
	addFirst(digrp.vertex[u].outEdges, i);
	addFirst(digrp.vertex[w].inEdges, i);

	digrp.vertex[w].inDegree++;

	return;
}

void buildGraph() {
	char vName;
	char uName, wName;	// direction of vertex

	scanf("%d", &n);				// number of vertex
	getchar();
	init();

	// set vertex name & make header
	for (int i = 0; i < n; i++) {
		scanf("%c", &vName);
		insertVertex(vName, i);
		getchar();
	}

	scanf("%d", &m);				// number of edges
	for (int i = 0; i < m; i++) {
		while (getchar() != '\n') {}
		scanf("%c %c", &uName, &wName);
		insertDirectedEdge(uName, wName, i);
	}

	return;
}
void initQ() {
	Q = (int*)malloc(sizeof(int) * n);
	memset(Q, 0, sizeof(int) * n);
}
void topologicalSort() {
	IncidentList* e = NULL;
	int t, u, w;
	int* in = calloc(n, sizeof(int));
	
	initQ();

	for (int i = 0; i < n; i++) {
		in[i] = digrp.vertex[i].inDegree;
		if (in[i] == 0)
			enqueue(i);
	}

	t = 1;
	while (!isEmpty()) {
		u = dequeue();
		topOrder[t] = u;
		t++;

		e = digrp.vertex[u].outEdges->next;
		while (e != NULL) {
			w = digrp.edges[e->edgeindex].dest;
			in[w] = in[w] - 1;
			if (in[w] == 0)
				enqueue(w);

			e = e->next;
		}
	}

	if (t <= n)
		topOrder[0] = 0;
	else
		topOrder[0] = 1;

	return;
}


int main(void) {
	buildGraph();

	topologicalSort();

	if (topOrder[0] == 0)
		printf("0");
	else {
		for (int i = 1; i <= n; i++) {
			printf("%c ", digrp.vertex[topOrder[i]].name);
		}
	}

	return;
}