# -----------------------------------------------------------------------------
# Stages
# -----------------------------------------------------------------------------

ARG BASE_BUILDER_IMAGE=debian:13.1-slim@sha256:66b37a5078a77098bfc80175fb5eb881a3196809242fd295b25502854e12cbec
ARG BASE_IMAGE=senzing/senzingapi-runtime:3.13.0@sha256:edca155d3601238fab622a7dd86471046832328d21f71f7bb2ae5463157f6e10

# -----------------------------------------------------------------------------
# Stage: builder
# -----------------------------------------------------------------------------

FROM ${BASE_BUILDER_IMAGE} AS builder

ENV REFRESHED_AT=2024-06-24

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

ENV REFRESHED_AT=2024-06-24

LABEL Name="senzing/data-encryption-aes256cbc-sample" \
  Maintainer="support@senzing.com" \
  Version="2.0.4"

USER 1001

# Copy files from prior step.

COPY --from=builder "/src/dist/lib/libg2EncryptDataAES256CBC.so" "/results/libg2EncryptDataAES256CBC.so"
COPY --from=builder "/src/dist/lib/libg2EncryptDataClearText.so" "/results/libg2EncryptDataClearText.so"

# Output will be in /results
