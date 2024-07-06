FROM ubuntu:22.04

RUN apt update
RUN apt install -y ninja-build cmake bear g++ git python3 curl xz-utils zlib1g-dev 
WORKDIR /app
COPY . .

RUN ./scripts/get_oclint.sh
RUN ./scripts/build_rules.sh

ENTRYPOINT ["scripts/entry.sh"]