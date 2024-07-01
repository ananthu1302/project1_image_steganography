#include <stdio.h>
#include "encode.h"
#include "types.h"
#include <string.h>
#include "common.h"

/* Function Definitions */
Status read_and_validate_encode_args(char *argv[],EncodeInfo *encInfo)
{
      /*check argv[2] is .bmp file or not
	.b,p=> encInfo->src_image_fname=argv[2]
	not -> return e_failure;*/
      if (argv[3] == NULL || argv[2] == NULL)
      {
	    printf("ERROR: Please pass sufficient command line arguments,usage:./a.out -e beautiful.bmp secret.txt output.bmp\n");
	    return e_failure;
      }
      else
      {
      if(strstr(argv[2],".") && strstr(argv[3],"."))
      {	    
      if (strcmp(strstr(argv[2],"."),".bmp") == 0)
      {
            encInfo->src_image_fname = argv[2];
      }
      else
      {
	    printf("ERROR ! Please pass file name with correct extension,usage:beautiful.bmp\n");
            return e_failure;
      }
      /*check argv[3] is .txt or not
	.txt=> store argv[3] into encInfo ->secret_fname
	not->return e_failure;*/
      if (strcmp(strstr(argv[3],"."),".txt") == 0 || strcmp(strstr(argv[3],"."),".c") == 0 || strcmp(strstr(argv[3],"."),".sh") == 0)
      {
            encInfo->secret_fname = argv[3];
      }
      else
      {
	    printf("ERROR: Please pass file with correct extension,usage:.txt , .c, .sh\n");
            return e_failure;
      }
      }
      else
      {
	    printf("Error! Please pass file names with correct extensions\n");
	    return e_failure;
      }/*check argv[4] is passed or not
	 passed=>check argv[4] is .bmp or not
	 .bmp=> store argv[4] into encInfo->stego_image_fname
	 not return e_failure;
	 not passed encInfo ->stego_image_fname=deafult.bmp*/
      if (argv[4] != NULL)
      {
	    if(strstr(argv[4],".") && strcmp(strstr(argv[4],"."),".bmp") == 0)
	    {
		  encInfo->stego_image_fname = argv[4];
	    }
	    else
	    {
		  printf("Error! Please pass file name in bmp format,usage: ./a.out -e beautiful.bmp secret.txt stego.bmp\n");
		  return e_failure;
	    }
      }
      else
      {
	    encInfo->stego_image_fname = "default.bmp";
	    printf("INFO: Output File not mentioned. Creating default.bmp as default\n");
      }
      return e_success;
      }
}
Status do_encoding(EncodeInfo *encInfo)
{
      //function calls
      printf("INFO: Opening required files\n");
      if (open_files(encInfo) == e_success)
      {
	    printf("INFO: Done\n");
	    printf("INFO: ## Encoding procedure started ##\n");
	    printf("INFO: checking for %s capacity to handle %s\n",encInfo->src_image_fname,encInfo->secret_fname);
	    if (check_capacity(encInfo) == e_success)
	    {
		  printf("INFO: Done. Found OK\n");
		  printf("INFO: Copying Image Header\n");
		  if (copy_bmp_header(encInfo -> fptr_src_image,encInfo -> fptr_stego_image) == e_success)
		  {
			printf("INFO: Done\n");
			printf("INFO: Encoding magic string size\n");
			if(encode_magic_string_size(strlen(MAGIC_STRING), encInfo) == e_success)
			{
			      printf("INFO: Done\n");
			      printf("INFO: Encoding magic string signature\n");
			      if(encode_magic_string(MAGIC_STRING,encInfo) == e_success)
			      {
				    printf("INFO:Done\n");
	                            printf("INFO: Encoding %s File Extension size\n",encInfo->secret_fname);

			      if(encode_secret_file_extn_size(strlen(strstr(encInfo -> secret_fname,".")),encInfo) == e_success)
			      {
				     
				     printf("INFO: Done\n");
	                             printf("INFO: Encoding %s File Extension\n",encInfo->secret_fname);

                                     if(encode_secret_file_extn(strstr(encInfo -> secret_fname,"."),encInfo) == e_success)
				     {
					   printf("INFO: Done\n");
					   printf("INFO: Encoding %s File size\n",encInfo->secret_fname);
					   if(encode_secret_file_size(encInfo -> size_secret_file,encInfo) == e_success)
					   {
						 printf("INFO: Done\n");
	                                         printf("INFO: Encoding %s File Data\n",encInfo->secret_fname);
						 if(encode_secret_file_data(encInfo) == e_success)
						 {
						       printf("INFO: Done\n");
						       printf("INFO: Copying Left Over Data\n");
						       if(copy_remaining_img_data(encInfo -> fptr_src_image ,encInfo -> fptr_stego_image) == e_success)
						       {
							     printf("INFO: Done\n");
							    
						       }
						       else
						       {
							     printf("ERROR: Remaining data copying unsuccessful\n");
						       }
						 }
						  else
						  {
						       	printf("ERROR: 	Secret file data encoding unsuccessful\n");
						  }
					   }
					   else
					   {
						printf("ERROR: Secret file size encoding unsuccessful\n");
					   }	
				     }
				     else
				     {
					   printf("ERROR: Secret file extension encoding unsuccessful\n");
				     }

			      }
			             else
			             {
				        printf("ERROR: Secret file extension size encoding unsuccessful\n");
			             }
			      }			      
			      else
			      {
				    printf("ERROR: Magic_string encoding unsuccessful\n");
			      } 

			        
			}
			else
			{
			      printf("ERROR: magic string size encoding unsuccessful\n");
			}

		  }
		  else
		  {
			printf("ERROR: Header copying Unsuccessful\n");
		  }

	    }
	    else
	    {
		  printf("ERROR: No enough capacity\n");
	    }
      }
      else
      {
	    printf("ERROR: File opening unsuccessful\n");

      }
      return e_success;
}
Status check_capacity(EncodeInfo *encInfo)
{
      //find the bmp file size
      encInfo -> image_capacity = get_image_size_for_bmp(encInfo -> fptr_src_image);
      int magic_string_length = strlen(MAGIC_STRING);
      //find the extn
      int length_extn=strlen(strstr(encInfo->secret_fname,"."));
      //find the secret file size
     // printf("INFO: Checking for %s size\n",encInfo->secret_fname);

      encInfo -> size_secret_file = get_file_size(encInfo -> fptr_secret);
     // printf("INFO: Done. Not Empty\n");
     // printf("INFO: Checking for %s capacity to handle %s \n",encInfo->src_image_fname,encInfo->secret_fname);

      if (encInfo -> image_capacity > (54 + 32 + (magic_string_length * 8) + 32 + (length_extn * 8) + 32 + (encInfo -> size_secret_file * 8)))
      {
	    return e_success;
      }
      else
      {
	    return e_failure;
      }
}
uint get_file_size(FILE *fptr)
{
      //move the fptr to the end
      fseek(fptr,0,SEEK_END);
      return ftell(fptr);
}


