//prgm
//aleph-avr32

//asf
#include "print_funcs.h"

// avr32
#include "bfin.h"
#include "control.h"

//prgm
#include "handler.h"
#include "render.h"
#include "ctl.h"


//static functions
static void handle_encoder_0(s32 val);

static void ctrl_parameter(u32 pid, fract32 val);

void handle_encoder_0(s32 val) {
    print_dbg("\r\n encoder moving...");
    print_dbg_ulong(val);
    ctrl_parameter(0, val);
}

void ctrl_parameter(u32 pid, fract32 val) {
    print_dbg("\r\n parameter id...");
    print_dbg_ulong(pid);
    print_dbg("\r\n parameter value...");
    print_dbg_ulong(val);
    ctl_param_change(eParamFreq0 + pid, val); //defined in control.h
}


//external functions
void assign_prgm_event_handlers(void) {
    
//    app_event_handlers[ kEventAppCustom ]	= &net_poll_handler ;
    app_event_handlers[ kEventEncoder0 ] = &handle_encoder_0 ;
//    app_event_handlers[ kEventEncoder1 ] = &handler_Encoder1 ;
//    app_event_handlers[ kEventEncoder2 ] = &handler_Encoder2 ;
//    app_event_handlers[ kEventEncoder3 ] = &handler_Encoder3 ;
//    app_event_handlers[ kEventSwitch0 ]	= &handler_Switch0 ;
//    app_event_handlers[ kEventSwitch1 ]	= &handler_Switch1 ;
//    app_event_handlers[ kEventSwitch2 ]	= &handler_Switch2 ;
//    app_event_handlers[ kEventSwitch3 ]	= &handler_Switch3 ;
//    app_event_handlers[ kEventSwitch4 ]	= &handler_Switch4 ; //mode switch
//    app_event_handlers[ kEventSwitch5 ]	= &handler_Switch5 ; //power switch
//    app_event_handlers[ kEventSwitch6 ]	= &handler_Switch6 ; //my left foot
//    app_event_handlers[ kEventSwitch7 ]	= &handler_Switch7 ; //the other foot
}