#ifndef MIRROR_H
#define MIRROR_H

#include <material.h>

/// Espejo o metal opaco: Phong completo más un rayo reflejado de peso kr
/// constante.  I = I_local + kr * I(R)
/// El brillo de Phong sigue haciendo falta: una luz puntual no es geometría, el
/// rayo reflejado nunca la encuentra
class Mirror : public Material {
public:
    float kr = 0.8f;

    int secondary(const Ray& in, const Hit& h, Secondary out[2]) const override {
        // TODO (paso 8): rayo reflejado.
        // Escribir en out[0] el rayo reflejado y su peso, y devolver 1.
        //   - Dirección: la del rayo incidente (normalizada) reflejada
        //     respecto de la normal, con glm::reflect.
        //   - Origen: el punto de intersección corrido un poco hacia el lado
        //     de la normal, h.p + h.n * EPSILON, por la misma razón que en el
        //     rayo de sombra.
        //   - Peso: kr.
        
        // 1. Calculamos la dirección del rayo reflejado
        // Normalizamos la dirección incidente por seguridad matemática antes de reflejarla
        glm::vec3 dir_incidente = glm::normalize(in.d());
        glm::vec3 dir_reflejada = glm::reflect(dir_incidente, h.n);

        // 2. Corremos el origen del rayo levemente hacia afuera para evitar el "acné"
        glm::vec3 origen_corregido = h.p + h.n * EPSILON;

        // 3. Guardamos el rayo reflejado y su peso (reflectividad) en la estructura
        out[0].ray = Ray(origen_corregido, dir_reflejada);
        out[0].weight = kr;

        // 4. Devolvemos 1 indicando que este material generó exactamente 1 rayo secundario
        return 1;
    }
};

#endif // MIRROR_H
