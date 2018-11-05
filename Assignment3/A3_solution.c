/* FILE: A3_solutions.c is where you will code your answers for Assignment 3.
 * 
 * Each of the functions below can be considered a start for you. They have 
 * the correct specification and are set up correctly with the header file to
 * be run by the tester programs.  
 *
 * You should leave all of the code as is, especially making sure not to change
 * any return types, function name, or argument lists, as this will break
 * the automated testing. 
 *
 * Your code should only go within the sections surrounded by
 * comments like "REPLACE EVERTHING FROM HERE... TO HERE.
 *
 * The assignment document and the header A3_solutions.h should help
 * to find out how to complete and test the functions. Good luck!
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "A3_provided_functions.h"

unsigned char*
bmp_open( char* bmp_filename,        unsigned int *width, 
          unsigned int *height,      unsigned int *bits_per_pixel, 
          unsigned int *padding,     unsigned int *data_size, 
          unsigned int *data_offset                                  )
{
  unsigned char *img_data=NULL;
  // REPLACE EVERYTHING FROM HERE
  FILE *bmpfile = fopen(bmp_filename, "rb");

  //check if file is null
  if( bmpfile == NULL ){
    //clause in bmp info that prints a message
	  return img_data;
	}
  char b, m;
	fread (&b,1,1,bmpfile);
	fread (&m,1,1,bmpfile);

  //find the size of the file
	fread( data_size, 1, sizeof(unsigned int), bmpfile );
  rewind(bmpfile);

  //allocate the amount of heap memory
  img_data=(unsigned char*)malloc(*data_size*sizeof(unsigned char));
  fread( img_data, 1, *data_size, bmpfile );

  //assign the variables
  *width = *((unsigned int*)(img_data+18));
  unsigned int* hp=(unsigned int*)(img_data+22);
  *height = *hp;
  *bits_per_pixel = *((short int*)(img_data+28));
  *data_offset = *((unsigned int*)(img_data+10));
  *padding  = (4 - (*width*(*bits_per_pixel/8)) % 4) % 4;

  // TO HERE!  
  return img_data;  
}

void 
bmp_close( unsigned char **img_data )
{
  // REPLACE EVERYTHING FROM HERE
  if( *img_data != NULL ){
    free( *img_data );
    *img_data = NULL;
  }
  // TO HERE!  
}

unsigned char***  
bmp_scale( unsigned char*** pixel_array, unsigned char* header_data, unsigned int header_size,
           unsigned int* width, unsigned int* height, unsigned int num_colors,
           float scale )
{
 unsigned char*** new_pixel_array = NULL; 
  // REPLACE EVERYTHING FROM HERE
  //make variables for new_height and new_width to make enough space
  unsigned int scaled_height = (unsigned int) ((*height) * (scale));
  unsigned int* new_height = &scaled_height;
  unsigned int scaled_width = (unsigned int) ((*width) * (scale));
  unsigned int* new_width = &scaled_width;

  //reasign the values
  *width=*new_width;
  *height=*new_height;

  //use malloc to create space for new_pixel_array
  new_pixel_array = (unsigned char***)malloc( sizeof(unsigned char**) * (*new_height));
  if( new_pixel_array == NULL ){
    printf( "Error: bmp_scale failed to allocate memory for image of height %d.\n", (*height) );
    return NULL;
  }

//initialize enough space in the rows for new data
  for( int row=0; row<*new_height; row++ ){
    new_pixel_array[row] = (unsigned char**)malloc( sizeof(unsigned char*) * (*new_width) );
    for( int col=0; col<*new_width; col++ ){
      new_pixel_array[row][col] = (unsigned char*)malloc( sizeof(unsigned char) * (num_colors) );
    }
  }

//populate new pixel array
 for( int row=0; row<*new_height; row++ )
    for( int col=0; col<*new_width; col++ )
      for( int color=0; color<num_colors; color++ ){
        new_pixel_array[row][col][color] = pixel_array[(int)(row/scale)][(int)(col/scale)][color];
      }
        
//change the header data
int padding = ( 4 - ((*new_width) * num_colors) % 4) % 4;
int row_size = (*new_width)*num_colors+padding;
int image_size = header_size + (*height)*row_size;

*(unsigned int*)(header_data+2) = (image_size);

*(unsigned int*)(header_data+18) = *new_width;

*(unsigned int*)(header_data+22) = *new_height;

  // TO HERE! 
  return new_pixel_array;
}         

int 
bmp_collage( char* background_image_filename,     char* foreground_image_filename, 
             char* output_collage_image_filename, int row_offset,                  
             int col_offset,                      float scale )
{
  // REPLACE EVERYTHING FROM HERE
  //1. open each file
  //background
  unsigned int back_width;
  unsigned int back_height;
  unsigned int back_bpp;
  unsigned int back_pad;
  unsigned int back_data_size;
  unsigned int back_data_offset;
  unsigned char* back_img_data    = NULL;
  
  back_img_data= bmp_open( background_image_filename, &back_width, &back_height, &back_bpp, &back_pad, &back_data_size, &back_data_offset); 

  //foreground
  unsigned int fore_width;
  unsigned int fore_height;
  unsigned int fore_bpp;
  unsigned int fore_pad;
  unsigned int fore_data_size;
  unsigned int fore_data_offset;
  unsigned char* fore_img_data    = NULL;
  
  fore_img_data= bmp_open( foreground_image_filename, &fore_width, &fore_height, &fore_bpp, &fore_pad, &fore_data_size, &fore_data_offset);

  //1b. Check to make sure bpp =32
  if(fore_bpp != 32 || back_bpp != 32){
    printf( "Error: bpp is not 32 for one of the files" );
    return -1;
  }
  //close the images because we are going to open them again
  bmp_close(&fore_img_data);
  bmp_close(&back_img_data);

  //2. scale the foreground and turn both into pixel array
  unsigned char*   fore_header_data;
  unsigned int     fore_header_size, fore_num_colors;
  unsigned char*** fore_pixel_array = NULL;

  unsigned char*** back_pixel_array = NULL;
  unsigned char*   back_header_data;
  unsigned int     back_header_size, back_num_colors;

  //3D array opens the file to extract data
  fore_pixel_array = bmp_to_3D_array( foreground_image_filename, &fore_header_data, &fore_header_size,  &fore_width, &fore_height, &fore_num_colors);

  back_pixel_array = bmp_to_3D_array( background_image_filename, &back_header_data, &back_header_size,  &back_width, &back_height, &back_num_colors);
  
   if( fore_pixel_array == NULL || back_pixel_array == NULL){
    printf( "Error: bmp_to_3D_array failed for file %s.\n", foreground_image_filename );
    return -1;
  }
                                 
  unsigned char*** scaled_pixel_array = bmp_scale( fore_pixel_array, fore_header_data, fore_header_size,
                                                   &fore_width, &fore_height, fore_num_colors,  scale   );
                                                  
  if( scaled_pixel_array == NULL ){
    printf( "Error: Call to bmp_scale failed!\n" );
    return -1;
  } 
 
  //3. get the size of background and allocate memory for new file
  //get the size of the image in the foreground
  int fore_padding = ( 4 - (fore_width * fore_num_colors) % 4) % 4;
  int row_size = fore_width*fore_num_colors+fore_padding;
  int fore_image_size = fore_header_size + fore_height*row_size;

  //find the size the foreground image can take up //NEED TO MAKE SURE CORRECT  
  unsigned int max_width = back_width - col_offset;
  unsigned int max_height = back_height - row_offset;
  int off_padding = (4 - (max_width * 4) %4)%4;
  int max_row_size = max_width*4+off_padding;
  int max_pixel_size = back_header_size + max_height*max_row_size;

  //check to make sure forground will fit 
  if(max_pixel_size<fore_image_size){
    printf( "Error: foreground image too large\n" );
    return -1;
  }
  //4. overlay the files
  unsigned char*** new_pixel_array = NULL; 
  new_pixel_array = (unsigned char***)malloc( sizeof(unsigned char**) * (back_height));
  if( new_pixel_array == NULL ){
    printf( "Error: failed to allocate memory for image of height %d.\n", (back_height) );
    return -1;
  }

//initialize enough space in the rows for new data
  for( int row=0; row<back_height; row++ ){
    new_pixel_array[row] = (unsigned char**)malloc( sizeof(unsigned char*) * (back_width) );
    for( int col=0; col<back_width; col++ ){
      new_pixel_array[row][col] = (unsigned char*)malloc( sizeof(unsigned char) * (back_num_colors) );
    }
  }
  //set the new array equal to the background
  new_pixel_array = back_pixel_array;

  //find the value of color at the offset spot
  int color_offset = 0;
  for(int row = 0; row<back_height; row++){
    for(int col = 0; col<back_width; col++){
      for(int color = 0; color<back_num_colors; color++){
        if(row == row_offset && col == col_offset){
            color_offset = color;
            break;
        }
      }
    }
  }

  //overlay foreground where foreground has color
  for(int row = row_offset; row<fore_height + row_offset; row++){
    for(int col = col_offset; col<fore_width+col_offset; col++){
      for(int color = color_offset; color<fore_num_colors+color_offset; color++){
        if(scaled_pixel_array[row-row_offset][col-col_offset][color-color_offset] > 0 ){
          new_pixel_array[row][col][color] = scaled_pixel_array[row-row_offset][col-col_offset][color-color_offset];
        }
      }
    }
  }
  //transfer new_pixel_array to a file
    bmp_from_3D_array( output_collage_image_filename, back_header_data, back_header_size, new_pixel_array,
                     back_width,   back_height,      back_num_colors                       );
   
  // TO HERE! 
  return 0;
}              

