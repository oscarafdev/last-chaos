# Last Chaos 2018 — DirectX 12 y servidor reproducible

Esta carpeta consolida la codebase 2018 descargada en una única variante
mantenible. El material original fuera de `Games/` no fue modificado.

## Qué contiene

- `client/src`: cliente de 64 bits modernizado con backend DirectX 12.
- `client/{Bin,Data,...}`: runtime, assets y toolchain requeridos por los
  paths relativos de la solución.
- `server/src`: servidor Linux 2018 compilable desde fuente.
- `server/LC2018.Server.x64.tar.xz`: datos y configuración de runtime del
  servidor.
- `database/init`: los cuatro esquemas MySQL de la codebase 2018.
- `docker/server`: arranque y configuración del servidor en contenedor.
- `scripts`: comandos pequeños y reutilizables para los builds.

Se descartaron de esta copia las numerosas ramas intermedias `Z.0` a `Z.14`,
backups `buf`, variantes VS2010/2015/2017/2022 y el material de LC2015.

## Requisitos

- Docker Desktop en modo Linux para servidor y base de datos.
- Para compilar el cliente: Windows, Visual Studio Build Tools con C++.
  El script está preparado para el toolset `v145` y SDK `10.0.26100.0`
  instalados en esta máquina.

El cliente es Win32/DirectX y no se compila dentro del contenedor Linux.
El servidor sí se construye completamente dentro de Docker sobre Debian 12.

## Build del servidor

```powershell
.\scripts\build-server.ps1
```

Si la política local de PowerShell bloquea scripts:

```powershell
powershell -ExecutionPolicy Bypass -File .\scripts\build-server.ps1
```

O directamente:

```powershell
docker compose build server
```

La imagen usa un build multi-stage: GCC, headers y fuentes quedan fuera de la
imagen final. El locale predeterminado es `usa`; puede cambiarse con
`LC_LOCALE` o el parámetro `-Locale`.

## Levantar el entorno

```powershell
Copy-Item .env.example .env
# Definir un secreto diferente para cada servicio antes de iniciar.
.\scripts\up.ps1
```

La primera inicialización de MariaDB importa unos 50 MB de SQL y puede tardar.
Los datos y logs viven en volúmenes Docker. MariaDB no publica el puerto 3306 y
cada proceso utiliza una cuenta con permisos limitados.

Comandos de diagnóstico:

```powershell
docker compose ps
docker compose logs -f server
docker compose logs -f database
```

Para detener sin borrar datos:

```powershell
docker compose down
```

Para borrar también las bases persistidas:

```powershell
docker compose down -v
```

## Build del cliente

Con Visual Studio Build Tools:

```powershell
.\scripts\build-client.ps1
```

La configuración recomendada por el material original es `LCRelease|x64`. El
script construye el target `Nksp`, omite herramientas editoriales opcionales
que requieren MFC y escribe los resultados en
`client/build/<toolset>/<plataforma>`. De este modo una compilación no
sobrescribe el runtime legado verificado que está en `client/Bin`. El script
permite indicar otro SDK o toolset:

```powershell
.\scripts\build-client.ps1 `
  -PlatformToolset v145 `
  -WindowsSdkVersion 10.0.26100.0
```

El código fuente compila con Visual Studio Build Tools 2026, `v145` y SDK
`10.0.26100.0`. Sin embargo, los DLL recompilados con ese toolchain presentan
una incompatibilidad ABI al cargar clases de `EntitiesMP.dll`. Para ejecutar el
juego se conserva en `client/Bin` el conjunto coherente de binarios distribuido
con el proyecto.

## Ejecutar el cliente

Con el servidor saludable, desde la raíz del proyecto:

```powershell
.\scripts\run-client.ps1
```

También se puede abrir `Jugar.cmd` con doble clic. Es la forma recomendada: usa
`client/multi.bat` para proporcionar el parámetro requerido por el cliente.
Abrir `client/Bin/Nksp.exe` directamente muestra el mensaje
`This program could not be run itself`.

El launcher establece el directorio de trabajo correcto. `client/sl.dta`
configura el LoginServer local en `127.0.0.1:4001`. Para jugar desde otra PC hay
que generar ese archivo con la IP del host Docker y definir la misma dirección
en `LC_SERVER_PUBLIC_IP`.

La guía detallada para seleccionar español, operar eventos y administrar
permisos GM está en
[`docs/IDIOMAS-EVENTOS-ADMIN.md`](docs/IDIOMAS-EVENTOS-ADMIN.md).

El despliegue endurecido, el portal de registro y el paquete para testers se
documentan en [`docs/DEPLOY-BETA.md`](docs/DEPLOY-BETA.md).

La integración experimental de Chromium Embedded Framework y el comando
`/testcef` se documentan en [`docs/CEF-UI.md`](docs/CEF-UI.md).

## Decisiones y límites conocidos

1. La descarga original ocupa 8,4 GB sólo en `LC2018` porque replica el árbol
   completo para cada revisión. Esta copia conserva una sola línea base.
2. El servidor Linux estaba preparado para Debian 12/GCC 12; por eso es el
   componente reproducible en Docker.
3. `CashServer` sólo existe como binario legado de 32 bits. Se ejecuta mediante
   sus librerías de compatibilidad, puede desactivarse con
   `LC_ENABLE_CASH_SERVER=false` y no forma parte del build desde fuente.
4. Los archivos `newStobm.bin` contienen configuración en texto plano. El
   entrypoint reemplaza host, IP y contraseña al iniciar; no se versiona `.env`.
5. `LC_SERVER_BIND_IP=0.0.0.0` mantiene accesibles los puertos publicados y
   `LC_SERVER_PUBLIC_IP` define la dirección que se anuncia al cliente. Para un
   cliente en la misma PC se usa `127.0.0.1`.
6. Los assets y el código provienen de una descarga de Internet. Antes de
   publicar o comercializar un juego derivado, revisa licencias, marcas,
   derechos de autor y cualquier contenido propietario.

## Verificación realizada

- `docker compose build server`: correcto.
- MariaDB 10.11: saludable, con 81/157/53/17 tablas en los cuatro esquemas.
- Servidor: saludable con sus siete procesos, incluido `CashServer`.
- Cliente: build `LCRelease|x64` completado con Visual Studio Build Tools 2026
  (`v145`). El runtime legado de `client/Bin` fue verificado mediante
  `Jugar.cmd` hasta la ventana `Gamigo (Window 1600x900)`.
- Permanecen warnings de C/C++ legado y avisos de consistencia de datos
  (algunos NPC no están en `npc_protolist`), pero no bloquean build ni arranque.

## Siguiente refactor recomendado

Esta entrega prioriza una base compilable y trazable. Antes de desarrollar el
nuevo juego conviene crear un repositorio Git, mover assets pesados a Git LFS,
añadir CI para `docker compose build`, y luego modernizar por módulos
(`Engine`, `GameMP`, `EntitiesMP`) sin mezclar esa migración con la limpieza
inicial.
