#ifndef DECODE_H
#define DECODE_H


#include "types.h" //contains user defined types

/* structure to store information required for decoding encoded file ie, output file to origimal file or source image
   */

typedef struct DecodeInfo
{
      char *secret_fname;
      char new_secret[20];
      char *stego_image_fname;
      FILE *fptr_stego_image;
      int magic_string_size;
      int secret_file_extn_size;
      long secret_file_size;
      FILE *fptr_secret_fname; 
} DecodeInfo;








//check operation type
OperationType check_operation_type(char *argv[]);
//read and validate decode args from argv
Status read_and_validate_decode_args(char *argv[],DecodeInfo *decInfo);
//perform decoding
Status do_decoding(DecodeInfo *decInfo);

//get the file pointers for file
Status Open_files(DecodeInfo *decInfo);

//skipping header
Status skip_header(DecodeInfo *decInfo);

//decoding magic string size
Status decode_magic_string_size(DecodeInfo *decInfo);

//decoding magic string data
Status decode_magic_string(DecodeInfo *decInfo);

//decode size to lsb
int decode_size_to_lsb(char * image_buffer);

//decode byte to lsb 
char decode_byte_to_lsb(char *image_buffer);


//decode secret file extension size
Status decode_secret_file_extn_size(DecodeInfo *decInfo);
//decode secret file extension
Status decode_secret_file_extn(DecodeInfo *decInfo);

//decode secret file size

Status decode_secret_file_size(DecodeInfo *decInfo);

//decode secret file data

Status decode_secret_file_data(DecodeInfo *decInfo);

#endif
