/*** Java example:

class Shape {
  protected String color;
  public Shape() {
    color = "white";
  }
  public void print() {
    System.out.println("color: " + color);
  }
}

class Circle extends Shape {
  public Circle() {
    radius = 10;
  }
  public getRadius() {
    return radius;
  }
  private int radius;
}

class Square extends Shape {
  public Square() {
    side_len = 5;
    color = "black";
  }
  public void print() {
    System.out.println("side length: " + side_len + " ");
    super.print();
  }
  private int side_len = 5;
}

circle.print(); // color: white
square.print(); // side length: 5 color: black
Shape [] shapes = {new Circle, new Square, new Shape};
// polymorphism:
for (s in shapes) {
  s.print();
}
***/

#include <oo.h>

/* Shape class */

class_extend(shape, object)
	void (*print)(struct shape *this);
	char *color;
class_end(shape, object)

void 
shape_print(struct shape *this)
{
	printf("color: %s\n\n", this->color);
}

class_template(shape, object)
        .print = shape_print,
class_template_end(shape, object)
void
shape_cons(struct shape *this) 
{ 
	//SUPER_CONS(object);
	this->color = "white"; 
}

/* Circle class */

class_extend(circle, shape)
	int radius;
	int (*get_radius)(struct circle *this);
class_end(circle, shape)

int
get_radius(circle *this)
{
	return this->radius;
}

class_template(circle, shape)
        .get_radius = get_radius,
	.poly.print = shape_print,
class_template_end(circle, shape)
void 
circle_cons(circle *this) 
{ 
	SUPER_CONS(shape);
	this->poly.color = "white";
	this->radius     = 10;
}


/* Square class */
class_extend(square, shape)
	int side_len;
class_end(square, shape)

void
square_print(square *this)
{
	printf("side length: %d\n", this->side_len);
	SUPER(print, shape);
}

class_template(square, shape)
	/* overrides .poly.print in shape */
        .poly.print = square_print,
class_template_end(square, shape)
void
square_cons(square *this)
{
	SUPER_CONS(shape);
	this->side_len   = 5;
	this->poly.color = "white";
}

int
main(void)
{
	circle *c  = circle_new();
	square *sq = square_new();

	INVP(c, print);     // c->poly.print(c);
	INVP(sq, print);    // sq->poly.print(sq);

	return 0;
}
