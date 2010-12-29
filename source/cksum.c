#include "include/quemod.h"

/*  Various checksum routines taken from TCPDUMP and 
	http://www.bytebot.net/rrjcode/ratelimit-0.1/		*/
void IpChecksum(struct ip *ip)
{
  register u_int32_t	sum;

  ip->ip_sum = 0;
  sum = SumWords((u_int16_t *) ip, ip->ip_hl << 1);

  sum = (sum >> 16) + (sum & 0xFFFF);
  sum += (sum >> 16);
  sum = ~sum;

  ip->ip_sum = htons(sum);
}

int SumWords(u_int16_t *buf, int nwords)
{
  register u_int32_t	sum = 0;

  while (nwords >= 16)
  {
    sum += (u_int16_t) ntohs(*buf++);
    sum += (u_int16_t) ntohs(*buf++);
    sum += (u_int16_t) ntohs(*buf++);
    sum += (u_int16_t) ntohs(*buf++);
    sum += (u_int16_t) ntohs(*buf++);
    sum += (u_int16_t) ntohs(*buf++);
    sum += (u_int16_t) ntohs(*buf++);
    sum += (u_int16_t) ntohs(*buf++);
    sum += (u_int16_t) ntohs(*buf++);
    sum += (u_int16_t) ntohs(*buf++);
    sum += (u_int16_t) ntohs(*buf++);
    sum += (u_int16_t) ntohs(*buf++);
    sum += (u_int16_t) ntohs(*buf++);
    sum += (u_int16_t) ntohs(*buf++);
    sum += (u_int16_t) ntohs(*buf++);
    sum += (u_int16_t) ntohs(*buf++);
    nwords -= 16;
  }
  while (nwords--)
    sum += (u_int16_t) ntohs(*buf++);
  return(sum);
}

void UdpChecksum(struct ip *ip)
{
  struct udphdr	*const udp = (struct udphdr *) ((long *) ip + ip->ip_hl);
  u_int32_t	sum;

  udp->uh_sum = 0;

  sum = SumWords((u_int16_t *) &ip->ip_src, 4);
  sum += (u_int16_t) IPPROTO_UDP;
  sum += (u_int16_t) ntohs(udp->uh_ulen);

  sum += SumWords((u_int16_t *) udp, ((u_int16_t) ntohs(udp->uh_ulen)) >> 1);
  if (ntohs(udp->uh_ulen) & 1)
    sum += (u_int16_t) (((u_char *) udp)[ntohs(udp->uh_ulen) - 1] << 8);

  sum = (sum >> 16) + (sum & 0xFFFF);
  sum += (sum >> 16);
  sum = ~sum;

  udp->uh_sum = htons(sum);
}

void TcpChecksum(struct ip *ip)
{
  struct tcphdr	*const tcp = (struct tcphdr *) ((long *) ip + ip->ip_hl);
  u_int32_t	sum;
  int		tcp_len;

  tcp->th_sum = 0;

  tcp_len = (u_int16_t) ntohs(ip->ip_len) - (ip->ip_hl << 2);

  sum = SumWords((u_int16_t *) &ip->ip_src, 4);
  sum += (u_int16_t) IPPROTO_TCP;
  sum += (u_int16_t) tcp_len;

  sum += SumWords((u_int16_t *) tcp, tcp_len >> 1);

	if (tcp_len & 1)
	{
		sum += (u_int16_t) (((u_char *) tcp)[tcp_len - 1] << 8);
	}

  sum = (sum >> 16) + (sum & 0xFFFF);
  sum += (sum >> 16);
  sum = ~sum;

  tcp->th_sum = htons(sum);
}
