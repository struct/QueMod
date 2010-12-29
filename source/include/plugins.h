#include "vectors.h"

#define PLUGIN_VECTOR_SIZE 10
#define MAX_PLUGIN_PATH 512
#define PARENT_DIRECTORY ".."
#define CURRENT_DIRECTORY "."

typedef void PLUGIN_VOID;
typedef _CALLBACK PLUGIN_CALLBACK;

typedef void(*Init)();

struct plug_data
{
    char *ver;
    char *information;
    void *shared_obj;
    Init init;
} plug_data;

int init_plugins();
void *loadplugins(struct plug_data *plug, char *so);
void cleanup_plugins();

/* Plugin function pointer prototypes and vectors */
typedef void(*QueMod_Cleanup) ();
vector_t *vec_cleanup;

typedef void(*QueMod_GetPacket) (state_t *state);
vector_t *vec_get_packet;

typedef void(*QueMod_GetStats) (struct _stats *stats);
vector_t *vec_get_stats;

#define SAVE_PLUGIN_FUNC_PTR(vec, func_name, loc)               \
    if(vec == NULL)                                             \
    {                                                           \
        vec = vector_new(sizeof(void *), PLUGIN_VECTOR_SIZE);   \
    }                                                           \
    loc = dlsym(plug.shared_obj, func_name);                    \
    vector_push_back(vec, &loc);
