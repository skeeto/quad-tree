#ifndef VIZ_H
#define VIZ_H

#include "quad.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> // usleep

#define NOBJECTS 3000

static FILE* viz_plot_pipe;
static size_t viz_nrects;
static rect_t viz_rects[NOBJECTS];
static point_t viz_points[NOBJECTS];
static size_t irect;
static size_t ipoint;

void viz_init(unsigned width, unsigned height);
typedef struct args_1i_t {
    int i;
} args_1i_t;
#define viz_flush(...) _viz_flush((args_1i_t){__VA_ARGS__})
//void viz_flush();
void _viz_flush(args_1i_t);
void viz_write_rect(rect_t* rect);
void viz_write_point(point_t* point);
void viz_close();

#endif // VIZ_H

