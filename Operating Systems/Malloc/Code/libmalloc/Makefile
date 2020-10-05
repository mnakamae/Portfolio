.PHONY: malloc

malloc: libmalloc.so libmalloc.a
   echo done

libmalloc.a: malloc.o
   ar r libmalloc.a malloc.o

libmalloc.so: malloc.o
   gcc -shared -fPIC -o libmalloc.so malloc.o
