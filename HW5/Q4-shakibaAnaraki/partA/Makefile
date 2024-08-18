FLAGS=-lpthread -lrt

all: producerRun consumerRun consumer2Run

producerRun: producer.c
	gcc producer.c -o producerRun $(FLAGS)

consumerRun: consumer.c
	gcc consumer.c -o consumerRun $(FLAGS)
	
consumer2Run: consumer.c
	gcc consumer.c -o consumer2Run $(FLAGS)

run: all
	gnome-terminal -- ./producerRun
	gnome-terminal -- ./consumerRun
	gnome-terminal -- ./consumer2Run
	
clean:
	rm -f producerRun consumerRun consumer2Run

.PHONY: all clean
