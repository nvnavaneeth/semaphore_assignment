cigarette_smokers: cigarette_smokers.o
	g++ -o cigarette_smokers cigarette_smokers.o ../my_semaphore.o -lpthread

cigarette_smokers.o:
	g++ -c cigarette_smokers.cpp

clean:
	rm -r *.o
	rm cigarette_smokers
