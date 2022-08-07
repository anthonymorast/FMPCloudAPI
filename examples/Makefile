CFLAGS=--std=c++17 -Wno-psabi -lcurl -lpthread
EXES=Calendars CompanyInfo CryptoForex FinancialStatementsSEC PriceQuotes TechnicalIndicators

Test: all
	./Calendars 
	./CompanyInfo 
	./CryptoForex 
	./FinancialStatementsSEC
	./PriceQuotes
	./TechnicalIndicators

all: $(EXES)

%.o: %.cpp
	g++ $(CFLAGS) -c $<

Calendars: Calendars.o
	g++ $(CFLAGS) -o Calendars $<

CompanyInfo: CompanyInfo.o
	g++ $(CFLAGS) -o CompanyInfo $<

CryptoForex: CryptoForex.o
	g++ $(CFLAGS) -o CryptoForex $<

FinancialStatementsSEC: FinancialStatementsSEC.o
	g++ $(CFLAGS) -o FinancialStatementsSEC $<

PriceQuotes: PriceQuotes.o
	g++ $(CFLAGS) -o PriceQuotes $<

TechnicalIndicators: TechnicalIndicators.o
	g++ $(CFLAGS) -o TechnicalIndicators $<

clean:
	rm -rf *.o $(EXES) *.json
