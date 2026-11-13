#include <dielectric.h>
#include <scene.h>

glm::vec3 Dielectric::local(const Scene& scene, const Ray& r, const Hit& h) const
{
    return h.front_face ? scene.local_terms(r, h).specular : glm::vec3(0.0f);
}

/// Aproximación de Schlick de la reflectancia de Fresnel
///     F(θ) = F0 + (1 - F0) (1 - cos θ)^5,   F0 = ((1 - η) / (1 + η))^2
///
/// cosine es el coseno del ángulo del lado del medio menos denso: el de
/// incidencia si el rayo entra a un medio más denso (η < 1), el de
/// transmisión si no
static float schlick(float cosine, float eta)
{
    float r0 = (1.0f - eta) / (1.0f + eta);
    r0 = r0 * r0;
    return r0 + (1.0f - r0) * glm::pow(1.0f - cosine, 5.0f);
}

int Dielectric::secondary(const Ray& in, const Hit& h, Secondary out[2]) const
{
    // TODO (paso 9): reflexión y refracción.
    //
    // a) η = η_i / η_t. Si el rayo entra al objeto (h.front_face) es 1 / ior;
    //    si sale, ior.
    //
    // b) Rayo reflejado, en out[0]: igual que en el espejo. Existe siempre.
    //
    // c) Snell: sin θt = η sin θi, de donde cos²θt = 1 - η² (1 - cos²θi),
    //    con cos θi = -d . n (d normalizada).
    //    Si cos²θt da negativo, no hay rayo refractado (reflexión interna
    //    total): toda la luz va al reflejado (peso 1) y se devuelve 1.
    //    No usar glm::refract para decidir esto: hace su propia cuenta, y
    //    cerca del ángulo límite el redondeo puede hacer que una prueba pase y
    //    la otra no. En ese caso glm::refract devuelve el vector nulo, al
    //    normalizarlo da NaN y aparecen píxeles negros sueltos sobre el vidrio.
    //
    // d) Rayo refractado, en out[1]:  T = η d + (η cos θi - cos θt) n,
    //    normalizado. El origen se corre hacia el otro lado de la superficie,
    //    h.p - h.n * EPSILON.
    //
    // e) Pesos: F = schlick(coseno, η) para el reflejado y 1 - F para el
    //    refractado. El coseno es cos θi si η <= 1 y cos θt si no (ver el
    //    comentario de schlick). Se devuelve 2.
    
    // a) Relación de índices de refracción (eta)
    // Si el rayo entra al objeto, pasamos de aire (1.0) a vidrio (ior). 
    // Si sale, pasamos de vidrio a aire.
    float eta = h.front_face ? (1.0f / ior) : ior;

    // Vector dirección incidente normalizado
    glm::vec3 d = glm::normalize(in.d()); 

    // b) Rayo reflejado (siempre existe en un vidrio)
    glm::vec3 reflected_dir = glm::reflect(d, h.n);

    glm::vec3 origin_out = h.p + h.n * EPSILON; // Corrimiento hacia afuera
    
    out[0].ray = Ray(origin_out, reflected_dir);

    // c) Ley de Snell para preparar la refracción
    float cos_theta_i = glm::dot(-d, h.n);
    float sin2_theta_i = 1.0f - cos_theta_i * cos_theta_i; // Identidad trigonométrica
    float cos2_theta_t = 1.0f - eta * eta * sin2_theta_i;

    // Reflexión Interna Total (TIR)
    if (cos2_theta_t < 0.0f) {
        // La luz no puede salir del material, rebota 100% por dentro
        out[0].weight = 1.0f;
        return 1;
    }

    // d) Rayo refractado (atraviesa el material)
    float cos_theta_t = std::sqrt(cos2_theta_t);
    glm::vec3 refracted_dir = eta * d + (eta * cos_theta_i - cos_theta_t) * h.n;
    refracted_dir = glm::normalize(refracted_dir);
    
    glm::vec3 origin_in = h.p - h.n * EPSILON; // Corrimiento hacia ADENTRO del objeto
    
    out[1].ray = Ray(origin_in, refracted_dir);

    // e) Pesos usando la aproximación de Fresnel-Schlick
    float cos_schlick = (eta <= 1.0f) ? cos_theta_i : cos_theta_t;
    float F = schlick(cos_schlick, eta);

    out[0].weight = F;            // Proporción de luz que se refleja
    out[1].weight = 1.0f - F;     // Proporción de luz que atraviesa (se refracta)

    return 2;
}
