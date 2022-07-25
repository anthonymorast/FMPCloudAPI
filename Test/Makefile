CFLAGS=--std=c++17 -Wno-psabi -lcurl -lpthread
EXES=Quotes Calendar Profile FinancialStatements Valuations HoldersAndWeightings HistoricalData

Test: all
	./Quotes
	./Profile
	./Calendar
	./FinancialStatements 
	./Valuations
	./HoldersAndWeightings 
	./HistoricalData

all: $(EXES)

%.o: %.cpp
	g++ $(CFLAGS) -c $<

Profile: Profile.o
	g++ $(CFLAGS) -o Profile $<

Quotes: Quotes.o
	g++ $(CFLAGS) -o Quotes $<

Calendar: Calendar.o
	g++ $(CFLAGS) -o Calendar $<

FinancialStatements: FinancialStatements.o
	g++ $(CFLAGS) -o FinancialStatements $<

Valuations: Valuations.o
	g++ $(CFLAGS) -o Valuations $<

HoldersAndWeightings: HoldersAndWeightings.o
	g++ $(CFLAGS) -o HoldersAndWeightings $<

HistoricalData: HistoricalData.o
	g++ $(CFLAGS) -o HistoricalData $<

clean:
	rm -rf *.o $(EXES)
