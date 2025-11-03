CXX = g++
CXXFLAGS = -std=c++17 -O2 -Wall
INCLUDE = -Iinclude
SRC = src
OBJDIR = build
OBJS = $(OBJDIR)/account.o $(OBJDIR)/banking.o $(OBJDIR)/main.o $(OBJDIR)/queue.o $(OBJDIR)/stack.o

all: $(OBJDIR) BankingTransactionManager

$(OBJDIR):
	mkdir -p $(OBJDIR)

$(OBJDIR)/account.o: $(SRC)/account.cpp include/banking.h include/account.h
	$(CXX) $(CXXFLAGS) $(INCLUDE) -c $(SRC)/account.cpp -o $@

$(OBJDIR)/banking.o: $(SRC)/banking.cpp include/banking.h
	$(CXX) $(CXXFLAGS) $(INCLUDE) -c $(SRC)/banking.cpp -o $@

$(OBJDIR)/queue.o: $(SRC)/queue.cpp include/queue.h
	$(CXX) $(CXXFLAGS) $(INCLUDE) -c $(SRC)/queue.cpp -o $@

$(OBJDIR)/stack.o: $(SRC)/stack.cpp include/stack.h
	$(CXX) $(CXXFLAGS) $(INCLUDE) -c $(SRC)/stack.cpp -o $@

$(OBJDIR)/main.o: $(SRC)/main.cpp include/banking.h
	$(CXX) $(CXXFLAGS) $(INCLUDE) -c $(SRC)/main.cpp -o $@

BankingTransactionManager: $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o BankingTransactionManager

clean:
	rm -rf $(OBJDIR) BankingTransactionManager
