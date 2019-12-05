all:
	gcc -o trainer -I../include trainer.c ./libstemmer.o `pkg-config --cflags --libs glib-2.0` `pkg-config --libs glib-2.0`

clean:
	rm *.csv trainer
