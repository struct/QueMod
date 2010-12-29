#include "include/quemod.h"
#include "include/parse_config.h"

int parse_configuration_file(FILE *configuration_fd)
{

unsigned int filelines = 0;
char buf[MAX_CONFIGURATION_LINE_LEN];
char *index;

fseek(configuration_fd, 0, SEEK_SET);

memset(&buf, 0x0, sizeof(buf));

  while((fgets(buf, sizeof(buf)-1, configuration_fd)) != NULL)
  {
    index = buf;

    /* The following characters mean ignore the line #;\n */
    if((*index != '#') && (*index != 0x0a) && (*index != ';') && (!index != '\n') && (index != NULL))
    {
        filelines++;

        if(filelines >= MAX_CONFIGURATION_LINES)
        {
            return filelines;
        }
        else
        {
            ADD_NULL_BYTE(buf);
        }

        if(strlen(buf) > MAX_CONFIGURATION_LINE_LEN)
        {
            continue;
        }

        if(options.plugin_dir == NULL)
            options.plugin_dir = get_string_conf("plugin_dir", buf);
    
        get_yn_conf("write_packets", buf, options.write_packets);
        get_yn_conf("write_packet_hdrs", buf, options.write_packet_hdrs);

        get_int_conf("ip_version", buf, options.ip_version);

        if(options.ip_version != IPV4 || options.ip_version != IPV6)
        {
            options.ip_version = IPV4;
        }
    }

    memset(buf, 0x0, sizeof(buf));
  }

    return filelines;
}

char *get_string_conf(char *conf_str, char *buf)
{
    char *ptr1, *ptr2, *opt_conf;

    if((strncmp(buf, conf_str, strlen(conf_str))) == 0)
    {
        ptr1 = strchr(buf, '{');

        if(ptr1 == NULL)
            return NULL;

        ptr1++;

        ptr2 = strchr(buf, '}');

        if(ptr2 == NULL)
            return NULL;

        if((ptr2 - ptr1) > (MAX_CONFIGURATION_LINE_LEN - strlen(conf_str)))
            return NULL;

        opt_conf = (char *) x_malloc(ptr2 - ptr1 + 1);

        strlcpy(opt_conf, ptr1, ptr2 - ptr1 + 1);

#ifdef DEBUG
    fprintf(stdout, "Got string option: %s %s\n", conf_str, opt_conf);
#endif
    }

    return opt_conf;
}

int get_yn_conf(char *conf_str, char *buf, int opt_conf)
{
    char *ptr1, *ptr2;

    if((strncmp(buf, conf_str, strlen(conf_str))) == 0)
    {
        ptr1 = strchr(buf, '{');

        if(ptr1 == NULL)
            return ERR;

        ptr1++;

        ptr2 = strchr(buf, '}');
                                            
        if(ptr2 == NULL)
            return ERR;
                                            
        if((ptr2 - ptr1) > MAX_CONFIGURATION_LINE_LEN)
            return ERR;

        if((strncmp(ptr1, "no", 2)) == 0)
            opt_conf = NO;
        else
            opt_conf = YES;

#ifdef DEBUG
    fprintf(stdout, "Got yes/no option: %s %s\n", conf_str, opt_conf ? "YES" : "NO");
#endif
        return OK;
    }

    return ERR;
}

int get_int_conf(char *conf_str, char *buf, int opt_conf)
{
    char *ptr1, *ptr2;

    if((strncmp(buf, conf_str, strlen(conf_str))) == 0)
    {
        ptr1 = strchr(buf, '{');

        if(ptr1 == NULL)
            return ERR;

        ptr1++;

        ptr2 = strchr(buf, '}');

        if(ptr2 == NULL)
            return ERR;
                                            
        if((ptr2 - ptr1) > (MAX_CONFIGURATION_LINE_LEN - strlen(conf_str)))
            return ERR;

        opt_conf = atol(ptr1);

#ifdef DEBUG
    fprintf(stdout, "Got integer option: %s %d\n", conf_str, opt_conf);
#endif
    }

return ERR;
}
