#############################################################
#    C/C++ Makefile											#
#    Author: Thanos Manolis <thanos.m14@gmail.com			#
#############################################################
#															#
#   'make lib'	  	  build the libraries .a				#
#   'make clean'  	  removes .o .a files				    #
#															#
#############################################################

# define the C/C++ compiler to use, default here is gcc-7
CC = gcc-7

# define flags
CFLAGS = -Wall

# define command to remove files
RM = rm -rf

# all the libraries
LIBS = lib_seq

# always build those, even if "up-to-date"
.PHONY: $(LIBS)

lib: $(LIBS)

lib_seq:
	cd src; $(CC) -c rcm_sequential.c $(CFLAGS); cd ..
	cd src; $(CC) -c helper.c $(CFLAGS); cd ..
	cd src; ar rcs ../lib/lib_seq.a helper.o rcm_sequential.o; cd ..

clean:
	$(RM) src/*.o lib/*.a
