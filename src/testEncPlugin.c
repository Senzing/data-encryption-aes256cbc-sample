
/**********************************************************************************
 © Copyright Senzing, Inc. 2023
 The source code for this program is not published or otherwise divested
 of its trade secrets, irrespective of what has been deposited with the U.S.
 Copyright Office.
**********************************************************************************/


#include "interface/g2EncryptionPluginInterface.h"
#include <stdio.h>


int main()
{
  int ret = 0;
  size_t maxErrorSize = 1000;
  char error[maxErrorSize];
  size_t errorSize = 0;

  struct CParameterTuple params[2];
  params[0].paramName="ENCRYPTION_KEY";
  params[0].paramValue="68402346802394406802620602396369";
  params[1].paramName="ENCRYPTION_INITIALIZATION_VECTOR";
  params[1].paramValue="6432072349624624";
  struct CParameterList paramList;
  paramList.paramTuples = params;
  paramList.numParameters = 2;

  ret = G2Encryption_InitPlugin(&paramList, error, maxErrorSize, &errorSize);
  if (ret != 0)
  {
	  fprintf(stderr, "G2Encryption_InitPlugin: [%d] [%s]\n", ret, error);
	  return ret;
  }


  char input[] = "abcdefhijklmnopqrstuvwxyz _+-=[]{}?/:;<>";
  size_t inputSize = strlen(input);
  size_t resultSize = 0;

  ret = G2Encryption_EncryptDataField(input,inputSize, NULL, 0, &resultSize, error, maxErrorSize, &errorSize);
  if (ret != G2_ENCRYPTION_PLUGIN___OUTPUT_BUFFER_SIZE_ERROR)
  {
	  fprintf(stderr, "G2Encryption_EncryptDataField (1): [%d] [%s]\n", ret, "Succeeded but shouldn't");
	  return -1;
  }
  error[0] = '\0';

  size_t maxResultSize = 1;
  char result[maxResultSize];

  ret = G2Encryption_EncryptDataField(input,inputSize, result, maxResultSize, &resultSize, error, maxErrorSize, &errorSize);
  if (ret != G2_ENCRYPTION_PLUGIN___OUTPUT_BUFFER_SIZE_ERROR)
  {
	  fprintf(stderr, "G2Encryption_EncryptDataField (2): [%d] [%s]\n", ret, "Succeeded but shouldn't");
	  return -1;
  }
  error[0] = '\0';

  size_t maxEncryptedResult = 1024;
  char encryptedResult[maxEncryptedResult];
  size_t encryptedSize = 0;

  ret = G2Encryption_EncryptDataField(input,inputSize, encryptedResult, inputSize, &encryptedSize, error, maxErrorSize, &errorSize);
  if (ret != G2_ENCRYPTION_PLUGIN___OUTPUT_BUFFER_SIZE_ERROR)
  {
	  fprintf(stderr, "G2Encryption_EncryptDataField (3): [%d] [%s]\n", ret, "Succeeded but shouldn't");
	  return -1;
  }
  error[0] = '\0';

  ret = G2Encryption_EncryptDataField(input,inputSize, encryptedResult, maxEncryptedResult, &encryptedSize, error, maxErrorSize, &errorSize);
  if (ret != 0)
  {
	  fprintf(stderr, "G2Encryption_EncryptDataField (4): [%d] [%s]\n", ret, error);
	  return ret;
  }


  ret = G2Encryption_DecryptDataField(encryptedResult,encryptedSize, NULL, 0, &resultSize, error, maxErrorSize, &errorSize);
  if (ret != G2_ENCRYPTION_PLUGIN___OUTPUT_BUFFER_SIZE_ERROR)
  {
	  fprintf(stderr, "G2Encryption_DecryptDataField (1): [%d] [%s]\n", ret, "Succeeded but shouldn't");
	  return ret;
  }
  error[0] = '\0';

  ret = G2Encryption_DecryptDataField(encryptedResult,encryptedSize, result, maxResultSize, &resultSize, error, maxErrorSize, &errorSize);
  if (ret != G2_ENCRYPTION_PLUGIN___OUTPUT_BUFFER_SIZE_ERROR)
  {
	  fprintf(stderr, "G2Encryption_DecryptDataField (2): [%d] [%s]\n", ret, "Succeeded but shouldn't");
	  return -1;
  }
  error[0] = '\0';

  size_t maxDecryptedResult = 1024;
  char decryptedResult[maxDecryptedResult];
  size_t decryptedSize = 0;

  ret = G2Encryption_DecryptDataField(encryptedResult,encryptedSize, decryptedResult, encryptedSize, &decryptedSize, error, maxErrorSize, &errorSize);
  if (ret != 0)
  {
	  fprintf(stderr, "G2Encryption_DecryptDataField (3): [%d] [%s]\n", ret, error);
	  return ret;
  }
  error[0] = '\0';

  ret = G2Encryption_DecryptDataField(encryptedResult,encryptedSize, decryptedResult, maxDecryptedResult, &decryptedSize, error, maxErrorSize, &errorSize);
  if (ret != 0)
  {
	  fprintf(stderr, "G2Encryption_DecryptDataField (4): [%d] [%s]\n", ret, error);
	  return ret;
  }

  ret = strncmp(input,decryptedResult,decryptedSize);
  if (ret != 0 || inputSize != decryptedSize)
  {
	  fprintf(stderr, "strcmp results: [%d] [%s] != [%s]\n", ret, input, decryptedResult);
	  return ret;
  }


  ret = G2Encryption_ClosePlugin(error, maxErrorSize, &errorSize);
  if (ret != 0)
  {
	  fprintf(stderr, "G2Encryption_ClosePlugin: [%s]\n", error);
	  return ret;
  }

  return 0;
}
