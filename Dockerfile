FROM debian:bookworm-slim AS builder

RUN apt-get update && apt-get install -y build-essential gcc && rm -rf /var/lib/apt/lists/*

WORKDIR /app

COPY . .

RUN gcc src/*.c -o /usr/local/bin/ft_ping -lm

FROM debian:bookworm-slim

COPY --from=builder /usr/local/bin/ft_ping /usr/local/bin/ft_ping

CMD ["tail", "-f", "/dev/null"]
