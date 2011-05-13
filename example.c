#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "graph.h"

#define SZ 10000

struct node;
struct link;

struct node {
	int id;
	DIGRAPH_ENTRY(node, link) entry;
};

struct link {
	DIGRAPH_EDGE(node, link) edge;
};

int verbose = 0;

struct node *create_node(int i);
int echo(struct node *);
int cmp(struct node *, struct node *);

DIGRAPH_CLOUD(digraph, node, SZ);
DIGRAPH_GENERATE(digraph, node, entry, link, edge, SZ, echo, cmp);

int
main(int argc, char **argv)
{
	int i;
	int j;
	struct node *p;
	struct node ksrc;
	struct node kdst;
	struct link *e;

	struct digraph dg;

	if (argc > 1 && strcmp(argv[1], "v") == 0) {
		verbose = 1;
	}

	DIGRAPH_INIT(digraph, &dg);

	printf("Inserting nodes...");
	for (i = 0; i < SZ; ++i) {
		p = create_node(i);
		if (DIGRAPH_INSERT(digraph, &dg, p) != NULL) {
			warn("insert conflict: %d", p->id);
			continue;
		}
	}
	printf("done.\n");

	printf("Connecting nodes...");
	for (i = 0; i < SZ; ++i) {
		// hope everything's where it should be :O
		ksrc.id = i;
		for (j = 0; j < SZ; ++j) {
			if (j == i) continue;
			kdst.id = j;
			if (e == NULL) { // reuse unused edges
				e = calloc(1, sizeof(*e));
			}
			if (DIGRAPH_ADD_EDGE(digraph, &dg, e, &ksrc, &kdst) == NULL) {
				if (verbose) {
					printf("%d -> %d\n", ksrc.id, kdst.id);
				}
				e = NULL;
			}
		}
	}
	printf("done.\n");

	return (0);
}

int
echo(struct node *p)
{
	return (p->id);
}

int
cmp(struct node *p, struct node *q)
{
	return (p->id - q->id);
}

struct node *
create_node(int i)
{
	struct node *p;

	p = calloc(1, sizeof(*p));
	p->id = i;
	return (p);
}


