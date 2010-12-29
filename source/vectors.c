#include "include/quemod.h"
#include "include/vectors.h"

/* A partial bit vector implementation.
Not complete, but better than a linked list

This code is based on vector code by TQBF */

vector_t *vector_new(size_t es, size_t h)
{
    vector_t *v = malloc(sizeof(*v));
    h = MAX(h, (size_t)1);
    v->start = x_malloc(h * es);
    memset(v->start, 0x0, h * es);
    v->finish = v->start;
    v->end = v->start + (h * es);
    v->size = h;
    v->element_sz = es;

    return v;
}

int vector_resize(vector_t *v)
{
    size_t off = (size_t) (v->finish - v->start);
    v->size*=2;
    v->start = realloc(v->start, v->element_sz * v->size);

    if(v->start == NULL)
    {
        error("vector resize failed! OOM?!");
        return ERR;
    }

    v->finish = v->start + off;
    v->end = v->start + (v->element_sz * v->size);
    return OK;
}

void vector_push_back(vector_t *v, void *e)
{
    void *base = v->finish;
    v->finish += v->element_sz;

    if(v->finish == v->end)
    {
        vector_resize(v);
        base = v->finish - v->element_sz;
    }

    memcpy(base, e, v->element_sz);
}

void *vector_get(vector_t *v, size_t offset)
{
    if(v == NULL || v->start == NULL)
        return NULL;

    return (v->start + (v->element_sz * offset));
}

void *vector_back(vector_t *v)
{
    if(v->finish != v->start)
        return (v->finish - v->element_sz);
    else
        return NULL;
}

void vector_release(vector_t **v)
{
    vector_t *vt = *v;
    vt = *v;
    *v = NULL;
    if(vt)
    {
        SAFE_FREE(vt->start);
        SAFE_FREE(vt);
    }
}

size_t vector_count(vector_t *v)
{
    if(v != NULL)
        return ((v->finish - v->start) / v->element_sz);
    else
        return 0;
}
