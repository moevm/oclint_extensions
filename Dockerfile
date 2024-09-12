FROM ubuntu:22.04

RUN apt update && apt install -y ninja-build cmake bear g++ git python3 curl xz-utils zlib1g-dev 
WORKDIR /app

COPY ./scripts/get_oclint.sh get_oclint.sh
RUN ./get_oclint.sh

COPY ./rules ./rules
COPY ./CMakeLists.txt .
COPY ./scripts/build_rules.sh ./scripts/build_rules.sh
RUN ./scripts/build_rules.sh

COPY . .

ENTRYPOINT ["python3", "scripts/entry.py"]
