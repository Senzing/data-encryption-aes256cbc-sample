# -----------------------------------------------------------------------------
# Stages
# -----------------------------------------------------------------------------

ARG BASE_BUILDER_IMAGE=debian:11.11-slim@sha256:6d3c63184632046054ae709964befc943ecffa140adc697ca955a10002a79c08
ARG BASE_IMAGE=senzing/senzingapi-runtime:3.10.3

# -----------------------------------------------------------------------------
# Stage: builder
# -----------------------------------------------------------------------------

FROM ${BASE_BUILDER_IMAGE} AS builder

ENV REFRESHED_AT=2024-06-24

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

HEALTHCHECK CMD [[ -f /src/dist/lib/libg2EncryptDataAES256CBC.so ]] || exit 1

# Output will be in:
#  - /src/dist/lib/libg2EncryptDataAES256CBC.so
#  - /src/dist/lib/libg2EncryptDataClearText.so

# -----------------------------------------------------------------------------
# Stage: final
# -----------------------------------------------------------------------------

FROM ${BASE_IMAGE} AS final

ENV REFRESHED_AT=2024-06-24

LABEL Name="senzing/data-encryption-aes256cbc-sample" \
  Maintainer="support@senzing.com" \
  Version="2.0.4"

USER 1001

# Copy files from prior step.

COPY --from=builder "/src/dist/lib/libg2EncryptDataAES256CBC.so" "/results/libg2EncryptDataAES256CBC.so"
COPY --from=builder "/src/dist/lib/libg2EncryptDataClearText.so" "/results/libg2EncryptDataClearText.so"

# Output will be in /results
