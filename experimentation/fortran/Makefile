
# ➤ gfortran plplot_example_29.f90 -o plplot_example_29 -J /usr/lib/x86_64-linux-gnu/fortran/modules/plplot/ -I /usr/include/plplot -lplplotfortran

BASE_NAME = plplot_basic_example

INCLUDE_DIR = /usr/include/plplot
MOD_DIR = /usr/lib/x86_64-linux-gnu/fortran/modules/plplot/
LIB = lplplotfortran

COMPILER = /usr/bin/gfortran
FLAGS = 

all: example

example: $(BASE_NAME).f90
	$(COMPILER) $(FLAGS) -I $(INCLUDE_DIR) -J $(MOD_DIR) -o $(BASE_NAME) $(BASE_NAME).f90 -$(LIB)

clean: 
	rm -rf $(BASE_NAME)

