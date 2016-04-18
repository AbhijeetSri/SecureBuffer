#define _GNU_SOURCE
//mymalloc.c
// gcc -shared -fPIC -o mymalloc.so mymalloc.c

#include <stdlib.h>
#include <dlfcn.h>

#define debug 0

static int global_array[10000][2] ;
static int cur_position = 0; 

void print_globals();

void *malloc(size_t size){
	static void * (*f_ptr)(size_t);

	if(!f_ptr)
		f_ptr = (void *(*)()) dlsym(RTLD_NEXT, "malloc");
	//printf("%s %d\n", " size to malloc: ", size );

	void* caller = f_ptr(size);
	// add caller and size to global array

	global_array[cur_position %10000][0] = &caller;
	global_array[cur_position %10000][1] = (int)size;
	cur_position++;

	if(debug)
	{
		cur_position % 1000 == 0 ? cur_position %= 1000 : cur_position;
		if(cur_position == 10) print_globals();
	}

	return (caller);
}


void print_globals()
{
	int i =0;
	printf("%s\n", "Printing global array" );
	for ( i = 0; i < cur_position; ++i)
	{
		printf("%d %d\n", global_array[i][0], global_array[i][1]);
	}
}

int search_global(int pos)
{
	int i = 0;
	for (i = 0; i< cur_position; i++)
	{
		if (global_array[i][0] == pos)
		{
			return global_array[i][1];
		}
	}
	return -1;
}

char *strcpy(char *d, const char *s){
	static char* (*f_ptr)(char * d, const char* s);
	int dest_size = 0;
	int source_size = 0;
	char * sp = s;
	
	if(!f_ptr)
		f_ptr = (char *(*)()) dlsym(RTLD_NEXT, "strcpy");

	dest_size = search_global(&d);
	if (dest_size <0) 
	{	
		printf("%s\n", "Destination not allocated on heap" );
		return NULL;
	}

	while(*sp != '\0')
	{
		source_size++;
		sp++;
	}
	if(!(source_size <= dest_size -1))
	{
		printf("%s\n", "Invalid buffer size" );
		return NULL;
	}

	printf("%s\n","Test strcpy" );
	char* res = f_ptr(d, s);
	return res;
}