/* 
   net_midi.h

   midi management routines for the bees control network.
   handles operator focus, routes handler/refresh requests

*/

#ifndef _ALEPH_BEES_NET_MIDI_H_
#define _ALEPH_BEES_NET_MIDI_H_


#include "types.h"
#include "midi.h"

typedef void(*midi_handler_t)(void* op, u32 event);

// abstract superclass for midi operators
// has event handler and focus flag
typedef struct _op_midi {
  // handler function, will connect to app event handler
  midi_handler_t handler;
  // pointer to operator subclass
  void* op;
  // pointers to adjacent list elements
  struct _op_midi* next;
  struct _op_midi* prev;

} op_midi_t;

//---- linked list of instantiated midi operators

// list structure
typedef struct _op_list {
  void* top;
  void* cur;
  u32 num;
} op_midi_list_t;


//-------------------------
//--- extern functions

// add op pointer after the current position
extern void net_midi_list_add(op_midi_t* op);
// delete op pointer at current position, return next pointer in list
extern op_midi_t* net_midi_list_remove(op_midi_t* op);
// handle incoming midi packet
extern void net_handle_midi_packet(u32 data);

#endif // h guard