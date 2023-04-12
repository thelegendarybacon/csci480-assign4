# Standard compiler variables
ARGS = g++ -o

# Rules start here

z1856158_project4: z1856158_project4.cc
	$(ARGS) z1856158_project4 z1856158_project4.cc -lpthread

z1856158_project4_p2: z1856158_project4_p2.cc
	$(ARGS) z1856158_project4_p2 z1856158_project4_p2.cc -lpthread

clean:
	-rm -f *.o z1856158_project4 z1856158_project4_p2
