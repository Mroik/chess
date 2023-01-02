.PHONY: build clean
build:
	gcc -o chess -lSDL2 -lSDL2_image *.c
clean:
	@rm chess
