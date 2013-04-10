/* redraw.c
 * bees
 * aleph
 *
 * page-specific redraw functinos.
 */

// undefine to use fixed-point output formatting
//#define SNPRINTF

// std
#include <string.h>
//#include <stdio.h>

#ifdef ARCH_AVR32
#include "print_funcs.h"
#endif

// aleph
#include "files.h"
#include "fix.h"
#include "param.h"
#include "preset.h"
#include "redraw.h"
#include "scene.h"
#include "screen.h"
#include "types.h"

//------------------------
//----- static variables

// line buffer
static char lineBuf[CHAR_COLS];
static char numBuf[FIX_DIG_TOTAL] = "           ";
static char *pline = lineBuf;
static char * const pLineEnd = lineBuf + CHAR_COLS_1;

//---------------------------------
//---- static functions

// write to top of line buffer
static inline void println(const char* str, int pos);
// append to line buffer
static inline void appendln(const char* str);
// write int to top of line buffer
static inline void println_int(int val, int pos);
// append int to line buffer (right justified, with bound argument)
static inline void appendln_int_rj(int val, int len);
// append int to line buffer (left justified, no bounds)
static inline void appendln_int_lj(int val);
// append char to line buffer
static inline void appendln_char(char c);
// clear line buffer
static inline void clearln(void);
// move to given position in line buffer
//static inline void mvln(u8 pos);
// end line buffer (write a zero to next position)
static inline void endln(void);

//// line redraws
// op
static void draw_line_ops(s32 n, u16 num, u8 y, u8 hl);
// in
static void draw_line_ins(s32 n, u16 num, u8 y, u8 hl);
// out
static void draw_line_outs(s32 n, u16 num, u8 y, u8 hl);
// presets
static void draw_line_presets(s32 n, u16 num, u8 y, u8 hl);
// scenes
static void draw_line_scenes(s32 n, u16 num, u8 y, u8 hl);
// dsp
static void draw_line_dsp(s32 n, u16 num, u8 y, u8 hl);

//==================================================
//==== redraw ops page
void redraw_ops(void) {
  u8 y = 0;                       // which line
  s32 n, nCenter;         // which list entry
  // total count of ops, including system-controlled
  const u16 num = net_num_ops();
  // draw the header
  screen_line(0, 0, "OPERATORS", 15);
  // print selection at center
  nCenter = curPage->selected;
  if (nCenter >= num) {
    nCenter = num;
  }
  n = nCenter;
  y = SCREEN_ROW_CENTER;
  draw_line_ops(n, num, y, 15);
  screen_hl_line(0, y, 1);
  // print lower entries
  while (y > 1) {
    draw_line_ops(--n, num, --y, 4);
  }
  // re-center
  n = nCenter;
  y = SCREEN_ROW_CENTER;
  // print higher entries
  while (y < CHAR_ROWS_2) {
    draw_line_ops(++n, num, ++y, 4);
  }
  // draw footer 
  // (new op type)
  screen_blank_line(0, CHAR_ROWS_2);
  println(" + ", 0);
  appendln(op_registry[userOpTypes[newOpType]].name);
  endln(); screen_line(0, CHAR_ROWS_2, lineBuf, 8);
  // (function labels)
  // don't allow deletion of system operators
  if (net_op_status(net_num_ops() - 1) == eUserOp) {
    screen_line(0, CHAR_ROWS_1, "PARAM ROUTE CREATE DELETE", 3);
  } else  { 
    screen_line(0, CHAR_ROWS_1, "PARAM ROUTE CREATE", 3);
  }
}

//==================================================
//==== redraw inputs page
void redraw_ins(void) { 
  u8 y = 0;                       // which line
  s32 n, nCenter;         // which list entry
  const u16 num = net_num_ins(); // how many 

  // print selection at center
  nCenter = curPage->selected;
  if (nCenter >= num) {
    nCenter = num;
  }
  n = nCenter;
  y = SCREEN_ROW_CENTER;
  draw_line_ins(n, num, y, 15);
  screen_hl_line(0, y, 1);
  // print lower entries
  while (y > 1) {
    n--;
    y--;
    draw_line_ins(n, num, y, 4);
  }
  // re-center
  n = nCenter;
  y = SCREEN_ROW_CENTER;
  // print higher entries
  while (y < CHAR_ROWS_2) {
    n++;
    y++;
    draw_line_ins(n, num, y, 4);
  }    
  // draw the header
  screen_line(0, 0, "PARAMS", 12);
  // draw footer (function labels)
  screen_line(0, CHAR_ROWS_1, "GATHER DISCONNECT STORE  PRESET ", 10);
}

