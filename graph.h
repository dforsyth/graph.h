#ifndef __GRAPH_H__
#define __GRAPH_H__

/* Build with c99, decls in for loops... */

#define DIGRAPH_CLOUD(name, vert_t, sz)					\
struct name {								\
	struct vert_t *map[sz];						\
}

#define DIGRAPH_ENTRY(vert_t, edge_t)					\
struct {								\
	struct vert_t *next;						\
	struct edge_t *edges;						\
	struct edgt_t *in;						\
	struct vert_t *stack;						\
}

#define DIGRAPH_EDGE(vert_t, edge_t)					\
struct {								\
	struct vert_t *src, *dst;					\
	struct edge_t *next;						\
	struct edge_t *in;						\
}

#define DIGRAPH_GENERATE(name, vert_t, v_fld, edge_t, e_fld, sz, v_hash, v_cmp)\
static __inline__ void							\
name##_DIGRAPH_INIT(struct name *cloud)					\
{									\
	for (int i = 0; i < sz; ++i) (cloud)->map[i] = NULL;		\
}									\
									\
static __inline__ struct vert_t *					\
name##_DIGRAPH_INSERT(struct name *cloud, struct vert_t *elm)		\
{									\
	int pos = v_hash(elm);						\
	for (struct vert_t *v = (cloud)->map[pos];			\
		v != NULL;						\
		v = (v)->v_fld.next)					\
		if (v_cmp(v, elm) == 0) return (v);			\
	(elm)->v_fld.next = (cloud)->map[pos];				\
	(cloud)->map[pos] = (elm);					\
	(elm)->v_fld.edges = NULL;					\
	(elm)->v_fld.in = NULL;						\
	return (NULL);							\
}									\
									\
static __inline__ struct vert_t *					\
name##_DIGRAPH_FIND(struct name *cloud, struct vert_t *elm)		\
{									\
	for (struct vert_t *v = (cloud)->map[v_hash(elm)];		\
		v != NULL;						\
		v = (v)->v_fld.next)					\
		if (v_cmp(v, elm) == 0) return (v);			\
	return (NULL);							\
}									\
									\
static __inline__ struct edge_t *					\
_##name##_DIGRAPH_EDGE_FIND(struct vert_t *src, struct vert_t *dst)	\
{									\
	for (struct edge_t *e = (src)->v_fld.edges;			\
		e != NULL;						\
		e = (e)->e_fld.next)					\
		if ((e)->e_fld.dst == dst) return (e);			\
	return (NULL);							\
}									\
									\
static __inline__ void							\
_##name##_DIGRAPH_ADD_EDGE(struct edge_t *edg, struct vert_t *src,	\
	struct vert_t *dst)						\
{									\
	(edg)->e_fld.next = (src)->v_fld.edges;				\
	(src)->v_fld.edges = edg;					\
	(edg)->e_fld.src = src;						\
	(edg)->e_fld.dst = dst;						\
}									\
									\
static __inline__ struct edge_t *					\
name##_DIGRAPH_ADD_EDGE(struct name *cloud, struct edge_t *edg,		\
	struct vert_t *src, struct vert_t *dst)				\
{									\
	struct vert_t *f_src, *f_dst;					\
	struct edge_t *f_edg;						\
	f_src = name##_DIGRAPH_FIND(cloud, src);			\
	f_dst = name##_DIGRAPH_FIND(cloud, dst);			\
	f_edg = _##name##_DIGRAPH_EDGE_FIND(f_src, f_dst);		\
	if (f_edg != NULL) return (f_edg);				\
	_##name##_DIGRAPH_ADD_EDGE(edg, f_src, f_dst);			\
	return (NULL);							\
}									\
									\
