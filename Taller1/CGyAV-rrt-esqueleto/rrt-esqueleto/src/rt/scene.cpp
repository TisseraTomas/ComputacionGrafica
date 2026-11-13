#include <scene.h>

bool Scene::find_nearest(const Ray& r, Hit& closest) const
{
    bool any = false;
    Hit current;

    for( const auto& obj : objects_ ){
        if( obj->intersect(r, current) && current.t < closest.t ){
            closest = current;
            any = true;
        }
    }
    return any;
}

glm::vec3 Scene::background(const Ray& r) const
{
    const float up = glm::max(glm::normalize(r.d()).y, 0.0f);
    return glm::mix(bg_horizon_, bg_zenith_, up);
}

glm::vec3 Scene::normal_color(const Ray& r) const
{
    // TODO (paso 3): normales como color.
    // Buscar la intersección más cercana (find_nearest); si no hay, devolver
    // el color del fondo. Si hay, ajustar el valor de la normal de [-1,1] a
    // [0,1] para cada componente y devolverla como color.
    // El resultado se puede ver con:  ./bin/rrt -n
    
    Hit closest;
    
    // 1. Buscamos el impacto más cercano en la escena
    if (!find_nearest(r, closest)) {
        // Si el rayo se pierde en el infinito, devolvemos el cielo
        return background(r);
    }

    // 2. Extraemos la normal del punto de impacto (valores entre -1 y 1)
    // 3. Mapeamos algebraicamente el rango [-1, 1] al rango de color [0, 1]
    glm::vec3 color_normal = (closest.n + glm::vec3(1.0f)) * 0.5f;
    
    return color_normal;
}

LocalTerms Scene::local_terms(const Ray& r, const Hit& h) const
{
    LocalTerms terms;
    const Material& m = *h.mat;

    const glm::vec3 v = glm::normalize(-r.d());     // hacia el observador
    const glm::vec3& n = h.n;                       // ya orientada hacia v

    // TODO (paso 4): modelo de iluminación de Phong.
    // Por defecto cada objeto sale de un color sólido tomado de la componente
    // difusa del material (kd).
    // Eliminar esta línea al escribir el paso 4:

    // 1. Término Ambiente: Luz base indirecta
    // Obtenemos el coeficiente ambiental en el punto de impacto (puede variar si hay textura)
    terms.ambient = m.ka_at(h.p);

    // Inicializamos difuso y especular en negro (0) para ir sumando la contribución de cada luz
    terms.diffuse = glm::vec3(0.0f);
    terms.specular = glm::vec3(0.0f);



    // TODO (paso 4) Ambiente: ka en el punto (ka_at, no ka: una textura la
    // puede hacer variar)
    // terms.ambient = ...

    // Iteramos sobre todas las fuentes de luz de la escena
    for( const auto& L : lights_ ){
        glm::vec3 s = L.pos() - h.p;                // Vector hacia la luz
        const float dist = glm::length(s);
        s /= dist; // Normalizamos el vector 's'

        // Sombra: ¿qué fracción de la luz llega hasta el punto? (paso 5)
        const float visibility = light_transmission(h.p, n, L.pos());
        if( visibility <= 0.0f ){
            continue;
        }

        // TODO (paso 4), por cada luz:
        //   - difuso:    kd_at(p) * max(n · s, 0)
        //   - especular: ks * max(R · v, 0)^shininess, con R la dirección de s
        //     reflejada respecto de n (glm::reflect(-s, n)). Si n · s <= 0 la
        //     luz está detrás de la superficie y no hay brillo.
        //   - los dos se multiplican por la intensidad que llega del foco:
        //     L.color() * L.attenuation(dist) * visibility
        //   - sumar en terms.diffuse y terms.specular.

        // Producto punto entre la normal y la luz (coseno del ángulo)
        float n_dot_s = glm::dot(n, s);

        // Si n_dot_s > 0, la luz está iluminando el frente de la superficie
        if (n_dot_s > 0.0f) {
            // Intensidad de la luz atenuada por la distancia y la visibilidad (sombras)
            glm::vec3 light_intensity = L.color() * L.attenuation(dist) * visibility;

            // 2. Término Difuso (Ley de Lambert)
            terms.diffuse += m.kd_at(h.p) * n_dot_s * light_intensity;

            // 3. Término Especular (Modelo de Phong para el brillo)
            glm::vec3 R = glm::reflect(-s, n);                 // Reflejo del rayo de luz
            float r_dot_v = glm::max(glm::dot(R, v), 0.0f);    // Alineación entre reflejo y cámara
            
            // Elevamos a la potencia de brillo (shininess)
            float spec_factor = std::pow(r_dot_v, m.shininess); 
            
            terms.specular += m.ks * spec_factor * light_intensity;
        }
    }

    return terms;
}

