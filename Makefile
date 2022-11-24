all:
	gcc -g fs.c filefs.c bitmap.c -o filefs

clean:
	rm filefs fs
