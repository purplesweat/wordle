CXX=c++
CXXFLAGS=-O2

DICT=/usr/share/dict

SOURCES=src
ASSETS=assets

.PHONY: install uninstall

wordle: $(SOURCES)/main.cpp $(DICT)/words.sorted
	$(CXX) $(CXXFLAGS) -o wordle $(SOURCES)/main.cpp
	strip wordle

$(DICT)/words.sorted: $(ASSETS)/words.sorted
	mkdir -p $(DICT)
	cp $(ASSETS)/words.sorted $(DICT)/words.sorted

install: wordle
	mv wordle /usr/bin

uninstall: /usr/bin/wordle
	rm -f /usr/bin/wordle
	rm -f $(DICT)/words.sorted
