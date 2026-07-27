# Despliegue beta

## Servicios públicos

- `4001/tcp`: LoginServer.
- `4006/tcp`: Connector.
- `4101/tcp`: GameServer.
- `443/tcp`: registro de cuentas en `https://lc.somositconfig.com`.

MariaDB y los puertos internos `3000`, `4102`, `4112` y `50401` no deben
publicarse. El portal se enlaza al proxy inverso por una dirección privada del
host Docker.

## Secretos

El archivo `.env` no se versiona. Cada proceso usa una cuenta de base de datos
distinta y una contraseña aleatoria. El usuario del portal solamente puede
consultar e insertar filas en `2018_nov_db_auth.bg_user`.

Para un despliegue nuevo:

```bash
cp .env.example .env
chmod 600 .env
# Reemplazar todos los valores change-me y configurar las IP públicas.
docker compose config --quiet
docker compose up -d --build
```

No se debe publicar `3306`, habilitar acceso remoto de `root` ni reutilizar una
contraseña entre servicios.

## Verificación

```bash
docker compose ps
docker compose logs --tail=100 server
docker compose logs --tail=100 registration
ss -lnt
curl --fail http://172.18.0.1:8080/health
```

Desde otra máquina deben responder `4001`, `4006`, `4101` y HTTPS. La conexión
a `3306` debe fallar.

## Cliente beta

El paquete se genera con:

```powershell
powershell -ExecutionPolicy Bypass -File .\scripts\package-beta-client.ps1 `
  -ServerAddress 144.217.7.136
```

El empaquetador:

- incluye solamente el runtime y los assets;
- configura `sl.dta` con el servidor indicado;
- conserva el idioma español;
- desactiva “guardar ID” y elimina el usuario persistido;
- excluye perfiles, contraseñas, logs, símbolos y bibliotecas de desarrollo;
- calcula SHA-256 del ZIP resultante.

## Correcciones de seguridad incluidas

El tamaño recibido por la red se representa como `uint32_t` y se valida contra
el mínimo y el máximo antes de construir cualquier `boost::asio::buffer`.
También se prueba explícitamente `UINT32_MAX`, que representa el antiguo caso
`-1` convertido a entero sin signo.

Los builds release usan `-O2`, `-DNDEBUG`, símbolos mínimos y
`-Werror=return-type`. El test de límites de paquetes se ejecuta antes de
compilar la imagen completa.

## Modo sin servidor de cobros

`ENABLE_CASH_SERVER=false` desactiva en Connector tanto la conexión al servidor
de cobros como su temporizador de actividad. El proceso conserva su heartbeat y
registra una sola vez `Billing server disabled; free mode enabled.`. Si la
variable no existe se mantiene el comportamiento histórico, con el servidor de
cobros habilitado.

Después de iniciar el entorno gratuito, verificar que el mensaje anterior
aparezca una vez y que no haya reintentos:

```bash
docker compose logs --since=5m server |
  grep -E "Billing server disabled|Can't connected to billing server"
```

## Rechazos CSRF del formulario de registro

Algunas extensiones de privacidad pueden eliminar la cookie CSRF o el encabezado
`Origin` de un formulario legítimo. La validación acepta la solicitud cuando
coinciden al menos dos de estas tres señales independientes:

- token del formulario y cookie CSRF;
- origen público exacto;
- `Sec-Fetch-Site: same-origin`.

Así se tolera la pérdida de una señal sin aceptar formularios externos. Los
rechazos registran solamente qué señales coincidieron; nunca se escriben tokens
ni contraseñas en los logs. Las respuestas del formulario incluyen
`Cache-Control: no-store` para evitar tokens obsoletos.
