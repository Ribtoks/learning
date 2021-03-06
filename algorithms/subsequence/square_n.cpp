#include <iostream>

#define MAXN 10000

int main () {
   int d[MAXN];
   int p[MAXN];
   int arr[MAXN];

   int n;
   std::cin >> n;
   int m = n, i = 0, a;
   while (m--) {
      std::cin >> a;
      arr[i++] = a;
   }

   // d[i] - length of longeset subsequence which ends in i
   memset(d, 0, n*sizeof(int));
   for (int i = 0; i < n; ++i) { p[i] = -1; }

   d[0] = 1;
   for (int i = 1; i < n; ++i) {
      d[i] = 1;
      for (int j = 0; j < i; ++j) {
         // just a[j] which is less than a[i] and is possible next element
         if (arr[j] < arr[i] && (d[j] + 1 > d[i])) {
            d[i] = d[j] + 1;
            p[i] = j;
         }
      }
   }

   int pmax = -1, dmax = -1;
   for (int i = 0; i < n; ++i) {
      if (d[i] > dmax) {
         dmax = d[i];
         pmax = i;
      }
   }

   int curr = pmax;
   while (curr != -1) {
      std::cout << arr[curr] << " ";
      curr = p[curr];
   }

   return 0;
}
