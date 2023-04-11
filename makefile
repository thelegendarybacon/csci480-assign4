# Standard compiler variables
ARGS = gcc -Wall -Werror -std=c++11 -lpthread

# Rules start here

z1856158_project4: z1856158_project4.o
	$(ARGS) -o z1856158_project4 z1856158_project4.o

z1856158_project4.o: z1856158_project4.cpp
	$(ARGS) -c z1856158_project4.cpp

clean:
	-rm -f *.o z1856158_project4
