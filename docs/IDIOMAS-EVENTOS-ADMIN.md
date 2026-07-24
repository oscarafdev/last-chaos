# Idiomas, eventos y administración

Esta guía describe el comportamiento confirmado en el código de esta variante
2018, compilada con localización de servidor `USA`. Los comandos se escriben en
el chat del juego comenzando con `/`.

## 1. Cambiar el idioma del cliente

El idioma no se recibe del servidor. El cliente lee `g_iCountry` desde el
archivo codificado `client/Data/etc/ps.dat` y, a partir de ese valor, carga los
recursos `client/Local/<idioma>/String/*`.

Idiomas presentes en esta entrega:

| Código | Idioma | `g_iCountry` |
|---|---|---:|
| `us` | Inglés (USA) | 7 |
| `de` | Alemán | 10 |
| `es` | Español | 11 |
| `fr` | Francés | 12 |
| `pl` | Polaco | 13 |
| `ru` | Ruso | 14 |
| `it` | Italiano | 17 |
| `uk` | Inglés (UK) | 24 |

Con el juego completamente cerrado, desde la raíz del proyecto:

```powershell
powershell -ExecutionPolicy Bypass -File .\scripts\set-client-language.ps1 -Language es
.\Jugar.cmd
```

También se puede abrir `Jugar-Espanol.cmd` con doble clic; configura el idioma
y luego usa el launcher correcto automáticamente.

Para volver al inglés de esta distribución:

```powershell
.\scripts\set-client-language.ps1 -Language us
```

El script conserva una copia inicial en `ps.dat.bak`, cambia únicamente
`g_iCountry` y verifica el resultado. La descarga no incluye
`Local/es/FontSpain0.tex`, aunque el ejecutable la exige. Para español y otros
paquetes latinos que no traen su textura, el script crea el nombre esperado
usando `Local/common/FontLatin0.tex`, recuperada del cliente LC2015 incluido en
la descarga. No se puede usar `FontChineseT0.tex`: el modo `PutTextExBrz`
interpreta otra cuadrícula y muestra glifos corruptos. El selector detecta y
reemplaza automáticamente ese fallback antiguo.

El atlas USA tampoco trae caracteres españoles en las posiciones CP1252 que
usan los `.lod`: esos bytes contienen símbolos internos. Por eso antes se veía
`más`, `pequeño` o `Información` con glifos incorrectos. El script
`scripts/patch-latin-font.js` instala de forma idempotente `áéíóúüñ`, sus
mayúsculas, `ç`, `¿` y `¡` en el atlas latino. El selector lo ejecuta y
sincroniza `FontSpain0.tex` cada vez que se elige un idioma latino. Es
necesario cerrar y volver a abrir el cliente para recargar la textura.

Además, el paquete español descargado solo contiene 2 de las 34 imágenes de
carga y no incluye carteles, mapa, filtros ni las cadenas de ayuda/tienda NPC.
El selector completa únicamente esos archivos ausentes desde `Local/us`,
renombrando los que llevan sufijo y sin sobrescribir traducciones. Por eso
algunos textos o imágenes pueden seguir en inglés.

Los `.lod` españoles proceden de otra revisión y contienen huecos que el
`TableLoader` de 2018 no valida. El selector conserva cada archivo en
`*.lod.original` y genera una tabla compatible: mantiene las traducciones
válidas y completa índices o campos ausentes con el paquete USA.

El código de país español también solicita `MakeItem_spn.lod`,
`event_spn.lod` y `lacarette_spn.lod`, ausentes en la descarga. El selector
instala equivalentes compatibles. Los nombres de Lacarette quedan como
fallback pendiente de traducción.

## 2. Eventos

Hay dos sistemas distintos: eventos de contenido almacenados en base de datos
y multiplicadores temporales administrados por comandos.

### 2.1 Eventos de contenido (`t_event`)

`2018_nov_data.t_event` contiene la definición. Al arrancar, GameServer carga
las filas cuyo `a_enable` vale `1`. `a_notice` controla los avisos,
`a_npclist` la regeneración de NPC y `a_drop_1` a `a_drop_4` los grupos de
drop asociados.

