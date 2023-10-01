app:
	g++ -std=c++17 -I ./eigen-3.4.0 -I ./spectra-1.0.1/include main.cpp

result:
	 ./a.out Configuration.txt

all:
	 make app
	 make result
