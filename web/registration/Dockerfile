# syntax=docker/dockerfile:1

FROM golang:1.24-bookworm AS builder

WORKDIR /src
COPY go.mod go.sum ./
RUN go mod download
COPY . .
RUN CGO_ENABLED=0 GOOS=linux go build \
    -trimpath \
    -ldflags="-s -w" \
    -o /out/registration \
    ./cmd/registration

FROM gcr.io/distroless/static-debian12:nonroot

COPY --from=builder /out/registration /registration
EXPOSE 8080
USER nonroot:nonroot
ENTRYPOINT ["/registration"]
