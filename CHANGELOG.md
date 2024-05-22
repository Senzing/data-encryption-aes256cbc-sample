# Changelog

All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
[markdownlint](https://dlaa.me/markdownlint/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [2.0.3] - 2024-05-22

### Changed in 2.0.3

- In `Dockerfile`,
  - `debian:11.9-slim@sha256:0e75382930ceb533e2f438071307708e79dc86d9b8e433cc6dd1a96872f2651d`
  - `senzing/senzingapi-runtime:3.10.1`

## [2.0.2] - 2024-02-05

### Changed in 2.0.2

- Add data reverse-text plugin.

## [2.0.1] - 2023-10-13

### Changed in 2.0.1

- Update to most recent API specification submodule.
- Add documentation.

## [2.0.0] - 2023-10-11

### Changed in 2.0.0

- Update to [senzing-data-encryption-specification](https://github.com/Senzing/senzing-data-encryption-specification) version 1.0.0
- Add in deterministic versions of the encryption/decryption functions.

## [1.0.9] - 2023-09-29

### Changed in 1.0.9

- In `Dockerfile`,
  - `debian:11.7-slim@sha256:c618be84fc82aa8ba203abbb07218410b0f5b3c7cb6b4e7248fda7785d4f9946`
  - `senzing/senzingapi-runtime:3.7.1`

## [1.0.8] - 2023-05-11

### Changed in 1.0.8

- In `Dockerfile`,
  - `debian:11.7-slim@sha256:f4da3f9b18fc242b739807a0fb3e77747f644f2fb3f67f4403fafce2286b431a`
  - `senzing/senzingapi-runtime:3.5.2`

## [1.0.7] - 2023-04-03

### Changed in 1.0.7

- In `Dockerfile`,
  - `debian:11.6-slim@sha256:7acda01e55b086181a6fa596941503648e423091ca563258e2c1657d140355b1`
  - `senzing/senzingapi-runtime:3.5.0`

## [1.0.6] - 2023-01-12

### Changed in 1.0.6

- In `Dockerfile`,
  - `debian:11.6-slim@sha256:98d3b4b0cee264301eb1354e0b549323af2d0633e1c43375d0b25c01826b6790`
  - `senzing/senzingapi-runtime:3.4.0`

## [1.0.5] - 2022-10-11

### Changed in 1.0.5

- In `Dockerfile`, updated FROM instruction to `senzing/senzingapi-runtime:3.3.1`

## [1.0.4] - 2022-10-05

### Changed in 1.0.4

- In `Dockerfile`, updated FROM instruction to `senzing/senzingapi-runtime:3.3.0`

## [1.0.3] - 2022-09-29

### Changed in 1.0.3

- In `Dockerfile`, updated FROM instruction to `busybox:1.34.1@sha256:ad9bd57a3a57cc95515c537b89aaa69d83a6df54c4050fcf2b41ad367bec0cd5`

## [1.0.2] - 2021-10-30

### Changed to 1.0.2

- Updated Debian version 10.10

## [1.0.1] - 2021-10-05

### Changed to 1.0.1

- Created multi-stage docker build, using `busybox` in final image

## [1.0.0] - 2021-10-04

### Added to 1.0.0

- Initial example
