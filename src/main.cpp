#include <cstdlib>
#include <cstdio>

#include <Python.h>

int main(int argc, char** argv)
{
	Py_Initialize();

	printf("Python Version " PY_VERSION "\n");

	return EXIT_SUCCESS;
}