//==================================================
//==== redraw outputs page
void redraw_outs(void) {
  u8 y = 0;                       // which line
  s32 n, nCenter;         // which list entry
  //s16 target;
  const u16 num = net_num_outs(); // how many ops
  // print selection at center
  nCenter = curPage->selected;
  if (nCenter >= num) {
    nCenter = num;
  }
  n = nCenter;
  y = SCREEN_ROW_CENTER;
  draw_line_outs(n, num, y, 15);  
  screen_hl_line(0, y, 1);
  // print lower entries
  while (y > 1) {
    n--;
    y--;
    draw_line_outs(n, num, y, 4);
  }
  // re-center
  n = nCenter;
  y = SCREEN_ROW_CENTER;
  // print higher entries
  while (y < CHAR_ROWS_2) {
    n++;
    y++;
    draw_line_outs(n, num, y, 4);
  }
  // header
  screen_line(0, 0, "ROUTING", 12);
  // (footer)
  screen_line(0, CHAR_ROWS_1, "FOLLOW DISCONNECT STORE PRESET", 10);
}

/// redraw gathered outputs
void redraw_gathered(void) {
  //// TODO
}

//==================================================
//==== redraw presets page
void redraw_presets(void) {
  u8 y = 0;                       // which line
  s32 n, nCenter;         // which list entry
  const u16 num = NET_PRESETS_MAX; // how many 

  // print selection at center
  nCenter = curPage->selected;
  if (nCenter >= num) {
    nCenter = num;
  }
  n = nCenter;
  y = SCREEN_ROW_CENTER;
  // modes
  switch(curPage->mode) { 
  case eModeClear:
    screen_line(0, y, "CLEAR?", 14);
    break;
  case eModeCopy:
    screen_line(0, y, "COPY?", 14);
    break;
  case eModeStore:
    screen_line(0, y, "STORE?", 14);
    break;
  case eModeRecall:
    screen_line(0, y, "RECALL?", 14);
    break;
  case eModeNone:
  default:
    draw_line_presets(n, num, y, 15);  
    break;
  }
  screen_hl_line(0, y, 1);
  // print lower entries
  while (y > 1) {
    n--;
    y--;
    draw_line_presets(n, num, y, 4);
  }
  // re-center
  n = nCenter;
  y = SCREEN_ROW_CENTER;
  // print higher entries
  while (y < CHAR_ROWS_2) {
    n++;
    y++;
    draw_line_presets(n, num, y, 4);
  }
  // draw footer (function labels)
  screen_line(0, CHAR_ROWS_1, "CLEAR COPY STORE RECALL", 10);
  // draw the header
  screen_line(0, 0, "PRESETS", 12);
}

//==================================================
//==== redraw scenes page
void redraw_scenes(void) {
  u8 y = 0;                       // which line
  s32 n, nCenter;         // which list entry
  const u16 num = SCENE_COUNT; // how many 
  // print selection at center
  nCenter = curPage->selected;
  if (nCenter >= num) {
    nCenter = num;
  }
  n = nCenter;
  y = SCREEN_ROW_CENTER;
  // modes
  switch(curPage->mode) { 
  case eModeClear:
    screen_line(0, y, "CLEAR?", 14);
    break;
  case eModeCopy:
    screen_line(0, y, "COPY?", 14);
    break;
  case eModeStore:
    screen_line(0, y, "STORE?", 14);
    break;
  case eModeRecall:
    screen_line(0, y, "RECALL?", 14);
    break;
  case eModeNone:
  default:
    draw_line_scenes(n, num, y, 15);  
    break;
  }
  screen_hl_line(0, y, 1);
  // print lower entries
  while (y > 1) {
    n--;
    y--;
    draw_line_scenes(n, num, y, 4);
  }
  // re-center
  n = nCenter;
  y = SCREEN_ROW_CENTER;
  // print higher entries
  while (y < CHAR_ROWS_2) {
    n++;
    y++;
    draw_line_scenes(n, num, y, 4);
  }
  // draw header
  screen_line(0, 0, "SCENES", 12);
  // draw footer (function labels)
  screen_line(0, CHAR_ROWS_1, "CLEAR COPY STORE RECALL", 10);
}

