FROM ubuntu:22.04

RUN apt update && apt install -y ninja-build cmake bear g++ git python3 curl xz-utils zlib1g-dev
WORKDIR /app

COPY ./ ./
RUN ./scripts/get_oclint.sh && ./scripts/build_rules.sh && rm -rf /var/lib/apt/lists/*

ENTRYPOINT ["python3", "scripts/entry.py"]
