CSE 6010
Assignment 3 - Graph Analytics
Hongyang Wang, Yunhe Song
Oct 27, 2016


1. Folder Structure

bin/
	graph_gen					# exe file for part1 - graph generation
	graph_analy					# exe file for part2 - graph anaylsis

result/							# all the execution results will be stored under this folder
	graph10.txt					# a sample graph that contains 10 nodes
	hist10.txt					# the histogram of the sample graph
	analy10.txt					# the analytics of the sample graph

run/
	run.sh 						# shell script to run the two parts together with the option to direct all the outputs to files stored in the result folder

src/
	Makefile 					# overall makefile which will call the makefile of each of the two parts
	graph_gen/					# source code for part1
		Makefile 				# makefile for this part, which will generate part1's executable in the bin folder
		graphgen.c
		graphgen.h
		main.c
	graph_analy/
		Makefile 				# makefile for this part
		graph_utils.c
		graph_utils.h
		main.c
	queue/						# this is the generic queue borrowed from assignment 2
		Makefile 				# makefile of the queue, which will be called in part2's makefile
		Queue.c
		Queue.h


2. Compile and Run

(1) cd to src
(2) make, and two executables will be generated in the bin folder
(3) cd to run
(4) sh run.sh. A set of graphs with histograms and analytics will be generated with N ranging from 1000 to 10000, and the result (.txt files) will be stored in the result folder. For convenience, some very concise results (N and diameter) will be printed to the screen



