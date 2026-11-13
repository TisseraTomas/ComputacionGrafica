#include <memory>
#include <escenas.h>
#include <plane.h>
#include <sphere.h>
#include <checker.h>
#include <mirror.h>
#include <triangle.h>

void build_scene_03(Scene& scene, Camera& camera)
{
    camera.look_at(glm::vec3(-2.0f, 2.0f, 7.5f), // ojo
                    glm::vec3(3.0f, 1.4f, 0.0f), // hacia donde mira
                    glm::vec3(0.0f, 1.0f, 0.0f), // arriba
                    45.0f); // fov vertical

    scene.set_background(glm::vec3(0.02f, 0.03f, 0.08f));

    auto floor = std::make_shared<Checker>();
    floor->size = 1.0f;
    floor->ka = glm::vec3(0.07f);
    floor->kd = glm::vec3(0.7f);
    floor->ka2 = glm::vec3(0.01f);
    floor->kd2 = glm::vec3(0.1f);
    scene.add_object(std::make_shared<Plane>(glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f), floor));
    
    // Definimos el material de las paredes
    // Nota: A diferencia de la escena 2, este espejo tiene un leve componente difuso (kd).
    auto wall_mirror = std::make_shared<Mirror>();
    wall_mirror->ka = glm::vec3(0.0f, 0.0f, 0.0f);
    wall_mirror->kd = glm::vec3(0.02f, 0.02f, 0.02f);
    wall_mirror->ks = glm::vec3(0.0f, 0.0f, 0.0f);
    wall_mirror->kr = 0.9f;

    // 2. Pared Izquierda (x = -3)
    glm::vec3 la(-3.0f, 0.0f, 14.0f);
    glm::vec3 lb(-3.0f, 0.0f, -12.0f);
    glm::vec3 lc(-3.0f, 5.0f, -12.0f);
    glm::vec3 ld(-3.0f, 5.0f, 14.0f);

    scene.add_object(std::make_shared<Triangle>(la, lb, lc, wall_mirror));
    scene.add_object(std::make_shared<Triangle>(la, lc, ld, wall_mirror));

    // 3. Pared Derecha (x = 3)
    glm::vec3 ra(3.0f, 0.0f, 14.0f);
    glm::vec3 rb(3.0f, 0.0f, -12.0f);
    glm::vec3 rc(3.0f, 5.0f, -12.0f);
    glm::vec3 rd(3.0f, 5.0f, 14.0f);

    scene.add_object(std::make_shared<Triangle>(ra, rb, rc, wall_mirror));
    scene.add_object(std::make_shared<Triangle>(ra, rc, rd, wall_mirror));

    auto red = std::make_shared<Diffuse>();
    red->ka = glm::vec3(0.8f, 0.01f, 0.01f);
    red->kd = glm::vec3(0.75f, 0.08f, 0.06f);
    red->ks = glm::vec3(0.4f);
    red->shininess = 60.0f;
    scene.add_object(std::make_shared<Sphere>(glm::vec3(0.2f, 1.0f, 0.0f), 1.0f, red));

    auto green = std::make_shared<Diffuse>();
    green->ka = glm::vec3(0.01f, 0.06f, 0.02f);
    green->kd = glm::vec3(0.1f, 0.6f, 0.2f);
    green->ks = glm::vec3(0.4f);
    green->shininess = 40.0f;
    scene.add_object(std::make_shared<Sphere>(glm::vec3(1.2f, 0.6f, 2.5f), 0.6f, green));
    
    auto mirror = std::make_shared<Mirror>();
    mirror->ka = glm::vec3(0.02f);
    mirror->kd = glm::vec3(0.01f);
    mirror->ks = glm::vec3(1.0f);
    mirror->shininess = 150.0f;
    mirror->kr = 0.85f;
    scene.add_object(std::make_shared<Sphere>(glm::vec3(-1.3f, 0.9f, -2.5f), 0.9f, mirror));


    const glm::vec3 att(1.0f, 0.12f, 0.02f);
    scene.add_light(Light(glm::vec3(0.0f, 9.0f, 5.0f), glm::vec3(4.0f), att));
    
    scene.set_max_depth(8U);
}