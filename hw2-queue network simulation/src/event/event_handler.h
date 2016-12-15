/*
 * event.h
 *
 *  Created on: Sep 16, 2016
 *      Author: Frank
 */

#ifndef EVENT_EVENT_H_
#define EVENT_EVENT_H_

#include "event_data.h"

// create and add next event to the priority queue
void schedule_next_event(double start_time, double station_id, void (*event_handler)());
// event handler: when a part leaves a station
void leave(Event *event);
// event handler: when a part is ready to get serviced
void start_service(Event *event);
// event handler: when a part is generated at the source
void create_part(Event *event);

#endif /* EVENT_EVENT_H_ */
