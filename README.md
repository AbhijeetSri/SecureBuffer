# SecureBuffer

#FileName: 
<br/ > SecureBuffer.c 
<br/ > test.c

#Description: 
``SecureBuffer.c`` contains source code for various functions secured from buffer overflow attacks.
Functions included:
* malloc <br/> keeps track of size of memory allocated
* gets	<br/>	Does not let the input get past the set buffer size
* strcpy <br/>		Does not let the input string copied past destination size - sets last character as '\0'
* strcat <br/>		Does not let the resultant string go past Buffer Size, also terminates with '\0' 
* free 	<br/>	  Deallocates the memory

File ``test.c`` includes functions to test the library
	
# How to Use?

Using gcc, simply compile SecureBuffer.c to .so file using below command <br/>
 ``$ gcc -w -shared -fPIC -o SecureBuffer.so SecureBuffer.c -ldl ``
 
Compile your program using gcc as usual. 
<br/>
`` $ gcc normal.c``
<br/>
To Use the library in any program, use LD_PRELOAD construct <br/>
`` $ LD_PRELOAD=./SecureBuffer.so ./<YOUR PROGRAM> ``

LD_PRELOAD is used to load a shared library before loading any other librray. Interestig info about LD_PRELOAD can be found [here] (http://stackoverflow.com/questions/426230/what-is-the-ld-preload-trick).

