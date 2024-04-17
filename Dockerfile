FROM ubuntu:22.04



RUN apt update
RUN apt install -y make python3 git curl
COPY ./ /app
WORKDIR /app

RUN make install

CMD cd ./examples && PATH="../:$PATH" ./test_on_repo.sh /repo && python3 ./json2csv.py && cat ./report.csv
