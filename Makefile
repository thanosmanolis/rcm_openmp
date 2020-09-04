############################################################
#    C/C++ Makefile											#
#    Author: Thanos Manolis <thanos.m14@gmail.com			#
#############################################################
#															#
#   'make'  		  build all executable files			#
#   'make exec_name'  build executable file 'test_*'		#
#   'make clean'  	  removes .o .a and executable files    #
#															#
#############################################################

# define the C/C++ compiler to use, default here is gcc-7
CC = gcc-7

# all the executables
EXECS = sequential

# define flags
CFLAGS = -Wall -O3
LDFLAGS =

# define command to remove files
RM = rm -rf

# always build those, even if "up-to-date"
.PHONY: $(EXECS)

all: $(EXECS)
	
sequential:
	cd rcm; make lib_seq; cd ..
	cd rcm; cp lib/lib_seq.a inc/rcm.h ../; cd ..
	$(CC) main.c lib_seq.a -o $@ $(CFLAGS) $(LDFLAGS)

clean:
	$(RM) *.h *.a rcm/src/*.o rcm/lib/*.a $(EXECS)