//==================================================
//==== redraw dsp page
void redraw_dsp(void) {
  u8 y = 0;                       // which line
  s32 n, nCenter;         // which list entry
  u16 num = files_get_dsp_count();
  /* print_dbg("\r\n count of DSP files: "); */
  /* print_dbg_ulong(num); */
  // print selection at center
  nCenter = curPage->selected;
  if (nCenter >= num) {
    nCenter = num;
  }
  n = nCenter;
  y = SCREEN_ROW_CENTER;
  draw_line_dsp(n, num, y, 15);
  screen_hl_line(0, y, 1);
  // print lower entries
  while (y > 1) {
    n--;
    y--;
    draw_line_dsp(n, num, y, 4);
  }
  // re-center
  n = nCenter;
  y = SCREEN_ROW_CENTER;
  // print higher entries
  while (y < CHAR_ROWS_2) {
    n++;
    y++;
    draw_line_dsp(n, num, y, 4);
  }
  screen_line(0, 0, "DSP", 12);
  // draw footer (function labels)
  screen_line(0, CHAR_ROWS_1, "LOAD DEFAULT", 10);
}

//==================================================
//==== redraw play page
void redraw_play(void) {
  u8 y;
  u8 n = CHAR_ROWS_1;
  u8 idx;

  for(y = 1; y < CHAR_ROWS; y++ ) {
    //    println("", 0);
    idx = touchedParams[n].idx;

    /*
    screen_blank_line(0, y);
    println_int(idx, 0);
    endln(); screen_line(0, y, lineBuf, 1);
    
    println( net_op_name(net_in_op_idx(idx)), 0 );
    appendln_char('/');
    appendln( net_in_name(idx) );
    endln(); screen_string(16, y, lineBuf, 12);

    print_fix16(numBuf, touchedParams[n].val);
    screen_line(0, 80, numBuf, 10);
    */
    n--;
  }
  // draw the header
  screen_line(0, 0, "PLAY", 12);
}

/////---------------------------
/////  line redraws

// draw line of ops page
static void draw_line_ops(s32 n, u16 num, u8 y, u8 hl) {
  // wrap
  if (n < 0) {
    n += num;
  } else if (n >= num) {
    n -= num;
  } 

  // clearln();
  screen_blank_line(0, y);
  println_int(n, 0); endln();
  screen_string(0, y, lineBuf, hl);
  //  clearln();
  println(net_op_name(n), 0); endln();
  screen_line(16, y, lineBuf, hl);
}

// draw line of inputs page
static void draw_line_ins(s32 n, u16 num, u8 y, u8 hl) {
  s16 opIdx;
  //  char pch;
  // wrap
  if (n < 0) {
    n += num;
  } else if (n >= num) {
    n -= num;
  } 
  opIdx = net_in_op_idx(n);
  //  if (net_get_in_preset(n)) { pch = '*'; } else { pch = '.'; }
  screen_blank_line(0, y);
  if (opIdx >=0 ) { // this is an operator input
    println_int(opIdx, 0);
    endln(); screen_string(0, y, lineBuf, hl);
    println( net_op_name(opIdx), 0 );
    appendln_char('/');
    appendln( net_in_name(n) );
    endln(); screen_string(16, y, lineBuf, hl);
    print_fix16(numBuf, net_get_in_value(n) );
    screen_line(80, y, numBuf, hl);
  } else { // this is a parameter input
    println("P_", 0);
    appendln_int_lj( (int)net_param_idx(n));
    endln(); screen_string(0, y, lineBuf, hl);
    println( net_in_name(n) , 0);
    endln(); screen_string(16, y, lineBuf, hl);
    print_fix16(numBuf, net_get_in_value(n) );
    screen_line(80, y, numBuf, hl);
  }
}

