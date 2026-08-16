receiver: src/main.c src/util.c
	gcc -Wall -Wextra -Wpedantic -Iinc -g -o build/receiver src/main.c src/util.c
	
generator: src/packet_generator.c src/util.c
	gcc -Wall -Wextra -Wpedantic -Iinc -g -o build/generator src/packet_generator.c src/util.c

all: receiver generator
	
clean: 
	rm -f generator
	rm -f receiver