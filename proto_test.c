/*** Javascript example:

var shape = {
  color: "white",
  print: function () { console.log("color: " + this.color); }
};
// Same as:
// shape.color = "white";
// shape.print = function () { console.log("color: " + this.color); }

var circle = {
  __proto__: Shape,
  radius: 10
};

var square = {
  __proto__: Shape,
  color : "black",
  side_len: 5
};

circle.print(); // color: white
square.print = function () { console.log("side length: " + side_len + " "); this.__proto__.print(); };
square.print(); // side length: 5 color: black
shape.color = "red";
circle.print(); // color: red
// the craziness of prototype inheritance:  changes to parents (shape) reflect dynamically on children (circle)!

***/

#include <proto.h>
#include <stdio.h>

void shape_print(struct proto_obj *this)
{
	printf("color: %s\n\n", (char*)PGETV(this, color));
}

void square_print(struct proto_obj *this)
{
	printf("side length: %d\n", (unsigned int)PGETV(this, side_len));
	PINV(PSUPER(this), print); /* invoking the super's print function */
}

struct proto_obj *
shape_new(void)
{
	struct proto_obj *o = PALLOC();

	if (!o) return NULL;
	// shape.color = "white"
	PSETV(o, color, "white"); 
	// shape.print = shape_print (function)
	PSETF(o, print, (void*)shape_print);

	return o;
}

struct proto_obj *
circle_new(struct proto_obj *parent)
{
	struct proto_obj *o = PALLOC();

	if (!o) return NULL;
	// circle.__proto__ = shape
	PSET_PARENT(o, parent);
	// circle.radius = 10
	PSETV(o, radius, 10);

	return o;
}

struct proto_obj *
square_new(struct proto_obj *parent)
{
	struct proto_obj *o = PALLOC();

	if (!o) return NULL;
	PSET_PARENT(o, parent);
	PSETV(o, side_len, 5);

	return o;
}


int
main(void)
{
	struct proto_obj *shape, *circle, *square;

	shape  = shape_new();
	circle = circle_new(shape);
	square = square_new(shape);
		
	PINV(circle, print); // circle.print();

	PSETF(square, print, square_print); //square.print = square_print
	PINV(square, print);

	PSETV(shape, color, "red"); /* set in _parent_ */
	PINV(circle, print);	    /* yet printed in child */

	PFREE(circle);
	PFREE(square);
	PFREE(shape);
	
	return 0;
}
