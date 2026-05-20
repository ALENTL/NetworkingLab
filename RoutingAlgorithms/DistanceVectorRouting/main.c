#include <stdio.h>

#define MAX 20

struct Node {
  unsigned int from[MAX];
  unsigned int dist[MAX];
} rt[MAX];

int main() {
  int n, cost[MAX][MAX], count;

  printf("Enter the number of nodes (Max: %d): ", MAX);
  scanf("%d", &n);

  if (n <= 0 || n > MAX) {
    printf("Invalid number of nodes!\n");
    return -1;
  }

  printf("Use 999 to represent infinity!\n");
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      printf("Cost from node %d to node %d: ", i + 1, j + 1);
      scanf("%d", &cost[i][j]);
      rt[i].dist[j] = cost[i][j];
      rt[i].from[j] = j;
    }

    cost[i][i] = 0;
    rt[i].dist[i] = 0;
    rt[i].from[i] = i;
  }

  do {
    count = 0;

    for (int i = 0; i < n; i++) {
      for (int j = 0; j < n; j++) {
        for (int k = 0; k < n; k++) {
          if (rt[i].dist[j] > cost[i][k] + rt[k].dist[j]) {
            rt[i].dist[j] = cost[i][k] + rt[k].dist[j];
            rt[i].from[j] = k;
            count++;
          }
        }
      }
    }
  } while (count != 0);

  printf("\n---Routing Tables---\n");

  for (int i = 0; i < n; i++) {
    printf("\nRouting Table for Router %d:\n", i + 1);
    printf("Destination\tNext Hop\tDistance\n");

    for (int j = 0; j < n; j++) {
      printf("%d\t\t%d\t\t%d\n", j + 1, rt[i].from[j], rt[i].dist[j]);
    }
  }

  return 0;
}
