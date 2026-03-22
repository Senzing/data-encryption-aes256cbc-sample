# -----------------------------------------------------------------------------
# Stages
# -----------------------------------------------------------------------------

ARG BASE_BUILDER_IMAGE=debian:13.3-slim@sha256:1d3c811171a08a5adaa4a163fbafd96b61b87aa871bbc7aa15431ac275d3d430
ARG BASE_IMAGE=senzing/senzingapi-runtime:3.13.0@sha256:c9c3502b35fbcc30d3cdbe3597392f964c7a15db52736dac938d28916d121f70

# -----------------------------------------------------------------------------
# Stage: builder
# -----------------------------------------------------------------------------

FROM ${BASE_BUILDER_IMAGE} AS builder

ENV REFRESHED_AT=2026-01-05

# Install packages via apt.

RUN apt-get update \
  && apt-get -y --no-install-recommends install \
  librdkafka-dev \
  cmake \
  build-essential \
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

ENV REFRESHED_AT=2026-01-05

LABEL Name="senzing/data-encryption-aes256cbc-sample" \
  Maintainer="support@senzing.com" \
  Version="2.0.4"

USER 1001

# Copy files from prior step.

COPY --from=builder "/src/dist/lib/libg2EncryptDataAES256CBC.so" "/results/libg2EncryptDataAES256CBC.so"
COPY --from=builder "/src/dist/lib/libg2EncryptDataClearText.so" "/results/libg2EncryptDataClearText.so"

# Output will be in /results
