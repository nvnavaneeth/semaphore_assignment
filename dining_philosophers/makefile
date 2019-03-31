dining_philosophers: dining_phil.o phil.o sched_phil.o
	g++ -o dining_philosophers dining_phil.o phil.o sched_phil.o ../my_semaphore.o -lpthread

dining_phil.o:
	g++ -c dining_phil.cpp

phil.o:
	g++ -c phil.cpp

sched_phil.o:
	g++ -c sched_phil.cpp

clean:
	rm -r *.o
	rm dining_philosophers
