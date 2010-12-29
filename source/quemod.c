#include "include/quemod.h"
#include "include/plugins.h"

int main(int argc, char *argv[])
{
    if(getuid() != 0 || geteuid() != 0)
    {
        fprintf(stderr, "\n[!] You need root to run this tool\n");
        /*return ERR; */
    }

#ifdef __linux__
    /* QueMod is Linux only anyway */
    setup_signal_handlers();
#endif

    int opt;
    int ret = 0;

    FILE *configuration_fd;
    char *conf_fd = "/opt/quemod/quemod.conf";

    fprintf(stdout,"\n[ QueMod Framework ]\n");

    if((configuration_fd = fopen(conf_fd, "r")) == NULL)
    {
        error("[!] Could not open the configuration file");
        return ERR;
    }

    if((parse_configuration_file(configuration_fd)) == 0)
    {
#ifdef DEBUG
        fprintf(stdout, "Parsed 0 lines in configuration file\n");
#endif
    }

    if(configuration_fd)
    {
        fclose(configuration_fd);
    }

    while((opt = getopt(argc, argv,"u:p:v:wd")) != ERR)
    {
        switch(opt)
        {
            case 'u': options.load_plugins = optarg;     break;
            case 'p': options.plugin_dir = optarg;       break;
            case 'v': options.ip_version = atol(optarg); break;
            case 'w': options.write_packets = YES;       break;
            case 'd': options.write_packet_hdrs = YES;  break;
            case '?': help();                            break;
        }
    }

    ret = init_plugins();

    if(ret == ERR)
        return ERR;

    memset(&stats, 0x0, sizeof(struct _stats));

    ret = setup_nfq();

    cleanup();

    printf("\n");

	return ret;
}
