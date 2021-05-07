CC = g++
src = $(shell find ./ -name "*.cpp")
obj = $(shell find ./ -name "*.o")

calculator: $(obj)
	$(CC) -o calculator $(obj)
calculator.o: calculator.cpp calculator.h
	$(CC) -c calculator.cpp
display.o: display.cpp display.h calculator.h
	$(CC) -c display.cpp
main.o: main.cpp display.h calculator.h
	$(CC) -c main.cpp
all:
	$(CC) -o calculator $(src)
debug:
	$(CC) -g $(src)
clean:
	rm $(obj)
	rm a.out
