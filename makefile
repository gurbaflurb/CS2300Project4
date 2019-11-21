generate:
	@echo "Compiling libraries in to object files..."
	@g++ -c lib/matrix.cpp -o lib/matrix.o
	@echo "Compiling source.cpp into executable main..."
	@g++ source.cpp lib/matrix.o -std=c++11 -o main