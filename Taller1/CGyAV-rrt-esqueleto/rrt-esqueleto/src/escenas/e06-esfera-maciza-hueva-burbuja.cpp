#include <memory>
#include <escenas.h>
#include <plane.h>
#include <sphere.h>
#include <checker.h>
#include <mirror.h>
#include <triangle.h>
#include <dielectric.h>

// ============================================================================
// ESCENA 6: Maciza, hueca y burbuja
// Concepto: Índice relativo menor a 1 (n_aire / n_agua) y reflexión interna total.
// ============================================================================
void build_scene_06(Scene& scene, Camera& camera) {
    camera.look_at(glm::vec3(0.0f, 2.2f, 6.0f), glm::vec3(0.0f, 0.8f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), 45.0f);
    scene.set_background(glm::vec3(0.05f, 0.07f, 0.1f));

    auto floor = std::make_shared<Checker>();
    floor->size = 0.4f; floor->kd = glm::vec3(0.85f); floor->ka = glm::vec3(0.05f);
    floor->kd2 = glm::vec3(0.06f, 0.3f, 0.1f); floor->ka2 = glm::vec3(0.004f, 0.018f, 0.006f);
    scene.add_object(std::make_shared<Plane>(glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f), floor));

    // 1. Izquierda: Esfera maciza de vidrio (invierte la imagen como lente convergente)
    auto solid = std::make_shared<Dielectric>();
    solid->ior = 1.5f; solid->ks = glm::vec3(1.0f); solid->shininess = 150.0f; solid->transparency = 0.9f;
    scene.add_object(std::make_shared<Sphere>(glm::vec3(-2.0f, 0.9f, 0.0f), 0.8f, solid));

    // 2. Centro: Cáscara hueca (dos esferas concéntricas). No invierte la imagen
    auto hollow_out = std::make_shared<Dielectric>();
    hollow_out->ior = 1.5f; hollow_out->ks = glm::vec3(1.0f); hollow_out->shininess = 150.0f; hollow_out->transparency = 0.9f;
    scene.add_object(std::make_shared<Sphere>(glm::vec3(0.0f, 0.9f, 0.0f), 0.8f, hollow_out));

    // Cara interior: pasa de vidrio a aire (ior = 1/1.5) y ks=0 para evitar reflejos falsos
    auto hollow_in = std::make_shared<Dielectric>();
    hollow_in->ior = 1.0f / 1.5f; hollow_in->ks = glm::vec3(0.0f); hollow_in->shininess = 150.0f; hollow_in->transparency = 0.9f;
    scene.add_object(std::make_shared<Sphere>(glm::vec3(0.0f, 0.9f, 0.0f), 0.768f, hollow_in));

    // 3. Derecha: Burbuja de aire en agua (ior < 1). Actúa como lente divergente y genera un anillo de reflexión total
    auto bubble = std::make_shared<Dielectric>();
    bubble->ior = 1.0f / 1.33f; bubble->ks = glm::vec3(1.0f); bubble->shininess = 150.0f; bubble->transparency = 0.9f;
    scene.add_object(std::make_shared<Sphere>(glm::vec3(2.0f, 0.9f, 0.0f), 0.8f, bubble));

    const glm::vec3 att(1.0f, 0.12f, 0.02f);

    scene.add_light(Light(glm::vec3(-3.0f, 6.0f, 4.0f), glm::vec3(2.6f), att));
    scene.add_light(Light(glm::vec3(4.0f, 4.0f, 3.0f), glm::vec3(1.0f), att));
    scene.set_max_depth(8U);
}