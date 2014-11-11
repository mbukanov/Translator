.PHONY: all clean install uninstall
	
all:
	g++ main.cpp Translator.h Translator.cpp -o translator -lcurl -ljsoncpp -L.

install:
	install ./translator /usr/local/bin

uninstall:
	rm -rf /usr/local/bin/translator

clean:
	rm translator