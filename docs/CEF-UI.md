# Integración de CEF

Esta rama incorpora Chromium Embedded Framework como una capa de UI
reutilizable controlada por el servidor.

## Arquitectura

El servidor no crea ni controla procesos Chromium. Envía una orden tipada con:

- acción (`OPEN` o `CLOSE`);
- ruta lógica de la vista;
- ancho y alto;
- parámetros;
- contexto del jugador.

El cliente valida la ruta y la transforma en un documento local confiable. La
primera ruta es `lcui://test`, cuyo HTML está en
`client/Bin/cef-ui/test/index.html`. Este contrato
permite agregar futuras vistas sin añadir un comando o paquete diferente por
cada pantalla.

CEF está aislado en `CWebPage.dll`, detrás de la API que el Engine ya cargaba
dinámicamente. `CefSubprocess.exe` ejecuta los procesos renderer/GPU. Así se
evita enlazar el ABI de Chromium directamente con `Nksp.exe` y se conserva un
límite apto para migrar las UI gradualmente.

El diálogo web es una ventana hija del cliente. Sus coordenadas siempre son
relativas al área de juego, por lo que acompaña a la ventana principal al
moverla y nunca puede salir de sus límites. Sólo el rectángulo del panel recibe
entrada de Chromium; el resto de la pantalla conserva el movimiento, la cámara
y las UI nativas.

## Interacción del panel

La vista `test` usa un panel de 520×360:

- la barra superior se puede arrastrar;
- la `X` cierra y destruye la instancia web;
- el panel queda limitado al área visible del juego;
- mover la ventana del juego no altera su posición relativa;
- los parámetros continúan llegando desde el servidor.

Las acciones HTML no manipulan ventanas Win32 directamente. La vista llama a
rutas internas `lcui://action/*`; `CWebPage.dll` las traduce a un conjunto
pequeño de mensajes privados (`drag-start`, `drag-move`, `drag-end` y `close`)
y el cliente ejecuta la operación. Este puente explícito es reutilizable para
futuras vistas sin exponer una API nativa arbitraria a JavaScript.

## Compilar

Desde PowerShell:

```powershell
.\scripts\build-client.ps1
.\scripts\build-cef-client.ps1 -DeployRebuiltClient
.\scripts\build-server.ps1
```

`build-cef-client.ps1` descarga una distribución oficial fijada de CEF,
comprueba su SHA-1, compila el DLL y el subprocess, y copia el runtime necesario
a `client/Bin`. La descarga y los intermediarios viven en `.itconfig`, fuera de
Git.

El despliegue debe copiar en conjunto `Nksp`, `Engine`, `EntitiesMP`, `GameMP`
y `Shaders`. Mezclar un Engine recompilado con DLL legados rompe el ABI antes
de crear la ventana; `-DeployRebuiltClient` preserva primero los binarios
anteriores y despliega el conjunto coherente.

CEF 150 requiere Windows 10 o posterior y un compilador con C++20. El script
detecta Visual Studio Build Tools 2022/2026.

## Probar

1. Reconstruir y desplegar el servidor.
2. Ejecutar el cliente con el Engine que contiene esta rama.
3. Entrar con una cuenta GM de nivel 9 o superior.
4. Escribir:

```text
/testcef
```

También se pueden enviar parámetros:

```text
/testcef mensaje enviado por el servidor
```

La página muestra el nombre del jugador y el texto recibido. Los parámetros se
escapan como componentes de URL antes de entrar en Chromium.

## Agregar una vista

Para una vista nueva:

1. Agregar sus archivos bajo `client/Bin/cef-ui/<ruta>/`.
2. Registrar `<ruta>` en `CefRuntime::ResolveUrl`.
3. Autorizar la ruta en el receptor del cliente.
4. Enviar `pTypeCefUi` desde el servicio o sistema del servidor que sea dueño de
   la funcionalidad.

No se deben enviar rutas de archivos arbitrarias desde el servidor. La lista
blanca cliente evita que un paquete remoto navegue a recursos locales
inesperados.

## Límites de esta primera iteración

- Existe una sola ventana web, igual que en la implementación anterior.
- El puente JavaScript→cliente sólo expone las acciones de ventana documentadas;
  cada nueva capacidad deberá agregarse a una lista explícita.
- HTTP y HTTPS siguen disponibles para usos heredados de `CWebPage`, pero la
  orden `/testcef` sólo acepta la ruta lógica `test`.
- Los parámetros están limitados a 1024 bytes en el servidor.
