# -----------------------------------------------------------------------------
# Stages
# -----------------------------------------------------------------------------

ARG BASE_BUILDER_IMAGE=debian:11.6-slim@sha256:98d3b4b0cee264301eb1354e0b549323af2d0633e1c43375d0b25c01826b6790
ARG BASE_IMAGE=senzing/senzingapi-runtime:3.4.0

# -----------------------------------------------------------------------------
# Stage: builder
# -----------------------------------------------------------------------------

FROM ${BASE_BUILDER_IMAGE} as builder

ENV REFRESHED_AT=2023-01-12

LABEL Name="senzing/data-encryption-aes256cbc-sample-builder" \
      Maintainer="support@senzing.com" \
      Version="1.0.5"

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

ENV REFRESHED_AT 2022-10-05

LABEL Name="senzing/data-encryption-aes256cbc-sample" \
      Maintainer="support@senzing.com" \
      Version="1.0.5"

# Copy files from prior step.

COPY --from=builder "/src/dist/lib/libg2EncryptDataAES256CBC.so" "/results/libg2EncryptDataAES256CBC.so"
COPY --from=builder "/src/dist/lib/libg2EncryptDataClearText.so" "/results/libg2EncryptDataClearText.so"

# Output will be in /results
