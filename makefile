CC = g++ -std=c++11
src = $(shell find ./ -name "*.cpp")
obj = $(patsubst %.cpp,%.o,$(src))
object = $(shell find ./ -name "*.o")

calculator: $(obj)
	$(CC) -o calculator $(obj)
calculator.o: calculator.cpp calculator.h
	$(CC) -c calculator.cpp
display.o: display.cpp display.h calculator.h
	$(CC) -c display.cpp
terminal.o: terminal.cpp terminal.h
	$(CC) -c terminal.cpp
main.o: main.cpp display.h calculator.h
	$(CC) -c main.cpp
debug:
	$(CC) -g $(src)
clean:
	rm $(object)
