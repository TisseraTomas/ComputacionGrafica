#include <memory>
#include <escenas.h>
#include <plane.h>
#include <sphere.h>
#include <checker.h>
#include <mirror.h>
#include <triangle.h>
#include <dielectric.h>

// ============================================================================
// ESCENA 8: Matrioshka de vidrio
// Concepto: Costo exponencial del árbol binario de rayos (2^(n+1) - 1).
// ============================================================================
void build_scene_08(Scene& scene, Camera& camera) {
    camera.look_at(glm::vec3(0.0f, 2.0f, 6.0f), glm::vec3(0.0f, 1.5f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), 45.0f);
    scene.set_background(glm::vec3(0.05f, 0.07f, 0.1f));

    auto floor = std::make_shared<Checker>();
    floor->size = 0.4f; floor->kd = glm::vec3(0.85f); floor->ka = glm::vec3(0.05f);
    floor->kd2 = glm::vec3(0.08f, 0.08f, 0.3f); floor->ka2 = glm::vec3(0.005f, 0.005f, 0.018f);
    scene.add_object(std::make_shared<Plane>(glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f), floor));

    // Tres cáscaras huecas concéntricas (6 superficies dieléctricas en total)
    glm::vec3 center(0.0f, 1.6f, 0.0f);
    float r_out[] = {1.5f, 1.0f, 0.6f};
    float r_in[] = {1.44f, 0.96f, 0.576f};

    for(int i=0; i<3; ++i) {
        // Superficie exterior de cada capa (aire a vidrio)
        auto d_out = std::make_shared<Dielectric>();
        d_out->ior = 1.5f; d_out->transparency = 0.9f; d_out->ks = glm::vec3(1.0f); d_out->shininess = 150.0f;
        scene.add_object(std::make_shared<Sphere>(center, r_out[i], d_out));

        // Superficie interior de cada capa (vidrio a aire, sin brillo propio)
        auto d_in = std::make_shared<Dielectric>();
        d_in->ior = 1.0f / 1.5f; d_in->transparency = 0.9f; d_in->ks = glm::vec3(0.0f); d_in->shininess = 150.0f;
        scene.add_object(std::make_shared<Sphere>(center, r_in[i], d_in));
    }

    // Esfera roja maciza en el centro (núcleo)
    auto core = std::make_shared<Diffuse>();
    core->ka = glm::vec3(0.05f, 0.0f, 0.0f); core->kd = glm::vec3(0.85f, 0.08f, 0.05f);
    core->ks = glm::vec3(0.5f); core->shininess = 40.0f;
    scene.add_object(std::make_shared<Sphere>(center, 0.3f, core));

    const glm::vec3 att(1.0f, 0.12f, 0.02f);
    
    scene.add_light(Light(glm::vec3(-3.0f, 6.0f, 4.0f), glm::vec3(2.8f), att));
    scene.add_light(Light(glm::vec3(4.0f, 4.0f, 3.0f), glm::vec3(1.0f), att));
    
    // Profundidad elevada a 12 para que los rayos puedan atravesar las 6 fronteras de ida y vuelta sin cortarse en negro
    scene.set_max_depth(12U); 
}