#include <stdio.h>
#include <string.h>
#include "encode.h"
#include "types.h"
#include "decode.h"
#include "common.h"
OperationType check_operation_type(char *[]);
int main(int argc, char *argv[])
{
      EncodeInfo encInfo;
      DecodeInfo decInfo;
      if(argv[1] == NULL)
      {
	    printf("ERROR: Please pass command line arguments usage: ./a.out -e beautiful.bmp secret.txt for encoding./a.out -d output.bmp for decoding\n");
	    return 1;
      }
    int ret=check_operation_type(argv);
    if (ret == e_encode)
    {
	  printf("INFO: Selected Encoding\n");
	  if (read_and_validate_encode_args(argv,&encInfo) == e_success)
	  {
		printf("INFO: Read and validation successfully done\n");
		if (do_encoding(&encInfo) == e_success)
		{
		      printf("INFO: ## Encoding Done Successfully ##\n");
		}
		else
		{
		      printf("ERROR: Encoding unsuccessfully\n");
		}
	  }
	  else
	  {
		printf("ERROR: Read and validation unsuccessfull\n");
	  }

    }
    else if (ret == e_decode)
    {
	  printf("INFO: Selected decoding\n");
	  if(read_and_validate_decode_args(argv, &decInfo) == e_success)
	  {
		printf("INFO: Read and validation successfully done\n");
		if(do_decoding(&decInfo) == e_success)
		{
		      printf("INFO: ## Decoding Done Successfully ##\n");
		}
		else
		{
		      printf("ERROR: decoding unsuccessfull\n");
		}
	  }
	  else
	  {
		printf("ERROR: read and validation not successful\n");
	  }
    }
    else
    {
	  printf("ERROR : pass args for \nencoding : ./a.out -e beautiful.bmp secret.txt\n Decoding : ./a.out -d output.bmp\n");
    }
    return 0;
}


OperationType check_operation_type(char *argv[])
{
      if (strcmp(argv[1],"-e") == 0)
      {
	    return e_encode;
      }
      else if(strcmp(argv[1],"-d") == 0)
      {
	    return e_decode;
      }
      else
      {
	    return e_unsupported;
      }
}






