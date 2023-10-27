app:
	g++ -std=c++17 -I ./eigen-3.4.0 -I ./spectra-1.0.1/include -I src/utils -o bcm src/main.cpp

result:
	./bcm Configuration.txt ./results

plot:
	python3 matrix_plotting.py results/graph.csv

all:
	 make app
	 make result
	 make plot
