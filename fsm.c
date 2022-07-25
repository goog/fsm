#include <stdio.h>
#include <string.h>



typedef enum
{
    STATE_INIT = 0,
    STATE_WORK,    // normal work state
    STATE_NETWORK_EXCEPTION,  // bad network
    STATE_ERR,   // if some error happened
    STATE_NUM
}fsm_state_t;


// input event
typedef enum
{
    EVENT_NONE,   // no new event 
    NETWORK_CONNECTED,
    NETWORK_DISCONNECTED,
    BAD_STATE_TOO_LONG,   // maybe need to restart
    OPERATE_FAIL,
    EVENT_NUM
}fsm_event_t;

// one state one handler
typedef void (*state_handler_func)(fsm_event_t event);

typedef struct
{
    fsm_state_t state;           // current state
    state_handler_func handler;  // function for current state
    fsm_state_t next_state[EVENT_NUM]; // transist to next state after handler
}fsm_transist_rule_t;


void fsm_init_state_handler(fsm_event_t event)
{
    printf("current state: STATE_INIT\n");
}

void fsm_work_state_handler(fsm_event_t event)
{
    printf("current state: STATE_WORK\n");
}


void fsm_nw_exception_state_handler(fsm_event_t event)
{
    printf("current state: STATE_NETWORK_EXCEPTION\n");
    // network bad
}


fsm_transist_rule_t fsm_state_table[] =
{
{STATE_INIT, fsm_init_state_handler,
         {[NETWORK_CONNECTED] = STATE_WORK,
          [NETWORK_DISCONNECTED] = STATE_INIT} },  //one transist
{STATE_WORK, fsm_work_state_handler,
         {[NETWORK_CONNECTED] = STATE_WORK,
          [NETWORK_DISCONNECTED] = STATE_NETWORK_EXCEPTION} },  //one transist

{STATE_NETWORK_EXCEPTION, fsm_nw_exception_state_handler,
         {[NETWORK_CONNECTED] = STATE_WORK} },  //one transist

};



int main()
{
    fsm_state_t state = STATE_INIT;
    fsm_state_t prev_state = STATE_INIT; // previous state
    fsm_event_t prev_evt = EVENT_NONE;
    while(1)
    {
        
        char buf[128] = {0};
        fsm_event_t evt; // get event
        scanf("%s", buf);
        printf("you have input %s\n", buf);
        if(strcmp(buf, "con") == 0)
        {
            evt = NETWORK_CONNECTED;
        }
        else if(strcmp(buf, "dis") == 0)
        {
            evt = NETWORK_DISCONNECTED;
        }
        else
        {
            evt = EVENT_NONE;   
        }

        printf("prev state %d state %d\n", prev_state, state);
        // if state and event unchange then continue
        if(state == prev_state && evt == prev_evt)
        {
            printf("state and input event both unchange\n");
            continue;
        }
        
        fsm_state_table[state].handler(evt);
        prev_state = state;
        state = fsm_state_table[state].next_state[evt];
        
        prev_evt = evt;
        printf("after event %d, state change to %d\n", evt, state);
    }
       
}

