FROM ubuntu:22.04

RUN apt update && apt install -y ninja-build cmake bear g++ git python3 curl xz-utils zlib1g-dev
WORKDIR /app

COPY ./scripts/get_oclint.sh ./scripts/get_oclint.sh
RUN ./scripts/get_oclint.sh

COPY ./CMakeLists.txt ./CMakeLists.txt
COPY ./rules ./rules
COPY ./scripts/build_rules.sh ./scripts/
RUN ./scripts/build_rules.sh

COPY . .
RUN rm -rf /var/lib/apt/lists/*

ENTRYPOINT ["python3", "scripts/entry.py"]
