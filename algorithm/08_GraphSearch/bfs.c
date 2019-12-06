#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <memory.h>

#define FRESH 1
#define VISITED 2
#define TREE 3
#define CROSS 4

typedef struct _vertices Vertices;
typedef struct _edges Edges;

struct _vertices {
	int num;
	int v_label;
};
struct _edges {
	int writed;
	int e_label;
	Vertices* endpoints[2];
};
typedef struct _graph {
	Vertices vertex[100];
	Edges edge[1000];
}Graph;
typedef struct _levelQueue {
	Vertices *v;
	struct _levelQueue* next;
}	LevelQ;

Graph grp;
int adjacentMatrix[100][100];
int adjMatrixcnt = 0;
LevelQ** levelQ;

void makeEdges(Vertices* v1, Vertices* v2) {
	grp.edge[adjMatrixcnt].writed = 1;
	grp.edge[adjMatrixcnt].endpoints[0] = v1;
	grp.edge[adjMatrixcnt++].endpoints[1] = v2;
}
void makeVertex(int vt1, int vt2) {
	Vertices* v1, * v2;

	v1 = &grp.vertex[vt1];
	v2 = &grp.vertex[vt2];

	v1->num = vt1 + 1;
	v2->num = vt2 + 1;

	// Set Vertice at adjacentMatrix
	if (adjacentMatrix[vt1][vt2] == -1 || adjacentMatrix[vt2][vt1] == -1){
		adjacentMatrix[vt1][vt2] = adjMatrixcnt;
		adjacentMatrix[vt2][vt1] = adjMatrixcnt;

		// make Edges
		makeEdges(v1, v2);
	}

	return;
}
bool isEmpty(LevelQ* lv) {
	if (lv->v == NULL)
		return true;
	return false;
}
LevelQ* makeLevelQ() {
	LevelQ* new_levelQ = (LevelQ*)malloc(sizeof(LevelQ));
	new_levelQ->v = NULL;
	new_levelQ->next = NULL;

	return new_levelQ;
}
void addLast(LevelQ* lv, Vertices* vx) {
	LevelQ* p = lv,* new_levelQ;

	if (p->v == NULL) {
		p->v = vx;
		p->next = NULL;
		return;
	}
	new_levelQ = (LevelQ*)malloc(sizeof(LevelQ));
	new_levelQ->v = vx;
	new_levelQ->next = NULL;

	while (p->next != NULL) {
		p = p->next;
	}
	p->next = new_levelQ;
	return;
}
void rBFS(Vertices* v) {
	LevelQ* p;
	Vertices* vp, *w;
	Edges* ep;
	int i, v_index;

	levelQ[0] = makeLevelQ();	// L0 <- empty list;
	addLast(levelQ[0], v);				// L0.addLast(v)
	v->v_label = VISITED;				// l(v) <- visited
	i = 0;
	printf("%d\n", v->num);
	while (!isEmpty(levelQ[i])) {
		levelQ[i+1] = makeLevelQ();	// L(i+1) <- empty list
		p = levelQ[i];

		for (; p != NULL; p = p->next) {	// for each v E L(i).elements()
			vp = p->v;
			v_index = vp->num - 1;

			for (int j = 0; j < 100; j++) {				// for each e E G.incidentEdges(v)

				if (adjacentMatrix[v_index][j] == -1)
					continue;

				ep = &grp.edge[adjacentMatrix[v_index][j]];
				if (ep->e_label == FRESH) {						// if (l(e) = Fresh)
					if (ep->endpoints[0]->num == vp->num)		// w <- G.opposite(v,e)
						w = ep->endpoints[1];
					else if (ep->endpoints[1]->num == vp->num)
						w = ep->endpoints[0];

					if (w->v_label == FRESH) {					// if(l(w) = Fresh)
						printf("%d\n", w->num);
						ep->e_label = TREE;						// l(e) = Tree
						w->v_label = VISITED;					// l(w) = Visited
						addLast(levelQ[i+1], w);				// L(i+1).addLast(w)
					}
					else {
						ep->e_label = CROSS;					// l(e) = Cross
					}
				}
			}
		}
		i++;
	}
	return;
}
void BFS(int s) {
	for (int i = 0; i < 100; i++) {
		if (grp.vertex[i].num == 0)
			continue;
		grp.vertex[i].v_label = FRESH;
	}

	for (int i = 0; i < 1000; i++) {
		if (grp.edge[i].writed == 0)
			continue;
		grp.edge[i].e_label = FRESH;
	}

	// make level queue
	levelQ = (LevelQ * *)malloc(sizeof(LevelQ*) * (adjMatrixcnt - 1));

	for (int i = 0; i < 100; i++) {
		if (grp.vertex[i].num == 0)
			continue;
		if (grp.vertex[i].v_label == FRESH && i == s-1)
			rBFS(&grp.vertex[i]);
	}
}

int main(void) {
	int n, m, s;	// 정점 수, 간선 수, 순회 시작 정점 번호
	int v1, v2;

	memset(&grp.vertex, 0, sizeof(Vertices));
	memset(&grp.edge, 0, sizeof(Edges));
	memset(&adjacentMatrix, -1, sizeof(adjacentMatrix));

	scanf("%d %d %d", &n, &m, &s);

	for (int i = 0; i < m; i++) {
		scanf("%d %d", &v1, &v2);

		makeVertex(v1 - 1, v2 - 1);
	}

	BFS(s);

	return 0;
}