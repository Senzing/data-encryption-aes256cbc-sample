
/**********************************************************************************
 © Copyright Senzing, Inc. 2020-2025
 The source code for this program is not published or otherwise divested
 of its trade secrets, irrespective of what has been deposited with the U.S.
 Copyright Office.
**********************************************************************************/


/* header files */
#include "interface/g2EncryptionPluginInterface.h"
#include <openssl/conf.h>
#include <openssl/evp.h>
#include <openssl/err.h>
#include <stdio.h>


/**********************************************************************************
*
*  Basic encryption/decryption example code came from this web site.
*
*  https://wiki.openssl.org/index.php/EVP_Symmetric_Encryption_and_Decryption
*
**********************************************************************************/


/* encryption initialization values */
static char* mEncryptionKey;
static char* mEncryptionIV;


/* helper function for loading error message data into the appropriate data structure */
void handleErrors(const char* errorMessage, struct ErrorInfoData* errorData)
{
  errorData->mErrorOccurred = true;
  strncpy(errorData->mErrorMessage, errorMessage, G2_ENCRYPTION_PLUGIN___MAX_ERROR_MESSAGE_LENGTH-1);
  errorData->mErrorMessage[G2_ENCRYPTION_PLUGIN___MAX_ERROR_MESSAGE_LENGTH - 1] = '\0';
  /*fprintf(stderr,"DEBUG: [%s]\n",errorData->mErrorMessage);*/
}



/* define a size for the plugin encryption signature
 */
#define PLUGIN_SIGNATURE_MAX_LENGTH 1024


/* define a simple plugin signature, for the encryption.
 */
void getPluginSignature(char* signature)
{
  /* define the plugin encryption signature */
  char mSignature[PLUGIN_SIGNATURE_MAX_LENGTH];
  strcpy(mSignature,"{\"NAME\":\"");
  strcat(mSignature,"g2EncryptDataAES256CBC");
  strcat(mSignature,"\",\"KEY\":\"");
  strcat(mSignature,mEncryptionKey);
  strcat(mSignature,"\",\"IV\":\"");
  strcat(mSignature,mEncryptionIV);
  strcat(mSignature,"\"}");
  {
    /* Jenkins' one-at-a-time hash, public domain */
    /* http://www.burtleburtle.net/bob/hash/doobs.html */
    unsigned hash = 0;
    const char *cp;
    for ( cp = mSignature; *cp; ++cp )
    {
      hash += (*cp & 0xFF);
      hash += (hash << 10);
      hash ^= (hash >> 6);
    }
    hash += (hash << 3);
    hash ^= (hash >> 11);
    hash += (hash << 15);

    char hashString[20];
    sprintf(hashString,"%u",hash);
    strcpy(signature,hashString);
  }
}



/* Function used to initialize a plugin.
 * See the function prototype definitions for more information.
 */
G2_ENCRYPTION_PLUGIN_FUNCTION_INIT_PLUGIN
{
  /* initialize the init-plugin function */
  INIT_PLUGIN_FUNCTION_PREAMBLE

  /* default initial variables */
  mEncryptionKey = NULL;
  mEncryptionIV = NULL;

  /* get the encryption keys */
  if (configParams != NULL)
  {
    size_t paramIndex;
    for (paramIndex = 0; paramIndex < configParams->numParameters; ++paramIndex)
    {
      const char* paramName = (configParams->paramTuples[paramIndex].paramName);
      const char* paramValue = (configParams->paramTuples[paramIndex].paramValue);
      if (strcmp(paramName,"ENCRYPTION_KEY")==0)
      {
        mEncryptionKey = malloc((strlen(paramValue)+1)*(sizeof(char)));
        strcpy(mEncryptionKey,paramValue);
      }
      else if (strcmp(paramName,"ENCRYPTION_INITIALIZATION_VECTOR")==0)
      {
        mEncryptionIV = malloc((strlen(paramValue)+1)*(sizeof(char)));
        strcpy(mEncryptionIV,paramValue);
      }
    }
  }

  /* verify the input parameters */
  if ((mEncryptionKey == NULL) || (strlen(mEncryptionKey) == 0))
  {
    handleErrors("Encryption key is empty",&initializationErrorData);
  }
  else if ((mEncryptionIV == NULL) || (strlen(mEncryptionIV) == 0))
  {
    handleErrors("Encryption initialization vector is empty",&initializationErrorData);
  }

  /* finalize the init-plugin function */
  INIT_PLUGIN_FUNCTION_POSTAMBLE
}


