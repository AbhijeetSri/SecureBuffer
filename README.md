# SecureBuffer

FileName: SecureBuffer.c
Description: Source code for various functions secured from buffer overflow attacks.
Functions included:
	malloc		keeps track of size of memory allocated
	gets		Does not let the input get past the set buffer size
	strcpy		Does not let the input string copied past destination size - sets last character as '\0'
	strcat		Does not let the resultant string go past Buffer Size, also terminates with '\0' 
	free 		deallocates the memory