#define _GNU_SOURCE
//mymalloc.c
// gcc -shared -fPIC -o mymalloc.so mymalloc.c

#include <stdlib.h>
#include <dlfcn.h>

#define debug 0
#define ROW 20000

//static int **global_array;// = malloc( sizeof(int)*10000 ); //[10000][2] ;
static int* global_array = NULL;//(int*) malloc(ROW * 2 * sizeof(int));
static int cur_position = 0; 
static int cur_size = ROW;
static int first_time = 1;

void __attribute__ ((constructor)) setup(void){
	printf("%s\n", "in the constructor" );
	global_array = (int*) malloc(cur_size * 2 * sizeof(int));
}

void print_globals();
void resize();

void insert_global (int pointer, int size)
{
	//printf("%s %d %s %d \n", "current pos: ", cur_position, "current size: ", cur_size);
	if (cur_position >= cur_size)
	{
		first_time = 1;
		resize();
	}
	*(global_array + cur_position * 2) = pointer;
	*(global_array + cur_position * 2 + 1) = size;
//	global_array[cur_position][0] = pointer;
//	global_array[cur_position][1] = size;
	cur_position++;
}

void resize()
{
	//int new_array[2*cur_size][2]; // = new int[2*cur_size][2];
	printf("%s\n","resizing" );
	int* new_array = (int*) malloc(cur_size * 2 * 2 * sizeof(int)); // size double * 2 col/row
	int i =0;
	for(i = 0 ; i < cur_size; i++)
	{
		*(new_array + i*2) = *(global_array + i*2);
		*(new_array + i*2 + 1) = *(global_array + i*2 +1);
		//new_array[i][0] = global_array[i][0];
		//new_array[i][1] = global_array[i][1];
	}
	global_array = new_array;
	cur_size = cur_size * 2;
	printf("%s %d\n", "Current SIZE: ", cur_size);
}


void *malloc(size_t size){
	static void * (*f_ptr)(size_t);
	if(!f_ptr)
		f_ptr = (void *(*)()) dlsym(RTLD_NEXT, "malloc");
	//printf("%s %d\n", " size to malloc: ", size );
	void* caller = f_ptr(size);
	// add caller and size to global array
	if(!first_time)
	{
		insert_global(&caller, (int) size);
	}
	first_time = 0;	
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
		printf("%d %d\n", *(global_array + 2*i), *(global_array + 2*i +1));
	}
}

int search_global(int pos)
{
	int i = 0;
	for (i = 0; i< cur_position; i++)
	{
		//if (global_array[i][0] == pos)
		if(*(global_array + 2*i) == pos)
		{
			return *(global_array + 2*i +1);
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
	printf("%s %x\n", "Address: ",&d);
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