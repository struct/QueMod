#include "include/quemod.h"

int setup_nfq()
{
    int fd, rv;
    char pkt[MAX_PKTSIZE];

    nfqh = nfq_open();

    if(!nfqh)
    {
        error("nfq_open() Failed");
        return ERR;
    }

    if(nfq_unbind_pf(nfqh, AF_INET) == ERR)
    {
        error("nfq_unbind_pf() Failed");
        return ERR;
    }

    if(nfq_bind_pf(nfqh, AF_INET) == ERR)
    {
        error("nfq_bind_pf() Failed");
        return ERR;
    }

    if((qh = nfq_create_queue(nfqh, 0, &callback, NULL)) == NULL)
    {
        error("nfq_create_queue() Failed");
        return ERR;
    }

    if(nfq_set_mode(qh, NFQNL_COPY_PACKET, MAX_PKTSIZE) < 0)
    {
        error("nfq_set_mode() Failed");
        return ERR;
    }

    nh = nfq_nfnlh(nfqh);
    fd = nfnl_fd(nh);

    while((rv = recv(fd, pkt, sizeof(pkt), 0)) && rv >= 0)
    {
        nfq_handle_packet(nfqh, pkt, rv);
        memset(pkt, 0x0, sizeof(pkt));
    }

    nfq_destroy_queue(qh);
    nfq_close(nfqh);

return OK;
}

int callback(struct nfq_q_handle *qh, struct nfgenmsg *nfmsg, struct nfq_data *nfa, void *data)
{
    if(data != NULL)
        error("you should never see this");

    struct nfqnl_msg_packet_hdr *phdr;
    char *full_packet;
    int len = 0;

    phdr = nfq_get_msg_packet_hdr(nfa);

    len = nfq_get_payload(nfa, &full_packet);

    if(options.ip_version == IPV4)
    {
        if(len <= 0 || len <= sizeof(struct ip))
            goto done;
    }
    else if(options.ip_version == IPV6)
    {
        if(len <= 0 || len <= sizeof(struct ip6_hdr))
            goto done;
    }

    enum protocol proto = return_packet_type(full_packet);

    state_t *state = (state_t *) x_malloc(sizeof(state_t));
    state->original_packet = full_packet;
    state->packet_size = len;
    state->packet = x_malloc(len);
    state->proto = proto;
    memcpy(state->packet, state->original_packet, len);

    handout_packet(state);

    process_stats(state);

    struct ip *iphdr = (struct ip *) full_packet;

    if(state->checksum_flag)
    {
        if(proto == TCP)
            TcpChecksum(iphdr);

        if(proto == UDP)
            UdpChecksum(iphdr);

        IpChecksum(iphdr);
    }

done:

    if(state->drop == YES)
    {
        nfq_set_verdict(qh, ntohl(phdr->packet_id), NF_DROP, len, (unsigned char *)full_packet);
    }
    else
    {
        nfq_set_verdict(qh, ntohl(phdr->packet_id), NF_ACCEPT, len, (unsigned char *)full_packet);

        if(options.write_packets == YES)
        {
            write_packets(state);
        }
    }

    SAFE_FREE(state->packet);
    SAFE_FREE(state);

    return OK;
}
