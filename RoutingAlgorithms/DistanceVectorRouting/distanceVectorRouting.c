#include <stdio.h>
#include <stdlib.h>

struct Node {
  int n;
  int *from;
  int *dist;
};

int main() {
  int n;

  printf("Enter the number of nodes: ");
  scanf("%d", &n);

  if (n <= 0) {
    printf("Invalid number of nodes\n");
    return -1;
  }

  int count;
  struct Node *routingTable = (struct Node *)malloc(n * sizeof(struct Node));

  int **cost = (int **)malloc(n * sizeof(int *));
  for (int i = 0; i < n; i++) {
    cost[i] = (int *)malloc(n * sizeof(int));
    routingTable[i].dist = (int *)malloc(n * sizeof(int));
    routingTable[i].from = (int *)malloc(n * sizeof(int));
  }

  printf("Use 999 to represent infinity\n");

  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      printf("Cost from node %d to node %d: ", i + 1, j + 1);
      scanf("%d", &cost[i][j]);
      routingTable[i].dist[j] = cost[i][j];
      routingTable[i].from[j] = j;
    }
    printf("\n");

    cost[i][i] = 0;
    routingTable[i].dist[i] = 0;
    routingTable[i].from[i] = i;
  }

  do {
    count = 0;

    for (int i = 0; i < n; i++) {
      for (int j = 0; j < n; j++) {
        for (int k = 0; k < n; k++) {
          if (routingTable[i].dist[j] > cost[i][k] + routingTable[k].dist[j]) {
            routingTable[i].dist[j] = cost[i][k] + routingTable[k].dist[j];
            routingTable[i].from[j] = k;
            count++;
          }
        }
      }
    }
  } while (count != 0);

  printf("--- Routing Tables ---\n");

  for (int i = 0; i < n; i++) {
    printf("Routing Table for the Router %d: \n", i + 1);
    printf("Destination\tNext Hop\tDistance\n");
    for (int j = 0; j < n; j++) {
      printf("\t%d\t\t%d\t\t%d\n", j + 1, routingTable[i].from[j] + 1,
             routingTable[i].dist[j]);
    }
  }

  return 0;
}
