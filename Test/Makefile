CFLAGS=--std=c++17 -Wno-psabi -lcurl

Test: all
	./Quotes
	./Profile

all: Profile \
	Quotes

%.o: %.cpp
	g++ $(CFLAGS) -c $<

Profile: Profile.o
	g++ $(CFLAGS) -o Profile $<

Quotes: Quotes.o
	g++ $(CFLAGS) -o Quotes $<

clean:
	rm -rf *.o Quotes Profile