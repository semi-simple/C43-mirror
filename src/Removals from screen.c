Removals from screen.c

void Wait_loop2() {
#ifdef PC_BUILD                                                           //JM LONGPRESS FN
    now = g_get_monotonic_time();                   //JM usec
  while (now + (JM_FN_DOUBLE_TIMER + 6) * 1000 > g_get_monotonic_time());
#endif
#ifdef DMCP_BUILD
#define TIMER_IDX 1
  sys_timer_start(TIMER_IDX, JM_FN_DOUBLE_TIMER + 6);  // wake up for key
  sys_sleep();
  sys_timer_disable(TIMER_IDX);
#endif
}




void Wait_loop3(uint16_t tim) {
#ifdef PC_BUILD                                                           //JM LONGPRESS FN
    uint32_t now;
    now = g_get_monotonic_time() + tim * 1000;                   //JM usec
  while (now > g_get_monotonic_time());
#endif
#ifdef DMCP_BUILD
#define TIMER_IDX 1
  sys_timer_start(TIMER_IDX, tim);  // wake up for key
  sys_sleep();
  sys_timer_disable(TIMER_IDX);
#endif
}



//refreshScreen(NULL);

//gtk_widget_queue_draw(screen);  
//while (g_main_context_pending(NULL)) {
//    g_main_context_iteration(NULL,FALSE);
//}


//  while(gtk_events_pending()) {
//    gtk_main_iteration();
//  }



void Wait_loop1() {
  while (TC_compare( JM_FN_DOUBLE_TIMER + 6 ) == 1);  //1: verloopte tyd LANGER as (t).
}


