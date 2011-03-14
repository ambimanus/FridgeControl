/*
 * events.c
 *
 *  Created on: 13.03.2009
 *      Author: chh
 *
 *  Adapted from:
 *  http://www.sics.se/~adam/contiki/docs/a02333.html
 */

#include "events.h"

static struct {
    uint8_t head;
    uint8_t tail;
    event events[MAX_EVENTS];
} event_queue;

uint8_t events_is_event_pending() {
    return event_queue.head != event_queue.tail;
}

void events_push_event(event* ev) {
    // check queue size
    if ((event_queue.head + 1) % MAX_EVENTS == event_queue.tail) {
        // queue full
        return;
    }
    // store in queue
    event_queue.events[event_queue.head] = *ev;
    // resort queue per reverse-bubblesort
    uint8_t index = event_queue.head;
    uint8_t index_tmp;
    event* tmp = NULL;
    while (1) {
        index_tmp = index;
        index--;
        if (index < 0) {
            index = MAX_EVENTS - 1;
        }
        if (index != ((event_queue.tail - 1) % MAX_EVENTS) && ev->activation
                < event_queue.events[index].activation) {
            tmp = &event_queue.events[index];
            event_queue.events[index] = *ev;
            event_queue.events[index_tmp] = *tmp;
        } else {
            break;
        }
    }
    // update head
    uint8_t newhead;
    newhead = event_queue.head;
    newhead++;
    if (newhead >= MAX_EVENTS) {
        newhead = 0;
    }
    event_queue.head = newhead;
}

event* events_pop_event(uint32_t timestamp) {
    // get first pending event which has activation time <= timestamp
    event* ev = NULL;
    ev = &(event_queue.events[event_queue.tail]);
    if (ev->activation > timestamp) {
        // nothing to do yet, return null
        return NULL;
    }
    // update tail
    volatile uint8_t newtail;
    newtail = event_queue.tail;
    newtail++;
    if (newtail >= MAX_EVENTS) {
        newtail = 0;
    }
    event_queue.tail = newtail;
    // return found event
    return ev;
}
