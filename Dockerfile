# -----------------------------------------------------------------------------
# Stages
# -----------------------------------------------------------------------------

ARG BASE_BUILDER_IMAGE=debian:11.3-slim@sha256:06a93cbdd49a265795ef7b24fe374fee670148a7973190fb798e43b3cf7c5d0f
ARG BASE_IMAGE=busybox:1.34.1@sha256:ebadf81a7f2146e95f8c850ad7af8cf9755d31cdba380a8ffd5930fba5996095

# -----------------------------------------------------------------------------
# Stage: builder
# -----------------------------------------------------------------------------

FROM ${BASE_BUILDER_IMAGE} as builder

ENV REFRESHED_AT=2022-05-16

LABEL Name="senzing/data-encryption-aes256cbc-sample-builder" \
      Maintainer="support@senzing.com" \
      Version="1.0.2"

# Install packages via apt.

RUN apt-get update \
 && apt-get -y install \
      librdkafka-dev \
      cmake \
      libssl-dev \
 && rm -rf /var/lib/apt/lists/*

# Copy files from repository.

COPY ./src /src

# Build code.

WORKDIR /src
RUN cmake -DCMAKE_BUILD_TYPE=Release setup . \
 && make all \
 && make install

# Output will be in:
#  - /src/dist/lib/libg2EncryptDataAES256CBC.so
#  - /src/dist/lib/libg2EncryptDataClearText.so

# -----------------------------------------------------------------------------
# Stage: final
# -----------------------------------------------------------------------------

FROM ${BASE_IMAGE} as final

ENV REFRESHED_AT 2021-10-05

LABEL Name="senzing/data-encryption-aes256cbc-sample" \
      Maintainer="support@senzing.com" \
      Version="1.0.1"

# Copy files from prior step.

COPY --from=builder "/src/dist/lib/libg2EncryptDataAES256CBC.so" "/results/libg2EncryptDataAES256CBC.so"
COPY --from=builder "/src/dist/lib/libg2EncryptDataClearText.so" "/results/libg2EncryptDataClearText.so"

# Output will be in /results
