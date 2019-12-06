#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <memory.h>

#define FRESH 1
#define VISITED 2
#define TREE 3
#define BACK 4


typedef struct _incidentEdges IncidentEdges;
typedef struct _vertices Vertices;
typedef struct _edges Edges;

struct _incidentEdges {
	int edgeindex;
	struct _incidentEdges* next;
};
struct _vertices {
	int num;
	int v_label;
	IncidentEdges* head;
};
struct _edges {
	int e_label;
	int writed;
	Vertices* endpoints[2];
};
typedef struct _graph {
	Vertices vertex[101];
	Edges edge[1000];
}Graph;

Graph grp;
int edgecnt = 0;
int n, m, s;	// 정점 수, 간선 수, 순회 시작 정점 번호

IncidentEdges* makeNode(int i) {
	IncidentEdges* new_node = (IncidentEdges*)malloc(sizeof(IncidentEdges));
	new_node->edgeindex = i;
	new_node->next = 0;

	return new_node;
}
bool areadjacent(Vertices* u, Vertices* w) {
	IncidentEdges* p = w->head;
	while (p != NULL) {
		if (grp.edge[p->edgeindex].endpoints[0] == u ||
			grp.edge[p->edgeindex].endpoints[1] == u) {
			return true;
		}
		p = p->next;
	}
	return false;
}
//Vertices* opposite(Vertices* v, IncidentEdges* p) {
//	Vertices* w = NULL;
//
//	if (p->edgeindex->endpoints[0]->num == v->num)
//		w = p->edgeindex->endpoints[1];
//	else if (p->edgeindex->endpoints[1]->num == v->num)
//		w = p->edgeindex->endpoints[0];
//
//	return w;
//}
void makeEdge(int unum, int wnum) {
	grp.edge[edgecnt].endpoints[0] = &grp.vertex[unum - 1];
	grp.edge[edgecnt].endpoints[1] = &grp.vertex[wnum - 1];
	grp.edge[edgecnt].writed = 1;
}
void insertEdge(int unum, int wnum, int i) {
	Vertices* u, * w;
	IncidentEdges* new_incident, *p;

	u = &grp.vertex[unum - 1];
	w = &grp.vertex[wnum - 1];

	new_incident = makeNode(i);
	p = u->head;

	while (p->next != NULL) {
		if (grp.edge[p->edgeindex].endpoints[0] == u)

			break;
		p = p->next;
	}
	new_incident->next = p->next;
	p->next = new_incident;
	
	return;
}
void makeVertex(int vnum, int wnum) {
	Vertices* u, * w;

	u = &grp.vertex[vnum-1];
	w = &grp.vertex[wnum-1];

	u->num = vnum;
	w->num = wnum;

	// make header
	if(u->head == NULL)
		u->head = makeNode(0);
	if(w->head == NULL)
		w->head = makeNode(0);
}
void buildGraph() {
	int unum, wnum;

	scanf("%d %d %d", &n, &m, &s);

	memset(&grp.vertex, 0, sizeof(Vertices)*n);
	memset(&grp.edge, 0, sizeof(Edges)*m);

	for (int i = 0; i < m; i++) {
		scanf("%d %d", &unum, &wnum);

		makeVertex(unum, wnum);
		makeEdge(unum, wnum);
		insertEdge(unum, wnum, edgecnt);
		insertEdge(wnum, unum, edgecnt++);
	}	
}
//void rDFS(Vertices* v) {
//	Vertices* w;
//	IncidentEdges* p;
//
//	v->v_label = VISITED;					// l(v) <- visited
//	printf("%d\n", v->num);
//	p = v->head->next;
//	while (p != NULL) {						//for each e <- G.incidentEdges(v)
//		if (p->edgeindex->e_label == FRESH) {
//			w = opposite(v, p);				// w<-G.opposite(v,e)
//			if (w->v_label == FRESH) {		// if(l(w) = Fresh)
//				p->edgeindex->e_label = TREE;	// l(e) <- Tree
//				rDFS(w);						// rDFS(G,w)
//			}
//			else
//				p->edgeindex->e_label = BACK;	// l(e) <- Back
//		}
//		p = p->next;
//	}
//}
//void DFS(int s) {
//	for (int i = 0; i < 100; i++) {
//		if (grp.vertex[i].num == 0)
//			continue;
//		grp.vertex[i].v_label = FRESH;
//	}
//
//	for (int i = 0; i < 1000; i++) {
//		if (grp.edge[i].writed == 0)
//			continue;
//		grp.edge[i].e_label = FRESH;
//	}
//
//	for (int i = 0; i < 100; i++) {
//		if (grp.vertex[i].num == 0)
//			continue;
//		if (grp.vertex[i].v_label == FRESH && i == s)
//			rDFS(&grp.vertex[i]);
//	}
//}
void deallocate(void) {
	IncidentEdges* p, * removed;
	for (int i = 0; i < 100; i++) {
		if (grp.vertex[i].num == 0)
			continue;
		p = grp.vertex[i].head;
		while (p != NULL) {
			removed = p;
			p = p->next;
			free(removed);
		}
	}
}
void printIncident() {
	IncidentEdges* p;
	for (int i = 0; i < 100; i++) {
		if (grp.vertex[i].num == 0)
			continue;
		printf("v->num : %d \n", grp.vertex[i].num);
		p = grp.vertex[i].head->next;
		while (p != NULL) {
			printf("[0] %d -> [1] %d\t", grp.edge[p->edgeindex].endpoints[0]->num, grp.edge[p->edgeindex].endpoints[1]->num);
			p = p->next;
		}
		printf("\n");
	}
}
int main(void) {
	buildGraph();
	
	printIncident();
	//DFS(s);

	//deallocate();
}