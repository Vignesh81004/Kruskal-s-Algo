#include <stdio.h>
#include <stdlib.h>

// Structure to represent an edge in the graph
struct Edge {
    int src, dest, weight;
};

// Structure to represent a subset for union-find
struct Subset {
    int parent;
    int rank;
};

// Function to find the set of an element using path compression
int find(struct Subset subsets[], int i) {
    if (subsets[i].parent != i)
        subsets[i].parent = find(subsets, subsets[i].parent);
    return subsets[i].parent;
}

// Function to perform union of two sets using rank
void unionSets(struct Subset subsets[], int x, int y) {
    int xroot = find(subsets, x);
    int yroot = find(subsets, y);

    if (subsets[xroot].rank < subsets[yroot].rank)
        subsets[xroot].parent = yroot;
    else if (subsets[xroot].rank > subsets[yroot].rank)
        subsets[yroot].parent = xroot;
    else {
        subsets[yroot].parent = xroot;
        subsets[xroot].rank++;
    }
}

// Function to compare two edges based on their weights for sorting
int compareEdges(const void* a, const void* b) {
    return ((struct Edge*)a)->weight - ((struct Edge*)b)->weight;
}

// Function to implement Kruskal's algorithm
void kruskalMST(struct Edge edges[], int V, int E) {
    // Allocate memory for the subsets
    struct Subset* subsets = (struct Subset*)malloc(V * sizeof(struct Subset));

    // Initialize subsets with single elements
    for (int i = 0; i < V; i++) {
        subsets[i].parent = i;
        subsets[i].rank = 0;
    }

    // Result array to store the resultant MST
    struct Edge* result = (struct Edge*)malloc((V - 1) * sizeof(struct Edge));

    // Index for the result array
    int resultIndex = 0;

    // Sort all edges in non-decreasing order of their weights
    qsort(edges, E, sizeof(edges[0]), compareEdges);

    // Iterate through all sorted edges
    for (int i = 0; resultIndex < V - 1 && i < E; i++) {
        // Extract the next edge from the sorted array
        struct Edge nextEdge = edges[i];

        // Find the sets of the source and destination vertices
        int x = find(subsets, nextEdge.src);
        int y = find(subsets, nextEdge.dest);

        // If including this edge doesn't cause a cycle, add it to the result and increment the result index
        if (x != y) {
            result[resultIndex++] = nextEdge;
            unionSets(subsets, x, y);
        }
    }

    // Print the resulting MST
    printf("Edges in the Minimum Spanning Tree:\n");
    for (int i = 0; i < resultIndex; i++) {
        printf("%d -- %d\tWeight: %d\n", result[i].src, result[i].dest, result[i].weight);
    }

    // Free allocated memory
    free(subsets);
    free(result);
}

// Driver program to test the above functions
int main() {
    // Example: Graph with 4 vertices and 5 edges
    int V = 4, E = 5;
    struct Edge edges[] = { {0, 1, 10}, {0, 2, 6}, {0, 3, 5}, {1, 3, 15}, {2, 3, 4} };

    // Call Kruskal's algorithm
    kruskalMST(edges, V, E);

    return 0;
}
