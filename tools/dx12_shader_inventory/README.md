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
7. Reproduce el ensamblado final del motor para cada cantidad de pesos,
   normalización, niebla y tipo de normal map; agrega la declaración de
   vértices y calcula el mismo FNV-1a 64 usado por el backend.

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

Para regenerar el catálogo de bytecode usado por los adaptadores D3D9:

```powershell
python scripts\generate-legacy-shader-bytecode.py
```

Ese paso usa D3DX9 únicamente fuera del juego y escribe un header
determinista. El cliente consulta el catálogo por el fingerprint de la fuente;
no carga D3DX9 ni D3DCompiler y falla explícitamente si aparece una variante
que no fue inventariada.

El script requiere Python x64, Visual Studio Build Tools con C++ x64 y
`d3dx9_43.dll` para ensamblar el código histórico. D3DX9 se usa solamente como
herramienta offline: el analizador no crea un dispositivo D3D9 ni agrega esa
dependencia a la ruta DX12 del juego. Los binarios temporales y los informes se
guardan bajo `.itconfig`, por lo que no se agregan al repositorio.

## Salidas

- `summary.md`: resumen legible y cobertura por manifiesto.
- `inventory.json`: inventario completo y versionado por `schema_version`.
- `asset-shader-references.csv`: relación material/asset a manifiesto.
- `sources/`: ensamblador legacy devuelto por cada export de `Shaders.dll`.

Dentro de `inventory.json`, `exact_runtime_inventory` contiene:

- todas las variantes VS/PS compilables y sus fingerprints;
- el superset de parejas compatibles por manifiesto o shader interno;
- la correlación exacta con las parejas implementadas y validadas de DX12;
- variantes históricas que la propia D3DX9 rechaza, conservadas como errores
  auditables.

## Alcance de la garantía

Si `scan_complete` es `true`, todos los archivos accesibles bajo las raíces
indicadas fueron abiertos y todos sus bytes participaron del digest. Esto
garantiza el inventario de referencias explícitas presentes en ese snapshot y
permite comprobar que dos ejecuciones analizaron exactamente el mismo
contenido.

`potential_program_pairs` conserva el superset de fuentes por manifiesto.
`exact_runtime_inventory` sí reconstruye las parejas finales: aplica las reglas
de `Shader.cpp`, la conversión de `CompileVertexProgram_D3D`, la declaración de
`GetShaderDeclaration_D3D9` y el hash de
`DirectX12Legacy3DCommandBatch.cpp`. También incluye los shaders internos de
terreno y los programas creados directamente por los efectos.

Los manifiestos que apuntan a exports ausentes no se descartan: aparecen con
`extraction_error`. Así se distingue contenido obsoleto de una familia
realmente disponible.
