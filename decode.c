#include <stdio.h>
#include "decode.h"
#include "types.h"
#include <string.h>
#include "common.h"

/* Function Definitions */
Status read_and_validate_decode_args(char *argv[], DecodeInfo *decInfo)
{ 
// checking the file format
      if(argv[2] != NULL)
      {
	  if(strstr(argv[2],"."))
	  {
	       	if(strcmp (strstr(argv[2], "."), ".bmp") == 0)
		{
	      	      decInfo -> stego_image_fname = argv[2];
		      if(argv[3] != NULL)
		      {
			    decInfo -> secret_fname=argv[3];
		      }
		      else
		      {
			    printf("INFO: Selecting the file name as default\n");
			    decInfo -> secret_fname = "default_name";
		      }
		}
		else
		{
		     printf("ERROR! Please pass file name in bmp format,usage ./a.ouy -d output.bmp\n");
		      return e_failure;
		}
	  }
	  else
	  {
		printf("ERROR! Please pass file name in bmp format,usage ./a.ouy -d output.bmp\n");
		return e_failure;
	  }
      }
      else
      {
	    printf("Error! Please pass file name through command line arguments,usage : ./a.out -d output.bmp\n");
	    return e_failure;
      }
      return e_success;
}

Status do_decoding(DecodeInfo *decInfo)
{
    //function calls
     printf("INFO: ## Decoding procedure started\n");
     printf("INFO: Opening required files\n");
    if(Open_files(decInfo) == e_success)
    {
        printf("INFO: Done\n");
        printf("INFO: Skipping the header\n");
	if(skip_header(decInfo) == e_success)
	{

	     printf("INFO: Done\n");
	     printf("INFO: Decoding magic string size\n");
	      if(decode_magic_string_size(decInfo) == e_success)
	      {

	            printf("INFO: Done\n");
		    printf("INFO: Decoding magic string signature\n");
		    if(decode_magic_string(decInfo) == e_success)
		    {
			  printf("INFO: Done\n");
			  printf("INFO: Decoding extension size\n");
			  if(decode_secret_file_extn_size(decInfo) == e_success)
			  {
			        printf("INFO: Done\n");
				printf("INFO: Decoding extension\n");
				if(decode_secret_file_extn(decInfo) == e_success)
				{
				      printf("INFO: Done\n");
				      printf("INFO: Decoding %s file size\n",decInfo->secret_fname);
				      if(decode_secret_file_size(decInfo) == e_success)
				      {
					    printf("INFO: Done\n");
					    printf("INFO: Deocding %s file data\n",decInfo->secret_fname);
					    if(decode_secret_file_data(decInfo) == e_success)
					    {
						  printf("INFO: Done\n");
						  return e_success;
						 
					    }
					   else
					   {
						  printf("ERROR: Secret File data decoding failed\n");
					   }
				      }
				      else
				      {
					    printf("ERROR: Secret File size decoding failed\n");
				      }
				}
				else
			        {
				      printf("ERROR: Secret File extension decoding failed\n");
				}
			  }
			  else
			  {
				printf("ERROR: Secret File extn size decoding failed\n");
			  }
		    }
		    else
		    {
			  printf("ERROR: Magic String decoding failed\n");
		    }
	      }
	      else
	      {
		    printf("ERROR: Magic String size decoding failed\n");
	      }
	}
	else
	{
	      printf("ERROR: Header skipping failed\n");
	}
    }
    else
    {
	  printf("ERROR: Files opening unsuccessful\n");
    }


}

Status Open_files(DecodeInfo *decInfo)
{
      //stego image file
     
      decInfo -> fptr_stego_image= fopen(decInfo -> stego_image_fname,"r");
      printf("INFO: Opened %s\n",decInfo->stego_image_fname);
	//do error handling
      if(decInfo -> fptr_stego_image == NULL)
      {
	    perror("fopen");
	    fprintf(stderr, "ERROR : Unable to open file %s\n",decInfo -> stego_image_fname);
	    return e_failure;
       }
      //no failure return e_success
      return e_success;

}
Status skip_header(DecodeInfo *decInfo)
{
     //move the file pointer to 54th position
      fseek(decInfo -> fptr_stego_image,54,SEEK_SET);
      return e_success;
}
int  decode_size_to_lsb(char *image_buffer)
{
      //run the loop for 32 times
      int num=0x00;
      for(int i=0; i<32; i++)
      {
	    if(image_buffer[i] & 1)
	    {
		  num=num | (1<< (31-i));
	    }
      }
      return num;
}
char decode_byte_to_lsb(char *image_buffer)
{
      char ch=0x00;
      for(int i=0; i<8; i++)
      {
	    ch=((image_buffer[i] & 0x01) << (7-i)) | ch;
      }
      return ch; 

}
Status decode_magic_string_size(DecodeInfo *decInfo)
{
      char buffer[32];
      fread(buffer,32,1,decInfo -> fptr_stego_image);
      if(decode_size_to_lsb(buffer)<1)
      {
	    printf("ERROR: Data not found,please pass correct file\n");
	    return e_failure;
      }
      decInfo -> magic_string_size = decode_size_to_lsb(buffer);
      return e_success;
}
Status decode_magic_string(DecodeInfo *decInfo)
{
      int i;
      char buffer[8];
      char check_magic[decInfo -> magic_string_size + 1];
      for (i=0; i < decInfo -> magic_string_size; i++)
      {
	    fread(buffer,8,1,decInfo -> fptr_stego_image);
	    check_magic[i]=decode_byte_to_lsb(buffer);
      }
      check_magic[i]='\0';
      if(strcmp(check_magic,"ANANTHU") == 0)
      {
	    printf("INFO: magic string matched\n");
	    return e_success;
      }
      else
      {
	    printf("ERROR: Magic string is not matching\n");
	    return e_failure;
      }

}
Status decode_secret_file_extn_size(DecodeInfo *decInfo)
{
      char buffer[32];
      fread(buffer,32,1,decInfo -> fptr_stego_image);
      decInfo -> secret_file_extn_size = decode_size_to_lsb(buffer);
      return e_success;
}
Status decode_secret_file_extn(DecodeInfo *decInfo)
{
      char buffer[8];
      int i;
      char extn_data[decInfo -> secret_file_extn_size + 1];
      for (i=0; i < decInfo -> secret_file_extn_size; i++)
      {
	    fread(buffer,8,1,decInfo -> fptr_stego_image);
	    extn_data[i] = decode_byte_to_lsb(buffer);
      }
      extn_data[i]='\0';
      strcpy(decInfo -> new_secret,decInfo -> secret_fname);
      strcat(decInfo -> new_secret,extn_data);
      return e_success;
}
Status decode_secret_file_size(DecodeInfo *decInfo)
{
	    char buffer[32];
	    fread(buffer,32,1,decInfo -> fptr_stego_image);
	    decInfo -> secret_file_size = decode_size_to_lsb(buffer);
            return e_success;
}
Status decode_secret_file_data(DecodeInfo *decInfo)
{
      int i;
      char buffer[8];
      decInfo->fptr_secret_fname=fopen(decInfo->new_secret,"w");
      printf("INFO: Opened %s\n",decInfo->new_secret);
      if(decInfo->fptr_secret_fname==NULL)
      {
	    printf("ERROR: Cant open the file %s",decInfo->new_secret);
      }
      for(i=0; i<decInfo -> secret_file_size; i++)
      {
	    fread(buffer,8,1,decInfo -> fptr_stego_image);
	    char ch=decode_byte_to_lsb(buffer);
	    fwrite(&ch,1,1,decInfo -> fptr_secret_fname);
      }
      return e_success;
}







      
    



