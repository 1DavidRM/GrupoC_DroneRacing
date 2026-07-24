# GrupoC_DroneRacing

## Ingeniería Control

## Ingeniería Comunicaciones

## Ingeniería Eléctrica 

## Ingeniería Mecánica

### Ingeniería Inversa
En la parte de Ingeniería Mecánica se llevó a cabo un proceso de ingeniería inversa sobre un dron KE88 de la marca Kiwo, el cual fue adquirido para su análisis. Primero se realizó el desarme del equipo para identificar y extraer los componentes internos, lo que permitió comprender su estructura y funcionamiento. Posteriormente, dentro del trabajo asignado a la ingeniería mecánica, se modelaron en SolidWorks las distintas piezas del dron, desde las hélices hasta la carcasa, con el objetivo de recrear de forma precisa el diseño original. Este proceso permitió obtener un mejor conocimiento del sistema mecánico del dron, así como de la relación entre sus componentes y su funcionamiento general.
#### 1. Desarmado del drón:
<img src="Solid_IngInversa/Imagenes/DesarmadoTodo.jpeg" width="100" alt="Desarmado Todo">

#### 2. Medición de piezas:
<img src="Solid_IngInversa/Imagenes/MedicionHelices.jpeg" width="100" alt="MedicionHelices">

#### 3. Modelado de piezas:
<img src="Solid_IngInversa/Imagenes/ModeladoEnsambleHelices.png" width="200" alt="ModeladoEnsambleHelices">

### Modelado de Dron y Simulación Dinámica

Una vez concluida la fase de ingeniería inversa y teniendo el modelo base ensamblado en SolidWorks, el siguiente paso fue llevar el sistema físico a un entorno de simulación para analizar su dinámica y facilitar el futuro diseño del sistema de control. Para ello, realizamos una exportación a **MATLAB / Simulink** utilizando la herramienta **Simscape Multibody**.

El proceso de integración consistió en los siguientes pasos:

1. **Exportación desde SolidWorks:** Utilizando el plugin *Simscape Multibody Link*, exportamos el ensamblaje completo. Este proceso extrae automáticamente las propiedades físicas del modelo CAD (masa, centro de gravedad, momentos de inercia de cada pieza) y genera un archivo `.xml` junto con las geometrías tridimensionales asociadas.
2. **Importación a MATLAB:** A través del comando `smimport` en la ventana de comandos de MATLAB, leímos el archivo `.xml` generado previamente. 
3. **Generación del Diagrama de Bloques:** Simulink interpretó la información y construyó de forma automática el diagrama de bloques del sistema multicuerpo. 

**Estructura del modelo en Simscape:**
El diagrama resultante traduce fielmente las relaciones mecánicas del dron:
* **Sólidos (Rigid Solids):** Representan el chasis principal y cada una de las hélices, conservando la masa y geometría real del dron.
* **Articulaciones (Revolute Joints):** Las relaciones de posición (mates) creadas en SolidWorks se transformaron en juntas de revolución, las cuales representan el eje de giro de cada uno de los 4 motores, permitiendo el movimiento independiente de cada hélice.
* **Marco de Referencia:** El sistema cuenta con un bloque de configuración de mecanismo que define las leyes de la física (como la gravedad) sobre las cuales interactuará el dron durante las pruebas.

Este modelo base en Simulink es fundamental, ya que nos permitirá inyectar señales a los motores, simular el comportamiento en vuelo y probar los algoritmos de control (PID) antes de implementarlos en el hardware físico.

<img src="multibodyDron.png" width="200" alt="Diagrama de bloques Simscape">
<img src="simulinkDron.png" width="200" alt="Simulación">

## Ingeniería Software



