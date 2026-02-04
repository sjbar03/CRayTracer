#include "windows_display.h"
#include <pthread.h>
#include "cglm/cglm.h"

typedef struct {
    vec3 pos;
    vec3 target;
} Camera;

Camera camera;
vec3 worldUp = {0.0, 1.0, 0.0};
vec4 sphere = {0.0, 0.0, 0.0, 2.0};

void render_orthogonal()
{
    DEBUG_PRINTF("Render called\n");
    vec3 cam_dir;
    glm_vec3_sub(camera.target, camera.pos, cam_dir); 
    glm_vec3_normalize(cam_dir);
    
    vec3 cam_right;
    glm_vec3_cross(cam_dir, worldUp, cam_right);
    glm_vec3_normalize(cam_right);

    vec3 cam_up;
    glm_vec3_cross(cam_right, cam_dir, cam_up);

    float tmin, tmax;

    for (int x = 0; x < frame.width; x++) {
        for (int y = 0; y < frame.height; y++) {
            float vprop = ((float) y) / ((float) frame.height) - 0.5;
            float hprop = ((float) x) / ((float) frame.width)  - 0.5;

            vec3 off1, ray_origin;
            glm_vec3_scale(cam_up, vprop, off1);
            glm_vec3_scale(cam_right, hprop, ray_origin);
            glm_vec3_addadd(camera.pos, off1, ray_origin);

            if(glm_ray_sphere(ray_origin, cam_dir, sphere, &tmin, &tmax) && tmax > 0.0f)
            {
                frame.pixels[y * frame.width + x] = (uint16_t) 0xff00;    
            } else
            {
                frame.pixels[y * frame.width + x] = (uint16_t) 0x0000;
            }
            
        }
    }
}

void render(void)
{
    int w = frame.width, h = frame.height;
    if (!frame.pixels || w <= 0 || h <= 0) return;

    vec3 cam_dir;
    glm_vec3_sub(camera.target, camera.pos, cam_dir);
    if (glm_vec3_norm(cam_dir) < 1e-6f) return;
    glm_vec3_normalize(cam_dir);

    // robust up selection
    vec3 up_ref = {0,1,0};
    if (fabsf(glm_vec3_dot(cam_dir, up_ref)) > 0.999f) {
        glm_vec3_copy((vec3){1,0,0}, up_ref);
    }

    vec3 cam_right, cam_up;
    glm_vec3_cross(cam_dir, up_ref, cam_right);
    glm_vec3_normalize(cam_right);
    glm_vec3_cross(cam_right, cam_dir, cam_up);

    // camera lens params
    float fov = 60.0f * (3.1415926f / 180.0f);
    float aspect = (float)w / (float)h;
    float scale = tanf(fov * 0.5f);

    for (int y = 0; y < h; y++) {
        float ndc_y = ( (y + 0.5f) / (float)h ) * 2.0f - 1.0f;
        for (int x = 0; x < w; x++) {
            float ndc_x = ( (x + 0.5f) / (float)w ) * 2.0f - 1.0f;

            float px = ndc_x * aspect * scale;
            float py = -ndc_y * scale; // minus to make +y up on screen

            vec3 ray_dir, tmp;
            glm_vec3_scale(cam_right, px, ray_dir);
            glm_vec3_scale(cam_up,    py, tmp);
            glm_vec3_add(ray_dir, tmp, ray_dir);
            glm_vec3_add(ray_dir, cam_dir, ray_dir);
            glm_vec3_normalize(ray_dir);

            float tmin, tmax;
            int hit = glm_ray_sphere(camera.pos, ray_dir, sphere, &tmin, &tmax) && tmax > 0.0f;

            frame.pixels[y * w + x] = hit ? 0xFFFF : 0x0000;
        }
    }
}

vec3 offset = {0.0, 0.0, 0.01};

void * graphics(void *arg)
{
    (void) arg; // compiler silencing
    for(;;)
    {
        // static unsigned int p = 0;
        // // frame.pixels[(p++)%(frame.width*frame.height)] = rand();
        // // frame.pixels[rand()%(frame.width*frame.height)] = 0;
        // int n = frame.width * frame.height;
        // if (frame.pixels && n > 0) {
        //     frame.pixels[(p++) % n] = (uint16_t)rand();
        //     frame.pixels[rand() % n] = 0;
        // } 
        render();
        DEBUG_PRINTF("After render.");
        draw();   
        glm_vec3_add(camera.pos, offset, camera.pos );
        Sleep(16);
    } 


    return NULL;
}

void * window(void *arg)
{
    (void) arg; // compiler silencing
    window_init();
    return NULL;
}

int main()
{
    vec3 camera_pos = {0.0, 0.0, 5.0};
    vec3 camera_target = {0.0, 0.0, 0.0};
    glm_vec3_copy(camera_pos, camera.pos);
    glm_vec3_copy(camera_target, camera.target);
    pthread_t thread_graphics;
    pthread_t thread_window;

    pthread_create(&thread_window, NULL, &window, NULL);
    // Wait for the window thread to create the window before starting graphics
    while (frame.hwnd == NULL) {
        Sleep(1);
    }
    pthread_create(&thread_graphics, NULL, &graphics, NULL);

    pthread_join(thread_graphics, NULL);
    pthread_join(thread_window, NULL);
}