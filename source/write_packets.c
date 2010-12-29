#include "include/quemod.h"

void *write_packets(state_t *s)
{
    char *ptr = s->packet;
    size_t size = s->packet_size;
    char fnb[32];
    FILE *f;

    if(options.write_packet_hdrs == NO)
    {
        enum protocol p = return_packet_type(s->packet);

        /* TODO need to define the rest of these here */
        if(p == TCP)
        {
            ptr += sizeof(struct ip) + sizeof(struct tcphdr);
            size -= sizeof(struct ip) + sizeof(struct tcphdr); 
        }

        if(p == UDP)
        {
            ptr += sizeof(struct ip) + sizeof(struct udphdr);
            size -= sizeof(struct ip) + sizeof(struct udphdr);
        }
    }

    snprintf(fnb, sizeof(fnb)-1, "%d.bin", stats.packet_count);

    if((f = fopen(fnb, "w+")) == NULL)
    {
        error("Failed to open packet file for writing!");
        return 0;
    }

    if((fwrite(ptr, 1, size, f)) != size)
    {
        error("Did not write out full packet!");
    }

#ifdef DEBUG
    fprintf(stdout, "Wrote out packet %d\n", stats.packet_count);
#endif

    fclose(f);

    return OK;
}
