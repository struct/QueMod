#include "include/quemod.h"
#include "include/plugins.h"

void help()
{
    fprintf(stdout, "QueMod Fuzzer %d.%d\n\n"
            "\t-h\tHelp\n"
            "\t-u\tComma separated list of plugins to load\n"
            "\t-p\tPlugin directory\n"
            "\t-w\tWrite out the packets to the current directory as numbered files\n"
            "\t-d\tWrite out packet headers (default is OFF)\n"
            "\t-v\tIP Version (4 or 6 (experimental))\n",
    VERSION_MAJOR, VERSION_MINOR);

    #ifdef DEBUG
        fprintf(stdout, "\n[!] QueMod Debug messages turned on\n");
    #endif

    fprintf(stdout, "\n");
sleep(10);
    exit(0);
}

void cleanup()
{
    /* Call the cleanup hooks in each plugin */
    cleanup_plugins();

    /* Release each global vector */
    free_plugin_vectors();
}

void *x_malloc(unsigned int size)
{
    void *ptr;

    if(size < 0)
    {
        error("x_malloc() cannot allocate a negative size");
        return NULL;
    }

    ptr = malloc(size);

    if(ptr != NULL)
    {
        memset(ptr, 0x0, size);
        return ptr;
    }

    if(ptr == NULL || ptr == 0)
    {
        error("Out Of Memory");
        return NULL;
    }

    error("x_malloc() died somehow");
    return NULL;
}

/* Ctrl+C will get you here */
void sig_handler(int signum)
{
    nfq_destroy_queue(qh);
    nfq_close(nfqh);

    cleanup();

    exit(ERR);
}

void error(char *str)
{
    fprintf(stderr, "%s\n", str);
}

void setup_signal_handlers()
{
#ifdef __linux__

    if(signal(SIGINT, sig_handler) == SIG_IGN)
    {
        signal(SIGINT, SIG_IGN);
    }

    if(signal(SIGHUP, sig_handler) == SIG_IGN)
    {
        signal(SIGINT, SIG_IGN);
    }

    if(signal(SIGTERM, sig_handler) == SIG_IGN)
    {
        signal(SIGINT, SIG_IGN);
    }

#endif
}

/* strlcpy/strlcat copied from OpenBSD - Licensed under BSD */
size_t strlcpy(char *dst, const char *src, size_t siz)
{

register char *d = dst;
register const char *s = src;
register size_t n = siz;

    /* Copy as many bytes as will fit */
    if (n != 0 && --n != 0)
    {
        do {
            if ((*d++ = *s++) == 0)
                break;
        } while (--n != 0);
    }

    /* Not enough room in dst, add NUL and traverse rest of src */
    if (n == 0) {
        if (siz != 0)
            *d = '\0';      /* NUL-terminate dst */
        while (*s++);
    }

    return(s - src - 1);    /* count does not include NUL */
}
