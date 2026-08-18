receiver: src/main.c src/util.c src/ring_buffer.c
	gcc -Wall -Wextra -Wpedantic -Iinc -lpthread -g -o build/receiver src/main.c src/util.c src/ring_buffer.c
	
generator: src/packet_generator.c src/util.c
	gcc -Wall -Wextra -Wpedantic -Iinc -lpthread -g -o build/generator src/packet_generator.c src/util.c
test: src/race_test.c
	gcc -Wall -Wextra -Wpedantic -Iinc -lpthread -g -o build/test src/race_test.c

all: receiver generator
	
clean: 
	rm -f test
	rm -f generator
	rm -f receiver