/* Function used to close a plugin.
 * See the function prototype definitions for more information.
 */
G2_ENCRYPTION_PLUGIN_FUNCTION_CLOSE_PLUGIN
{
  /* initialize the close-plugin function */
  CLOSE_PLUGIN_FUNCTION_PREAMBLE

  /* default initial variables */
  free(mEncryptionKey);
  mEncryptionKey = NULL;
  free(mEncryptionIV);
  mEncryptionIV = NULL;

  /* finalize the close-plugin function */
  CLOSE_PLUGIN_FUNCTION_POSTAMBLE
}


/* Function used to retrieve the plugin signature.
 * See the function prototype definitions for more information.
 */
G2_ENCRYPTION_PLUGIN_FUNCTION_GET_SIGNATURE
{
  /* initialize get-signature function */
  GET_SIGNATURE_FUNCTION_PREAMBLE

  /* get the native plugin encryption signature */
  char pluginSignature[PLUGIN_SIGNATURE_MAX_LENGTH];
  getPluginSignature(pluginSignature);

  /* return the signature, or track errors */
  if (!(getSignatureErrorData.mErrorOccurred))
  {
    /* return the signature */
    const size_t pluginSignatureSize = strlen(pluginSignature);
    if (pluginSignatureSize < (size_t)maxSignatureSize)
    {
      strcpy(signature, pluginSignature);
      signature[maxSignatureSize - 1] = '\0';
      *signatureSize = pluginSignatureSize;
    }
    else
    {
      signatureSizeErrorOccurred = true;
    }
  }

  /* finalize get-signature function */
  GET_SIGNATURE_FUNCTION_POSTAMBLE
}


/* Function used to validate the plugin signature compatibility.
 * See the function prototype definitions for more information.
 */
G2_ENCRYPTION_PLUGIN_FUNCTION_VALIDATE_SIGNATURE_COMPATIBILITY
{
  /* initialize get-signature function */
  VALIDATE_SIGNATURE_COMPATIBILITY_FUNCTION_PREAMBLE

  /* get the native plugin encryption signature */
  char pluginSignature[PLUGIN_SIGNATURE_MAX_LENGTH];
  getPluginSignature(pluginSignature);

  /* validate the signature is compatible */
  if (strcmp(pluginSignature,signatureToValidate) == 0)
  {
    signatureIsCompatible = true;
  }

  /* finalize validate-signature-compatibility function */
  VALIDATE_SIGNATURE_COMPATIBILITY_FUNCTION_POSTAMBLE
}


/* Function to do a simple encryption.
 */
int64_t encrypt(unsigned char *plaintext, size_t plaintext_len, unsigned char *key, unsigned char *iv, unsigned char *ciphertext, struct ErrorInfoData* errorData)
{
    EVP_CIPHER_CTX *ctx = NULL;
    int len = 0;
    size_t ciphertext_len = 0;

    /* Create and initialise the context */
    if(!(ctx = EVP_CIPHER_CTX_new()))
    {
        handleErrors("Failed to create cipher context for encryption",errorData);
        if (errorData->mErrorOccurred) { return -1; }
    }

    /*
     * Initialise the encryption operation. IMPORTANT - ensure you use a key
     * and IV size appropriate for your cipher
     * In this example we are using 256 bit AES (i.e. a 256 bit key). The
     * IV size for *most* modes is the same as the block size. For AES this
     * is 128 bits
     */
    if(1 != EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, key, iv))
    {
        handleErrors("Failed to initialize encryption",errorData);
        if (errorData->mErrorOccurred) { return -1; }
    }

    /*
     * Provide the message to be encrypted, and obtain the encrypted output.
     * EVP_EncryptUpdate can be called multiple times if necessary
     */
    if(1 != EVP_EncryptUpdate(ctx, ciphertext, &len, plaintext, plaintext_len))
    {
        handleErrors("Failed to update encryption",errorData);
        if (errorData->mErrorOccurred) { return -1; }
    }
    ciphertext_len = len;

    /*
     * Finalise the encryption. Further ciphertext bytes may be written at
     * this stage.
     */
    if(1 != EVP_EncryptFinal_ex(ctx, ciphertext + len, &len))
    {
        handleErrors("Failed to finalize encryption",errorData);
        if (errorData->mErrorOccurred) { return -1; }
    }
    ciphertext_len += len;

    /* Clean up */
    EVP_CIPHER_CTX_free(ctx);

    return ciphertext_len;
}


