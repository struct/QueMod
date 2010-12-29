#define MAX(a, b) (a > b ? a : b)

struct vector_ {
    unsigned short *start;
    unsigned short *finish;
    unsigned short *end;
    size_t size;
    size_t element_sz;
};

typedef struct vector_ vector_t;

vector_t *vector_new(size_t es, size_t h);
int vector_resize(vector_t *v);
void vector_push_back(vector_t *v, void *e);
void *vector_get(vector_t *v, size_t offset);
void *vector_back(vector_t *v);
void vector_release(vector_t **v);
size_t vector_count(vector_t *v);
