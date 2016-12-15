/*
 ============================================================================
 Name        : queueing-model-simulation.c
 Author      : Hongyang Wang & YunHe Song
 Version     : 1.0
 Copyright   : © Fall 2016 CSE 6010 Hongyang Wang & YunHe Song
 Description : CSE 6010 assignment 2, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include "event/event_data.h"
#include "event/event_handler.h"
#include "utils/queue/Queue.h"
#include "utils/priority_queue/PriorityQueue.h"
#include "utils/random/random.h"

// global variables
double S = 0, A = 0;  // average service time, average inter-arrival time
int num_stations = 0;  // total number of stations (3 for this assignment)
int part_counter = 0;  // global counter of how many parts created so far
double simu_time = 0.0;  // total simulation duration
int debug = -1;  // whether to output debug info
Queue **q_list = NULL;  // queues at each station holding Parts
PriorityQueue *pq = NULL;  // global priority queue holding Events
// statistic variables
double sum_total_time = 0.0, avg_total_time = 0.0;  // records the total/average time a part stays in the system
double sum_waiting_time = 0.0, avg_waiting_time = 0.0;  // records the total/average queueing time for a part
int count_finished_parts = 0;  // total number of parts finished during the simulation

int main(int argc, char *argv[]) {
	// parse input arguments
	// .executable, avg_service_time, avg_interval, num_stations, total_simulation_time, isdebug
	if (argc != 6) {
		printf("The program takes in 4 arguments: avg_service_time, avg_interval, \
				num_stations, total_simulation_time, debug");
		return -1;
	}
	S = atof(argv[1]);
	A = atof(argv[2]);
	num_stations = atoi(argv[3]);
	simu_time = atof(argv[4]);
	debug = atoi(argv[5]);
	printf("Parameters: avg_service_time=%f, avg_interval=%f, num_stations=%d, "
			"total_simulation_time=%f\n", S, A, num_stations, simu_time);

	// initialize
	part_counter = 0;
	rand_init(-1);
	q_list = (Queue **) malloc(num_stations * sizeof(Queue *));
	for (int i = 0; i < num_stations; i++) {
		q_list[i] = (Queue *) malloc(sizeof(Queue));
		q_init(q_list[i]);
	}
	pq = (PriorityQueue *) malloc(sizeof(PriorityQueue));
	pq_init(pq, &event_cmp);

	// simulation
	// add the first event
	schedule_next_event(0.0, -1, &create_part);
	// simulate
	// ----
	if (debug) {
		printf("------------------------------------------------------------------------------------------\n");
		printf("Event_Type\tTimestamp\tStation_ID\tPart_ID \tServ_Times(for Source) OR Wait_Times(for Sink)\n");
		printf("------------------------------------------------------------------------------------------\n");
	}
	// ----
	while (pq_size(pq) > 0) {
		Event *event = pq_pop(pq);
		// do something

		(*(event->event_handler))(event);
	}
	if (debug) {
		printf("------------------------------------------------------------------------------------------\n");
	}
	// print simulation result
	printf("Result: avg_total_time=%f, avg_waiting_time=%f, total_part_finished=%d\n", \
		avg_total_time, avg_waiting_time, count_finished_parts);
	printf("\n");
	
	// clean
	for (int i = 0; i < num_stations; i++) {
		q_clear(q_list[i], &free_part);
		free(q_list[i]);
		q_list[i] = NULL;
	}
	free(q_list);
	q_list = NULL;

	pq_clear(pq, &free_event);
	free(pq);
	pq = NULL;
}