/* Function to do a simple decryption.
 */
int64_t decrypt(unsigned char *ciphertext, size_t ciphertext_len, unsigned char *key, unsigned char *iv, unsigned char *plaintext, struct ErrorInfoData* errorData)
{
    EVP_CIPHER_CTX *ctx = NULL;
    int len = 0;
    size_t plaintext_len = 0;

    /* Create and initialise the context */
    if(!(ctx = EVP_CIPHER_CTX_new()))
    {
        handleErrors("Failed to create cipher context for decryption",errorData);
        if (errorData->mErrorOccurred) { return -1; }
    }

    /*
     * Initialise the decryption operation. IMPORTANT - ensure you use a key
     * and IV size appropriate for your cipher
     * In this example we are using 256 bit AES (i.e. a 256 bit key). The
     * IV size for *most* modes is the same as the block size. For AES this
     * is 128 bits
     */
    if(1 != EVP_DecryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, key, iv))
    {
        handleErrors("Failed to initialize decryption",errorData);
        if (errorData->mErrorOccurred) { return -1; }
    }

    /*
     * Provide the message to be decrypted, and obtain the plaintext output.
     * EVP_DecryptUpdate can be called multiple times if necessary.
     */
    if(1 != EVP_DecryptUpdate(ctx, plaintext, &len, ciphertext, ciphertext_len))
    {
        handleErrors("Failed to update decryption",errorData);
        if (errorData->mErrorOccurred) { return -1; }
    }
    plaintext_len = len;

    /*
     * Finalise the decryption. Further plaintext bytes may be written at
     * this stage.
     */
    int64_t retVal = EVP_DecryptFinal_ex(ctx, plaintext + len, &len);
    if(1 != retVal)
    {
        handleErrors("Failed to finalize decryption",errorData);
        if (errorData->mErrorOccurred) { return -1; }
    }
    plaintext_len += len;

    /* Clean up */
    EVP_CIPHER_CTX_free(ctx);

    return plaintext_len;
}


/* Function used to encrypt a data value.
 * See the function prototype definitions for more information.
 */
G2_ENCRYPTION_PLUGIN_FUNCTION_ENCRYPT_DATA_FIELD
{
  /* initialize encryption function */
  ENCRYPT_DATA_FIELD_FUNCTION_PREAMBLE

  /* encrypt the data */
  const size_t bufferLength = inputSize + EVP_MAX_BLOCK_LENGTH;
  unsigned char* ciphertext = malloc(bufferLength * sizeof(char));
  size_t ciphertext_len = encrypt((unsigned char*)input,(int64_t)inputSize,(unsigned char*)mEncryptionKey,(unsigned char*)mEncryptionIV,ciphertext,&encryptionErrorData);
  if (!(encryptionErrorData.mErrorOccurred))
  {
    if (((size_t) ciphertext_len) < maxResultSize)
    {
      memcpy(result, (const char*)ciphertext, ciphertext_len);
      result[maxResultSize - 1] = '\0';
      *resultSize = ciphertext_len;
    }
    else
    {
      resultSizeErrorOccurred = true;
    }
  }
  free(ciphertext);

  /* finalize encryption function */
  ENCRYPT_DATA_FIELD_FUNCTION_POSTAMBLE
}


