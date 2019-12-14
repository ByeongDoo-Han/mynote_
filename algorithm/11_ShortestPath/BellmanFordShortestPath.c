#include <stdio.h>
#include <memory.h>

#define INFINITE 30000

typedef struct _vertices Vertices;
typedef struct _edges Edges;

struct _vertices {
	int num;
	int d_label;
};
struct _edges {
	int origin;
	int dest;
	int weight;
};
typedef struct _digraph {
	Vertices vertex[102];
	Edges edges[1002];
}digraph;

digraph digrp;

int n;	// number of vertex
int m;	// number of direct edge
int s;	// start vertex

void init() {
	memset(&digrp.vertex, 0, sizeof(Vertices) * n);
	memset(&digrp.edges, 0, sizeof(Edges) * m);

	for (int i = 1; i <= n; i++) {
		digrp.vertex[i].num = i;
	}
}

void setDirectedEdge(int u, int w, int i, int weight) {
	digrp.edges[i].origin = u;
	digrp.edges[i].dest = w;
	digrp.edges[i].weight = weight;
	return;
}

void buildGraph() {
	int weight;
	int u, w;

	scanf("%d %d %d", &n, &m, &s);				// n = number of vertex, m = number of edges, s = start vertex
	init();
	
	for (int i = 1; i <= m; i++) {
		scanf("%d %d %d", &u, &w, &weight);
		setDirectedEdge(u, w, i, weight);
	}
	return;
}

void BellmanFordShortestPath(Vertices* s) {
	Vertices* u, * z;
	int min;
	for (int i = 1; i <= n; i++) {										// 1. for each v E G.vertices();
		digrp.vertex[i].d_label = INFINITE;								//		d(v) <- INFINITE
	}
	s->d_label = 0;														// 2. d(s) <- 0
	
	for (int i = 1; i < n; i++) {										// 3. for i <- 1 to n - 1
		for (int j = 1; j <= m; j++) {									//		for each e E G.edges()
			u = &digrp.vertex[digrp.edges[j].origin];					//			u <- G.origin(e)
			z = &digrp.vertex[digrp.edges[j].dest];						//			z <- G.opposite(u, e)
			if (u->d_label == INFINITE || digrp.edges[j].weight == INFINITE)
				continue;
			min = u->d_label + digrp.edges[j].weight;
			if (z->d_label > min) {	// d(z) <- min (d(z), d(u) + w(u,z))
				z->d_label = min;
			}
		}
	}
}
int main(void) {
	buildGraph();
	BellmanFordShortestPath(&digrp.vertex[s]);
	for (int i = 1; i <= n; i++) {
		if (i == s)
			continue;
		if (digrp.vertex[i].d_label == INFINITE)
			continue;
		printf("%d %d\n", digrp.vertex[i].num, digrp.vertex[i].d_label);
	}
	return;
}