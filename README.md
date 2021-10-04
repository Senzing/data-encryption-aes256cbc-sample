# data-encryption-aes256cbc-sample

## Synopsis

Sample encryption plugins for a G2 data repository.

## Overview

### Contents

1. [Preamble](#preamble)
    1. [Legend](#legend)
1. [Expectations](#expectations)
1. [Develop]()

## Preamble

At [Senzing](http://senzing.com),
we strive to create GitHub documentation in a
"[don't make me think](https://github.com/Senzing/knowledge-base/blob/master/WHATIS/dont-make-me-think.md)" style.
For the most part, instructions are copy and paste.
Whenever thinking is needed, it's marked with a "thinking" icon :thinking:.
Whenever customization is needed, it's marked with a "pencil" icon :pencil2:.
If the instructions are not clear, please let us know by opening a new
[Documentation issue](https://github.com/Senzing/template-python/issues/new?template=documentation_request.md)
describing where we can improve.   Now on with the show...

### Legend

1. :thinking: - A "thinker" icon means that a little extra thinking may be required.
   Perhaps there are some choices to be made.
   Perhaps it's an optional step.
1. :pencil2: - A "pencil" icon means that the instructions may need modification before performing.
1. :warning: - A "warning" icon means that something tricky is happening, so pay attention.

## Expectations

- **Space:** This repository and demonstration require 6 GB free disk space.
- **Time:** Budget 40 minutes to get the demonstration up-and-running, depending on CPU and network speeds.
- **Background knowledge:** This repository assumes a working knowledge of:
  - [Docker](https://github.com/Senzing/knowledge-base/blob/master/WHATIS/docker.md)

## Develop

The following instructions are used when modifying and building the Docker image.

### Prerequisites for development

:thinking: The following tasks need to be complete before proceeding.
These are "one-time tasks" which may already have been completed.

1. The following software programs need to be installed:
    1. [git](https://github.com/Senzing/knowledge-base/blob/master/HOWTO/install-git.md)
    1. [make](https://github.com/Senzing/knowledge-base/blob/master/HOWTO/install-make.md)
    1. [docker](https://github.com/Senzing/knowledge-base/blob/master/HOWTO/install-docker.md)

### Clone repository

For more information on environment variables,
see [Environment Variables](https://github.com/Senzing/knowledge-base/blob/master/lists/environment-variables.md).

1. Set these environment variable values:

    ```console
    export GIT_ACCOUNT=senzing
    export GIT_REPOSITORY=data-encryption-AES256CBC-sample
    export GIT_ACCOUNT_DIR=~/${GIT_ACCOUNT}.git
    export GIT_REPOSITORY_DIR="${GIT_ACCOUNT_DIR}/${GIT_REPOSITORY}"
    ```

1. Using the environment variables values just set, follow steps in [clone-repository](https://github.com/Senzing/knowledge-base/blob/master/HOWTO/clone-repository.md) to install the Git repository.

### Building the plugins

#### Linux

1. Using command line.
   Example:

    ```console
    cd ${GIT_REPOSITORY_DIR}/src
    cmake -DCMAKE_BUILD_TYPE=Release setup .
    make all
    make install
    ```

1. Using docker.
   Example:

    ```console
    cd ${GIT_REPOSITORY_DIR}
    make package
    ```

    The newly created binaries will be in the `${GIT_REPOSITORY_DIR}/target` directory.

1. Alternative docker.
   Example:

    1. :pencil2: Define where to put the files.
       Example:

        ```console
        export MY_OUTPUT_DIR=/tmp
        ```

    1. Extract the files to the output directory.
       Example:

        ```console
        sudo docker run \
          --interactive \
          --rm \
          --tty \
          --volume ${MY_OUTPUT_DIR}:/output \
          senzing/data-encryption-aes256cbc-sample \
            cp /src/dist/lib/* /output/
        ```

#### Mac OS

1. Using command line.
   Example:

    ```console
    cd ${GIT_REPOSITORY_DIR}/src
    export CC=clang
    export CXX=clang++
    cmake -DCMAKE_BUILD_TYPE=Release setup .
    gmake all
    gmake install
    ```

#### Windows

1. Using command.
   Example:

    ```console
    cd %GIT_REPOSITORY_DIR%/src
    cmake -DCMAKE_BUILD_TYPE=Release -G "Visual Studio 14 2015" -A x64 setup .
    ```

1. Build the project through visual studio, or via the command line.
