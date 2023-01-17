FROM ubuntu:22.04

RUN apt-get update && apt-get install -y apt-transport-https \
    ca-certificates \
    gnupg \
    software-properties-common \
    wget && \
    wget -O - https://apt.kitware.com/keys/kitware-archive-latest.asc 2>/dev/null | gpg --dearmor - | tee /etc/apt/trusted.gpg.d/kitware.gpg >/dev/null && \
    apt-add-repository 'deb https://apt.kitware.com/ubuntu/ jammy main' && \
    add-apt-repository ppa:git-core/ppa && \
    apt-get update && apt-get install -y tzdata && apt-get install -y \
    build-essential \
    ca-certificates \
    clang-format-14 \
    cmake \
    curl \
    fakeroot \
    g++ --no-install-recommends \
    gcc \
    gdb \
    git \
    java-common \
    libc6-dev \
    libgl1-mesa-dev \
    libglu1-mesa-dev \
    libisl-dev \
    libopencv-dev \
    libvulkan-dev \
    libx11-dev \
    libxcursor-dev \
    libxi-dev \
    libxinerama-dev \
    libxrandr-dev \
    make \
    mesa-common-dev \
    openjdk-17-jdk \
    python-all-dev \
    python3-dev \
    python3-pip \
    python3-setuptools \
    sudo \
    unzip \
    wget \
    zip \
  && rm -rf /var/lib/apt/lists/*

ENV JAVA_HOME /usr/lib/jvm/java-17-openjdk-amd64

WORKDIR /tmp

RUN curl -SL https://github.com/wpilibsuite/opensdk/releases/download/v2023-8/cortexa9_vfpv3-roborio-academic-2023-aarch64-bullseye-linux-gnu-Toolchain-12.1.0.tgz | sh -c 'mkdir -p /usr/local && cd /usr/local && tar xzf - --strip-components=2'


RUN wget https://github.com/protocolbuffers/protobuf/releases/download/v3.18.0/protobuf-cpp-3.18.0.tar.gz \
	&& tar -xzf protobuf-cpp-3.18.0.tar.gz \
	&& cd /tmp/protobuf-3.18.0 \
	&& ./configure \
	&& make -j4 && make install \
	&& cd /tmp \
	&& rm -Rf /tmp/* \
	&& ldconfig

WORKDIR /tmp
RUN curl -SLO https://github.com/zeromq/libzmq/releases/download/v4.3.4/zeromq-4.3.4.tar.gz \
	&& tar -xzf zeromq-4.3.4.tar.gz \
	&& cd /tmp/zeromq-4.3.4 \
	&& ./configure --enable-drafts --host=arm-frc${YEAR}-linux-gnueabi CC=arm-frc${YEAR}-linux-gnueabi-gcc CXX=arm-frc${YEAR}-linux-gnueabi-g++ \
	&& make -j4 \
	&& chown -R `id -u`:`id -g` src/.libs/libzmq.so \
	&& arm-frc${YEAR}-linux-gnueabi-strip src/.libs/libzmq.so \
	&& cp src/.libs/libzmq.so /usr/local/arm-frc2022-linux-gnueabi/lib/libzmq.so \
	&& cd /tmp \
	&& rm -Rf /tmp/* 

WORKDIR /tmp
RUN curl -SLO https://github.com/protocolbuffers/protobuf/releases/download/v3.18.0/protobuf-cpp-3.18.0.tar.gz \
	&& tar -xzf protobuf-cpp-3.18.0.tar.gz \
	&& cd /tmp/protobuf-3.18.0 \
	&& ./configure --host=arm-frc${YEAR}-linux-gnueabi CC=arm-frc${YEAR}-linux-gnueabi-gcc CXX=arm-frc${YEAR}-linux-gnueabi-g++ \
	&& cd /tmp/protobuf-3.18.0/src \
	&& make -j4 \
	&& chown -R `id -u`:`id -g` .libs/libprotobuf.so \
	&& arm-frc${YEAR}-linux-gnueabi-strip .libs/libprotobuf.so \
	&& cp .libs/libprotobuf.so /usr/local/arm-frc2022-linux-gnueabi/lib/libprotobuf.so \
	&& cd /tmp \
	&& rm -Rf /tmp/*

WORKDIR /tmp
RUN git clone https://github.com/frcteam195/CKRIOPassthru \
	&& cd /tmp/CKRIOPassthru \
	&& ./gradlew \
	&& ./gradlew --stop \
	&& cd /tmp \
	&& rm -Rf /tmp/*

WORKDIR /build
SHELL ["/bin/bash", "-c"]
