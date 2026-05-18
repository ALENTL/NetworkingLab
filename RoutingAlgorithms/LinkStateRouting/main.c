#include <stdio.h>

#define INFINITY 999
#define MAX 10

void dijkstra(int G[MAX][MAX], int vertices, int startNode);

int main() {
  int vertices, startNode, G[MAX][MAX];

  printf("Enter number of vertices: ");
  scanf("%d", &vertices);

  printf("Enter the adjacency matrix: \n");
  for (int i = 0; i < vertices; i++) {
    for (int j = 0; j < vertices; j++) {
      scanf("%d", &G[i][j]);
    }
  }
  printf("\nEnter the starting node: ");
  scanf("%d", &startNode);

  dijkstra(G, vertices, startNode);

  return 0;
}

void dijkstra(int G[MAX][MAX], int vertices, int startNode) {
  int cost[MAX][MAX], distance[MAX], pred[MAX], visited[MAX], count,
      minDistance, nextNode;

  for (int i = 0; i < vertices; i++) {
    for (int j = 0; j < vertices; j++) {
      if (G[i][j] == 0) {
        cost[i][j] = INFINITY;
      } else {
        cost[i][j] = G[i][j];
      }
    }
  }

  for (int i = 0; i < vertices; i++) {
    distance[i] = cost[startNode][i];
    pred[i] = startNode;
    visited[i] = 0;
  }

  distance[startNode] = 0;
  visited[startNode] = 1;
  count = 1;

  while (count < vertices - 1) {
    minDistance = INFINITY;

    for (int i = 0; i < vertices; i++) {
      if (distance[i] < minDistance && !visited[i]) {
        minDistance = distance[i];
        nextNode = i;
      }
    }

    visited[nextNode] = 1;
    for (int i = 0; i < vertices; i++) {
      if (!visited[i]) {
        if (minDistance + cost[nextNode][i] < distance[i]) {
          distance[i] = minDistance + cost[nextNode][i];
          pred[i] = nextNode;
        }
      }
    }

    count++;
  }

  for (int i = 0; i < vertices; i++) {
    if (i != startNode) {
      printf("\nDistance of Node %d = %d", i, distance[i]);
      printf("\nPath = %d", i);

      int j = i;

      do {
        j = pred[j];
        printf("<-%d", j);
      } while (j != startNode);
    }
  }
}
