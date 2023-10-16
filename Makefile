app:
	g++ -std=c++17 -I ./eigen-3.4.0 -I ./spectra-1.0.1/include -I ./utils main.cpp

result:
	 ./a.out Configuration.txt ./results/graph.csv

all:
	 make app
	 make result
