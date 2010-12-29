#include "include/quemod.h"

int return_packet_type(char *packet)
{
    struct ip *iphdr = (struct ip *) packet;
    enum protocol p = iphdr->ip_p;

    return p;
}