static __inline__ struct edge_t *					\
name##_DIGRAPH_REMOVE_EDGE(struct name *cloud, struct vert_t *src,	\
	struct vert_t *dst)						\
{									\
	struct vert_t *f_src = name##_DIGRAPH_FIND(cloud, src);		\
	struct vert_t *f_dst = name##_DIGRAPH_FIND(cloud, dst);		\
	struct edge_t *f_edg, *l_edg;					\
	for (f_edg = (f_src)->v_fld.edges, l_edg = NULL;		\
		f_edg != NULL;						\
		l_edg = f_edg, f_edg = (f_edg)->e_fld.next) {		\
		if ((f_edg)->e_fld.dst == f_dst) {			\
			if ((f_src)->v_fld.edges == f_edg)		\
				(f_src)->v_fld.edges = (f_edg)->e_fld.next; \
			else						\
				(l_edg)->e_fld.next = (f_edg)->e_fld.next; \
			(f_edg)->e_fld.next = NULL;			\
			break;						\
		}							\
	}								\
	return (f_edg);							\
}									\
									\
static __inline__ struct vert_t *					\
name##_DIGRAPH_EDGE_DST(struct edge_t *edg)				\
{									\
	return (((edg == NULL) ? NULL : (edg)->e_fld.dst));		\
}									\
									\
static __inline__ struct edge_t *					\
name##_DIGRAPH_FIRST_EDGE(struct vert_t *v)				\
{									\
	return ((v)->v_fld.edges);					\
}									\
									\
static __inline__ struct edge_t *					\
name##_DIGRAPH_EDGE_NEXT(struct edge_t *edg)				\
{									\
	return ((edg)->e_fld.next);					\
}									\
									\
static __inline__ struct vert_t *					\
name##_DIGRAPH_FIRST(struct name *cloud)				\
{									\
	return ((cloud)->map[0]);					\
}									\
									\
static __inline__ struct vert_t *					\
name##_DIGRAPH_NEXT(struct vert_t *elm)					\
{									\
	return ((elm)->v_fld.next);					\
}									\
									\
static __inline__ struct vert_t *					\
name##_DIGRAPH_REMOVE(struct name *cloud, struct vert_t *elm)		\
{									\
	unsigned int i = v_hash(elm);					\
	struct vert_t *v, *b = (cloud)->map[i];				\
	for (v = b;							\
		v != NULL;						\
		v = name##_DIGRAPH_NEXT(v))				\
		if (v_cmp(v, elm) == 0) {				\
			if ((v)->v_fld.edges != NULL)			\
				return (NULL);				\
			if (v == (cloud)->map[i])			\
				(cloud)->map[i] = (v)->v_fld.next;	\
			else 						\
				(b)->v_fld.next = (v)->v_fld.next;	\
			break;						\
		}							\
	return (v);							\
}

#define DIGRAPH_INIT(name, x)		name##_DIGRAPH_INIT(x)
#define DIGRAPH_INSERT(name, x, y)	name##_DIGRAPH_INSERT(x, y)
#define DIGRAPH_FIND(name, x, y)	name##_DIGRAPH_FIND(x, y)
#define DIGRAPH_ADD_EDGE(name, w, x, y, z)	name##_DIGRAPH_ADD_EDGE(w, x, y, z)
#define DIGRAPH_REMOVE_EDGE(name, x, y, z)	name##_DIGRAPH_REMOVE_EDGE(x, y, z)
#define DIGRAPH_EDGE_DST(name, x)	name##_DIGRAPH_EDGE_DST(x)
#define DIGRAPH_FIRST_EDGE(name, x)	name##_DIGRAPH_FIRST_EDGE(x)
#define DIGRAPH_EDGE_NEXT(name, x)	name##_DIGRAPH_EDGE_NEXT(x)
#define DIGRAPH_REMOVE(name, x, y)	name##_DIGRAPH_REMOVE(x, y)

/* x - v, y - e, z - src */
#define DIGRAPH_FOREACH_NEIGHBOR(x, y, name, z)				\
for (x = DIGRAPH_EDGE_DST(name, y = DIGRAPH_FIRST_EDGE(name, z));	\
	y != NULL;							\
	x = DIGRAPH_EDGE_DST(name, y = DIGRAPH_EDGE_NEXT(name, y)))

#endif
