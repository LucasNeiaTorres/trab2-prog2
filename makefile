links = -lm -lallegro_ttf -lallegro_font -lallegro_primitives -lallegro_acodec -lallegro_audio -lallegro_image -lallegro -lallegro_main
parametrosCompilacao=-Wall -std=c99
arquivos=main.o game.o states.o
all: jewel

jewel: $(arquivos)
	gcc -o jewel $(arquivos) $(parametrosCompilacao) $(links)

.o: .c
	gcc -c *.c $(parametrosCompilacao)

clean:
	rm -f *.o *.gch jewel 

purge: clean
	-rm -f jewel