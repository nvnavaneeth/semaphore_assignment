producer_consumer: producer_consumer.o
	  g++ -o producer_consumer producer_consumer.o ../my_semaphore.o -lpthread
	
producer_consumer.o:
	g++ -c producer_consumer.cpp
clean:
	rm -r *.o
	rm producer_consumer
