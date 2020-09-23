lab2: lab2.c
	gcc -Wall -g -O2 $^ -o $@

.PHONY: clean
clean:
	rm -rf *.o *~ lab2 
