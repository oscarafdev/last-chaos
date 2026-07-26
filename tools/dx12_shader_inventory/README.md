# Analizador offline de shaders

Esta herramienta crea un inventario reproducible del contenido instalado que
puede activar shaders legacy durante la migración a DirectX 12. No inicia el
juego, no crea un dispositivo gráfico y no necesita conectarse al servidor.

## Qué verifica

1. Lee todos los manifiestos `client/Shaders/*.sha`.
2. Compila un `Engine.dll` mínimo en `.itconfig` para satisfacer la ABI de
   `Shaders.dll` sin cargar el motor real.
3. Extrae de la DLL real sus descriptores, stream flags y todas las variantes
   VP/PP, incluidos los tres modos de niebla.
4. Lee cada byte de `client/Data`, buscando referencias `.sha` tanto ASCII como
   UTF-16LE y calculando un SHA-256 del snapshot analizado.
5. Inventaría shaders definidos internamente en C++ y sitios que crean
   programas directamente.
6. Cruza el resultado con las familias, parejas implementadas y parejas
   validadas del backend DirectX 12.

## Uso

Desde la raíz del repositorio:

```powershell
python tools\dx12_shader_inventory\analyze.py
```

Para escribir siempre en una ruta conocida:

```powershell
python tools\dx12_shader_inventory\analyze.py `
  --output .itconfig\dx12-shader-inventory\latest
```

El script requiere Python x64 y Visual Studio Build Tools con C++ x64. Los
binarios temporales y los informes se guardan bajo `.itconfig`, por lo que no
se agregan al repositorio.

## Salidas

- `summary.md`: resumen legible y cobertura por manifiesto.
- `inventory.json`: inventario completo y versionado por `schema_version`.
- `asset-shader-references.csv`: relación material/asset a manifiesto.
- `sources/`: ensamblador legacy devuelto por cada export de `Shaders.dll`.

## Alcance de la garantía

Si `scan_complete` es `true`, todos los archivos accesibles bajo las raíces
indicadas fueron abiertos y todos sus bytes participaron del digest. Esto
garantiza el inventario de referencias explícitas presentes en ese snapshot y
permite comprobar que dos ejecuciones analizaron exactamente el mismo
contenido.

El superset de parejas se calcula por manifiesto. Todavía no equivale a las
parejas finales del runtime, porque el motor agrega código según pesos por
vértice, niebla, tipo de normal map y declaración de streams antes de
ensamblarlas. Los shaders construidos directamente desde C++ se informan por
separado para que tampoco queden invisibles.

Los manifiestos que apuntan a exports ausentes no se descartan: aparecen con
`extraction_error`. Así se distingue contenido obsoleto de una familia
realmente disponible.
