#include "app.h"
#include "events.h"
#include "menu.h"
#include "scene,h"

void app_handle_event(event_t* e) {
  //// APP 
  switch(e->eventType) {

  case kEventRefresh:
    screen_refresh();
    break;
    //----- function switches
  case kEventSwitchDown0:
    menu_handleKey(eKeyFnDownA, e.eventData);
    break;
  case kEventSwitchUp0:
    menu_handleKey(eKeyFnUpA, e.eventData);
    break;
  case kEventSwitchDown1:
    menu_handleKey(eKeyFnDownB, e.eventData);
    break;
  case kEventSwitchUp1:
    menu_handleKey(eKeyFnUpB, e.eventData);
    break;
  case kEventSwitchDown2:
    menu_handleKey(eKeyFnDownC, e.eventData);
    break;
  case kEventSwitchUp2:
    menu_handleKey(eKeyFnUpC, e.eventData);
    break;
  case kEventSwitchDown3:
    menu_handleKey(eKeyFnDownD, e.eventData);
    break;
  case kEventSwitchUp3:
    menu_handleKey(eKeyFnUpD, e.eventData);
    break;
    /// footswitches
  case kEventSwitchDown6:
    print_dbg("\r\n footswitch1 down");
    break;
  case kEventSwitchUp6:
    print_dbg("\r\n footswitch1 up");
    break;
  case kEventSwitchDown7:
    print_dbg("\r\n footswitch2 down");
    break;
  case kEventSwitchUp7:
    print_dbg("\r\n footswitch2 up");
    break;
    // mode switch
  case kEventSwitchDown4:
    mode ^= 1;
    if(mode) { gpio_set_gpio_pin(LED_MODE_PIN); }
    else { gpio_clr_gpio_pin(LED_MODE_PIN); }
    menu_handleKey(eKeyMode, e.eventData);
    break;
  case kEventSwitchUp4:
    break;

  case kEventSwitchUp5:
    break;
  case kEventEncoder0:
    //	 print_dbg("\r\n encoder 0");
    if(e.eventData > 0) {
      menu_handleKey(eKeyEncUpD, e.eventData);
    } else {
      menu_handleKey(eKeyEncDownD, e.eventData);
    }
    break;
  case kEventEncoder1:
    //	print_dbg("\r\n encoder 1");
    if(e.eventData > 0) {
      menu_handleKey(eKeyEncUpC, e.eventData);
    } else {
      menu_handleKey(eKeyEncDownC, e.eventData);
    }
    break;
  case kEventEncoder2:
    //	print_dbg("\r\n encoder 2");
    if(e.eventData > 0) {
      menu_handleKey(eKeyEncUpB, e.eventData);
    } else {
      menu_handleKey(eKeyEncDownB, e.eventData);
    }
    break;
  case kEventEncoder3:
    //	print_dbg("\r\n encoder 3");
    if(e.eventData > 0) {
      menu_handleKey(eKeyEncUpA, e.eventData);
    } else {
      menu_handleKey(eKeyEncDownA, e.eventData);
    }
    break;

  case kEventAdc0:
    //	print_dbg("\r\nadc val 0: ");
    //	print_dbg_hex(e.eventData);
    //	displayAdcVal(0, e.eventData);
    break;
  case kEventAdc1:
    //	 print_dbg("\r\nadc val 1: ");
    //	 print_dbg_hex(e.eventData);
    //	 displayAdcVal(1, e.eventData);
    break;
  case kEventAdc2:
    //	 print_dbg("\r\nadc val 2: ");
    //	 print_dbg_hex(e.eventData);
    //	 displayAdcVal(2, e.eventData);
    break;
  case kEventAdc3:
    //     	print_dbg("\r\nadc val 3: ");
    //     	print_dbg_hex(e.eventData);
    //     	displayAdcVal(3, e.eventData);
    break;
  }
}


void app_init(u8 firstrun) {
  net_init();
  print_dbg("\r\n net_init");
  
  preset_init();
  print_dbg("\r\n preset_init");

  scene_init();
  print_dbg("\r\n scene_init");

  menu_init();
  print_dbg("\r\n menu_init");
}

void app_launch(void) {
  scene_read_default();
}