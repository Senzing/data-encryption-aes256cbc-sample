# data-encryption-aes256cbc-sample

## Synopsis

Sample encryption plugins for a G2 data repository.

## Overview

### Contents

1. [Preamble]
   1. [Legend]
1. [Expectations]
1. [Setting Up G2 Data Repository Encryption]
   1. [General Setup Steps]
   1. [Creating an encryption plugin library]
   1. [Configuring the engine to use encryption]
   1. [Encrypting the data in the repository]
   1. [Using the engine with the encrypted data repository]
1. [Develop]
   1. [Prerequisites for development]
   1. [Clone repository]
   1. [Build the plugins]
      1. [Linux]
      1. [Mac OS]
      1. [Windows]
1. [Errors]
1. [References]

## Preamble

At [Senzing], we strive to create GitHub documentation in a
"[don't make me think]" style. For the most part, instructions are copy and paste.
Whenever thinking is needed, it's marked with a "thinking" icon :thinking:.
Whenever customization is needed, it's marked with a "pencil" icon :pencil2:.
If the instructions are not clear, please let us know by opening a new
[Documentation issue] describing where we can improve. Now on with the show...

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
  - [Docker]

## Setting Up G2 Data Repository Encryption

Data in the G2 engine repository may be encrypted using an external encryption plugin.
This allows for users to decide when and how to encrypt their sensitive data.

A data repository may be set up for encryption,
whether it has data loaded into it previously or not.
If data has been already loaded,
then the data repository must be encrypted with the utility program prior to using the system.

### General Setup Steps

These are the general steps for setting up G2 data encryption.

1. Create an encryption plugin executable, for use in encrypting/decrypting data
1. Configure the engine to use encryption
1. Encrypt the data already in the datastore
1. Use the G2 engine normally

More specific details follow.

### Creating an encryption plugin library

In order to use encrypted data in the data repository,
the engine must be configured with an executable library with encryption/decryption methods defined.  
The user may use an existing library, or create their own.  
In this way, they may use any kind of encryption/decryption algorithm they choose.

To create such a library,
a program must be compiled that implements the G2 standard encryption interface.
That interface is available in the
[senzing-data-encryption-specification].  
The actual interface header file is located at
[g2EncryptionPluginInterface.h].

The source code for a sample library is available on [GitHub].  
This library may be compiled on any operating system that G2 supports.  
When compiled, it will create two libraries:

1. A simple cleartext demonstration library
1. An encryption library based on the AES-256 cipher block chain.

The source code for these two plugins is meant as an example of how to create encryption plugins.  
For ideal security, a new plugin should be created which implements
the security and data standards of the user's organization.

### Configuring the engine to use encryption

To enable encryption on the data repository,
the engine must be set up with the encryption library in place.

Copy the encryption library into the `lib` folder of the G2 installation
(e.g. `/opt/senzing/g2/lib`).
Alternately, if you wish to have the library in a separate location,
put the new location on your system path, so that the engine may find the library.

Add needed parameters to your engine startup parameters.  
This will tell the engine what plugin to use, what encryption keys to use.
For example, the "AES-256-CBC" sample plugin requires the following parameters in its INI setup.

**Note:**  The parameter names may be different, depending on what kind of encryption library you are using.  
The parameters that are in the "DATA_ENCRYPTION" group will be made available to the encryption plugin,
and they may be accessed as demonstrated in the source code for the "AES-256-CBC" sample plugin.

Example:

