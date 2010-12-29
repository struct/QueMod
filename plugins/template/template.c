/* This is an QueMod plugin template. It is always
up to date with the current plugin API. You can copy
this entire directory, rename it and build your own
plugin from it */

#include <quemod/quemod.h>
#include <quemod/plugins.h>

char plug_info[] = "Template";
char plug_version[] = "1.0";
char plug_type[] = "Template";

/* Initialize the plugin. Called once at load time */
PLUGIN_VOID QueMod_init()
{
    printf("Plugin Function: %s\n", __func__);
    return;
}

/* Clean up before exit. Called once at exit */
PLUGIN_VOID QueMod_cleanup()
{
    printf("Plugin Function: %s\n", __func__);
    return;
}

/* Handle a single packet. Called each time a packet is received */
PLUGIN_VOID QueMod_get_packet(state_t *state)
{
    printf("Plugin Function: %s\n", __func__);
    return;
}

/* Manage any statistical data. Called after each packet is processed */
PLUGIN_VOID QueMod_get_stats(struct _stats *stats)
{
    printf("Plugin Function: %s\n", __func__);
    return;
}
