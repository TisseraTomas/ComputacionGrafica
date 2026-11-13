#include <memory>
#include <escenas.h>
#include <plane.h>
#include <sphere.h>
#include <checker.h>
#include <mirror.h>

void build_scene_02(Scene& scene, Camera& camera)
{
    camera.look_at(glm::vec3(2.0f, 1.6f, 2.5f), // ojo
                    glm::vec3(-6.0f, 0.7f, -1.5f), // hacia donde mira
                    glm::vec3(0.0f, 1.0f, 0.0f), // arriba
                    55.0f); // fov vertical

    scene.set_background(glm::vec3(0.02f, 0.02f, 0.03f));

    auto floor = std::make_shared<Checker>();
    floor->size = 0.5f;
    floor->ka = glm::vec3(0.05f);
    floor->kd = glm::vec3(0.85f);
    floor->ka2 = glm::vec3(0.006f);
    floor->kd2 = glm::vec3(0.1f);
    scene.add_object(std::make_shared<Plane>(glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f), floor));
    
    auto left_mirror = std::make_shared<Mirror>();
    left_mirror->ka = glm::vec3(0.0f);
    left_mirror->kd = glm::vec3(0.0f);
    left_mirror->ks = glm::vec3(0.0f);
    left_mirror->kr = 0.8f;
    scene.add_object(std::make_shared<Plane>(glm::vec3(-2.5f, 0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f), left_mirror));

    auto right_mirror = std::make_shared<Mirror>();
    right_mirror->ka = glm::vec3(0.0f);
    right_mirror->kd = glm::vec3(0.0f);
    right_mirror->ks = glm::vec3(0.0f);
    right_mirror->kr = 0.8f;
    scene.add_object(std::make_shared<Plane>(glm::vec3(2.5f, 0.0f, 0.0f), glm::vec3(-1.0f, 0.0f, 0.0f), right_mirror));

    auto orange = std::make_shared<Diffuse>();
    orange->ka = glm::vec3(0.05f, 0.02f, 0.0f);
    orange->kd = glm::vec3(0.9f, 0.45f, 0.05f);
    orange->ks = glm::vec3(0.5f);
    orange->shininess = 50.0f;
    scene.add_object(std::make_shared<Sphere>(glm::vec3(0.0f, 0.7f, -1.0f), 0.7f, orange));
    
    const glm::vec3 att(1.0f, 0.12f, 0.02f);
    scene.add_light(Light(glm::vec3(0.0f, 5.0f, 2.0f), glm::vec3(3.5f), att));
    
    scene.set_max_depth(8U);
}