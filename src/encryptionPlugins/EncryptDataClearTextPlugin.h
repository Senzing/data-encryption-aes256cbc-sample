
/**********************************************************************************
 © Copyright Senzing, Inc. 2020
 The source code for this program is not published or otherwise divested
 of its trade secrets, irrespective of what has been deposited with the U.S.
 Copyright Office.
**********************************************************************************/

#ifndef CLEAR_TEXT_ENCRYPTION_DATA_PLUGIN_H
#define CLEAR_TEXT_ENCRYPTION_DATA_PLUGIN_H


#include "plugin/g2EncryptionPluginInterface.h"


#if defined(_WIN32)
  #define _DLEXPORT __declspec(dllexport)
#else
  #define _DLEXPORT __attribute__ ((visibility ("default")))
#endif


_DLEXPORT G2EncryptionPluginInitPluginFunc G2Encryption_InitPlugin;
_DLEXPORT G2EncryptionPluginClosePluginFunc G2Encryption_ClosePlugin;

_DLEXPORT G2EncryptionPluginGetSignatureFunc G2Encryption_GetSignature;
_DLEXPORT G2EncryptionPluginValidateSignatureCompatibilityFunc G2Encryption_ValidateSignatureCompatibility;

_DLEXPORT G2EncryptionPluginEncryptDataFieldFunc G2Encryption_EncryptDataField;
_DLEXPORT G2EncryptionPluginDecryptDataFieldFunc G2Encryption_DecryptDataField;


#endif /* header file */


