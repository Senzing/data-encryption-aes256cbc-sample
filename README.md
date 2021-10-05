# data-encryption-aes256cbc-sample

## Synopsis

Sample encryption plugins for a G2 data repository.

## Overview

### Contents

1. [Preamble](#preamble)
    1. [Legend](#legend)
1. [Expectations](#expectations)
1. [Setting Up G2 Data Repository Encryption](#setting-up-g2-data-repository-encryption)
1. [Develop](#develop)
    1. [Prerequisites for development](#prerequisites-for-development)
    1. [Clone repository](#clone-repository)
    1. [Build the plugins](#build-the-plugins)
        1. [Linux](#linux)
        1. [Mac OS](#mac-os)
        1. [Windows](#windows)
1. [Errors](#errors)
1. [References](#references)

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

## Setting Up G2 Data Repository Encryption

Data in the G2 engine repository may be encrypted using an external encryption plugin.
This allows for users to decide when and how to encrypt their sensitive data.

A data repository may be set up for encryption, whether it has data loaded into it previously or not.
If data has been already loaded, then the data repository must be encrypted
with the utility program prior to using the system.

### General Setup Steps

These are the general steps for setting up G2 data encryption.
More specific details are given below.

1. Create an encryption plugin executable, for use in encrypting/decrypting data.
1. Configure the engine to use encryption
1. Encrypt the data already in the datastore
1. Use the G2 engine normally

### Creating an encryption plugin library

In order to use encrypted data in the data repository,
the engine must be configured with an executable library with encryption/decryption methods defined.  
The user may use an existing library, or create their own.  
In this way, they may use any kind of encryption/decryption algorithm they choose.

To create such a library, a program must be compiled that implements the G2 standard encryption interface.
That interface is available on
[GitHub](https://github.com/Senzing/senzing-data-encryption-specification).  
The actual interface header file is located at
[g2EncryptionPluginInterface.h](https://github.com/Senzing/senzing-data-encryption-specification/blob/main/src/interface/g2EncryptionPluginInterface.h).

The source code for a sample library is available on
[GitHub](https://github.com/Senzing/data-encryption-AES256CBC-sample).  
This library may be compiled on any operating system that G2 supports.  
When compiled, it will create two libraries: a simple cleartext demonstration library,
and an encryption library based on the AES-256 cipher block chain.

The source code for these two plugins is meant as an example of how to create encryption plugins.  
For ideal security, a new plugin should be created which implements the security and data standards of the user's organization.

### Configuring the engine to use encryption

To enable encryption on the data repository,
the engine must be set up with the encryption library in place.  
To do so...

1. Copy the encryption library into the "lib" folder of the G2 installation.
   Alternately, if you wish to have the library in a separate location,
   put the new location on your system path, so that the engine may find the library.
1. Add needed parameters to your engine startup parameters.  
   This will tell the engine what plugin to use, what encryption keys to use, etc...  
   For example, the "AES-256-CBC" sample plugin requires the following parameters in its INI setup.  
   Note:  The parameter names may be different, depending on what kind of encryption library you are using.  
   The parameters that are in the "DATA_ENCRYPTION" group will be made available to the encryption plugin,
   and they may be accessed as demonstrated in the source code for the "AES-256-CBC" sample plugin.

1. Example:

    ```json
    {
        "PIPELINE": {
            "SUPPORTPATH": "/home/username/senzing/data",
            "CONFIGPATH": "/home/username/senzing/etc",
            "RESOURCEPATH": "/home/username/senzing/resources"
        },
        "SQL": {
            "CONNECTION": "sqlite3://na:na@/home/username/senzing/var/sqlite/G2C.db"
        },
        "DATA_ENCRYPTION": {
            "ENCRYPTION_PLUGIN_NAME": "g2EncryptDataAES256CBC",
            "ENCRYPTION_KEY": "68402346802394406802620602396369",
            "ENCRYPTION_INITIALIZATION_VECTOR": "6432072349624624"
        }
    }
    ```

1. With the library in place, and the encryption parameters set,
   the G2 engine is now ready to encrypt/decrypt data.

### Encrypting the data in the repository

If data has been previously loaded into the data repository,
then the data repository must be encrypted using the "g2db2encrypt" application.  
This program will step through the data repository, encrypting the sensitive contents of the repository.  
It can also be used to decrypt a repository.

1. Command to Encrypt

    ```console
    ./bin/g2dbencrypt -c etc/G2Module.ini -e
    ```

1. Command to Decrypt:  

    ```console
    ./bin/g2dbencrypt -c etc/G2Module.ini -e
    ```

Note:  If you are changing the method of encryption from one form to another,
you must entirely decrypt the repository using the configuration for the old encryption method,
and then encrypt it using the configuration for the new method.

### Using the engine with the encrypted data repository

Once the data repository has been encrypted,
the engine can be used in the same manner as with unencrypted.  
There should be no external indications from the API that the encryption was done.  
The data will be seamlessly protected.

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
    export GIT_REPOSITORY=data-encryption-aes256cbc-sample
    export GIT_ACCOUNT_DIR=~/${GIT_ACCOUNT}.git
    export GIT_REPOSITORY_DIR="${GIT_ACCOUNT_DIR}/${GIT_REPOSITORY}"
    ```

1. Using the environment variables values just set, follow steps in [clone-repository](https://github.com/Senzing/knowledge-base/blob/master/HOWTO/clone-repository.md) to install the Git repository.

### Build the plugins

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
        docker run \
          --interactive \
          --rm \
          --tty \
          --volume ${MY_OUTPUT_DIR}:/output \
          senzing/data-encryption-aes256cbc-sample \
            cp /src/dist/lib/libg2EncryptDataAES256CBC.so /output/
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

## Errors

1. See [docs/errors.md](docs/errors.md).

## References
