/*
*	command to run LD_PRELOAD=./mymalloc.so a.out
*/
#include <stdlib.h>
#include <stdio.h>
//#include <mymalloc.c>

//void print_globals();
int main()
{
	char * s = "Hello string12356";
	char * d = malloc(sizeof(char)*15);
	printf("%s %x\n"," destination address: ", d);
	

// TEST CASE : strcpy
	/*
	char *res = strcpy(d,s);
	if(res == NULL) 
	{
		printf("%s\n", "value returned NULL" );
	}
	else{
		printf("%s\n", res);		
	}
	*/

// TEST CASE: malloc
/*
	printf("Hello World ! %s", "Abhijeet\n");

	int* p = (int*) malloc(sizeof(int));

	if (p ==0)
	{
		printf("%s\n", "Error ! memory not allocated");
		return 1;
	}

	*p = 10;
	printf("%s %d\n", "Value is :", *p);
*/

// TEST CASE : strcat

char dest[]= "test";
d = strcpy(d, dest);
printf("%s \n" ,d);
char * dest2 = " longer long longer longest";
d = strcat(d , dest2);
printf("%s\n", d);




	return 0;
}