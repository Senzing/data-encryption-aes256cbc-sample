# -----------------------------------------------------------------------------
# Stages
# -----------------------------------------------------------------------------

ARG BASE_BUILDER_IMAGE=debian:11.7-slim@sha256:924df86f8aad741a0134b2de7d8e70c5c6863f839caadef62609c1be1340daf5
ARG BASE_IMAGE=senzing/senzingapi-runtime:3.6.0

# -----------------------------------------------------------------------------
# Stage: builder
# -----------------------------------------------------------------------------

FROM ${BASE_BUILDER_IMAGE} as builder

ENV REFRESHED_AT=2023-06-29

LABEL Name="senzing/data-encryption-aes256cbc-sample-builder" \
      Maintainer="support@senzing.com" \
      Version="1.0.6"

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

ENV REFRESHED_AT=2023-06-29

LABEL Name="senzing/data-encryption-aes256cbc-sample" \
      Maintainer="support@senzing.com" \
      Version="1.0.6"

# Copy files from prior step.

COPY --from=builder "/src/dist/lib/libg2EncryptDataAES256CBC.so" "/results/libg2EncryptDataAES256CBC.so"
COPY --from=builder "/src/dist/lib/libg2EncryptDataClearText.so" "/results/libg2EncryptDataClearText.so"

# Output will be in /results
