#include "include/quemod.h"
#include "include/plugins.h"

int init_plugins()
{

DIR *dir;
struct dirent *plug_dirent;
struct plug_data plug;

char plugin_path[MAX_PLUGIN_PATH];

    if(options.plugin_dir != NULL)
    {
        dir = opendir(options.plugin_dir);
    }
    else
    {
        error("Failed to open plugins directory!");
        return ERR;
    }

    fprintf(stdout, "[ Loading QueMod Plugins ... ]\n");

    if(dir)
    {
        while((plug_dirent = readdir(dir)))
        {
            memset(plugin_path, 0x0, sizeof(plugin_path));

            if(!(strcmp(plug_dirent->d_name, PARENT_DIRECTORY)) || !(strcmp(plug_dirent->d_name, CURRENT_DIRECTORY)))
            {
                continue;
            }

            snprintf(plugin_path, sizeof(plugin_path)-1, "%s/%s", options.plugin_dir, plug_dirent->d_name);

            if(options.load_plugins != NULL)
            {
                if((strstr(options.load_plugins, plug_dirent->d_name)) == NULL)
                {
                    continue;
                }
            }

            QueMod_Cleanup QueMod_cleanup;
            QueMod_GetPacket QueMod_get_packet;
            QueMod_GetStats QueMod_get_stats;

            if(loadplugins(&plug, plugin_path))
            {
                (*plug.init)();

                SAVE_PLUGIN_FUNC_PTR(vec_cleanup, "QueMod_cleanup", QueMod_cleanup);
                SAVE_PLUGIN_FUNC_PTR(vec_get_packet, "QueMod_get_packet", QueMod_get_packet);
                SAVE_PLUGIN_FUNC_PTR(vec_get_stats, "QueMod_get_stats", QueMod_get_stats);
            }

            memset(&plug, 0x0, sizeof(struct plug_data));
        }

        closedir(dir);
    }
    else
    {
        error("Error opening plugins directory!");
        return ERR;
    }

    fprintf(stdout, "\n\n");

return OK;
}


void *loadplugins(struct plug_data *plug, char *so)
{
    plug->shared_obj = dlopen(so, RTLD_LAZY);

    if(plug->shared_obj)
    {
        /* All plugins MUST have an init routine, even if its empty */
        plug->init = dlsym(plug->shared_obj, "QueMod_init");

        /* This should be caught during plugin loading */
        if(plug->init == NULL)
        {
#ifdef DEBUG
    fprintf(stdout, "%s: Plugin [%s] has no init function\n", __func__, so);
#endif
            return NULL;
        }

        plug->information = dlsym(plug->shared_obj, "plug_info");
        plug->ver = dlsym(plug->shared_obj, "plug_version");

        if(plug->information && plug->ver)
        {
            fprintf(stdout, "\n-> %s  [Version: %s]\n", plug->information, plug->ver);
        }
    }
    else
    {
        fprintf(stdout, "\nError loading plugin [%s]\n", dlerror());
    }

    return plug->shared_obj;
}

/* Give each plugin the chance to modify the packet */
void handout_packet(state_t *state)
{
    size_t i, count = 0;
    QueMod_GetPacket *qgp;

    count = vector_count(vec_get_packet);

    if(count == 0)
        return;

    for(i=0;i<count;i++)
    {
        qgp = vector_get(vec_get_packet, i);

        if(qgp != NULL)
            (*qgp)(state);
    }
}

void cleanup_plugins()
{
    size_t i, count = 0;
    QueMod_Cleanup *cleanup;

    count = vector_count(vec_cleanup);

    if(count == 0)
        return;

    for(i=0;i<count;i++)
    {
        cleanup = vector_get(vec_cleanup, i);

        if(cleanup != NULL)
            (*cleanup)();
    }   

return;
}

void handle_stats(struct _stats *s)
{
    size_t i, count = 0;
    QueMod_GetStats *get_stats;

    count = vector_count(vec_cleanup);

    if(count == 0)
        return;

    for(i=0;i<count;i++)
    {
        get_stats = vector_get(vec_cleanup, i);

        if(get_stats != NULL)
            (*get_stats)(s);
    }   

return;
}

void free_plugin_vectors()
{
    vector_release(&vec_cleanup);
    vector_release(&vec_get_packet);
    vector_release(&vec_get_stats);
}
