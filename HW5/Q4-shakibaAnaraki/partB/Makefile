FLAGS=-lpthread -lrt

all: producerRun controllerRun consumerRun

producerRun: producer.c
	gcc producer.c -o producerRun $(FLAGS)

controllerRun: controller.c
	gcc controller.c -o controllerRun $(FLAGS)
	
consumerRun: consumer.c
	gcc consumer.c -o consumerRun $(FLAGS)

run: all
	gnome-terminal -- ./producerRun
	gnome-terminal -- ./controllerRun
	
clean:
	rm -f producerRun controllerRun consumerRun

.PHONY: all clean
