#define _GNU_SOURCE
//mymalloc.c
// gcc -w -shared -fPIC -o mymalloc.so mymalloc.c -ldl

#include <stdlib.h>
#include <dlfcn.h>
#include <stdio.h>

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
void insert_global (int , int );
void *malloc(size_t );
 int search_global(int);
void print_globals();
char *strcpy(char *dest, const char * source);
char *strcat(char *dest, const char *src);
char *gets(char *buf);
void free(void *ptr);
int return_index_global(int);


//char *strcpy(char *d, char s[]){ return strcpy(d, (char *)s);}

void insert_global (int pointer, int size)
{
	//printf("%s %d %s %d %s %x \n", "current pos: ", cur_position, "current size: ", cur_size, " Pointer ", pointer);
	if (cur_position >= cur_size)
	{
		first_time = 1;
		resize();
	}
	*(global_array + cur_position * 2) = pointer;
	*(global_array + cur_position * 2 + 1) = size;
	cur_position++;
	//printf("%s\n", "Test after insert global" );

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
		insert_global(caller, (int) size);
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
	int diff = 0;

	for (i = 0; i< cur_position; i++)
	{
		//if (global_array[i][0] == pos)
		if(*(global_array + 2*i) == pos)
		{
			return *(global_array + 2*i +1);
		}else if(*(global_array + 2*i) < pos && pos < *(global_array + 2*i)+ *(global_array + 2*i +1)){
			// Calculate difference between actual size and pointer size.
			diff = *(global_array + 2*i +1) - (pos - *(global_array + 2*i));
			return diff;		
		}
	}
	return -1;
}

// return the index , used for freeing the array
int get_index(int pos)
{
	int i = 0;
	for (i = 0; i< cur_position; i++)
	{
		//if (global_array[i][0] == pos)
		if(*(global_array + 2*i) == pos)
		{
			return i;
		}
	}
	return -1;
}



void free(void *ptr)
{
	// remove from global_array
	// decrement cur_position
	static void (*f_ptr)(void *);
	if(!f_ptr)
		f_ptr = (void (*)()) dlsym(RTLD_NEXT, "free");


	int pos_in_array = 0;
	int i = 0;
	int temp = 0;
	printf("Free Pointer: %x", ptr);
	pos_in_array = get_index(ptr);

	if (pos_in_array < 0 ) {
		printf("FREE: Pointer not allocated in heap\n");
		return f_ptr;
	}
	else
	{
		for(i = pos_in_array; i < cur_position-1; i++)
		{

			*(global_array + i * 2) = *(global_array + (i+1) * 2);
			*(global_array + ((i*2) + 1)) = *(global_array + ((i+1) * 2 + 1));
		}
		// for safety: overwriting last value
		*(global_array + cur_position *2) = 0;
		*(global_array + cur_position *2 +1) = 0;
		
		cur_position --;
		printf("%s %x\n","FREE: freeing ", ptr );
		return f_ptr;
	}

}


char *strcpy(char *d, const char *s){
	
	printf("%s\n", "In our strcpy." );
	static char* (*f_ptr)(char * d, const char* s);
	//static char* (*strncp_ptr)(char * d, const char * s, size_t size);

	int dest_size = 0;
	int source_size = 0;
	char * sp = s;
	
	if(!f_ptr)
		f_ptr = (char *(*)()) dlsym(RTLD_NEXT, "strcpy");

	dest_size = search_global(d);
	printf("%s %x\n", "Address: ",d);
	if (dest_size <0) 
	{	
		printf("%s\n", "Destination not allocated on heap" );
		char* res = f_ptr(d, s);
		return res;
	}

	while(*sp != '\0')
	{
		source_size++;
		sp++;
	}
	if(source_size > dest_size -1)
	{
		printf("%s\n", "Destination size less.. copying only the allocated chars");
		char * res = strncpy(d, s, (size_t) dest_size);
		res[dest_size - 1] = '\0';
		return res;
	}

	printf("%s\n","Test strcpy" );
	char* res = f_ptr(d, s);
	return res;
}

/*
* 	appends the string pointed to by src to the end of the string pointed to by dest.
*/
char *strcat(char *dest, const char *src)
{

printf("%s\n", "In function strcat");
	static char* (*f_ptr)(char * d, const char* s);
	if(!f_ptr)
		f_ptr = (char *(*)()) dlsym(RTLD_NEXT, "strcat");
	
	int dest_size = search_global(dest);
	if( dest_size < 0){
		printf("%s\n", "Destination not allocated on heap" );
		//Call default function
		char* res = f_ptr(dest, src);
		return res;	
	}
	int ss = 0;
	int ds = 0;
	char * res = NULL;
	char * it = src;
	
	while (* it++ != '\0') ss++;

	it = dest;
	while(* it++ != '\0') ds++;

	printf("%s %d %s %d %s %d \n","Source: ",ss, " Destination Size: ", dest_size, "dest: ",ds );

	if ( (ss + ds +1) > dest_size )
	{
		// handle the exception
		res = strncat(dest, src, (size_t) (dest_size - ds -1));
		res[dest_size - 1] = '\0';

	}
	else
	{
		res = f_ptr(dest, src);

	}

	return res;

}

char *gets(char *buf)
{
	int buf_size = search_global(buf);
	register int c;
	register char *s;
	int input_size = 0;

	for (s = buf; (c = getchar()) != '\n' && (input_size < buf_size-1); input_size++)
	{
		// if (input_size > buf_size-1)
		// {
		// 	s[buf_size -1 ] = '\0';
		// 	break;
		// }
		if (c == EOF)
		{
			if (s == buf)
			{	return (NULL);}
			else
			{break;}
		}
		else
		{
			*s++ = c;
		}
	}
	*s = 0;
	return (buf);
}
