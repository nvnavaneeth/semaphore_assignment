sleeping_barber: sleeping_barber.o
	g++ -o sleeping_barber sleeping_barber.o ../my_semaphore.o -lpthread

sleeping_barber.o:
	g++ -c sleeping_barber.cpp

clean:
	rm -r *.o
	rm sleeping_barber