Status copy_bmp_header(FILE *fptr_src_image,  FILE *fptr_stego_image)
{
     
      char str[54];
      fseek(fptr_src_image,0,SEEK_SET);  //move the file pointer to 0th position
      fread(str,54,1,fptr_src_image);     
      fwrite(str,54,1,fptr_stego_image);
      return e_success;
}
Status encode_byte_to_lsb(char data,char *image_buffer)
{
      for(int i=7; i>=0; i--)
      {
	    image_buffer[7-i]=(image_buffer[7-i] &(~1)) | ((unsigned)(data &(1 << i)) >> i);
      }
      return e_success;
}
Status encode_size_to_lsb(int data, char *image_buffer)
{
      //run the loop for 32 times
      for (int i=31; i>=0; i--)
      {
	    image_buffer[31-i]=(image_buffer[31-i] & (~1)) | ((unsigned)(data &(1 << i)) >> i);
      }
      return e_success;
}
Status encode_magic_string_size(int size, EncodeInfo *encInfo)
{
      
      char str[32];
      fread(str,32,1,encInfo -> fptr_src_image);
      encode_size_to_lsb(size, str);
      fwrite(str,32,1,encInfo -> fptr_stego_image);
      return e_success;
}
Status encode_magic_string(const char *magic_string, EncodeInfo *encInfo)
{
      char str[8];
      for (int i=0; i<(strlen(magic_string)); i++)
      {
         fread(str,8,1,encInfo -> fptr_src_image);
	 encode_byte_to_lsb(magic_string[i], str);
	 fwrite(str,8,1,encInfo -> fptr_stego_image);
	 }
     return e_success;
}
Status encode_secret_file_extn_size(int size, EncodeInfo *encInfo)
{
      
     char str[32];
    fread(str,32,1,encInfo -> fptr_src_image);
   encode_size_to_lsb(size, str);
   fwrite(str,32,1,encInfo -> fptr_stego_image);
   return e_success;
} 
Status encode_secret_file_extn(const char *file_extn,EncodeInfo *encInfo)
{
      char str[8];
      for(int i=0; i<(strlen(file_extn)); i++)
      {
	    fread(str,8,1,encInfo -> fptr_src_image);
	    encode_byte_to_lsb(file_extn[i],str);
	    fwrite(str,8,1,encInfo -> fptr_stego_image);
      }
      return e_success;
}
Status encode_secret_file_size(long file_size, EncodeInfo *encInfo)
{
      
      char str[32];
      fread(str,32,1,encInfo -> fptr_src_image);
      encode_size_to_lsb(file_size,str);
      fwrite(str,32,1,encInfo -> fptr_stego_image);
      return e_success;
}
Status encode_secret_file_data(EncodeInfo *encInfo)
{
      //to take data from secret.txt file
      
      char secret_data[encInfo -> size_secret_file];
      fseek(encInfo -> fptr_secret,0,SEEK_SET);
      fread(secret_data,encInfo -> size_secret_file,1,encInfo -> fptr_secret);
      //encode characters
      char str[8];
      for (int i=0; i<(encInfo -> size_secret_file); i++)
      {
	    fread(str,8,1,encInfo -> fptr_src_image);
	    encode_byte_to_lsb(secret_data[i],str);
	    fwrite(str,8,1,encInfo -> fptr_stego_image);
      }
      return e_success;
} 
Status copy_remaining_img_data(FILE *fptr_src_image, FILE *fptr_stego_image)
{
      
      char ch;
      while(fread(&ch,1,1,fptr_src_image) >= 1)
      {
	    
	    fwrite(&ch,1,1,fptr_stego_image);
      }
      return e_success;
}
      










