A lesson in Object Models, and how to implement them in C
=========================================================

An object model refers to the semantics of key features of the objects
in a language.  These include, for example, the behavior of
polymorphism, inheritance, and method dispatch.

Included in this package is a couple of _very_ simple object models
implemented in C, for use in C.  Though writing programs with these
would likely be tedious, they are typically intended to be the target
of a higher-level language.  In this case, they are mainly
instructional.  I omitted many features (see the =challenges=
throughout) in favor of simplicity.

The object models are:

- =oo.h=: A simple, Java-like model including single inheritance,
  dispatching to =super=, and proper polymorphism.  Though much of the
  code is there, safe casts are not yet supported.

- =proto.h=: prototype-based object model similar to the one in
  javascript.  This is most common in dynamically typed language that
  employ "duck-typing".  For simplicity, all fields are indexed by
  their name (i.e. a string), and expanding this to be more efficient
  is left as a simple to accomplish, but hard to figure out _how_ to
  implement challenge.

Simple tests with comments comparing to comparable Java and Javascript
programs are included.

The =Makefile= is pretty naive.
