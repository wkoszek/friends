#include <stdlib.h>
#include <cassert>
#include <iostream>
#include <fstream>
#include <list>
#include <string>

using namespace std;

class obj;
typedef void obj_func_t(obj *obj, void *arg);

class obj {
	private:
		list <pair<obj_func_t *, void *> > names;
	public:
		void FunctionAdd(obj_func_t *func, void *arg);
		void RunAll(void);
		void Func3(int a, int b, int c);
};

void
obj::FunctionAdd(obj_func_t *func, void *arg)
{
	pair <obj_func_t *, void *> *cb;

	cb = new pair<obj_func_t *, void *>();
	cb->first = func;
	cb->second = arg;
	names.push_back(*cb);
}

void
obj::RunAll(void)
{
	list <pair <obj_func_t *,  void *> >::const_iterator it;
	obj_func_t *func;
	void *arg;

	it = names.begin();
	while (it != names.end()) {
		func = (*it).first;
		arg = (*it).second;

		func(this, arg);
		it++;
	}
}

void
obj::Func3(int a, int b, int c)
{

	cout << __func__ << endl;
	cout << a << endl;
	cout << b << endl;
	cout << c << endl;
}

obj_func_t sample_cb;

void
sample_cb(obj *obj, void *arg)
{
	int *a = (int *)arg;

	assert(a != NULL);

	cout << "---" << endl;
	cout << " V: " << *a << endl;
}

obj_func_t sample_cb3;

struct sample_cb3_arg {
	int aa;
	int ab;
	int ac;
};

void
sample_cb3(obj *obj, void *arg)
{
	struct sample_cb3_arg *a;
	
	a = (struct sample_cb3_arg *)arg;
	assert(a != NULL);
	cout << "Passed to callback: ";
	cout << a->aa << " ";
	cout << a->ab << " ";
	cout << a->ac << "\n";
	cout << "Calling Func3(...)" << endl;
	obj->Func3(a->aa, a->ab, a->ac);
}

int
main(int argc, char **argv)
{
	obj *o;
	int a, b, c;
	sample_cb3_arg arg = { 1000, 2000, 3000 };


	a = 10;
	b = 20;
	c = 30;

	o = new obj;

	o->FunctionAdd(sample_cb, &a);
	o->FunctionAdd(sample_cb, &b);
	o->FunctionAdd(sample_cb, &c);
	o->FunctionAdd(sample_cb3, &arg);


	o->RunAll();
	return (EXIT_SUCCESS);
}
