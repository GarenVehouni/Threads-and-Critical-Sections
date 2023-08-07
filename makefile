CC = gcc
CFLAGS = -g -Wall -lm -pthread

all: pi_serial pi_semaphore pi_mutex run

pi_serial: pi_serial.c
	$(CC) $(CFLAGS) -o pi_serial pi_serial.c

pi_mutex: pi_mutex.c
	$(CC) $(CFLAGS) -o pi_mutex pi_mutex.c

pi_semaphore: pi_semaphore.c
	$(CC) $(CFLAGS) -o pi_semaphore pi_semaphore.c

run: pi_serial pi_semaphore pi_mutex
	./pi_serial
	./pi_semaphore
	./pi_mutex

clean:
	rm pi_serial pi_semaphore pi_mutex
























