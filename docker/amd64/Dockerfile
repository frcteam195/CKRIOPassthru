ARG UBUNTU=18.04
ARG YEAR=2021
FROM wpilib/roborio-cross-ubuntu:${YEAR}-${UBUNTU}
ARG YEAR
ARG GRADLERIOSRC=https://github.com/wpilibsuite/GradleRIO/archive/refs/tags/v2022.1.1.tar.gz

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
	&& cat arm-x11-files.tar.xz | sh -c "cd /usr/local/arm-frc${YEAR}-linux-gnueabi && tar xJf -"

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

RUN rm -Rf /tmp/*
WORKDIR /tmp
RUN wget ${GRADLERIOSRC}
RUN tar -xzf *.tar.gz
RUN cd GradleRIO* && cd examples/cpp && ./gradlew installRoboRioToolchain && ./gradlew wrapper
RUN rm -Rf /tmp/*

WORKDIR /build
