#include <memory>
#include <escenas.h>
#include <plane.h>
#include <sphere.h>
#include <checker.h>
#include <mirror.h>
#include <triangle.h>
#include <dielectric.h>

// ============================================================================
// ESCENA 4: Esferas de vidrio y de agua
// Concepto: Refracción (lentes convergentes) y sombras atenuadas por transparencia.
// ============================================================================
void build_scene_04(Scene& scene, Camera& camera) {
    camera.look_at(glm::vec3(0.0f, 2.4f, 9.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), 40.0f);
    // Fondo degradado vertical: celeste claro en el horizonte, azul profundo en el cenit
    scene.set_background(glm::vec3(0.55f, 0.65f, 0.8f), glm::vec3(0.05f, 0.12f, 0.35f));

    // Piso cuadriculado blanco y azul
    auto floor = std::make_shared<Checker>();
    floor->size = 1.0f; floor->kd = glm::vec3(0.75f); floor->ka = glm::vec3(0.12f);
    floor->kd2 = glm::vec3(0.05f, 0.15f, 0.45f); floor->ka2 = glm::vec3(0.01f, 0.03f, 0.08f);
    scene.add_object(std::make_shared<Plane>(glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f), floor));

    // Esfera maciza de vidrio (ior=1.5). Deja pasar 0.8 de luz por superficie (0.8^2 = 0.64 total)
    auto glass = std::make_shared<Dielectric>();
    glass->ior = 1.5f; glass->transparency = 0.8f; glass->ks = glm::vec3(1.0f); glass->shininess = 100.0f;
    scene.add_object(std::make_shared<Sphere>(glm::vec3(-1.3f, 1.2f, 2.5f), 1.2f, glass));

    // Esfera maciza de agua (ior=1.33). Deja pasar 0.9 por superficie (0.9^2 = 0.81 total, sombra más clara)
    auto water = std::make_shared<Dielectric>();
    water->ior = 1.33f; water->transparency = 0.9f; water->ks = glm::vec3(1.0f); water->shininess = 100.0f;
    scene.add_object(std::make_shared<Sphere>(glm::vec3(1.7f, 0.8f, 3.0f), 0.8f, water));

    // Tres esferas opacas ubicadas detrás para verse invertidas a través de las lentes
    auto red = std::make_shared<Diffuse>(); red->ka = glm::vec3(0.075f, 0.01f, 0.008f); red->kd = glm::vec3(0.75f, 0.1f, 0.08f); red->ks = glm::vec3(0.4f); red->shininess = 50.0f;
    scene.add_object(std::make_shared<Sphere>(glm::vec3(-2.6f, 0.8f, 1.5f), 0.8f, red));

    auto yellow = std::make_shared<Diffuse>(); yellow->ka = glm::vec3(0.085f, 0.06f, 0.005f); yellow->kd = glm::vec3(0.85f, 0.6f, 0.05f); yellow->ks = glm::vec3(0.4f); yellow->shininess = 50.0f;
    scene.add_object(std::make_shared<Sphere>(glm::vec3(-0.5f, 0.8f, -2.0f), 0.8f, yellow));

    auto green = std::make_shared<Diffuse>(); green->ka = glm::vec3(0.01f, 0.055f, 0.02f); green->kd = glm::vec3(0.1f, 0.55f, 0.2f); green->ks = glm::vec3(0.4f); green->shininess = 50.0f;
    scene.add_object(std::make_shared<Sphere>(glm::vec3(1.8f, 0.8f, -1.0f), 0.8f, green));

    const glm::vec3 att(1.0f, 0.12f, 0.02f);
    // Dos luces para proyectar sombras dobles
    scene.add_light(Light(glm::vec3(-5.0f, 9.0f, 7.0f), glm::vec3(3.0f), att));
    scene.add_light(Light(glm::vec3(6.0f, 7.0f, 5.0f), glm::vec3(2.0f), att));
    scene.set_max_depth(8U);
}