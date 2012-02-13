typedef struct square { 
	struct shape poly; 
	struct shape *super; 
	void (*constructor)(struct square *);
	int side_len;
} square;

void
square_print(square *this)
{
	printf("side length: %d\n", this->side_len);
	this->super->print((shape*)this);
}

void square_cons(square *o); 
struct square square_template = { 
	.super = &shape_template, 
	.constructor = square_cons,
	.poly.print = square_print,
}; 

square * square_new(void) { 
	struct square *o = malloc(sizeof(struct square)); 
	if (!o) return ((void *)0); 
	memcpy(o, &square_template, sizeof(struct square)); 
	o->super = & shape_template; 
	square_cons(o); 
	return o; 
};

void
square_cons(square *this)
{
	this->super->constructor((shape*)this);
	this->side_len = 5;
	this->poly.color = "white";
}
