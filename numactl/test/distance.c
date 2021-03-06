/* Test numa_distance */
#include <numa.h>
#include <stdio.h>
#include <stdlib.h>

int main(void)
{
	int numnodes, maxnode, a, b, got_nodes = 0;
	int *node_to_use;
	long size, free_node_sizes;
	if (numa_available() < 0) {
		printf("no numa support in kernel\n");
		exit(1);
	}
	numnodes = numa_num_configured_nodes();
	maxnode = numa_max_node();
	node_to_use = (int *)malloc(numnodes * sizeof(int));
	for (a = 0; a <= maxnode; a++) {
		size = numa_node_size(a, &free_node_sizes);
		if(size != -1)
			node_to_use[got_nodes++] = a;
	}
	for (a = 0; a < got_nodes; a++){
		printf("%03d: ", node_to_use[a]);
		if (numa_distance(node_to_use[a], node_to_use[a]) != 10) {
			printf("%d: self distance is not 10 (%d)\n",
			       node_to_use[a], numa_distance(node_to_use[a],node_to_use[a]));
			exit(1);
		}
		for (b = 0; b < got_nodes; b++) {
			int d1 = numa_distance(node_to_use[a], node_to_use[b]);
			int d2 = numa_distance(node_to_use[b], node_to_use[a]);
			printf("%03d ", d1);
			if (d1 != d2) {
				printf("\n(%d,%d)->(%d,%d) wrong!\n",node_to_use[a],node_to_use[b],d1,d2);
				exit(1);
			}
		}
		printf("\n");
	}
	return 0;
}
