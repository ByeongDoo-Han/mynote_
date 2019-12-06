#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct _vertices Vertices;
typedef struct _edges Edges;

struct _vertices {
	int num;
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
int adjacentMatrix[6][6];
int adjMatrixcnt = 7;

void SetAdjacentMatrix() {
	int tmp[6][6] = { {-1,  0,   1, 2, -1,  3},
					  { 0, -1,  4, -1, -1, -1},
					  { 1,  4, -1, -1,  5, -1},
					  { 2, -1, -1, -1, -1, -1},
					  {-1, -1,  5, -1,  6,  7},
					  { 3, -1, -1, -1,  7, -1} };

	memcpy(adjacentMatrix, tmp, sizeof(tmp));
}
void setEdge(int v1, int v2, int weight) {
	int index = adjacentMatrix[v1][v2];

	grp.edge[index].endpoints[0] = &grp.vertex[v1];
	grp.edge[index].endpoints[1] = &grp.vertex[v2];
	grp.edge[index].weight = weight;
}
void makeGraph(void) {
	SetAdjacentMatrix();

	for (int i = 0; i < 6; i++) {
		grp.vertex[i].num = i + 1;
	}

	// vertex 1
	setEdge(0, 1, 1);	// 1->2
	setEdge(0, 2, 1);	// 1->3
	setEdge(0, 3, 1);	// 1->4
	setEdge(0, 5, 2);	// 1->6

	// vertex 2
	setEdge(1, 2, 1);	// 2->3

	// vertex 3
	setEdge(2, 4, 4);	// 3->5

	// vertex 4

	// vertex 5
	setEdge(4, 4, 4);	// 5->5
	setEdge(4, 5, 3);	// 5->6

	// vertex 6
}
void adjacnetVertices(int v) {
	int target;
	if (v < 0 || v >= 6) {
		printf("-1\n");
		return;
	}

	for (int i = 0; i < 6; i++) {
		if ((target = adjacentMatrix[v][i]) != -1) {
			if(grp.edge[target].endpoints[1]->num != v + 1)
				printf(" %d %d", grp.edge[target].endpoints[1]->num, grp.edge[target].weight);
			else
				printf(" %d %d", grp.edge[target].endpoints[0]->num, grp.edge[target].weight);
		}
	}
	printf("\n");

	return;
}
void modifyEdgeWeight(int v1, int v2, int change_weight) {
	int target;
	if (v1 < 0 || v1 >= 6 || v2 < 0 || v2 >= 6) {
		printf("-1\n");
		return;
	}
	if (change_weight == 0) {
		adjacentMatrix[v1][v2] = -1;
		adjacentMatrix[v2][v1] = -1;
	}
	else {
		if(adjacentMatrix[v1][v2] != -1)
			grp.edge[adjacentMatrix[v1][v2]].weight = change_weight;
		else {
			adjacentMatrix[v1][v2] = ++adjMatrixcnt;
			adjacentMatrix[v2][v1] = adjMatrixcnt;
			setEdge(v1, v2, change_weight);
		}
	}
	return;
}
int main(void) {
	char com;
	int node_num, v1, v2, change_weight;
	makeGraph();

	while (1) {
		scanf("%c", &com);

		switch (com) {
		case'a':
			scanf("%d", &node_num);
			adjacnetVertices(node_num-1);
			break;
		case'm':
			scanf("%d %d %d", &v1, &v2, &change_weight);
			modifyEdgeWeight(v1 - 1, v2 - 1, change_weight);
			break;
		case'q':
			return 0;
		}
		while (getchar() != '\n') {}
	}


	return 0;
}