CXX=c++
CXXFLAGS=-O2
#CXXOPTS=-D DEBUG

DICT=/usr/share/dict
PREFIX=/usr/local

SOURCES=src
ASSETS=assets

.PHONY: install uninstall

wordle: $(SOURCES)/main.cpp $(DICT)/words.sorted
	$(CXX) $(CXXFLAGS) $(CXXOPTS) -o wordle $(SOURCES)/main.cpp
	strip wordle

$(DICT)/words.sorted: $(ASSETS)/words.sorted
	mkdir -p $(DICT)
	cp $(ASSETS)/words.sorted $(DICT)/words.sorted

install: wordle
	cp wordle $(PREFIX)/bin/wordle

uninstall: $(PREFIX)/bin/wordle
	rm -f $(PREFIX)/bin/wordle $(DICT)/words.sorted
