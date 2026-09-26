#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Node {
    int v;
    struct Node *next;
} Node;

// Add directed edge u -> v to adjacency list
void addEdge(Node **adj, int u, int v) {
    Node *newNode = (Node *)malloc(sizeof(Node));
    newNode->v = v;
    newNode->next = adj[u];
    adj[u] = newNode;
}

void freeAdj(Node **adj, int n) {
    for (int i = 0; i < n; ++i) {
        Node *cur = adj[i];
        while (cur) {
            Node *tmp = cur;
            cur = cur->next;
            free(tmp);
        }
    }
}

// DFS to fill order (stack)
void dfs1(Node **adj, int u, int *visited, int *order, int *orderIdx) {
    visited[u] = 1;
    for (Node *p = adj[u]; p; p = p->next) {
        if (!visited[p->v]) dfs1(adj, p->v, visited, order, orderIdx);
    }
    order[(*orderIdx)++] = u;
}

// DFS on transposed graph to collect component
void dfs2(Node **adjT, int u, int *visited, int *component, int *compSize) {
    visited[u] = 1;
    component[(*compSize)++] = u;
    for (Node *p = adjT[u]; p; p = p->next) {
        if (!visited[p->v]) dfs2(adjT, p->v, visited, component, compSize);
    }
}

int main() {
    char filename[1024];
    printf("Enter input filename: ");
    if (!fgets(filename, sizeof(filename), stdin)) return 0;
    // strip newline
    filename[strcspn(filename, "\n")] = '\0';

    FILE *fp = fopen(filename, "r");
    if (!fp) {
        fprintf(stderr, "Cannot open file '%s'\n", filename);
        return 1;
    }

    int n, m;
    if (fscanf(fp, "%d %d", &n, &m) != 2) {
        fprintf(stderr, "Invalid input file format\n");
        fclose(fp);
        return 1;
    }

    // allocate adjacency lists
    Node **adj = (Node **)calloc(n, sizeof(Node *));
    Node **adjT = (Node **)calloc(n, sizeof(Node *));
    for (int i = 0; i < n; ++i) { adj[i] = NULL; adjT[i] = NULL; }

    for (int i = 0; i < m; ++i) {
        int u, v;
        if (fscanf(fp, "%d %d", &u, &v) != 2) break;
        if (u < 0 || u >= n || v < 0 || v >= n) continue;
        addEdge(adj, u, v);
        addEdge(adjT, v, u); // transpose edge
    }
    fclose(fp);

    int *visited = (int *)calloc(n, sizeof(int));
    int *order = (int *)malloc(n * sizeof(int));
    int orderIdx = 0;

    // Fill vertices in order of finishing times
    for (int i = 0; i < n; ++i) if (!visited[i]) dfs1(adj, i, visited, order, &orderIdx);

    // prepare for second pass
    for (int i = 0; i < n; ++i) visited[i] = 0;

    int sccCount = 0;
    int *component = (int *)malloc(n * sizeof(int));

    // Process vertices in reverse finishing order
    for (int i = orderIdx - 1; i >= 0; --i) {
        int v = order[i];
        if (!visited[v]) {
            int compSize = 0;
            dfs2(adjT, v, visited, component, &compSize);
            ++sccCount;
            printf("SCC %d: {", sccCount);
            for (int k = 0; k < compSize; ++k) {
                printf("%d", component[k]);
                if (k + 1 < compSize) printf(", ");
            }
            printf("}\n");
        }
    }

    printf("No. of SCCs: %d\n", sccCount);

    free(component);
    free(order);
    free(visited);
    freeAdj(adj, n);
    freeAdj(adjT, n);
    free(adj);
    free(adjT);

    return 0;
}