Con un personaje administrador nivel 10:

```text
/automationevent list
/automationevent activelist
/automationevent on 50
/automationevent off 50
```

- `list`: muestra todos los IDs disponibles.
- `activelist`: muestra los activos.
- `on <ID>`: activa el evento en memoria y guarda `a_enable=1`.
- `off <ID>`: lo desactiva y guarda `a_enable=0`.

Ejemplos de IDs cargados por esta base:

| ID | Evento | Estado inicial |
|---:|---|:---:|
| 20 | Rain Drop | activo |
| 22 | Collect bug testing | activo |
| 29 | Valentine testing | activo |
| 31 | White Day dev | activo |
| 49 | Halloween testing | inactivo |
| 50 | XMAS | inactivo |
| 80 | World Cup toto | activo |
| 83 | Holy Water Drop | inactivo |
| 84 | PVP Artifact Hunter | activo |

Usar el comando es preferible a editar SQL: actualiza el servidor en ejecución,
propaga el cambio y lo persiste. Si se modifica `a_enable` directamente, el
cambio no entra en memoria hasta reiniciar GameServer, lo que desconecta a los
jugadores.

Antes de activar un evento conviene probarlo en un servidor de desarrollo.
Varios registros están rotulados `testing` y pueden depender de NPC, mapas o
recompensas incompletos. `automationevent reward` es una función de prueba y
no se recomienda para operación normal.

### 2.2 Multiplicadores disponibles en la build USA

Los siguientes comandos afectan al servidor vivo y no cambian `t_event`:

```text
/doublepetexp_event all start 200
/doublepetexp_event all ing
/doublepetexp_event all stop

/doubleattack_event all start 150
/doubleattack_event all ing
/doubleattack_event all stop

/event itemdrop all start 150
/event itemdrop all ing
/event itemdrop all stop
```

`200` equivale a 200 %. Pet EXP admite 100–1000 en pasos de 10; ataque admite
0–200; item drop admite 100–200. En lugar de `all` se puede usar el número de
grupo de servidor o `cur`.

El evento doble general usa grupo y subservidor, seis valores y un intervalo:

```text
/double_event all all start 200 200 200 200 200 0 2026 7 24 18 0 0 2026 7 24 23 0 0
/double_event all all ing
/double_event all all stop
```

Los seis valores, en el orden que muestra el propio servidor, son
`nasdrop`, `nasget`, `exp`, `sp`, `produce` y `producenum`; el último está
limitado a 0–10 y los demás a 0–600. Las fechas son doce campos:
`AAAA MM DD hh mm ss` de inicio y luego de fin, en la hora local del servidor.

El evento de mejora admite probabilidades 100, 125, 150, 175 o 200:

```text
/upgrade_event all start 150 2026 7 24 18 0 0 2026 7 24 23 0 0
/upgrade_event all ing
/upgrade_event all stop
```

Aunque están registrados como comandos, en la localización USA actual
`/doubleitem_event`, `/event_dropitem`, `/npcdropitem_event` y
`/double_event_auto` quedaron compilados sin implementación efectiva.
`/eventshow`, `/eventsetting` y `/chance_event` son stubs vacíos. No deben
usarse para operar eventos.

## 3. Administración

### 3.1 Cómo se concede

El nivel GM se almacena por personaje, no por cuenta:
`2018_nov_db.t_characters.a_admin`. GameServer lo carga al entrar con el
personaje. Los niveles son acumulativos de 0 (jugador) a 10 (control total).

Estado encontrado en esta base: todos los personajes existentes tienen nivel
10 y la columna usa `DEFAULT 10`. Eso convierte también a cada personaje nuevo
en administrador total. El propio `PatchManual.txt` del servidor indica que el
valor por defecto debe ser 0.

Configuración segura desde el cliente de MariaDB de Docker:

```powershell
docker compose exec database mariadb -uroot -p
```

Dentro de MariaDB, primero inspeccionar:

```sql
SELECT a_index, a_server, a_name, a_admin
FROM `2018_nov_db`.t_characters
ORDER BY a_server, a_name;
```

Corregir el valor por defecto:

