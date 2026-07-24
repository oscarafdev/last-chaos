# Auditoría técnica de la descarga

## Inventario

- `LC2015`: codebase anterior.
- `LC2018`: cliente Windows y servidores Windows, con muchas copias completas.
- `LinuxServer`: distribución Linux 2015/2018 preparada para GCC 12/Clang 16.
- `DB` y `MySQL`: dumps y una instalación histórica de MySQL para Windows.
- `SeriousSam`: SDK/motor de referencia separado; no participa del build
  seleccionado.

## Selección

El README original recomienda Visual Studio 2019 para el cliente. Entre las
variantes disponibles se eligió `Sources.VS2019.DX9.NoAsm.x64.Z.15`: revisión
final sin los experimentos `BL.VK` y sin backups intermedios.

Para el servidor se eligió `LinuxServer/LCS2018/Server.Sources`, porque es la
única rama documentada y probada con librerías de distribución modernas.

## Dependencias del servidor

- GCC/G++, make y Subversion (el Makefile genera metadata con `svn info`).
- MariaDB/MySQL client.
- APR/APR-util, Boost thread/system, Botan 2, cURL, Expat, JsonCpp, Log4cxx y
  zlib.

El Dockerfile expresa estas dependencias en una etapa de build independiente.

## Base de datos

Los dumps no crean sus bases. `database/init/00-create-databases.sql` crea y
selecciona explícitamente:

- `2018_nov_data`
- `2018_nov_db`
- `2018_nov_db_auth`
- `2018_nov_post`

## Riesgos

- Código C/C++ legado con configuraciones regionales y macros abundantes.
- Configuraciones de red y DB históricamente incrustadas en archivos de texto.
- Un componente (`CashServer`) sin fuente y con ABI de 32 bits.
- Sin tests automatizados encontrados.
- Sin manifiesto de licencia unificado en la descarga.

## Correcciones de build aplicadas

- Se unificó `OutDir` desde el script de cliente; originalmente `GameMP`
  buscaba `EntitiesMP.lib` en su propia carpeta.
- Se sincronizaron las declaraciones de `StartNewMode` y
  `TryToSetDisplayMode` con sus implementaciones borderless.
- Se definieron localmente los tres settings borderless/aspect ratio que la
  revisión `Z.15` declaraba como imports inexistentes.
- El resource script de `Nksp` usa los headers del Windows SDK en vez de MFC.
- El copy step de `Nksp` ahora tolera que origen y destino sean el mismo
  archivo cuando el build usa el `Bin` compartido.
