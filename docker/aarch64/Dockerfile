FROM ubuntu:18.04
ARG YEAR=2022
RUN apt-get update && apt-get install -y apt-transport-https \
    ca-certificates \
    gnupg \
    software-properties-common \
    wget && \
    wget -O - https://apt.kitware.com/keys/kitware-archive-latest.asc 2>/dev/null | gpg --dearmor - | tee /etc/apt/trusted.gpg.d/kitware.gpg >/dev/null && \
    apt-add-repository 'deb https://apt.kitware.com/ubuntu/ bionic main' && \
    apt-get update && apt-get install -y tzdata && apt-get install -y \
    build-essential \
    ca-certificates \
    clang-format-10 \
    cmake \
    curl \
    fakeroot \
    g++-8 --no-install-recommends \
    gcc-8 \
    gdb \
    git \
    git-lfs \
    java-common \
    libc6-dev \
    libgl1-mesa-dev \
    libglu1-mesa-dev \
    libisl15 \
    libopencv-dev \
    libvulkan-dev \
    libx11-dev \
    libxcursor-dev \
    libxi-dev \
    libxinerama-dev \
    libxrandr-dev \
    make \
    mesa-common-dev \
    openjdk-11-jdk \
    python-all-dev \
    python3-dev \
    python3-pip \
    python3-setuptools \
    sudo \
    unzip \
    wget \
    zip \
  && rm -rf /var/lib/apt/lists/*

RUN update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-8 800 --slave /usr/bin/g++ g++ /usr/bin/g++-8

ENV JAVA_HOME /usr/lib/jvm/java-11-openjdk-arm64

WORKDIR /tmp
RUN apt-get update \
	&& apt-get upgrade -y \
	&& apt-get install -y apt-utils software-properties-common wget dh-autoreconf libcurl4-gnutls-dev libexpat1-dev gettext libz-dev libssl-dev git git-lfs \
		asciidoc xmlto docbook2x install-info \
	&& wget https://www.kernel.org/pub/software/scm/git/git-2.34.1.tar.gz \
	&& tar -xzvf git-2.34.1.tar.gz \
	&& cd git-2.34.1 \
	&& ./configure --prefix=/usr \
	&& make -j$(($(nproc)-1)) \
	&& make install install-info \
	&& cd /tmp \
	&& mkdir gitlfs \
	&& cd /tmp/gitlfs \
	&& wget https://github.com/git-lfs/git-lfs/releases/download/v3.0.2/git-lfs-linux-arm64-v3.0.2.tar.gz \
	&& tar -xzvf git-lfs-linux-arm64-v3.0.2.tar.gz \
	&& PREFIX=/usr ./install.sh \
	&& cd /tmp \
	&& apt-mark hold git \
	&& apt-mark hold git-lfs \
	&& rm -Rf /tmp/* \
	&& rm -rf /var/lib/apt/lists/*

WORKDIR /


# Install toolchain
RUN curl -SL https://github.com/frcteam195/roborio-toolchain/releases/download/v${YEAR}-1/FRC-${YEAR}-Linux-arm64-Toolchain-7.3.0.tar.gz | sh -c 'mkdir -p /usr/local && cd /usr/local && tar xzf - --strip-components=2'

WORKDIR /


RUN apt-get update && apt-get install -y \
    autoconf \
    build-essential \
    ca-certificates \
    cmake \
    curl \
    file \
    g++ --no-install-recommends \
    gcc \
    gdb \
    java-common \
    libc6-dev \
    libcups2-dev \
    libfontconfig1-dev \
    libfreetype6-dev \
    libisl15 \
    libpython2.7 \
    libx11-dev \
    libxext-dev \
    libxrandr-dev \
    libxrender-dev \
    libxtst-dev \
    libxt-dev \
    make \
    mercurial \
    unzip \
    wget \
    zip

WORKDIR /tmp
RUN wget https://github.com/wpilibsuite/frc-openjdk-roborio/raw/2022/arm-x11-files.tar.xz \
	&& cat arm-x11-files.tar.xz | sh -c "cd /usr/local/arm-frc${YEAR}-linux-gnueabi && tar xJf -" \
	&& rm -Rf arm-x11-files.tar.xz

RUN wget \
    https://download.ni.com/ni-linux-rt/feeds/2019/arm/cortexa9-vfpv3/alsa-lib-dev_1.1.5-r0.6_cortexa9-vfpv3.ipk \
    https://download.ni.com/ni-linux-rt/feeds/2019/arm/cortexa9-vfpv3/alsa-server_1.1.5-r0.6_cortexa9-vfpv3.ipk \
    https://download.ni.com/ni-linux-rt/feeds/2019/arm/cortexa9-vfpv3/cups-dev_2.2.6-r0.14_cortexa9-vfpv3.ipk \
    https://download.ni.com/ni-linux-rt/feeds/2019/arm/cortexa9-vfpv3/libasound2_1.1.5-r0.6_cortexa9-vfpv3.ipk \
    https://download.ni.com/ni-linux-rt/feeds/2019/arm/cortexa9-vfpv3/libfontconfig-dev_2.12.6-r0.6_cortexa9-vfpv3.ipk \
    https://download.ni.com/ni-linux-rt/feeds/2019/arm/cortexa9-vfpv3/libfontconfig1_2.12.6-r0.6_cortexa9-vfpv3.ipk \
    https://download.ni.com/ni-linux-rt/feeds/2019/arm/cortexa9-vfpv3/libfreetype-dev_2.9-r0.6_cortexa9-vfpv3.ipk \
    https://download.ni.com/ni-linux-rt/feeds/2019/arm/cortexa9-vfpv3/libfreetype6_2.9-r0.6_cortexa9-vfpv3.ipk \
    https://download.ni.com/ni-linux-rt/feeds/2019/arm/cortexa9-vfpv3/libz1_1.2.11-r0.71_cortexa9-vfpv3.ipk \
	&& for f in *.ipk; do \
		ar p $f data.tar.gz | sh -c 'cd /usr/local/arm-frc${YEAR}-linux-gnueabi && tar xzf -'; \
	done \
	&& rm *.ipk

# Copy over a couple missing headers for building JRE
RUN cp -n /usr/include/X11/extensions/Xrandr.h /usr/local/arm-frc${YEAR}-linux-gnueabi/usr/include/X11/extensions/ \
	&& cp -n /usr/include/X11/extensions/randr.h /usr/local/arm-frc${YEAR}-linux-gnueabi/usr/include/X11/extensions/

WORKDIR /tmp
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
