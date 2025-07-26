FROM debian:12-slim

WORKDIR /app

RUN apt-get update && apt-get install -y \
    cmake \
    make \
    gcc \
    g++ \
    wget \
    unzip \
    libc6-dev-i386 \
    && rm -rf /var/lib/apt/lists/*

RUN wget https://www.sqlite.org/2018/sqlite-amalgamation-3260000.zip -O sqlite-amalgamation-3260000.zip && \
    unzip sqlite-amalgamation-3260000.zip -d sqlite-amalgamation-3260000 && \
    rm sqlite-amalgamation-3260000.zip && \
    wget https://raw.githubusercontent.com/verezyp/study/refs/heads/111/CMakeLists.txt -O CMakeLists.txt
