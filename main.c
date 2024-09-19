#include "quad.h"
#include "viz.h"
#include <limits.h>
#include <stdlib.h>
#include <time.h>
#define NPARTICLES 200 // make sure it's less than the visualiser's capacity

// assuming max > 0
#define ABS(x) (x)
#define CLAMP(x, max) ((x) < -ABS(max) ? -ABS(max) : ((x) > ABS(max) ? ABS(max) : (x)))

static size_t id_particle = 0;

typedef struct particle_t {
    float accelerationx, accelerationy;
    float velx, vely;
    point_t point;
} particle_t;

static particle_t particles[NPARTICLES];
static size_t ids = 0;
static float random_norm() {
    return (float) random() / RAND_MAX;
}
static const float accel = -6;
static const int niters = 600;
static const float dt = 0.1;

int main() {
    srand(time(NULL));
    rect_t boundary = {.x0=0, .y0=0, .x1=400, .y1=400};
    point_t points[NPARTICLES];
    particle_t particles[NPARTICLES];
    quadtree_t qtree;
    viz_init(boundary.x1, boundary.y1);
    for (int i = 0; i < NPARTICLES; ++i) {
        point_t p = {random() % boundary.x1, random() % boundary.y1, ids++};
        particles[i].point = p;
        particles[i].velx = (random_norm() > 0.66) ?  3 + random_norm()*8 : -3 - random_norm()*8;
        particles[i].vely = (random_norm() > 0.66) ? -3 - random_norm()*8 :  3 + random_norm()*8;
        particles[i].accelerationy = accel;
        particles[i].accelerationx = 0;
    }
    for (int i = 0; i < niters; ++i) {
        quadtree_t qtree;
        qtree_new(&qtree, &boundary);
        for (int ip = 0; ip < NPARTICLES; ++ip) {
            particles[ip].velx += particles[ip].accelerationx * dt;
            particles[ip].velx = CLAMP(particles[ip].velx, 20);
            particles[ip].vely += particles[ip].accelerationy * dt;
            particles[ip].vely = CLAMP(particles[ip].vely, 40);
            particles[ip].point.x += particles[ip].velx * dt;
            particles[ip].point.y += particles[ip].vely * dt;
            // reflection left and right, wrapping up and down
            particles[ip].point.x %= boundary.x1;
            particles[ip].point.y %= boundary.y1;
            if (particles[ip].point.x <= boundary.x0)
                particles[ip].point.x = boundary.x1;
            if (particles[ip].point.y <= boundary.y0)
                particles[ip].point.y = boundary.y1;
            point_t pnew = {particles[ip].point.x , particles[ip].point.y, particles[ip].point.id};
            qtree_insert(&qtree, &pnew);
        }
        qtree_graph(&qtree);
        viz_flush();
        qtree_del(&qtree);
    }
    sleep(1);
    viz_close();
}
