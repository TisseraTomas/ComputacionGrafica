#ifndef SPHERE_H
#define SPHERE_H

#include <object.h>

class Sphere : public Object {
public:
    Sphere(const glm::vec3& center, float radius, std::shared_ptr<Material> m) :
        Object(std::move(m)), c_(center), r_(radius) {}

    bool intersect(const Ray& r, Hit& out) const override {
        // TODO (paso 2): intersección rayo-esfera.
        // Un punto P está en la esfera si |P - c|^2 = r^2. Al reemplazar P por
        // el rayo, P(t) = o + t d: queda una cuadrática a t^2 + b t + cc = 0
        // (cc y no c, para no confundir el término independiente con el centro).
        //   - Discriminante negativo: el rayo no toca la esfera.
        //   - De las dos raíces, quedarse con la más chica que sea mayor que
        //     EPSILON. Si el origen del rayo está adentro de la esfera, la
        //     primera raíz es negativa y la segunda es la intersección que se
        //     ve desde dentro de la esfera.
        //   - Si ninguna es mayor que EPSILON, la esfera está detrás.
        //   - Completar out como en Plane. La normal hacia afuera es
        //     (P - c) normalizada.

        // 1. Vector desde el centro de la esfera al origen del rayo (O - C)
        glm::vec3 oc = r.o() - c_;

        // 2. Coeficientes de la ecuación cuadrática (a*t^2 + b*t + cc = 0)
        float a = glm::dot(r.d(), r.d());
        float b = 2.0f * glm::dot(oc, r.d());
        float cc = glm::dot(oc, oc) - r_ * r_;

        // 3. Calculamos el discriminante (b^2 - 4*a*c)
        float discriminante = b * b - 4.0f * a * cc;

        // Si el discriminante es negativo, el rayo pasa de largo y no toca la esfera
        if (discriminante < 0.0f) {
            return false;
        }

        // 4. Calculamos las raíces usando la fórmula resolvente
        float sqrt_d = std::sqrt(discriminante);
        float t1 = (-b - sqrt_d) / (2.0f * a); // La raíz "más cercana"
        float t2 = (-b + sqrt_d) / (2.0f * a); // La raíz "más lejana"

        // 5. Elegimos la intersección válida (la más chica que sea > EPSILON)
        float t = t1;
        if (t <= EPSILON) { // 1e-4f actúa como nuestro EPSILON
            t = t2;       // Si la primera no sirve, intentamos con la segunda
            if (t <= EPSILON) {
                return false; // Si ambas son <= EPSILON, la esfera está detrás
            }
        }

        // 6. El rayo impacta en la esfera. Guardamos los datos
        out.t = t;
        out.p = r.at(t);                             // Punto 3D del impacto (o + t*d)
        
        // La normal de una esfera es el vector que va del centro al punto de impacto
        glm::vec3 outward_normal = glm::normalize(out.p - c_); 
        
        out.mat = mat_.get();
        out.set_face(r, outward_normal);             // Orienta la normal respecto al rayo

        return true;
    }

private:
    glm::vec3 c_;
    float r_;
};

#endif // SPHERE_H
