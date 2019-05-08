#version 330 core

in vec3 skybox_coords;
out vec4 color;
uniform samplerCube skybox;
uniform float time;

const int ocount = 16;
const float eps = 0.001;
const int max_marching_steps = 100;
const float max_marching_dist = 50;

const float speed = 15.0;
float offset = - speed * time;

struct Object {
    vec3 center;
    float radius;
};

Object scene_objects[ocount] = Object[ocount](
Object(vec3(1.0, 1.0, -21.0), 0.01),
Object(vec3(1.0, 0.0, -25.0), 0.01),
Object(vec3(1.0, -1.0, -27.0), 0.01),
Object(vec3(0.0, 1.0, -24.0), 0.01),
Object(vec3(0.0, -1.0, -38.0), 0.01),
Object(vec3(-1.0, 1.0, -33.0), 0.01),
Object(vec3(-1.0, 0.0, -36.0), 0.01),
Object(vec3(-1.0, -1.0, -32.0), 0.01),
Object(vec3(2.0, 2.0, -11.0), 0.01),
Object(vec3(2.0, 0.0, -15.0), 0.01),
Object(vec3(2.0, -2.0, -17.0), 0.01),
Object(vec3(0.0, 2.0, -14.0), 0.01),
Object(vec3(0.0, -2.0, -8.0), 0.01),
Object(vec3(-2.0, 2.0, -3.0), 0.01),
Object(vec3(-2.0, 0.0, -6.0), 0.01),
Object(vec3(-2.0, -2.0, -2.0), 0.01)
);

float sdScene(vec3 point) {
    float min_dist = max_marching_dist;
    float dist;
    for (int i = 0; i < ocount; ++i) {
        dist = length(scene_objects[i].center + vec3(0.0, 0.0, int(offset) % 50 + offset - int(offset)) - point) - scene_objects[i].radius;
        if (min_dist > dist) {
            min_dist = dist;
        }
    }
    return min_dist;
}

float RayMarchingToObject(vec3 eye, vec3 dir) {
    float cur_dist = 100 * eps;
    for (int i = 0; i < max_marching_steps; i++) {
        float left_dist = sdScene(eye + cur_dist * dir);
        if (left_dist < eps) {
            return cur_dist;
        }
        cur_dist += left_dist;
        if (cur_dist >= max_marching_dist) {
            return max_marching_dist;
        }
    }
    return max_marching_dist;
}

void main() {
    float dist = RayMarchingToObject(vec3(0.0, 0.0, 0.0), normalize(skybox_coords));
    if (dist > max_marching_dist - eps) {
        color = texture(skybox, skybox_coords);
    } else {
        color = vec4(0.8, 0.8, 1.0, 1.0);
    }
}