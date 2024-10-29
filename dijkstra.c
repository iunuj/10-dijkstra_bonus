#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define MAX_NODES 10 

typedef struct {
    int destination;
    int weight;
    struct Edge* next;
} Edge;

typedef struct {
    Edge* head;
} GraphNode;

typedef struct {
    GraphNode nodes[MAX_NODES];
    int num_nodes;
} Graph;

typedef struct {
    int node;
    int distance;
} HeapNode;

typedef struct {
    HeapNode heap[MAX_NODES];
    int size;
} MinHeap;

void addEdge(Graph* graph, int src, int dest, int weight) {
    Edge* edge = (Edge*)malloc(sizeof(Edge));
    edge->destination = dest;
    edge->weight = weight;
    edge->next = graph->nodes[src].head;
    graph->nodes[src].head = edge;
}

void swap(HeapNode* a, HeapNode* b) {
    HeapNode temp = *a;
    *a = *b;
    *b = temp;
}

void minHeapify(MinHeap* minHeap, int idx) {
    int smallest = idx;
    int left = 2 * idx + 1;
    int right = 2 * idx + 2;

    if (left < minHeap->size && minHeap->heap[left].distance < minHeap->heap[smallest].distance)
        smallest = left;

    if (right < minHeap->size && minHeap->heap[right].distance < minHeap->heap[smallest].distance)
        smallest = right;

    if (smallest != idx) {
        swap(&minHeap->heap[smallest], &minHeap->heap[idx]);
        minHeapify(minHeap, smallest);
    }
}

HeapNode extractMin(MinHeap* minHeap) {
    HeapNode root = minHeap->heap[0];
    minHeap->heap[0] = minHeap->heap[--minHeap->size];
    minHeapify(minHeap, 0);
    return root;
}

void insertMinHeap(MinHeap* minHeap, int node, int distance) {
    int i = minHeap->size++;
    minHeap->heap[i].node = node;
    minHeap->heap[i].distance = distance;

    while (i && minHeap->heap[i].distance < minHeap->heap[(i - 1) / 2].distance) {
        swap(&minHeap->heap[i], &minHeap->heap[(i - 1) / 2]);
        i = (i - 1) / 2;
    }
}

void printState(int distances[], int found[], int num_nodes) {
    printf("Distance: ");
    for (int i = 0; i < num_nodes; i++) {
        if (distances[i] == INT_MAX) {
            printf("* ");
        }
        else {
            printf("%d ", distances[i]);
        }
    }
    printf("\nFound:    ");
    for (int i = 0; i < num_nodes; i++) {
        printf("%d ", found[i]);
    }
    printf("\n");
}

void dijkstra(Graph* graph, int start) {
    int distances[MAX_NODES];
    int found[MAX_NODES] = { 0 };
    int foundOrder[MAX_NODES];
    int orderIndex = 0;

    for (int i = 0; i < graph->num_nodes; i++) distances[i] = INT_MAX;
    distances[start] = 0;

    MinHeap minHeap;
    minHeap.size = 0;
    insertMinHeap(&minHeap, start, 0);

    while (minHeap.size > 0) {
        HeapNode minNode = extractMin(&minHeap);
        int u = minNode.node;

        if (found[u]) continue;
        found[u] = 1;
        foundOrder[orderIndex++] = u + 1; 

        Edge* edge = graph->nodes[u].head;
        while (edge != NULL) {
            int v = edge->destination;
            int weight = edge->weight;

            if (distances[u] + weight < distances[v]) {
                distances[v] = distances[u] + weight;
                insertMinHeap(&minHeap, v, distances[v]);
            }
            edge = edge->next;
        }

        printState(distances, found, graph->num_nodes);
    }

    printf("\n");
    printf("Found Order: ");
    for (int i = 0; i < orderIndex; i++) {
        printf("%d ", foundOrder[i]);
    }
    printf("\n");
}

int main() {
    Graph graph;
    graph.num_nodes = MAX_NODES;  
    for (int i = 0; i < graph.num_nodes; i++) {
        graph.nodes[i].head = NULL;
    }

    addEdge(&graph, 0, 1, 3);
    addEdge(&graph, 0, 2, 8);
    addEdge(&graph, 1, 2, 7);
    addEdge(&graph, 1, 3, 10);
    addEdge(&graph, 2, 4, 11);
    addEdge(&graph, 3, 4, 5);
    addEdge(&graph, 4, 5, 2);
    addEdge(&graph, 5, 6, 3);
    addEdge(&graph, 6, 7, 4);
    addEdge(&graph, 7, 8, 5);
    addEdge(&graph, 8, 9, 6);

    dijkstra(&graph, 0);

    return 0;
}
