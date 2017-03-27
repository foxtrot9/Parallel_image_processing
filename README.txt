Q1 and Q2 folder contains following details:
1. codes
2. 1 output folder which contains value of results obtained by us when we run this program on a cluster with 16 cores. (found on internet. Intel Xeon E5-2630.) .
3. 1 folder with Graphs and its tables.
4. Driver program. (Driver program contains compilation and everything.)

Hardware details will be found lscpu.txt in same folder as this file.
Tables of graph are stored in .html format for better viewing. They can be found in graphs folder in individual question. Also, .xls for the same is stored at same location for better viewing.

Notice: half-width=7 can take up 70-80 seconds in serial run.
2. Due to size constraints of image of size 1024 x 1024 and 768 x 768 , we have submitted them in .jpeg format. Please convert it into .ppm before testing. (Driver.sh will only test on image of lena512.ppm , other images are not included in driver program because of their large runtime.) 

Anomaly:
Sometimes serial code takes more time than parallel code with OMP_NUM_THREADS=1.
This is because at starting of execution CPUs are usually idle. And therefore they take more time for scaling up frequency.
A workaround for this is to set CPU to maximum allowed frequency(or a fix frequency) OR run a dummy program (warm up) before running driver.sh . (This will allow CPU to scale up frequency before it starts executing our code.)