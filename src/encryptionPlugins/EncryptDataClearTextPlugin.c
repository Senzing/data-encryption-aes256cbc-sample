
/**********************************************************************************
 © Copyright Senzing, Inc. 2020-2025
 The source code for this program is not published or otherwise divested
 of its trade secrets, irrespective of what has been deposited with the U.S.
 Copyright Office.
**********************************************************************************/


/* encryption interface header */
#include "interface/g2EncryptionPluginInterface.h"


/* define a simple plugin signature, meaning cleartext. */
const char* getPluginSignature()
{
  static const char mSignature[] = "[CLEARTEXT]";
  return mSignature;
}


/* Function used to initialize a plugin.
 * See the function prototype definitions for more information.
 */
G2_ENCRYPTION_PLUGIN_FUNCTION_INIT_PLUGIN
{
  /* initialize the init-plugin function */
  INIT_PLUGIN_FUNCTION_PREAMBLE

  /* nothing to do */
  ;

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

  /* nothing to do */
  ;

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
  const char* pluginSignature = getPluginSignature();

  /* return the signature, or track errors */
  if (!(getSignatureErrorData.mErrorOccurred))
  {
    /* return the signature */
    const size_t pluginSignatureSize = strlen(pluginSignature);
    if (pluginSignatureSize < (size_t)maxSignatureSize)
    {
      memcpy(signature, pluginSignature, pluginSignatureSize);
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
  const char* pluginSignature = getPluginSignature();

  /* validate the signature is compatible */
  if (strcmp(pluginSignature,signatureToValidate) == 0)
  {
    signatureIsCompatible = true;
  }

  /* finalize validate-signature-compatibility function */
  VALIDATE_SIGNATURE_COMPATIBILITY_FUNCTION_POSTAMBLE
}


/* Function used to encrypt a data value.
 * See the function prototype definitions for more information.
 */
G2_ENCRYPTION_PLUGIN_FUNCTION_ENCRYPT_DATA_FIELD
{
  /* initialize encryption function */
  ENCRYPT_DATA_FIELD_FUNCTION_PREAMBLE

  /* Just copy the input data to the result data (cleartext) */
  if (!(encryptionErrorData.mErrorOccurred))
  {
    if (inputSize < (size_t)maxResultSize)
    {
      memcpy(result, input, inputSize);
      result[maxResultSize - 1] = '\0';
      *resultSize = inputSize;
    }
    else
    {
      resultSizeErrorOccurred = true;
    }
  }

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

  /* Just copy the input data to the result data (cleartext) */
  if (!(decryptionErrorData.mErrorOccurred))
  {
    if (inputSize < maxResultSize)
    {
      memcpy(result, input, inputSize);
      result[maxResultSize - 1] = '\0';
      *resultSize = inputSize;
    }
    else
    {
      resultSizeErrorOccurred = true;
    }
  }

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

  /* Just copy the input data to the result data (cleartext) */
  if (!(encryptionErrorData.mErrorOccurred))
  {
    if (inputSize < (size_t)maxResultSize)
    {
      memcpy(result, input, inputSize);
      result[maxResultSize - 1] = '\0';
      *resultSize = inputSize;
    }
    else
    {
      resultSizeErrorOccurred = true;
    }
  }

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

  /* Just copy the input data to the result data (cleartext) */
  if (!(decryptionErrorData.mErrorOccurred))
  {
    if (inputSize < maxResultSize)
    {
      memcpy(result, input, inputSize);
      result[maxResultSize - 1] = '\0';
      *resultSize = inputSize;
    }
    else
    {
      resultSizeErrorOccurred = true;
    }
  }

  /* finalize decryption function */
  DECRYPT_DATA_FIELD_DETERMINISTIC_FUNCTION_POSTAMBLE
}

