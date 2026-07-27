# syntax=docker/dockerfile:1

FROM debian:12-slim AS server-builder

ARG DEBIAN_FRONTEND=noninteractive
ARG LC_LOCALE=usa

RUN apt-get update \
    && apt-get install -y --no-install-recommends \
        build-essential \
        default-libmysqlclient-dev \
        libapr1-dev \
        libaprutil1-dev \
        libboost-system-dev \
        libboost-thread-dev \
        libbotan-2-dev \
        libcurl4-gnutls-dev \
        libexpat1-dev \
        libjsoncpp-dev \
        liblog4cxx-dev \
        subversion \
        zlib1g-dev \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /build
COPY server/src/ ./
COPY server/tests/ ./tests/

RUN g++ -std=gnu++11 -I./ShareLib \
        ./tests/PacketSizeValidationTest.cpp \
        -o /tmp/packet-size-validation-test \
    && /tmp/packet-size-validation-test \
    && make OPT_DEF=-DSETTING_IF_INNER_IP_NEW "${LC_LOCALE}"

FROM debian:12-slim AS server

ARG DEBIAN_FRONTEND=noninteractive

RUN dpkg --add-architecture i386 \
    && apt-get update \
    && apt-get install -y --no-install-recommends \
        bash \
        default-libmysqlclient-dev \
        libapr1 \
        libaprutil1 \
        libboost-system1.74.0 \
        libboost-thread1.74.0 \
        libbotan-2-19 \
        libc6:i386 \
        libcurl3-gnutls \
        libexpat1 \
        libjsoncpp25 \
        liblog4cxx15 \
        libstdc++6:i386 \
        netcat-openbsd \
        xz-utils \
        zlib1g \
        zlib1g:i386 \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /opt/lastchaos
COPY server/LC2018.Server.x64.tar.xz /tmp/server.tar.xz
RUN tar -xJf /tmp/server.tar.xz --strip-components=1 \
    && rm /tmp/server.tar.xz

COPY --from=server-builder /build/CalcHash/CalcHash ./CalcHash/CalcHash
COPY --from=server-builder /build/Connector/Connector ./Connector/Connector
COPY --from=server-builder /build/GameServer/GameServer ./GameServer/GameServer
COPY --from=server-builder /build/Helper/Helper ./Helper/Helper
COPY --from=server-builder /build/LoginServer/LoginServer ./LoginServer/LoginServer
COPY --from=server-builder /build/Messenger/Messenger ./Messenger/Messenger
COPY --from=server-builder /build/SubHelper/SubHelper ./SubHelper/SubHelper
COPY docker/server/entrypoint.sh /usr/local/bin/lastchaos-entrypoint
COPY docker/server/healthcheck.sh /usr/local/bin/lastchaos-healthcheck

RUN chmod +x \
        /usr/local/bin/lastchaos-entrypoint \
        /usr/local/bin/lastchaos-healthcheck \
        ./CalcHash/CalcHash \
        ./Connector/Connector \
        ./GameServer/GameServer \
        ./Helper/Helper \
        ./LoginServer/LoginServer \
        ./Messenger/Messenger \
        ./SubHelper/SubHelper

EXPOSE 3000 4001 4006 4101 4102 4112 50401

HEALTHCHECK --interval=10s --timeout=3s --start-period=45s --retries=6 \
    CMD ["lastchaos-healthcheck"]

ENTRYPOINT ["lastchaos-entrypoint"]
