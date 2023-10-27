
This project based on work publised in 'On the Hausdorff 
Dimension of Bernoulli Convolutions.

The code depends on eigen-3.4.0 and spectra-1.0.1 libraries.
They simply need to be downloaded (see links below) and 
extracted in the main directory leaving it like this,

Configuration.txt  eigen-3.4.0  Makefile  matrix_plotting.py  
README.txt  results  spectra-1.0.1  src

https://spectralib.org/download
https://eigen.tuxfamily.org/index.php?title=Main_Page

The command 'make app' can be used to compile the executable 
which will be named 'bcm' and is the one computing the 
lattice matrix and it's spectrla radius. It takes two 
aruments, the first is the path of the configuration file 
that will determine the matrix requested and the second is
the path of the directory where the results are solved.
The script matrix_plotting.py takes the gaph file as an
argument and plots the non-zero entries of the matrix. See 
Make file for more automations.