// TODO: need IPV6 support here
#define __USE_BSD
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/ip6.h>
#define __FAVOR_BSD
#include <netinet/tcp.h>
#include <netinet/udp.h>
#include <arpa/inet.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>

#include <linux/netfilter.h>
#include <libnetfilter_queue/libnetfilter_queue.h>

#include <dlfcn.h>
#include <dirent.h>
#include <sys/types.h>

#define VERSION_MAJOR 0
#define VERSION_MINOR 1

#define ERR -1
#define OK 0

#define YES 1
#define NO 0

#define ON 1
#define OFF 0

#define IPV4 4
#define IPV6 6

typedef void(_CALLBACK) (void);

#define MAX_PKTSIZE 65535

#define SAFE_FREE(ptr)  if (ptr != NULL) { free(ptr); ptr = NULL; }

#define ADD_NULL_BYTE(string)           \
        string[sizeof(string)-1] = '\0';

struct nfq_handle *nfqh;
struct nfq_q_handle *qh;
struct nfnl_handle *nh;

// TODO: these need to be organized
void *x_malloc(unsigned int size);
int callback(struct nfq_q_handle *qh, struct nfgenmsg *nfmsg, struct nfq_data *nfa, void *data);
int parse_configuration_file(FILE *configuration_fd);
int setup_nfq();
void setup_signal_handlers();

void error(char *err);
void sig_handler(int signum) __attribute__((noreturn));
void help() __attribute__((noreturn));
size_t strlcpy(char *dst, const char *src, size_t siz);

void cleanup();
void free_plugin_vectors();

void IpChecksum(struct ip *ip);
void TcpChecksum(struct ip *ip);
void UdpChecksum(struct ip *ip);
int SumWords(u_int16_t *buf, int nwords);

    enum protocol {
        IP = 0,    /* Dummy protocol for TCP.               */
        HOPOPTS = 0,   /* IPv6 Hop-by-Hop options.              */
        ICMP = 1,      /* Internet Control Message Protocol.    */
        IGMP = 2,      /* Internet Group Management Protocol.   */
        IPIP = 4,      /* IPIP tunnels                          */
        TCP = 6,       /* Transmission Control Protocol.        */
        EGP = 8,       /* Exterior Gateway Protocol.            */
        PUP = 12,      /* PUP protocol.                         */
        UDP = 17,      /* User Datagram Protocol.               */
        IDP = 22,      /* XNS IDP protocol.                     */
        TP = 29,       /* SO Transport Protocol Class 4.        */
        DCCP = 33,     /* Datagram Congestion Control Protocol. */
        IPVSIX = 41,   /* IPv6 header.                          */
        ROUTING = 43,  /* IPv6 routing header.                  */
        FRAGMENT = 44, /* IPv6 fragmentation header.            */
        RSVP = 46,     /* Reservation Protocol.                 */
        GRE = 47,      /* General Routing Encapsulation.        */
        ESP = 50,      /* encapsulating security payload.       */
        AH = 51,       /* authentication header.                */
        ICMPVSIX = 58, /* ICMPv6.                               */
        NONE = 59,     /* IPv6 no next header.                  */
        DSTOPTS = 60,  /* IPv6 destination options.             */
        MTP = 92,      /* Multicast Transport Protocol.         */
        ENCAP = 98,    /* Encapsulation Header.                 */
        PIM = 103,     /* Protocol Independent Multicast.       */
        COMP = 108,    /* Compression Header Protocol.          */
        SCTP = 132,    /* Stream Control Transmission Protocol. */
        UDPLITE = 136, /* UDP-Lite protocol.                    */
        RAW = 255      /* Raw IP packets.                       */
    };

/* Global options */
struct {
    char *plugin_dir;       /* Plugin directory */
    char *load_plugins;     /* A list of plugins to load */
    int ip_version;         /* IPv4 or IPv6 */
    int write_packets;      /* Write packets to numbered files */
    int write_packet_hdrs;  /* Write packet headers */
} options;

/* This structure is passed to plugins. It describes
the state of the packet and other information */
typedef struct _state state_t;
struct _state {
    const char *original_packet;
    char *packet;
    uint32_t packet_size;
    int checksum_flag;
    int was_modified;
    int drop;
    enum protocol proto;
};

/* For stats */
struct _stats {
    uint32_t packet_count;
    uint32_t packets_modified;
} stats;

int process_stats(state_t *t);
void handout_packet(state_t *state);
void handle_stats(struct _stats *s);
void *write_packets(state_t *t);
int return_packet_type(char *packet);
char *get_string_conf(char *conf_str, char *buf);
int get_yn_conf(char *conf_str, char *buf, int opt_conf);
int get_int_conf(char *conf_str, char *buf, int opt_conf);
