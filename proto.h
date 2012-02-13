#ifndef PROTO_H
#define PROTO_H

/* 
 * Gabriel Parmer, gparmer@gwu.edu, 2012
 *
 * ACM presentation on how object orientation is implemented.  This
 * file demonstrates a simple structure for prototype-based object
 * orientation.  Please see the Challenges throughout.  Sounds like a
 * great activity for the weekend hackathon!
 *
 * This is similar in semantics to the object models of javascript,
 * ruby, and python.
 */

/*** The following macros define the prototype API ***/

/* 
 * Prototype get and set functions for variables and objects.
 */
#define STRX(x) #x  		/* standard macro trick to make a string */
#define STR(x)  STRX(x)
#define PGETV(o, f)    proto_field_get(o, STR(f), PROTO_VAR)
#define PGETO(o, f)    proto_field_get(o, STR(f), PROTO_OBJ)
#define PSETV(o, f, v) proto_field_set(o, STR(f), (void*)v, PROTO_VAR)
#define PSETO(o, f, v) proto_field_set(o, STR(f), (void*)v, PROTO_OBJ)
#define PSETF(o, f, v) proto_field_set(o, STR(f), (proto_fn)v, PROTO_FN)

/* 
 * Method invocation of method f on object o.  Challenge: Avoid NULL
 * pointer exceptions when accessing a function field that doesn't
 * exist.
 */
#define PINV(o, f,...) ((proto_fn)proto_field_get(o, STR(f), PROTO_FN))(o, ##__VA_ARGS__)

/* Prototype object allocation and deallocation */
#define PALLOC() proto_alloc()
#define PFREE(x) proto_free(x)

/* super */
#define PSUPER(this) proto_field_parent(this)
#define PSET_PARENT(o, p) (o->__proto__ = p)

/* 
 * Advanced Challenge: Inline caches
 * http://en.wikipedia.org/wiki/Inline_caching can greatly improve
 * performance in dynamic languages.  This must be done with macro
 * magic.  You can use the macros above to implement inline caches.
 * Note that you might find the __LINE__ and __FILE__ macro variables
 * particularly useful.
 */

/* Advanced challenge:  add garbage collection of proto_objs to the library! */

#include <assert.h>
#include <string.h>
#include <malloc.h>

typedef enum {
	PROTO_FN,
	PROTO_VAR,
	PROTO_OBJ
} proto_field_t;

/* Why 10 fields per object?  Hint:  sizeof(struct proto_obj) == 32 words, or 128 bytes*/
#define PROTO_DEF_NFIELDS 10
struct proto_field {
	proto_field_t type;
	char *name;
	void *value;
};

struct proto_obj {
	struct proto_field fields[PROTO_DEF_NFIELDS];
	struct proto_obj *__proto__;
	/* Currently unused.  Would hold additional fields, perhaps in a hash table.  See challenge below. */
	struct proto_field *fields_extra;
};

/* 
 * Prototype functions to invoke methods of an object.  These method
 * invocation calls are complicated to deal with functions with
 * different numbers of arguments.
 */
typedef void *(*proto_fn)(struct proto_obj *this);
typedef void *(*proto_fn1)(struct proto_obj *this, void *a1);
typedef void *(*proto_fn2)(struct proto_obj *this, void *a1, void *a2);
typedef void *(*proto_fn3)(struct proto_obj *this, void *a1, void *a2, void *a3);
typedef void *(*proto_fn4)(struct proto_obj *this, void *a1, void *a2, void *a3, void *a4);
typedef void *(*proto_fn5)(struct proto_obj *this, void *a1, void *a2, void *a3, void *a4, void *a5);
typedef void *(*proto_fn6)(struct proto_obj *this, void *a1, void *a2, void *a3, void *a4, void *a5, void *a6);

static struct proto_obj *proto_alloc(void)  { return malloc(sizeof(struct proto_obj)); }
static void proto_free(struct proto_obj *o) { assert(o); free(o); }

/* find the field in this object (and not its parents) */
static inline struct proto_field *
__proto_field_access(struct proto_obj *o, char *name)
{
	int i;
	assert(o && name);

	/*
	 * Advanced Challenge: Can you get rid of the string
	 * comparisons?  These are sloooow.
	 * 
	 * Hint: How can you represent these strings as integers
	 *       instead so that you only need integer comparison?
	 * Surprise: If you assume the user will use the above macros,
  	 *       and that your linker (yes!, your linker) is
  	 *       sufficiently intelligent, you can implement this
  	 *       change by modifying _one_ line of code
	 */
	for (i = 0 ; i < PROTO_DEF_NFIELDS ; i++) {
		struct proto_field *f = &o->fields[i];
		if (!f->name) break;
		if (!strcmp(f->name, name)) return f;
	}

	return NULL;
}

/* find the field in the object and its parents */
static inline struct proto_field *
proto_field_access(struct proto_obj *o, char *name)
{
	struct proto_field *f;

	assert(name);
	if (!o) return NULL; 	/* recursion base case */

	f = __proto_field_access(o, name);
	if (f) return f;
	return proto_field_access(o->__proto__, name); /* do our parents have the field? */
}

/* get the value for a field, ignoring type */
static inline void *
proto_field_get(struct proto_obj *o, char *name, proto_field_t type)
{
	struct proto_field *f;
	assert(o && name);

	f = proto_field_access(o, name);
	if (!f || f->type != type) return NULL;
	return f->value;
}

static int 
__proto_field_add(struct proto_obj *o, char *name, void *value, proto_field_t type)
{
	int i;
	assert(o && name);

	if (__proto_field_access(o, name)) return -1;
	for (i = 0 ; i < PROTO_DEF_NFIELDS ; i++) {
		struct proto_field *f = &o->fields[i];
		if (!f->name) {
			f->name  = name;
			f->value = value;
			f->type  = type;
			return 0;
		}
	}
	
	/* Not in the default fields! */
	/* 
	 * Challenge: Implement additional fields beyond the defaults
	 * PROTO_DEF_NFIELDS using ->fields_extra.  This might be
	 * implemented, for example, as a separate larger array or as
	 * a hash table.  A different approach might treat ->fields as
	 * a cache and actively move commonly accessed fields from
	 * ->fields_extra into ->fields.
	 */
	return -1;
}

/* find the field and set it */
static inline int
proto_field_set(struct proto_obj *o, char *name, void *val, proto_field_t type)
{
	struct proto_field *f;
	assert(o && name);

	f = __proto_field_access(o, name);
	if (f) {
		f->value = val;
		f->type  = type;
	} else {
		return __proto_field_add(o, name, val, type);
	}

	return 0;
}

/* get the inheritance parent to the current object */
static struct proto_obj *
proto_field_parent(struct proto_obj *o) { return o->__proto__; }

/* 
 * Challenge: create a library that uses this library to add object
 * constructors.  The library might use the convention that the
 * constructor function is always the first field in an object. 
 */

#endif /* PROTO_H */
