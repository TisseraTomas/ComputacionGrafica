# Observaciones y Preguntas - Práctico 10

## Escena 1: Luces y sombras de colores[cite: 4]

### Qué observar
Donde sólo se bloquea la luz roja, la sombra es cian (verde más azul); donde sólo se bloquea la verde, magenta; y donde sólo se bloquea la azul, amarilla (recordar Unidad III - Teoría del color)[cite: 4]. Fuera de las sombras el piso recibe las tres luces, pero cada una se atenúa con la distancia, por lo que su color tiende al de la luz más cercana[cite: 4]. Debajo de la esfera, donde se bloquean las tres, queda sólo el término ambiental[cite: 4].

Si en el bucle de luces el punto en sombra termina el cálculo (`break` en lugar de `continue`), la sombra de la primera luz de la lista sale negra, la de la segunda queda del color de la primera y la de la tercera no cambia[cite: 4].

### Preguntas
1. ¿De qué color es la zona donde se bloquean a la vez la luz roja y la verde?[cite: 4]

---

## Escena 2: Espejos paralelos[cite: 5]

### Qué observar
La cantidad de copias que se ven es igual a `max_depth`, y más allá de la última el espejo aporta negro, por lo que el piso reflejado termina en un borde recto[cite: 5]. Cada copia se ve más oscura que la anterior, con la caída dominada por $0,8^n$[cite: 5]. Si el trazador no multiplica por `kr` en la recursión, todas las copias tienen el mismo brillo[cite: 5].

### Preguntas
1. Con $kr = 0,8$, ¿a partir de qué rebote la contribución de una copia cae por debajo del 1 %? ¿Cuánto aporta todavía la última copia con `max_depth = 8`?[cite: 5]
2. ¿Qué ventaja tendría cortar la recursión cuando el peso acumulado por los rebotes es menor que un umbral en lugar de usar una profundidad fija?[cite: 5]

---

## Escena 3: Pasillo de espejos[cite: 6]

### Preguntas
1. Con `max_depth = 1`, ¿por qué la pared se ve casi negra donde el rayo reflejado llega a la pared opuesta?[cite: 6]
2. La esfera espejo refleja toda la escena. ¿Por qué necesita además un `ks` distinto de cero para que se vea el brillo de la luz?[cite: 6]
3. Si se invierte el orden de los vértices de los triángulos de una pared, ¿cambia la imagen?[cite: 6]

---

## Escena 4: Esferas de vidrio y de agua[cite: 7]

### Qué observar
Las esferas opacas y el tablero se ven invertidos a través de las esferas transparentes, que actúan como lentes convergentes[cite: 7]. Las sombras de las esferas transparentes son más claras que las de las opacas, porque el rayo de sombra pierde una fracción de la luz en cada superficie que cruza y sigue en línea recta hacia la luz[cite: 7].

### Preguntas
1. ¿Qué fracción de la luz deja pasar la esfera de vidrio a un rayo de sombra? ¿Y la de agua?[cite: 7]

---

## Escena 5: Banco de índices de refracción[cite: 8]

### Qué observar
La esfera de índice 1 no desvía los rayos, por lo que el tablero se ve a través de ella sin distorsión[cite: 8]. A medida que crece el índice, cada rayo se desvía más y el horizonte invertido se acerca al centro de la esfera[cite: 8]. En el diamante el reflejo de la superficie es más visible, porque la reflectancia con incidencia normal $F_0 = ((n-1)/(n+1))^2$ vale $0,17$, contra $0,04$ del vidrio y $0,02$ del agua[cite: 8].

Si al entrar a la esfera se usa `ior` en lugar de `1/ior`, la esfera de índice 1 no cambia, porque los dos valores son iguales[cite: 8]. Las otras tres, en cambio, pasan a verse como la burbuja de la Escena 6[cite: 8].

### Preguntas
1. Con índice 1 la física indica que no debería haber ninguna reflexión. ¿Por qué se ve un borde tenue en la esfera de aire? (Se sugiere revisar la aproximación de Schlick con $F_0 = 0$)[cite: 8]

---

## Escena 6: Esfera maciza, hueca y burbuja[cite: 9]

### Qué observar
A través de la esfera maciza el tablero se ve invertido, mientras que en la hueca casi no hay distorsión salvo cerca del borde de la cáscara[cite: 9]. A través de la burbuja, que actúa como lente divergente, el tablero se ve derecho y más chico[cite: 9]. Esa vista ocupa sólo un disco central, rodeado por un anillo exterior espejado donde la burbuja refleja el piso en la parte de abajo y el fondo en la de arriba[cite: 9].

Si la reflexión interna total se contempla sólo cuando el rayo sale del objeto, ese anillo se ve negro y grueso, y aparecen marcas negras en las otras dos esferas[cite: 9]. Esto se debe a que la dirección refractada queda indefinida (`NaN`) en los rayos que no pueden entrar a la burbuja[cite: 9].

### Preguntas
1. Una burbuja de aire en agua, ¿actúa como lente convergente o divergente? ¿Por qué?[cite: 9]

---

## Escena 7: La piscina[cite: 10]

### Qué observar
Al pie de la imagen la mirada llega al agua con unos $62^\circ$ de incidencia y la reflectancia es de un 6 %, por lo que se ven las baldosas y apenas un reflejo[cite: 10]. El reflejo de la esfera roja cae en una zona donde la reflectancia es de un 15 %, y por eso se ve tenue[cite: 10]. Los reflejos de las esferas lejanas caen donde la reflectancia ronda el 50 % y se ven con nitidez, con las baldosas todavía visibles[cite: 10]. Hacia el horizonte la reflectancia tiende a 1 y el agua pasa a verse del color del cielo[cite: 10].