float Scene::light_transmission(const glm::vec3& p, const glm::vec3& n,
                                const glm::vec3& light_pos) const
{
    // 1. Vector dirección hacia la luz (s)
    glm::vec3 s = light_pos - p;
    float distancia_restante = glm::length(s);
    s /= distancia_restante; // Normalizamos

    // 2. Origen inicial corrido ligeramente para evitar auto-intersección
    glm::vec3 origen_actual = p + n * EPSILON;
    
    // 3. Variables de control para atravesar objetos transparentes
    float fraccion_luz = 1.0f;
    int cruces = 0;
    const int MAX_CRUCES = 16;

    // 4. Ciclo de trazado del rayo de sombra
    while (cruces < MAX_CRUCES && fraccion_luz > 0.0f) {
        Ray rayo_sombra(origen_actual, s);
        Hit impacto;

        // Si el rayo choca con algo ANTES de llegar a la luz
        if (find_nearest(rayo_sombra, impacto) && impacto.t < distancia_restante) {
            // Multiplicamos la luz por la transmitancia del material (0.0 en opacos, >0 en vidrios)
            fraccion_luz *= impacto.mat->shadow_transmission();

            // Actualizamos el origen para que el rayo siga de largo, atravesando el objeto.
            // Se corre un poco hacia adelante en la dirección de la luz.
            origen_actual = impacto.p + s * EPSILON;
            
            // Restamos la distancia que ya viajó el rayo
            distancia_restante -= impacto.t;
            cruces++;
        } else {
            // No hay más obstáculos hasta la luz
            break; 
        }
    }

    return fraccion_luz;
}

glm::vec3 Scene::trace(const Ray& r, unsigned int depth) const
{
    // 1. Condición de corte de la recursión
    // Si la profundidad actual supera el límite establecido, el rayo muere y no aporta luz (negro)
    if (depth > max_depth_) {
        return glm::vec3(0.0f);
    }

    Hit h;
    // Buscamos el impacto más cercano. Si no choca con nada, devolvemos el fondo.
    if( !find_nearest(r, h) ){
        return background(r);
    }
    
    // 2. Iluminación Local
    // La escena calcula los términos de Phong con sombras y el material decide cómo combinarlos.
    glm::vec3 color = h.mat->local(*this, r, h);

    // 3. Iluminación Global (Rayos Secundarios)
    // Preparamos un arreglo para guardar hasta 2 rayos secundarios (reflexión y refracción)
    Secondary sec[2]; 
    
    // Le pedimos al material que genere los rayos secundarios correspondientes
    int num_sec = h.mat->secondary(r, h, sec);

    // Iteramos sobre los rayos secundarios generados
    for (int i = 0; i < num_sec; ++i) {
        // Sumamos al color actual el color que trae el rayo secundario (llamada recursiva),
        // multiplicado por el "peso" (qué tanta luz refleja o refracta el material).
        color += sec[i].weight * trace(sec[i].ray, depth + 1);
    }

    return color;
}
