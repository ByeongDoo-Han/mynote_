#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <memory.h>

#define INT_MAX	2147483647
typedef struct _incidentEdges IncidentEdges;
typedef struct _vertices Vertices;
typedef struct _edges Edges;

struct _incidentEdges {
	int edgeindex;
	struct _incidentEdges* next;
};
struct _vertices {
	int num;
	int d_label;		// distance
	bool isLocated;		// is vertex in heap(priority queue)?
	int p_label;		// parent
	IncidentEdges* head;
};
struct _edges {
	int weight;
	Vertices* endpoints[2];
};
typedef struct _graph {
	Vertices vertex[101];
	Edges edge[1001];
}Graph;
typedef struct _heap {
	int key;			// vertex's distance
	Vertices* v;		// locator of vertex in heap(priority queue)
}Heap;

Graph grp;
int edgecnt = 0;
int n, m;	// n = 정점 수, m = 간선 수
Heap heap[100];
int heap_size = 0;

/* makeNode*/
// 부착간선의 노드를 생성
IncidentEdges* makeNode(int edgeindex) {
	IncidentEdges* new_node = (IncidentEdges*)malloc(sizeof(IncidentEdges));
	new_node->edgeindex = edgeindex;
	new_node->next = 0;

	return new_node;
}
/* opposite */
// v에 연결된 부착간선 p에 연결된 다른 정점을 반환
// V -(p)- W
Vertices* opposite(Vertices* v, IncidentEdges* p) {
	Vertices* w = NULL;

	if (grp.edge[p->edgeindex].endpoints[0]->num == v->num)
		w = grp.edge[p->edgeindex].endpoints[1];
	else if (grp.edge[p->edgeindex].endpoints[1]->num == v->num)
		w = grp.edge[p->edgeindex].endpoints[0];

	return w;
}
/* SetEdge */
// vertex(u)와 vertex(w)를 연결하는 간선설정
void SetEdge(int unum, int wnum, int weight) {
	grp.edge[edgecnt].endpoints[0] = &grp.vertex[unum - 1];
	grp.edge[edgecnt].endpoints[1] = &grp.vertex[wnum - 1];
	grp.edge[edgecnt].weight = weight;
}
/* insertEdge*/
// vertex(u)와 vertex(w)를 연결하는 간선 생성
void insertEdge(int unum, int wnum, int edgeindex) {
	Vertices* u, * w;
	IncidentEdges* new_incident, * p;

	u = &grp.vertex[unum - 1];
	w = &grp.vertex[wnum - 1];

	new_incident = makeNode(edgeindex);
	p = u->head;

	// 간선의 순서는 삽입순서로 정렬한다. (오름차순 X)
	while (p->next != NULL) {
		if (grp.edge[p->edgeindex].endpoints[0] == u)
			break;
		p = p->next;
	}
	new_incident->next = p->next;
	p->next = new_incident;

	return;
}
/* SetVertex*/
// vertex(v)와 vertex(w)의 정점설정
void SetVertex(int vnum, int wnum) {
	Vertices* u, * w;

	u = &grp.vertex[vnum - 1];
	w = &grp.vertex[wnum - 1];

	u->num = vnum;
	w->num = wnum;

	// make header
	if (u->head == NULL)
		u->head = makeNode(0);
	if (w->head == NULL)
		w->head = makeNode(0);
}
/* buildGraph */
// 무방향 가중그래프 생성
void buildGraph() {
	int unum, wnum, weight;

	scanf("%d %d", &n, &m);

	memset(&grp.vertex, 0, sizeof(Vertices) * n);
	memset(&grp.edge, 0, sizeof(Edges) * m);

	for (int i = 0; i < m; i++) {
		scanf("%d %d %d", &unum, &wnum, &weight);

		SetVertex(unum, wnum);
		SetEdge(unum, wnum, weight);
		insertEdge(unum, wnum, edgecnt);
		insertEdge(wnum, unum, edgecnt++);
	}
}
/* deallocate */
// 부착간선의 동적메모리 할당해제
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
/* printIncident */
// 부착간선을 출력한다.
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
////////////////////////Priority queue (heap)
/* DownHeap */
// heap의 i번째를 시작으로 재귀적 HeapSort를 실시
// 최소힙이 구성된다.
void DownHeap(int i)
{
	Heap tmp;
	int smaller;

	if (2 * i > heap_size)
		return;
	smaller = 2 * i;						// smaller <- lchild

	if (2 * i + 1 <= heap_size) {
		if (heap[smaller].key > heap[2 * i + 1].key)	// lchild > rchild
			smaller = 2 * i + 1;				// smaller <- rchild
	}

	if (heap[i].key <= heap[smaller].key)
		return;

	if (heap[i].key >= heap[smaller].key) {			// swap
		tmp = heap[i];
		heap[i] = heap[smaller];
		heap[smaller] = tmp;
	}
	DownHeap(smaller);
}
/* rBuildHeap */
// 상향식재귀적 힙생성 O(n)
void rBuildHeap(int i)
{
	if (i > heap_size)
		return;
	rBuildHeap(2 * i);
	rBuildHeap(2 * i + 1);
	DownHeap(i);
	return;
}
/* makeHeap */
// Heap의 key값은 정점의 distance값으로 설정
// heap의 v값은 저장된 vertex의 위치를 의미
// heap에 들어갈시 들어가는 정점의 isLocated는 false에서 true로 전환해준다.
void makeHeap() {
	memset(heap, 0, sizeof(Heap) * 100);
	for (int i = 1; i <= n; i++) {
		heap[i].key = grp.vertex[i - 1].d_label;
		heap[i].v = &grp.vertex[i - 1];
		grp.vertex[i - 1].isLocated = true;
		heap_size++;
	}
	rBuildHeap(1);
}
/* printHeap */
// Heap의 key값을 출력
void printHeap()
{
	for (int i = 1; i <= heap_size; i++)
	{
		printf(" %d", heap[i].key);
	}
	printf("\n");
}
/* removeMin */
// heap의 처음 키값을 가진 vertex 반환
Vertices* removeMin()
{
	int deleted = heap[1].key;
	heap[1] = heap[heap_size];
	heap[heap_size].key = 0;
	heap_size--;
	rBuildHeap(1);

	for (int i = 0; i < n; i++) {
		if (grp.vertex[i].d_label == deleted) {
			grp.vertex[i].isLocated = false;
			return &grp.vertex[i];
		}
	}

	return NULL;
}
//////////////////////// Prim-JarnikMST
/* replaceKey */
// vertex(z)를 가리키는 heap의 key값을 weight(w(u,z))로 변경
void replaceKey(Vertices* z, int weight) {
	for (int i = 1; i <= heap_size; i++) {
		if (heap[i].v == z) {
			heap[i].key = weight;
			break;
		}
	}
	rBuildHeap(1);
}
void Prim_JarnikMST() {
	Vertices* s, * u, * z;
	IncidentEdges* p;
	int sum = 0;

	for (int i = 0; i < n; i++) {															// 1. for each v E G.vertices()
		grp.vertex[i].d_label = INT_MAX;													//		d(v) <- infinite value
		grp.vertex[i].p_label = 0;															//		p(v) <- NULL
	}
	s = &grp.vertex[0];																		// 2. s <- a vertex of G
	s->d_label = 0;																			// 3. d(s) <- 0
	makeHeap();																				// 4. Q <- a priorty queue containing all ther vertices of G using d labels as Keys
	for (int i = 0; i < n; i++) {															// 5. while(!Q.isEmpty())
		u = removeMin();																	//		u <- Q.removeMin()
		p = u->head->next;
		while (p != NULL) {																	//		for each e E G.incidentEdges(u)
			z = opposite(u, p);																//		z <- G.opposite(u,e)
			if (z->isLocated == true && (grp.edge[p->edgeindex].weight < z->d_label)) {		//		if((z E Q) &(w(u,z) < d(z))
				z->d_label = grp.edge[p->edgeindex].weight;									//			d(z) <- w(u, z)
				z->p_label = p->edgeindex;													//			p(z) <- e
				replaceKey(z, grp.edge[p->edgeindex].weight);								//			Q.replaceKey(z, w(u, z))
			}
			p = p->next;
		}
		printf("%d ", u->num);
		sum += u->d_label;
	}
	printf("\n");
	printf("%d", sum);
}

int main(void) {
	buildGraph();
	Prim_JarnikMST();
	deallocate();
}