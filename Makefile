CFLAGS= -std=c11
CC=gcc
WEBCC=emcc
WEBRUN=emrun
BROWSER=firefox
WEBCFLAGS= -s USE_SDL=2 -s USE_SDL_IMAGE=2 -s SDL2_IMAGE_FORMATS='["png"]' -s USE_SDL_TTF=2  
LIBS= -lSDL2 -lSDL2_image -lSDL2_ttf
CFLAGS= -std=c11
SOURCES=$(wildcard *.c)
HEADERS=$(wildcard *.h)
EXEC = game

all: $(EXEC)

$(EXEC): $(SOURCES) $(HEADERS)
	$(CC) -o $@ $(CFLAGS) $(LIBS) $(HEADERS) $(SOURCES)
run: $(EXEC)
	./$(EXEC)

web:
	$(WEBCC) --preload-file assets $(SOURCES) $(WEBCFLAGS) -o $(EXEC).html 

web-debug-build: clean
	$(WEBCC) --preload-file assets $(SOURCES) $(WEBCFLAGS) -o $(EXEC).html --emrun 

web-run: web-debug-build
	$(WEBRUN) $(EXEC).html --browser $(BROWSER)

clean:
	rm -f $(EXEC) $(EXEC).html $(EXEC).js $(EXEC).wasm $(EXEC).data
