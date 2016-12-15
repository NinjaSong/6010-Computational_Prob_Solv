/*
 * event_handler.c
 *
 *  Created on: Sep 16, 2016
 *      Author: Frank
 */

#include <stdio.h>
#include <stdlib.h>
#include "event_data.h"
#include "event_handler.h"
#include "../utils/random/random.h"
#include "../utils/queue/Queue.h"
#include "../utils/priority_queue/PriorityQueue.h"

extern double S, A;
extern int num_stations;
extern int part_counter;
extern double simu_time;
extern Queue **q_list;
extern PriorityQueue *pq;
extern int debug;
// statistic variables
extern double sum_total_time, avg_total_time;
extern double sum_waiting_time, avg_waiting_time;
extern int count_finished_parts;

void schedule_next_event(double start_time, double station_id, void (*event_handler)()) {
	if (start_time >= simu_time)
		return;
	Event *next_event = (Event *) malloc(sizeof(Event));
	next_event->start_time = start_time;
	next_event->station_id = station_id;
	next_event->event_handler = event_handler;
	pq_push(pq, next_event);
}

void leave(Event *event) {
	Part *part = q_pop(q_list[event->station_id]);
	// ----
	if (debug) {
		printf("Dequeue \t%f\t%-8c\t%d\n", event->start_time, 'A' + event->station_id, part->id);
	}
	// ----
	// schedule next service event for the next part in the queue of this station
	if (q_size(q_list[event->station_id]) > 0) {
		schedule_next_event(event->start_time, event->station_id, &start_service);
	}
	// ----
	if (debug) {
		printf("End_Serv\t%f\t%-8c\t%d\n", event->start_time, 'A' + event->station_id, part->id);
	}
	// ----
	// done if this is the last station
	if (event->station_id == num_stations - 1) {
		part->finish_time = event->start_time;
		// do some statistics
		sum_total_time += part->finish_time - part->create_time;
		avg_total_time = sum_total_time / part->id;
		// ----
		if (debug) {
			printf("All_Done\t%f\tSink    \t%-8d\t", event->start_time, part->id);
		}
		// ----
		for (int i = 0; i < num_stations; i++) {
			sum_waiting_time += part->dequeue_times[i] - part->enqueue_times[i];
			// ----
			if (debug) {
				printf("%f", part->dequeue_times[i] - part->enqueue_times[i]);
				if (i == num_stations - 1)
					printf("\n");
				else
					printf("\t");
			}
			// ----
		}
		avg_waiting_time = sum_waiting_time / part->id;
		count_finished_parts++;
		// free this part
		free_part(part);
	}
	else {  // enqueue the current part into next station's queue
		q_push(q_list[event->station_id + 1], part);
		// ----
		if (debug) {
			printf("Enqueue \t%f\t%-8c\t%d\n", event->start_time, 'A' + event->station_id + 1, part->id);
		}
		// ----
		part->enqueue_times[event->station_id + 1] = event->start_time;
		// schedule the service event for the next station if it's free
		if (q_size(q_list[event->station_id + 1]) == 1) {
			schedule_next_event(event->start_time, event->station_id + 1, &start_service);
		}
	}
	// free current event
	free_event(event);
}

void start_service(Event *event) {
	// get the part for this event
	Part *part = q_peek(q_list[event->station_id]);
	part->dequeue_times[event->station_id] = event->start_time;
	// schedule dequeue event for this part
	double next_start_time = event->start_time + part->service_times[event->station_id];
	schedule_next_event(next_start_time, event->station_id, &leave);
	// ----
	if (debug) {
		printf("Start_Serv\t%f\t%-8c\t%d\n", event->start_time, 'A' + event->station_id, part->id);
	}
	// ----
	// free this event
	free_event(event);
}

void create_part(Event *event) {
	// create part for this event
	part_counter++;
	Part *part = (Part *) malloc(sizeof(Part));
	part->create_time = event->start_time;
	part->id = part_counter;
	part->service_times = (double *) malloc(num_stations * sizeof(double));
	double serv_time = randexp(S);
	for (int i = 0; i < num_stations; i++) {
		part->service_times[i] = serv_time;
	}
	part->enqueue_times = (double *) malloc(num_stations * sizeof(double));
	part->dequeue_times = (double *) malloc(num_stations * sizeof(double));
	part->finish_time = -1;
	// ----
	if (debug) {
		printf("Create_Part\t%f\tSource  \t%-8d\t", event->start_time, part_counter);
		for (int i = 0; i < num_stations; i++) {
			printf("%f", part->service_times[i]);
			if (i == num_stations - 1)
				printf("\n");
			else
				printf("\t");
		}
	}
	// ----
	// schedule next create event
	double interval = randexp(A);
	schedule_next_event(event->start_time + interval, -1, &create_part);
	// send this part to the first station
	q_push(q_list[0], part);
	// ----
	if (debug) {
		printf("Enqueue \t%f\t%-8c\t%d\n", event->start_time, 'A' + event->station_id + 1, part->id);
	}
	// ----
	part->enqueue_times[0] = event->start_time;
	// schedule the service event if the first station is available
	if (q_size(q_list[0]) == 1) {
		schedule_next_event(event->start_time, 0, &start_service);
	}
	// free this event
	free_event(event);
}

