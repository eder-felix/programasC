/* Federal University of Maranhao
 * Author: FELIX, Eder M. S.
 * Date: June 13, 2019. Sao Luis, MA, Brazil
 * Description: This program calculates the mandelbrots fractal.
 * between -2 and 1 in the x-axis and 1 and -1 in the y-axis.
 *
 *
 */
/*
 * Includes
 */
#include<pthread.h>
#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include <jpeglib.h>
#include<complex.h>

/*
 * Defines
 */
#define RES 0.001 //Fractals resolution
#define WIDTH 12000 //Fractals width
#define HEIGHT 8000 //Fractals height
#define MAX_ITER 100 //Maximum number of iterations
#define COR_PER_PIXEL 3
/*
 * Generate fractal image FUNCTION
 */
void generate_fractal_image(unsigned char* _image_buffer){
	double a,b; /* a is the real parte, b is the imag part */
	double complex zn, point_c;
	double temp = 0.0;
	unsigned int pixelId;
	float width = (float) WIDTH;
	float height = (float) HEIGHT;
	int flag = 0;
	int pixel = 0;

	for(int x=0; x<HEIGHT; x++){
		for(int y=0; y<WIDTH; y++){
			pixelId = ((x*WIDTH)+y)*COR_PER_PIXEL;
			a = 3.0*((float)y/width) - 2.0;
			b = -2.0*((float)x/height) + 1.0;
			
			point_c = a + b * I;
			zn = 0 + 0*I;

			for(int k=0; k<MAX_ITER; k++){
				
				zn = zn*zn + point_c;
				if(cabs(zn) > 2.0){
					flag = 1;
					pixel = (255*k)/MAX_ITER;
					break;
				}
			}

			//temp = pow(xn,2.0) + pow(yn,2.0);
			if(flag==0){
				//then the point is part of mandelbroit set!
				_image_buffer[pixelId+0] = 0;   // r |-- Set r,g,b components to
      				_image_buffer[pixelId+1] = 0;     // g |   make this pixel red
      				_image_buffer[pixelId+2] = 0;     // b |   (255,0,0).
				
			}else{
				// the point is not part of the mandelbroit set
				_image_buffer[pixelId+0] = pixel;   // r |-- Set r,g,b components to
      				_image_buffer[pixelId+1] = pixel;   // g |   make this pixel white
      				_image_buffer[pixelId+2] = pixel;   // b |   (255,255,255).
				flag = 0;
			}
		}

	}


}
/*
 * Main FUNCTION
 */
void main(int argc, char* argv[]){
	unsigned char* image_buffer;	/*Array that contains the image info*/
	image_buffer = (unsigned char*)malloc(WIDTH*HEIGHT*COR_PER_PIXEL);
	
	printf("Calculating the mandelbroit set...");
	fflush(stdout);
	generate_fractal_image(image_buffer);
	printf("DONE\n");
	/*CONFIGURING JPEG PARAMETERS ************************/
	printf("Generating JPEG image...");
	fflush(stdout);
	char *filename = (char *)"fractal_jpeg.jpg";
  	int quality    = 100	;
  
  	struct jpeg_compress_struct cinfo;    // Basic info for JPEG properties.
  	struct jpeg_error_mgr jerr;           // In case of error.
  	FILE * outfile;                       // Target file.
  	JSAMPROW row_pointer[1];              // Pointer to JSAMPLE row[s].
  	int row_stride;                       // Physical row width in image buffer.
  
  	//## ALLOCATE AND INITIALIZE JPEG COMPRESSION OBJECT
  
  	cinfo.err = jpeg_std_error(&jerr);
  	jpeg_create_compress(&cinfo);
  
 	 //## OPEN FILE FOR DATA DESTINATION:
  
  	if ((outfile = fopen(filename, "wb")) == NULL) {
    		fprintf(stderr, "ERROR: can't open %s\n", filename);
    		exit(1);
  	}
  	jpeg_stdio_dest(&cinfo, outfile);

	//## SET PARAMETERS FOR COMPRESSION:
  
  	cinfo.image_width  = WIDTH;           // |-- Image width and height in pixels.
  	cinfo.image_height = HEIGHT;           // |
  	cinfo.input_components = COR_PER_PIXEL;         // Number of color components per pixel.
  	cinfo.in_color_space = JCS_RGB;     // Colorspace of input image as RGB.
  
  	jpeg_set_defaults(&cinfo);
  	jpeg_set_quality(&cinfo, quality, TRUE);
	
	//## START COMPRESSION:
	jpeg_start_compress(&cinfo, TRUE);
  	row_stride = cinfo.image_width * 3;        // JSAMPLEs per row in image_buffer
  
  	while (cinfo.next_scanline < cinfo.image_height)
  	{
    		row_pointer[0] = &image_buffer[cinfo.next_scanline * row_stride];
    		(void) jpeg_write_scanlines(&cinfo, row_pointer, 1);
  	}
            // NOTE: jpeg_write_scanlines expects an array of pointers to scanlines.
            //       Here the array is only one element long, but you could pass
            //       more than one scanline at a time if that's more convenient.
  
  	//## FINISH COMPRESSION AND CLOSE FILE:
  
  	jpeg_finish_compress(&cinfo);
  	fclose(outfile);
  	jpeg_destroy_compress(&cinfo);
  
  	printf("DONE\n");  
  
  	exit(0);
}
