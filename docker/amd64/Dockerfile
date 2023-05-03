FROM ubuntu:22.04
ARG YEAR=2023

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

RUN curl -SL https://github.com/wpilibsuite/opensdk/releases/download/v2023-8/cortexa9_vfpv3-roborio-academic-2023-x86_64-linux-gnu-Toolchain-12.1.0.tgz | sh -c 'mkdir -p /usr/local && cd /usr/local && tar xzf - --strip-components=2'

ARG PROTO_TAG=21.12
ARG PROTO_VER=3.21.12

RUN wget https://github.com/protocolbuffers/protobuf/releases/download/v${PROTO_TAG}/protobuf-cpp-${PROTO_VER}.tar.gz \
	&& tar -xzf protobuf-cpp-${PROTO_VER}.tar.gz \
	&& cd /tmp/protobuf-${PROTO_VER} \
	&& ./configure \
	&& make -j4 && make install \
	&& cd /tmp \
	&& rm -Rf /tmp/* \
	&& ldconfig

WORKDIR /tmp
RUN curl -SLO https://github.com/zeromq/libzmq/releases/download/v4.3.4/zeromq-4.3.4.tar.gz \
	&& tar -xzf zeromq-4.3.4.tar.gz \
	&& cd /tmp/zeromq-4.3.4 \
	&& export CFLAGS="-Wno-error" \
	&& export CXXFLAGS="-Wno-error" \
	&& ./configure --enable-drafts --host=arm-frc${YEAR}-linux-gnueabi CC=arm-frc${YEAR}-linux-gnueabi-gcc CXX=arm-frc${YEAR}-linux-gnueabi-g++ \
	&& make -j4 \
	&& chown -R `id -u`:`id -g` src/.libs/libzmq.so \
	&& arm-frc${YEAR}-linux-gnueabi-strip src/.libs/libzmq.so \
	&& cp src/.libs/libzmq.so /usr/local/arm-nilrt-linux-gnueabi/sysroot/lib/libzmq.so \
	&& cd /tmp \
	&& rm -Rf /tmp/* 

RUN wget https://download.ni.com/ni-linux-rt/feeds/2019/arm/cortexa9-vfpv3/libz1_1.2.11-r0.71_cortexa9-vfpv3.ipk \
	https://download.ni.com/ni-linux-rt/feeds/2019/arm/cortexa9-vfpv3/libz-dev_1.2.11-r0.71_cortexa9-vfpv3.ipk \
	https://download.ni.com/ni-linux-rt/feeds/2019/arm/cortexa9-vfpv3/libz-staticdev_1.2.11-r0.71_cortexa9-vfpv3.ipk \
	&& for f in *.ipk; do \
	ar p $f data.tar.gz | sh -c "cd /usr/local/arm-nilrt-linux-gnueabi/sysroot && tar xzf -"; \
	done \
	&& rm -Rf /tmp/*

ARG RR_TOOLCHAIN=/tmp/roboriotoolchain.cmake

RUN echo "set(CMAKE_SYSTEM_NAME Linux)" > ${RR_TOOLCHAIN} \
	&& echo "set(CMAKE_SYSTEM_PROCESSOR armv7)" >> ${RR_TOOLCHAIN} \
	&& echo "set(CMAKE_CXX_COMPILER /usr/local/bin/arm-frc${YEAR}-linux-gnueabi-g++ CACHE FILEPATH riocxx)" >> ${RR_TOOLCHAIN} \
	&& echo "set(CMAKE_C_COMPILER /usr/local/bin/arm-frc${YEAR}-linux-gnueabi-gcc CACHE FILEPATH rioc)" >> ${RR_TOOLCHAIN} \
	&& echo "set(CMAKE_LINKER /usr/local/bin/arm-frc${YEAR}-linux-gnueabi-ld CACHE FILEPATH riold)" >> ${RR_TOOLCHAIN} \
	&& echo "set(CMAKE_AR /usr/local/bin/arm-frc${YEAR}-linux-gnueabi-ar CACHE FILEPATH rioar)" >> ${RR_TOOLCHAIN} \
	&& echo "set(CMAKE_RANLIB /usr/local/bin/arm-frc${YEAR}-linux-gnueabi-ranlib CACHE FILEPATH rioranlib)" >> ${RR_TOOLCHAIN} \
	&& echo "set(CMAKE_STRIP /usr/local/bin/arm-frc${YEAR}-linux-gnueabi-strip CACHE FILEPATH riostrip)" >> ${RR_TOOLCHAIN} \
	&& echo "set(CMAKE_READELF /usr/local/bin/arm-frc${YEAR}-linux-gnueabi-readelf CACHE FILEPATH rioreadelf)" >> ${RR_TOOLCHAIN}


WORKDIR /tmp
RUN curl -SLO https://github.com/protocolbuffers/protobuf/releases/download/v${PROTO_TAG}/protobuf-cpp-${PROTO_VER}.tar.gz \
	&& tar -xzf protobuf-cpp-${PROTO_VER}.tar.gz \
	&& cd /tmp/protobuf-${PROTO_VER} \
	&& ./configure --host=arm-frc${YEAR}-linux-gnueabi CC=arm-frc${YEAR}-linux-gnueabi-gcc CXX=arm-frc${YEAR}-linux-gnueabi-g++ \
	&& cd /tmp/protobuf-${PROTO_VER}/src \
	&& make -j4 \
	&& chown -R `id -u`:`id -g` .libs/libprotobuf.so \
	&& arm-frc${YEAR}-linux-gnueabi-strip .libs/libprotobuf.so \
	&& cp .libs/libprotobuf.so /usr/local/arm-nilrt-linux-gnueabi/sysroot/lib/libprotobuf.so \
	&& cd /tmp 
#	&& rm -Rf /tmp/*


#ARG PROTO_VER=22.3.0
#WORKDIR /tmp
#RUN git clone https://github.com/protocolbuffers/protobuf.git \
#	&& cd /tmp/protobuf \
#	&& git checkout 482156c0f96dd670b9f1ba4f946e66edeb04535f \
#	&& cd third_party \
#	&& rm -Rf abseil-cpp \
#	&& git clone https://github.com/abseil/abseil-cpp.git \
#	&& cd abseil-cpp \
#	&& git checkout b971ac5250ea8de900eae9f95e06548d14cd95fe \
#	&& mkdir cppbuild \
#	&& cd cppbuild \
#       && cmake .. -DCMAKE_CXX_FLAGS=-std=c++17 -DCMAKE_POSITION_INDEPENDENT_CODE=ON -DCMAKE_TOOLCHAIN_FILE=/tmp/roboriotoolchain.cmake -DBUILD_SHARED_LIBS=OFF -DABSL_BUILD_TESTING=OFF -DCMAKE_BUILD_TYPE="Release" -DCMAKE_CXX_STANDARD=17 -DCMAKE_INSTALL_PREFIX=/tmp/abseil-cpp \
#	&& cmake --build . --target install \
#	&& cd /tmp/protobuf \
#	&& mkdir cppbuild \
#	&& cd /tmp/protobuf/cppbuild \
#	&& cmake .. -DCMAKE_CXX_FLAGS=-std=c++17 -Dprotobuf_BUILD_TESTS=OFF -DCMAKE_BUILD_TYPE="Release" -Dprotobuf_ABSL_PROVIDER=module -DCMAKE_POSITION_INDEPENDENT_CODE=ON -DCMAKE_TOOLCHAIN_FILE=/tmp/roboriotoolchain.cmake -Dprotobuf_BUILD_SHARED_LIBS=ON -Dprotobuf_WITH_ZLIB=ON  -DZLIB_LIBRARY=/usr/local/arm-nilrt-linux-gnueabi/sysroot/lib/libz.so.1.2.11 -DZLIB_INCLUDE_DIR=/usr/local/arm-nilrt-linux-gnueabi/sysroot/usr/include \
#	&& make -j4 CROSS_COMPILE_TARGET=yes \
#	&& chown -R `id -u`:`id -g` ./libprotobuf.so \
#	&& arm-frc${YEAR}-linux-gnueabi-strip ./libprotobuf.so \
#	&& cp ./libprotobuf.so /usr/local/arm-nilrt-linux-gnueabi/sysroot/lib/libprotobuf.so \
#	&& cp ./libprotobuf.so.${PROTO_VER} /usr/local/arm-nilrt-linux-gnueabi/sysroot/lib/libprotobuf.so.${PROTO_VER} \
#	&& cd /tmp 
#	&& rm -Rf /tmp/*

WORKDIR /tmp
RUN git clone https://github.com/frcteam195/CKRIOPassthru \
	&& cd /tmp/CKRIOPassthru \
	&& ./gradlew \
	&& ./gradlew --stop \
	&& cd /tmp 
#	&& rm -Rf /tmp/*

WORKDIR /build
SHELL ["/bin/bash", "-c"]