// draw line of outputs page
static void draw_line_outs(s32 n, u16 num, u8 y, u8 hl) {
  s16 target;
  //  char pch;
  // wrap
  if (n < 0) {
    n += num;
  } else if (n >= num) {
    n -= num;
  } 
  target = net_get_target(n);
  //  if (net_get_out_preset(n)) { pch = '*'; } else { pch = '.'; }
  screen_blank_line(0, y);
  if (target >= 0) {
    println_int(net_out_op_idx(n), 0);
    endln(); screen_string(0, y, lineBuf, hl);
    println( net_op_name(net_out_op_idx(n)) , 0);
    appendln_char('/');
    appendln( net_out_name(n) );
    endln(); screen_string(16, y, lineBuf, hl);
    println("-> ", 0);
    appendln_int_lj( net_in_op_idx(target) );
    appendln_char('.');
    appendln( net_op_name(net_in_op_idx(target)) );
    appendln_char('/');
    appendln( net_in_name(target) );
    endln(); screen_string(60, y, lineBuf, hl);
  } else {
    println_int(net_out_op_idx(n), 0);
    endln(); screen_string(0, y, lineBuf, hl);
    println( net_op_name(net_out_op_idx(n)) , 0);
    appendln_char('/');
    appendln( net_out_name(n) );
    endln(); screen_string(16, y, lineBuf, hl);
  }
}

// draw line of presets page
void draw_line_presets(s32 n, u16 num, u8 y, u8 hl) {
  if (n < 0) {
    n += num;
  } else if (n >= num) {
    n -= num;
  } 
  
  screen_blank_line(0, y);

  println_int((int)n, 0);
  endln(); screen_string(0, y, lineBuf, hl);
  println(preset_name(n), 0);
  endln(); screen_string(16, y, lineBuf, hl);
}

// draw line of scenes page
void draw_line_scenes(s32 n, u16 num, u8 y, u8 hl) {
  // wrap
  if (n < 0) {
    n += num;
  } else if (n >= num) {
    n -= num;
  } 

  screen_blank_line(0, y);

  ////// TODO
  return;
  //////

  println_int((int)n, 0);
  endln(); screen_string(0, y, lineBuf, hl);
  println( scene_name(n), 0 );
  endln(); screen_string(16, y, lineBuf, hl);
}

// draw line of dsp page
void draw_line_dsp(s32 n, u16 num, u8 y, u8 hl) {
    // wrap
  if (n < 0) {
    n += num;
  } else if (n >= num) {
    n -= num;
  } 
  screen_blank_line(0, y);
  println( (const char*)files_get_dsp_name(n), 0);
  endln(); screen_string(0, y, lineBuf, hl); 
}


///// snprintf replacement
// write to top of line buffer
static inline void println(const char* str, int pos) {
  pline = lineBuf + pos;
  appendln(str);
}

// append to line buffer
static inline void appendln(const char* str) {
  //  print_dbg("\n\r line buffer start: ");
  //  print_dbg_hex(pline);
  while((*str != 0) && (pline <= pLineEnd)) {
    *pline++ = *str++;
  }
}

// write int to top of line buffer
static inline void println_int(int val, int pos) {
  pline = lineBuf + pos;
  appendln_int_lj(val);
}

// append int to line buffer
static inline void appendln_int_rj(int val, int len) {
  // bounds check :
  //  int rem = (int)pLineEnd -  (int)pline;
  //  if (len > rem) len = rem;
  itoa_whole(val, pline, len); 
  pline += len; 
}

// append int to line buffer (left justified, no bounds)
static inline void appendln_int_lj(int val) {
  pline += itoa_whole_lj(val, pline); 
}

// append char to line buffer
static inline void appendln_char(char c) {  
  *pline++ = c;
}

// clear line buffer
static inline void clearln(void) {
  memset(pline, ' ', CHAR_COLS);
}

static inline void endln(void) {
  *(pline) = 0;
}