La sombra oscura sobre el fondo, que en la imagen coincide con el reflejo de la esfera verde, es la de la esfera roja[cite: 10]. Las esferas lejanas también proyectan sombra, pero caen más atrás, donde la reflectancia supera el 80 %, por lo que casi no se distinguen[cite: 10].

Si se usa una reflectancia constante, los reflejos de las esferas lejanas se ven tan tenues como los de la cercana y las baldosas se siguen viendo hasta el horizonte[cite: 10].

### Preguntas
1. ¿Por qué el agua refleja más a lo lejos que cerca de la cámara?[cite: 10]
2. ¿Por qué en el fondo no se forman los patrones de luz brillante típicos de una piscina (cáusticas)? ¿Y por qué las sombras de las esferas sobre el fondo no se desplazan por la refracción?[cite: 10]

---

## Escena 8: Matrioshka de vidrio[cite: 11]

### Qué observar
Con 3 niveles el interior de la cáscara exterior se ve negro, porque los rayos se cortan antes de salir[cite: 11]. Con 6 aparece el núcleo y sigue negro el interior de la segunda cáscara[cite: 11]. Con 8 se aclara la segunda cáscara y sigue negro el interior de la tercera, alrededor del núcleo[cite: 11]. Recién con 12 desaparecen las zonas negras[cite: 11].

El tiempo de cómputo crece rápidamente con la profundidad, por lo que conviene hacer las pruebas intermedias a baja resolución (por ejemplo, `r 640 360 ss 1`)[cite: 11]. A $1280 \times 720$ y $3 \times 3$, en un procesador de cuatro núcleos, la escena tarda $1,6\text{ [s]}$ con 3 niveles, $6,6\text{ [s]}$ con 6, $19,5\text{ [s]}$ con 8 y $212\text{ [s]}$ con 12[cite: 11].

### Preguntas
1. Si cada intersección con un dieléctrico genera un rayo reflejado y uno refractado, ¿cuántos rayos se trazan por píxel, en el peor caso, con profundidad $n$, sin contar los rayos de sombra?[cite: 11]
2. ¿Por qué subir de 12 a 16 niveles casi no cambia la imagen y sí el tiempo de cómputo?[cite: 11]

# Respuestas Orientativas - Práctico 10

## Escena 1
1. Azul, porque es la única luz que llega a esa zona.

## Escena 2
1. $0,8^n < 0,01$ implica $n > \ln(0,01) / \ln(0,8) \approx 20,6$, es decir, a partir del rebote 21. Con `max_depth = 8`, la última copia todavía aporta el 17 % ($0,8^8$), y por eso el corte se ve como un final abrupto.
2. Cada rama se corta cuando ya no puede cambiar el color de forma visible (control adaptativo de la profundidad, Hall y Greenberg). Las superficies poco reflectantes cortan enseguida, con lo que se ahorran rayos, y los espejos casi perfectos siguen más lejos, con lo que se evita el corte abrupto.

## Escena 3
1. El rayo primario (profundidad 0) llega a la pared y genera el rayo reflejado (profundidad 1), que llega a la pared opuesta. El rayo que sale de ahí tiene profundidad 2, supera el máximo y no aporta luz, por lo que de la pared opuesta queda sólo su término local, con $kd = 0,02$.
2. Porque una luz puntual no es geometría de la escena, y por lo tanto el rayo reflejado nunca la encuentra. Ese brillo sólo puede venir del término de Phong.
3. La imagen no cambia, salvo diferencias de redondeo en unos pocos píxeles. La intersección da el mismo punto y la normal se orienta siempre hacia el lado del que viene el rayo, y un espejo no depende de cuál es ese lado.

## Escena 4
1. El rayo de sombra cruza dos superficies. La esfera de vidrio deja pasar $0,8^2 = 0,64$ y la de agua $0,9^2 = 0,81$.

## Escena 5
1. La aproximación de Schlick viene dada por $F = F_0 + (1 - F_0)(1 - \cos\theta)^5$. Con índice de refracción 1 resulta $F_0 = 0$, pero el segundo término no se anula para ángulos rasantes, y por eso el borde refleja un poco. Las ecuaciones de Fresnel devuelven 0 para todo ángulo, por lo que ese borde es un error de la aproximación y no un efecto físico.

## Escena 6
1. Divergente. La luz pasa de un medio más denso (agua) a uno menos denso (aire), por lo que se aleja de la normal al entrar y la superficie curva separa los rayos. Por eso la imagen se ve derecha y más chica, al contrario que en la esfera maciza.

## Escena 7
1. A lo lejos los rayos de cámara llegan al agua casi rasantes, y la reflectancia crece hacia 1 cuando el ángulo de incidencia se acerca a $90^\circ$. Cerca de la cámara el ángulo es menor y la reflectancia se acerca a $F_0 = 0,02$, aunque en esta escena no baja del 6 % porque la cámara no llega a mirar en dirección perpendicular al agua.
2. Una cáustica se forma cuando la luz se refracta en el agua y se concentra sobre una superficie difusa. En el trazador de Whitted la iluminación de una superficie difusa se calcula con un rayo de sombra, que va en línea recta hacia la luz, por lo que no hay forma de que la luz llegue refractada. Por la misma razón la sombra no se desplaza: el rayo de sombra atraviesa el agua sin refractarse y sólo se atenúa según `transparency`.

## Escena 8
1. El árbol es binario, por lo que son hasta $2^{n+1} - 1$ rayos por píxel. Con $n = 12$ son 8191, y con supermuestreo de $3 \times 3$ se multiplican por 9.
2. Porque cada nivel multiplica la contribución por un coeficiente menor que 1 (la reflectancia o la transmitancia), de modo que los niveles profundos aportan una fracción mínima del color. En cambio, la cantidad de rayos crece de manera exponencial con la profundidad.