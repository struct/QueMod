#include "include/quemod.h"

int process_stats(state_t *state)
{
    if(state->was_modified)
    {
        stats.packets_modified++;
    }

    stats.packet_count++;

    handle_stats(&stats);    

return OK;
}