```json
{
  "PIPELINE": {
    "SUPPORTPATH": "/opt/senzing/data",
    "CONFIGPATH": "/etc/opt/senzing",
    "RESOURCEPATH": "/opt/senzing/g2/resources"
  },
  "SQL": {
    "CONNECTION": "sqlite3://na:na@/var/opt/senzing/sqlite/G2C.db"
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
then the data repository must be encrypted using the `g2db2encrypt` application.  
This program will step through the data repository, encrypting the sensitive contents of the repository.  
It can also be used to decrypt a repository.

1. :pencil2: Identify the `g2` directory.
   Example:

   ```console
   export SENZING_G2_DIR=/opt/senzing/g2
   ```

1. Command to Encrypt.
   Example:

   ```console
   cd {SENZING_G2_DIR}
   ./bin/g2dbencrypt -c etc/G2Module.ini -e
   ```

1. Command to Decrypt.
   Example:

   ```console
   cd {SENZING_G2_DIR}
   ./bin/g2dbencrypt -c etc/G2Module.ini -d
   ```

**Note:**  If you are changing the method of encryption from one form to another,
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
   1. [git]
   1. [make]
   1. [docker]

### Clone repository

For more information on environment variables, see [Environment Variables].

1. Set these environment variable values:

   ```console
   export GIT_ACCOUNT=senzing
   export GIT_REPOSITORY=data-encryption-aes256cbc-sample
   export GIT_ACCOUNT_DIR=~/${GIT_ACCOUNT}.git
   export GIT_REPOSITORY_DIR="${GIT_ACCOUNT_DIR}/${GIT_REPOSITORY}"
   ```

1. Using the environment variables values just set, follow steps in [clone-repository] to install the Git repository.

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
          cp /results/libg2EncryptDataAES256CBC.so /output/
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

## Programming tips

1. Each plugin function has a "PREAMBLE" and "POSTAMBLE" macro. These should be included in your plugin function implementations. They help with operations such as error handling, buffer size checking, and so forth. They are meant to simplify the handling of the data being passed into and out of the plugin.

2. Each encrypt/decrypt call is provided with a memory buffer for returning result data. If that buffer is found to be too small, then the function should return the code value G2_ENCRYPTION_PLUGIN\_\_\_OUTPUT_BUFFER_SIZE_ERROR. This will signal the G2 engine to retry, using a larger data buffer. (See the PREAMBLE/POSTAMBLE macros to see how that return code is applied.)

## Errors

1. See [docs/errors.md].

## References

[Build the plugins]: #build-the-plugins
[Clone repository]: #clone-repository
[clone-repository]: https://github.com/Senzing/knowledge-base/blob/main/HOWTO/clone-repository.md
[Configuring the engine to use encryption]: #configuring-the-engine-to-use-encryption
[Creating an encryption plugin library]: #creating-an-encryption-plugin-library
[Develop]: #develop
[docker]: https://github.com/Senzing/knowledge-base/blob/main/WHATIS/docker.md
[Docker]: https://github.com/Senzing/knowledge-base/blob/main/WHATIS/docker.md
[docs/errors.md]: docs/errors.md
[Documentation issue]: https://github.com/Senzing/template-python/issues/new?template=documentation_request.md
[don't make me think]: https://github.com/Senzing/knowledge-base/blob/main/WHATIS/dont-make-me-think.md
[Encrypting the data in the repository]: #encrypting-the-data-in-the-repository
[Environment Variables]: https://github.com/Senzing/knowledge-base/blob/main/lists/environment-variables.md
[Errors]: #errors
[Expectations]: #expectations
[g2EncryptionPluginInterface.h]: https://github.com/Senzing/senzing-data-encryption-specification/blob/main/src/interface/g2EncryptionPluginInterface.h
[General Setup Steps]: #general-setup-steps
[git]: https://github.com/Senzing/knowledge-base/blob/main/WHATIS/git.md
[GitHub]: https://github.com/Senzing/data-encryption-AES256CBC-sample
[Legend]: #legend
[Linux]: #linux
[Mac OS]: #mac-os
[make]: https://github.com/Senzing/knowledge-base/blob/main/WHATIS/make.md
[Preamble]: #preamble
[Prerequisites for development]: #prerequisites-for-development
[References]: #references
[senzing-data-encryption-specification]: https://github.com/Senzing/senzing-data-encryption-specification
[Senzing]: https://senzing.com
[Setting Up G2 Data Repository Encryption]: #setting-up-g2-data-repository-encryption
[Using the engine with the encrypted data repository]: #using-the-engine-with-the-encrypted-data-repository
[Windows]: #windows
