#ifndef PLANE_H
#define PLANE_H

#include <object.h>

/// Plano infinito, modelado como un punto y una normal
class Plane : public Object {
public:
    Plane(const glm::vec3& point, const glm::vec3& normal, std::shared_ptr<Material> m) :
        Object(std::move(m)), p0_(point), n_(glm::normalize(normal)) {}

    bool intersect(const Ray& r, Hit& out) const override {
        // TODO (paso 1): intersección rayo-plano.
        // Un punto P está en el plano si (P - p0) · n = 0. Reemplazar P por el
        // rayo, P(t) = o + t d, y despejar t.
        //   - Si d · n es (casi) cero, el rayo es paralelo al plano: no hay
        //     intersección (hacer comparación para punto flotante: valor
        //     absoluto con 1e-7 y no con 0.0).
        //   - Si t <= EPSILON, el plano está detrás del origen del rayo.
        //   - Si no, completar los datos en la intersección (Hit& out): t, el
        //     punto P(t) (ver Ray::at), el material (mat_.get()) y la normal
        //     con out.set_face(r, n_), que la orienta hacia el lado del que
        //     viene el rayo.

        //  1. Calculamos el denominador: d · n
        float denominador = glm::dot(r.d(), n_);

        // 2. Si es casi cero, el rayo es paralelo al plano (evitamos división por cero)
        if (std::abs(denominador) < 1e-7f) {
            return false;
        }

        // 3. Despejamos t. 
        // De (P - p0) · n = 0, reemplazando P por (o + t*d)
        // t = (p0 - o) · n / (d · n)
        float t = glm::dot(p0_ - r.o(), n_) / denominador;

        // 4. Verificamos que el choque no esté detrás de la cámara o sea un falso contacto
        // (Reemplaza 1e-4f por EPSILON si está definido en object.h o hit.h)
        if (t <= EPSILON) { 
            return false;
        }

        // 5. El rayo intercepta el plano. Llenamos la estructura Hit
        out.t = t;
        out.p = r.at(t);         // Calcula el punto exacto 3D usando el método del rayo
        out.mat = mat_.get();    // Obtenemos el puntero del material (heredado de Object)
        out.set_face(r, n_);     // Ajusta la normal para que siempre apunte hacia el rayo

        return true;
    }

private:
    glm::vec3 p0_;
    glm::vec3 n_;
};

#endif // PLANE_H