```sql
ALTER TABLE `2018_nov_db`.t_characters
  MODIFY a_admin TINYINT NOT NULL DEFAULT 0;
```

Después, asignar explícitamente los niveles deseados. Ejemplo para conservar
un solo GM de desarrollo:

```sql
UPDATE `2018_nov_db`.t_characters SET a_admin = 0;
UPDATE `2018_nov_db`.t_characters
SET a_admin = 10
WHERE a_server = 1 AND a_name = 'NOMBRE_DEL_GM';
```

Reemplazar el nombre y verificar el `a_server` con el `SELECT`; no ejecutar el
ejemplo literalmente. El personaje debe salir y volver a entrar para recargar
su nivel. La tabla `t_gm` no concede permisos: agenda comandos GM.

### 3.2 Comandos útiles

Comprobación y movimiento:

| Comando | Nivel | Función |
|---|---:|---|
| `/myadmin` | 1 | Muestra el nivel cargado por el servidor |
| `/whoami` | GM | Consulta GM especial del cliente |
| `/whereami` | 3 | Servidor, subservidor, zona y coordenadas |
| `/visible` | 3 | Alterna visibilidad |
| `/immortal` | 3 | Alterna invulnerabilidad |
| `/speedup <n>` | 3 | Cambia velocidad; menor que 1 restablece |
| `/go_zone <zona> <spawn>` | 3 | Teletransporte a una zona |
| `/goto <x> <z> <capaY>` | 3 | Teletransporte por coordenadas |
| `/go_pc <personaje>` | 3 | Teletransporte a un personaje |
| `/go_npc <id-o-nombre>` | 3 | Teletransporte a un NPC |

Operación y pruebas:

| Comando | Nivel | Función |
|---|---:|---|
| `/echo zone <texto>` | 1 | Aviso en la zona |
| `/echo server <texto>` | 1 | Aviso en el grupo de servidor |
| `/echo all <texto>` | 1 | Aviso global y persistido |
| `/kick <personaje>` | 5 | Expulsa al personaje |
| `/levelup <nivel>` | 8 | Fija el nivel |
| `/expup <0..100>` | 8 | Fija el porcentaje del nivel |
| `/skillpoint <valor>` | 7 | Fija puntos de habilidad |
| `/nas_set <cantidad>` | 10 | Fija la moneda del personaje |
| `/itemget <id> [plus] [flag] [cantidad]` | 10 | Crea un objeto en inventario |
| `/itemdrop <id> [plus] [flag] [cantidad]` | 10 | Crea un objeto en el suelo |
| `/summon <cantidad> <id-o-nombre-NPC>` | 9 | Invoca NPC |

El nivel 1 ya permite acciones potentes en este código heredado; no debe
considerarse un rol de moderador seguro sin auditar cada comando. Reservar
nivel 10 para desarrollo aislado. Comandos como `shutdown`, `reboot`,
`deletechar`, creación de objetos, moneda y recompensas pueden alterar o
detener el entorno y deben evitarse en producción.

### 3.3 Flujo recomendado

1. Corregir `DEFAULT 10` a `DEFAULT 0`.
2. Mantener una cuenta/personaje GM separado de los personajes de juego.
3. Conceder el menor nivel posible y volver a iniciar sesión.
4. Probar eventos en una copia de la base y revisar logs.
5. Usar `automationevent` para contenido y los comandos de multiplicador para
   bonificaciones temporales; verificar siempre con `activelist` o `ing`.

## Referencias del código

- Idioma y rutas: `client/src/Engine/GlobalDefinition.h`,
  `client/src/Engine/Help/DefineHelp.cpp`,
  `client/src/Engine/Help/LoadString.cpp`,
  `client/src/Engine/Interface/UITextureManager.cpp`.
- Permisos y comandos: `server/src/GameServer/GMCmdList.cpp`,
  `server/src/GameServer/doFuncAdmin.cpp`,
  `server/src/ShareLib/gm_command.h`,
  `server/src/ShareLib/Config_Localize_USA.h`.
- Eventos persistentes: `server/src/GameServer/eventAutomation.cpp`.
- Carga de permisos: `server/src/GameServer/DBProcess_SelectChar.cpp`.