/* Get image size
 * Input: Image file ptr
 * Output: width * height * bytes per pixel (3 in our case)
 * Description: In BMP Image, width is stored in offset 18,
 * and height after that. size is 4 bytes
 */
uint get_image_size_for_bmp(FILE *fptr_image)
{
    uint width, height;
    // Seek to 18th byte
    fseek(fptr_image, 18, SEEK_SET);

    // Read the width (an int)
    fread(&width, sizeof(int), 1, fptr_image);
    printf("width = %u\n", width);

    // Read the height (an int)
    fread(&height, sizeof(int), 1, fptr_image);
     printf("height = %u\n", height);

    // Return image capacity
    return width * height * 3;
}

/* 
 * Get File pointers for i/p and o/p files
 * Inputs: Src Image file, Secret file and
 * Stego Image file
 * Output: FILE pointer for above files
 * Return Value: e_success or e_failure, on file errors
 */
Status open_files(EncodeInfo *encInfo)
{
      
    // Src Image file
    encInfo->fptr_src_image = fopen(encInfo->src_image_fname, "r");
    printf("INFO: opened %s\n",encInfo->src_image_fname);
    // Do Error handling
    if (encInfo->fptr_src_image == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->src_image_fname);

    	return e_failure;
    }

    // Secret file
    encInfo->fptr_secret = fopen(encInfo->secret_fname, "r");
    printf("INFO: Opened %s\n",encInfo->secret_fname);
    // Do Error handling
    if (encInfo->fptr_secret == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->secret_fname);

    	return e_failure;
    }


    // Stego Image file
    encInfo->fptr_stego_image = fopen(encInfo->stego_image_fname, "w");
    printf("INFO: Opened %s\n",encInfo->stego_image_fname);
    // Do Error handling
    if (encInfo->fptr_stego_image == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->stego_image_fname);

    	return e_failure;
    }

    // No failure return e_success
    return e_success;
}
