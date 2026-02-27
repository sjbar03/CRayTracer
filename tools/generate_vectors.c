#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "../lib/Vec3.h"
#include "../lib/Fix.h"
#include <assert.h>

int main(int argc, char **argv)
{
    int N = 10000;
    const char *out = "vectors.csv";
    if (argc > 1) N = atoi(argv[1]);
    if (argc > 2) out = argv[2];

    srand((unsigned)time(NULL));

    FILE *f = fopen(out, "w");
    if (!f) { perror("fopen"); return 1; }

    // pick a single starting normal
    Vec3 n;
    do {
        n.x = randomFix(int2fix(-128), int2fix(128)); // 128 is safe to avoid overflow when normalizing
        n.y = randomFix(int2fix(-128), int2fix(128));
        n.z = randomFix(int2fix(-128), int2fix(128));
    } while ((n.x == 0 && n.y == 0 && n.z == 0) || dot(&n, &n) < float2fix(0.0005));

    printf("n: %f, %f, %f \n", fix2float(n.x), fix2float(n.y), fix2float(n.z));
    normalize(&n);
    printf("n: %f, %f, %f \n", fix2float(n.x), fix2float(n.y), fix2float(n.z));

    // write normal first (header + value), then samples
    fprintf(f, "normal_x,normal_y,normal_z\n");
    fprintf(f, "%f,%f,%f\n\n", fix2float(n.x), fix2float(n.y), fix2float(n.z));
    fprintf(f, "x,y,z\n");

    for (int i = 0; i < N; i++) {
        Vec3 r;
        randomSurf(&r, &n);
        fprintf(f, "%f,%f,%f\n", fix2float(r.x), fix2float(r.y), fix2float(r.z));
    }

    fclose(f);
    printf("Wrote %d vectors to %s\n", N, out);
    return 0;
}
