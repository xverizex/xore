LIBS=`pkg-config --libs gtk+-3.0,gstreamer-1.0,gstreamer-base-1.0 --cflags gtk+-3.0,gstreamer-1.0,gstreamer-base-1.0`
all:
	gcc main.c config.c $(LIBS) -o xore
clean:
	rm xore
