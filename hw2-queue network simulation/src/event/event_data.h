/*
 * event_data.h
 *
 *  Created on: Sep 16, 2016
 *      Author: Frank
 */

#ifndef EVENT_EVENT_DATA_H_
#define EVENT_EVENT_DATA_H_

#include <stdlib.h>

typedef struct {
	int id;  // ID for this part, starting from 1
	double create_time;  // the time when the source generates this part
	double finish_time;  // the time when the sink gets the part
    double* service_times;  // service times for each stations which obey exponential distribution
    double* enqueue_times, *dequeue_times;  // records the enqueue and dequeue time at each station

} Part;

typedef struct {
	double start_time;  // the starting time for this event
	int station_id;  // at which station it takes place
	void (*event_handler) ();  // how to handle this event (what type of event it is)

} Event;

// comparator of the Event type 
static int event_cmp(void *d1, void *d2) {
	Event *e1 = (Event *) d1, *e2 = (Event *) d2;
	if (e1->start_time < e2->start_time)
		return -1;
	else if (e1->start_time > e2->start_time)
		return 1;
	else
		return 0;
}

// how to free an Event
static void free_event(void *event) {
	Event *e = (Event *) event;
	free(e);
	e = NULL;
}

// how to free a Part
static void free_part(void *part) {
	Part *p = (Part *) part;
	free(p->service_times);
	free(p->enqueue_times);
	free(p->dequeue_times);
	free(p);
	p = NULL;
}

#endif /* EVENT_EVENT_DATA_H_ */
