CSE 6010
Assignment 2 - Queueing Network Simulation
Hongyang Wang, Yunhe Song
Sep 25, 2016


1. Folder Structure

bin/
	simulate					# exe file of the queueing system simulation program
	test-pq						# exe file of the priority queue performance testing program

result/
	result-1q.txt				# for literature survey use, the result with only one queue in the system
	result-debug.txt			# for debug use, run the simulation with debug info and valgrind for memory check
	result.txt					# resulf of the simulation with different arrival rates (1/A)

run/
	Makefile					# makefile for the program
	run.sh						# run the simulation with a set of different arrival rates
	run_1q.sh					# run the simulation with only one station in the system
	run_debug.sh				# run a short simulation with debug info printed; make sure you have valgrind installed

src/
	event/
		event_data.h			# defines Part and Event structs
		event_handler.c			# implementation of each type of event handler
		event_handler.h			# event handler APIs for main function

	main.c						# main function

	utils/
		priority_queue/
			PriorityQueue.c 	# implementation of pq using linked list (not heap)
			PriorityQueue.h 	# pq APIs
			test.txt 			# a sample program showing how to use the pq APIs
		queue/
			Queue.c 			# implementation of queue using linked list
			Queue.h 			# queue APIs
			test.txt 			# a sample program showing how to use the queue APIs
		random/
			random.c 			# implementation of random number generator
			random.h 			# rng APIs

test-pq/						# for cse6010, test the performance of the priority queue implemented
	Makefile 					# makefile
	pq-result.txt 				# running times for each insertion and deletion operations
	run.sh 						# run test-pq with a different set of Ns
	test-pq.c 					# main function for this test


2. Compile and Run

There are two pieces of softwares for this assignment:
	(1) Queueing model simulation
		a) cd to the run/ folder
		b) make
		c) run any of the .sh script with different parameters fed in to the program
	(2) Priority queue performance measurement
		a) cd to the test-pq/ folder
		b) make
		c) sh run.sh to run the program



