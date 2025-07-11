# Informe Técnico del Videojuego: Batalla Naval

## 1. Obstáculos enfrentados y soluciones adoptadas

### a. Compatibilidad entre plataformas (Windows/Linux)
Se debió abordar la diferencia en el manejo de la consola y temporizadores entre Windows y Linux. Esto se resolvió con preprocesadores `#ifdef _WIN32` y `#elif defined(__linux__)`, encapsulando funciones como `bs_clear_console`, `bs_sleep` y `bs_enable_utf8`.

### b. Entrada de texto con `fgets`
El uso de `fgets` para leer nombres de jugadores generaba conflictos con `scanf`, debido a los caracteres de nueva línea. Esto se resolvió limpiando la entrada con `strcspn`.

### c. Colisiones y validación de posición de barcos
Fue necesario implementar una lógica robusta en `bs_validate_position()` para validar las posiciones de barcos considerando orientación, límites del tablero y colisiones con otras naves.

### d. Interfaz visual en consola
Diseñar una cuadrícula clara con bordes y símbolos (`X`, `O`, `*`) fue un reto para asegurar legibilidad multiplataforma. Se optó por una tabla con caracteres UTF-8 y ANSI, verificando previamente la compatibilidad con `bs_enable_utf8()`.

## 2. Estructuras de datos utilizadas
* **Matrices bidimensionales (BSGrid)**:  
  Se utilza una matriz de 10×10 para representar el tablero del jugador.  
  Cada celda es un BSCell, que almacena información del estado (agua, barco, golpeado, etc.) y el tipo de barco.  
  Permite acceso directo O(1) y es fácil de visualizar como un tablero.  

* **Estructuras tipo struct**:  
  `bs_cell_t`, `bs_ship_t`, `bs_player_data_t`, `bs_gamecontext_t`: todas estas estructuras encapsulan los datos del juego.  
  Facilitan la modularización y el paso de datos entre funciones.  
  Se utiliza punteros y alias (typedef) para simplificar el código. 

* **Arreglo de salud de barcos**:  
  Cada jugador tiene un `int health_ships[5]` que registra cuántas partes vivas tiene cada barco.  
  Esto permite detectar cuándo un barco fue destruido sin recorrer toda la grilla.

---

## 3. Enlaces de distribución

- 🔗 **Repositorio del código fuente**: [https://github.com/EnzoSolis/BattleshipConsole](https://github.com/EnzoSolis/BattleshipConsole)
- 📦 **Ejecutable (Windows)**: [https://drive.google.com/file/d/ejecutable_juego](https://drive.google.com/file/d/ejecutable_juego)

---

## 4. Librerías utilizadas y referencias consultadas

### 🧩 Librerías externas:
- **Estándar del lenguaje C**: `stdio.h`, `stdlib.h`, `time.h`
- **Windows API**: `windows.h`
- **POSIX**: `unistd.h` (para `usleep`)

### 📚 Sitios de referencia:
- [cliambrown.com](https://cliambrown.com/battleship/play.php)

### 🤖 Uso de IA:
Se utilizó inteligencia artificial (ChatGPT) en distintas etapas del desarrollo:
* Consulta de documentación sobre librerías estándar y del sistema (Windows/Linux).
* Búsqueda de ejemplos para manipulación de consola y representación de grids.
* Asistencia en la detección de errores de validación y portabilidad.
* Revisión técnica y generación del informe final.

---

## 4. Conclusiones y aprendizajes
- **Modularidad y estructuras**: Se reforzó el conocimiento del diseño modular con structs como `BSCell`, `BSShip`, `BSPlayerData` y `BSGameContex`.
- **Portabilidad**: Aprendimos a escribir código compatible entre Windows y Linux usando directivas de preprocesador.
- **Gestión de memoria y estructuras**: Aunque no se usaron asignaciones dinámicas, se aplicó el macro `BS_ZEROMEMORY` para inicializar estructuras.
- **Manejo de entradas**: Se mejoró el control de entrada/salida para asegurar estabilidad en el flujo del juego.
- **Trabajo con consola**: Ganamos experiencia representando visualmente estados del juego con ASCII art y secuencias ANSI.
