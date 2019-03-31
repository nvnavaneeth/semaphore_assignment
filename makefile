producer_consumer: producer_consumer.o my_semaphore.o
		g++ -o producer_consumer producer_consumer.o my_semaphore.o -lpthread

reader_writer:
	  g++ -o reader_writer reader_writer_starvation_free.cpp -lpthread

producer_consumer.o:
	  g++ -c producer_consumer.cpp
temp_test.o: 
		g++ -c temp_test.cpp
	
my_semaphore.o:
		g++ -c my_semaphore.cpp

clean:
	rm -r *.o
	rm producer_consumer
	rm reader_writer