/* Function used to decrypt a data value.
 * See the function prototype definitions for more information.
 */
G2_ENCRYPTION_PLUGIN_FUNCTION_DECRYPT_DATA_FIELD
{
  /* initialize encryption function */
  DECRYPT_DATA_FIELD_FUNCTION_PREAMBLE

  /* decrypt the data */
  const size_t bufferLength = inputSize + EVP_MAX_BLOCK_LENGTH;
  unsigned char* decryptedtext = malloc(bufferLength * sizeof(char));
  size_t decryptedtext_len = decrypt((unsigned char*)input,(int64_t)inputSize,(unsigned char*)mEncryptionKey,(unsigned char*)mEncryptionIV,decryptedtext,&decryptionErrorData);
  if (!(decryptionErrorData.mErrorOccurred))
  {
    if (((size_t) decryptedtext_len) < maxResultSize)
    {
      memcpy(result, (const char*)decryptedtext, decryptedtext_len);
      result[maxResultSize - 1] = '\0';
      *resultSize = (size_t)decryptedtext_len;
    }
    else
    {
      resultSizeErrorOccurred = true;
    }
  }
  free(decryptedtext);

  /* finalize decryption function */
  DECRYPT_DATA_FIELD_FUNCTION_POSTAMBLE
}


/* Function used to encrypt a data value (deterministic methods.)
 * See the function prototype definitions for more information.
 */
G2_ENCRYPTION_PLUGIN_FUNCTION_ENCRYPT_DATA_FIELD_DETERMINISTIC
{
  /* initialize encryption function */
  ENCRYPT_DATA_FIELD_DETERMINISTIC_FUNCTION_PREAMBLE

  /* encrypt the data */
  const size_t bufferLength = inputSize + EVP_MAX_BLOCK_LENGTH;
  unsigned char* ciphertext = malloc(bufferLength * sizeof(char));
  size_t ciphertext_len = encrypt((unsigned char*)input,(int64_t)inputSize,(unsigned char*)mEncryptionKey,(unsigned char*)mEncryptionIV,ciphertext,&encryptionErrorData);
  if (!(encryptionErrorData.mErrorOccurred))
  {
    if (((size_t) ciphertext_len) < maxResultSize)
    {
      memcpy(result, (const char*)ciphertext, ciphertext_len);
      result[maxResultSize - 1] = '\0';
      *resultSize = ciphertext_len;
    }
    else
    {
      resultSizeErrorOccurred = true;
    }
  }
  free(ciphertext);

  /* finalize encryption function */
  ENCRYPT_DATA_FIELD_DETERMINISTIC_FUNCTION_POSTAMBLE
}


/* Function used to decrypt a data value (deterministic methods.)
 * See the function prototype definitions for more information.
 */
G2_ENCRYPTION_PLUGIN_FUNCTION_DECRYPT_DATA_FIELD_DETERMINISTIC
{
  /* initialize encryption function */
  DECRYPT_DATA_FIELD_DETERMINISTIC_FUNCTION_PREAMBLE

  /* decrypt the data */
  const size_t bufferLength = inputSize + EVP_MAX_BLOCK_LENGTH;
  unsigned char* decryptedtext = malloc(bufferLength * sizeof(char));
  size_t decryptedtext_len = decrypt((unsigned char*)input,(int64_t)inputSize,(unsigned char*)mEncryptionKey,(unsigned char*)mEncryptionIV,decryptedtext,&decryptionErrorData);
  if (!(decryptionErrorData.mErrorOccurred))
  {
    if (((size_t) decryptedtext_len) < maxResultSize)
    {
      memcpy(result, (const char*)decryptedtext, decryptedtext_len);
      result[maxResultSize - 1] = '\0';
      *resultSize = (size_t)decryptedtext_len;
    }
    else
    {
      resultSizeErrorOccurred = true;
    }
  }
  free(decryptedtext);

  /* finalize decryption function */
  DECRYPT_DATA_FIELD_DETERMINISTIC_FUNCTION_POSTAMBLE
}

