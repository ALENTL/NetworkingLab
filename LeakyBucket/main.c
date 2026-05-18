#include <stdio.h>

int main() {
  int in, out, size, n, store = 0;

  printf("Enter bucket size: ");
  scanf("%d", &size);

  printf("Enter number of inputs: ");
  scanf("%d", &n);

  printf("Enter outgoing rate: ");
  scanf("%d", &out);

  while (n != 0) {
    printf("Incoming packet size: ");
    scanf("%d", &in);

    if (in <= (size - store)) {
      store += in;
      printf("Bucket Buffer Size %d out of %d\n", store, size);
    } else {
      printf("Dropped %d number of packets\n", in - (size - store));
      store = size;
      printf("Bucket Buffer Size %d out of %d\n", store, size);
    }

    store -= out;

    if (store < 0) {
      store = 0;
      printf("After outgoing %d packets left out of %d in buffer\n", store,
             size);
    }
    n--;
  }
}
