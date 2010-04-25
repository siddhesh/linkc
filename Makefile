
objects = main.o design.o file.o help.o

main: $(objects)
	gcc -o $@ $(objects) -lmenu -lncurses
	 
design.o: design.c design.h
	gcc -c $<
	
main.o: main.c design.h
	gcc -c $<
	
file.o: file.c file.h 
	gcc -c $<

help.o: help.c help.h
	gcc -c $<
	
.PHONY: clean
clean:
	rm -f $(objects) main
