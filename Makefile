CC := gcc
LIBS := -lm -lcairo
INCLUDES := -I/usr/include/cairo -I/usr/include/glib-2.0 -I/usr/lib/x86_64-linux-gnu/glib-2.0/include -I/usr/include/pixman-1 -I/usr/include/uuid -I/usr/include/freetype2 -I/usr/include/libpng16

all: build/hilbert_targa build/hilbert_unifont build/linear_unifont

build/hilbert_targa: hilbert_targa.c
	mkdir -p build/
	${CC} ${INCLUDES} hilbert_targa.c -o $@ ${LIBS}

build/hilbert_unifont: hilbert_unifont.c
	mkdir -p build/
	${CC} ${INCLUDES} hilbert_unifont.c -o $@ ${LIBS}

build/linear_unifont: linear_unifont.c
	mkdir -p build/
	${CC} ${INCLUDES} linear_unifont.c -o $@ ${LIBS}

images: build/hilbert_targa build/hilbert_unifont build/linear_unifont
	cd build/ && ./hilbert_targa && ./hilbert_unifont && ./linear_unifont

clean:
	rm -rf build/

