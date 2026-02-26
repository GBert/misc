# Usage:
#  docker build -t srseii-sdk:bookworm-21.02-buildbase
#
#  docker run -it --rm srseii-sdk:bookwirm-21.02-buildbase
#
# Environment variables are set up for Cross-Compileing using the OpenWRT SDK Toolchain.
#

FROM srseii-openwrt:bookworm-21.02-buildbase

COPY openwrt-sdk-ramips-mt76x8_gcc-8.4.0_musl.Linux-x86_64.tar.xz /tmp/openwrt-sdk.tar.xz
RUN cd /openwrt && tar -xJf /tmp/openwrt-sdk.tar.xz

ENV STAGING_DIR=/openwrt/openwrt-sdk-ramips-mt76x8_gcc-8.4.0_musl.Linux-x86_64/staging_dir/
ENV CC=/openwrt/openwrt-sdk-ramips-mt76x8_gcc-8.4.0_musl.Linux-x86_64/staging_dir/toolchain-mipsel_24kc_gcc-8.4.0_musl/bin/mipsel-openwrt-linux-gcc
ENV CXX=/openwrt/openwrt-sdk-ramips-mt76x8_gcc-8.4.0_musl.Linux-x86_64/staging_dir/toolchain-mipsel_24kc_gcc-8.4.0_musl/bin/mipsel-openwrt-linux-g++
ENV LD=/openwrt/openwrt-sdk-ramips-mt76x8_gcc-8.4.0_musl.Linux-x86_64/staging_dir/toolchain-mipsel_24kc_gcc-8.4.0_musl/bin/mipsel-openwrt-linux-ld
