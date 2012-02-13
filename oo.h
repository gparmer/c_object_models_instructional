#ifndef OO_H
#define OO_H

#include <string.h>
#include <malloc.h>

/*
 * Challenge: Implement multiple inheritance or inheritance plus
 * "implements" (i.e. interface-based inheritance).  Difficulty: How
 * can the same object be polymorphic for more than one type at the
 * same time?  
 */
#define STRX(x) #x  		/* standard macro trick to make a string */
#define STR(x)  STRX(x)
#define class_extend(name, parent)				\
typedef struct name {    						\
        /* poly is the parent's structure.  This means that the \
	 * address of this structure (i.e. struct name) can be used as \
	 * a pointer to either name, or parent.  poly _must_ be at the \
	 * top of the struct. Why? */				\
        struct parent poly;					\
	struct parent *super;					\
	void (*constructor)(struct name *);

#define class_end(name, parent) } name;

/* 
 * You should define all of the functions for the class in the
 * template.
 */
#define class_template(name, parent)		\
void name##_cons(name *o);		        \
struct name name##_template = {			\
	.super       = &parent##_template,      \
	.constructor = name##_cons,

#define class_template_end(name, parent) };	\
CONSTRUCTOR(name, parent);

/* 
 * Assumes that you've defined name_template, and that you will define
 * the class's constructor as class_cons
 */
#define CONSTRUCTOR(name, parent)					\
name *    							\
name##_new(void)							\
{									\
	struct name *o = malloc(sizeof(struct name));			\
									\
	if (!o) return NULL;						\
	memcpy(o, &name##_template, sizeof(struct name));		\
	o->super = & parent##_template;					\
	name##_cons(o);							\
									\
	return o;							\
}

/* Assume you pass in the first argument to methods as "this" */
#define SUPER_CONS(parent) this->super->constructor((parent*)this)
#define SUPER(fn, parent)  this->super->fn((parent*)this)
/* Need to define SUPER1... for multi-argument functions */

/* normal/non-polymorphic invocation */
#define INV(obj, fn,...) obj->fn(obj, ##__VA_ARGS__);
/* polymorphic invocation on parent function */
#define INVP(obj, fn,...) obj->poly.fn(obj, ##__VA_ARGS__);
/* on the parent's parent function */
#define INVPP(obj, fn,...) obj->poly->poly.fn(obj, ##__VA_ARGS__);
/* ... */

/* The base class of all classes */
typedef struct object {
	struct parent *super;
	void (*constructor)(struct object *);
} object;
void object_cons(object *o) { return; }
struct object object_template = {
	.super       = NULL,
        .constructor = object_cons
};
object *
object_new(void)
{
	struct object *o = malloc(sizeof(struct object));

	if (!o) return NULL;
	memcpy(o, &object_template, sizeof(struct object));
	o->super = NULL;
	object_cons(o);

	return o;
}
/* 
 * Challenge: write safe casting functions.  You want to be able to
 * allow safe "down-casting" (from e.g. circle to shape) and
 * "up-casting" (i.e. from shape to circle).  You might want to add a
 * field to each object called .classname to provide run-time
 * information about what the type of an object is.
 */

#endif /* OO_H */
