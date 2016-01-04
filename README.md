# PROCESS_IN_C

compile program with gcc or clang complier. With clang you don't need -lpthread extension for the threading programs as with the case of gcc


RUNNING THE PROGRAM:


#program name args args args args ..............

OR

#a.out args args args args ..............

<args> are the datatsets which consist of floating point numbers or you can generate your own dataset with gen_float_nums like

#gcc gen_float_nums.c

#a.out redirect filename


NOTE# compile threading programs as gcc <prgname> -lpthread , make sure the thread module you load is thread safe.

Feel free to make this program better and do so at your our risk.
Lets make the world a better place.
