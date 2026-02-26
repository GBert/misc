# Usage:
#    docker build --target buildbase -t srseii-openwrt:bookworm-21.02-buildbase .
#    docker build -t srseii-openwrt:bookworm-21.02 .
#
#    docker run -it srseii-openwrt:bookworm-21.02 make -j9
#
FROM debian:bookworm AS buildbase

# TODO: Apple Silicon uses gcc-mips*-linux-gnu instead of gcc-multilib. The current Dockerfile cannot be built on ARM.

RUN apt-get update && apt-get install -y sudo build-essential gawk gcc-multilib g++-multilib flex git gettext libncurses5-dev libssl-dev zlib1g-dev unzip wget file rsync

ADD debian-stretch.list /etc/apt/sources.list.d/
RUN apt-get update && apt-get install -y python2.7-minimal
RUN rm /etc/apt/sources.list.d/debian-stretch.list

ADD https://www.python.org/ftp/python/3.10.0/Python-3.10.0.tgz /tmp
RUN cd /tmp && tar -xzf Python-3.10.0.tgz
RUN cd /tmp/Python-3.10.0 && ./configure --enable-optimizations
RUN cd /tmp/Python-3.10.0 && make -j9
RUN cd /tmp/Python-3.10.0 && make altinstall


RUN useradd -ms /bin/bash defaultuser
RUN echo "defaultuser ALL=(ALL) NOPASSWD: ALL" >> /etc/sudoers

RUN mkdir /openwrt && chown defaultuser /openwrt
USER defaultuser
WORKDIR /openwrt

FROM buildbase

RUN git clone -b openwrt-21.02 --single-branch --depth 1 https://github.com/openwrt/openwrt.git /openwrt
#RUN git clone https://github.com/openwrt/openwrt.git

# Add SRSEII Patch
COPY --chown=defaultuser patch_srseii.patch /openwrt/
COPY --chown=defaultuser --chmod=777 update-script-feeds.sh /openwrt/

RUN patch -p1 < patch_srseii.patch
RUN ./update-script-feeds.sh

COPY --chown=defaultuser config.txt /openwrt/.config
RUN scripts/feeds update -a && git pull
