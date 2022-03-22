#include <stdio.h>
#include <stdlib.h>
#include <png.h>
#include <string.h>
#include <errno.h>
#include <stdarg.h>

char* ReadIMG(char*, float);

char brightness[] = " .'`^\",:;I1!i><~+_-?][}{l)(|\\/tfjrxnuvczXYUJCLQ0OZmwqodbkhao*#MW&8%B@$";
int len;
float multipier = 0;
int main(int argc, char* argv[]){
    len = strlen(brightness);
    if(argc < 3){
        printf("ERROR : Usage => %s <image file> <scaleDown>\n",argv[0]);
        exit(0);
    }
    float ratio;
    sscanf(argv[2], "%f", &ratio);
    ReadIMG(argv[1], ratio);
    return 0;
}

char* ReadIMG(char* name, float ratio){
    FILE* outputFile;
    const char * png_file = name;
    png_structp	png_ptr;
    png_infop info_ptr;
    FILE * fp;
    png_uint_32 width;
    png_uint_32 height;
    int bit_depth;
    int color_type;
    int interlace_method;
    int compression_method;
    int filter_method;
    int j;
    png_bytepp rows;
    fp = fopen (png_file, "rb");
    if (! fp) {
	//fatal_error ("Cannot open '%s': %s\n", png_file, strerror (errno));
    }
    png_ptr = png_create_read_struct (PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (! png_ptr) {
	//fatal_error ("Cannot create PNG read structure");
    }
    info_ptr = png_create_info_struct (png_ptr);
    if (! png_ptr) {
	//fatal_error ("Cannot create PNG info structure");
    }
    char buf[64];
    sprintf(buf, "%s.txt", name);
    outputFile = fopen(buf, "w");
    png_init_io (png_ptr, fp);
    png_read_png (png_ptr, info_ptr, 0, 0);
    png_get_IHDR (png_ptr, info_ptr, & width, & height, & bit_depth,
		  & color_type, & interlace_method, & compression_method,
		  & filter_method);
    rows = png_get_rows (png_ptr, info_ptr);
    printf ("Width is %d, height is %d\n", width, height);
    int rowbytes;
    rowbytes = png_get_rowbytes (png_ptr, info_ptr);
    printf ("Row bytes = %d\n", rowbytes);
    int itter = 0;
    for (j = 0; j < height; j+=ratio) {
	int i;
	png_bytep row;
	row = rows[j];
	for (i = 0; i < rowbytes; i+=(ratio*2)) {
	    png_byte pixel;
	    pixel = row[i];
        int o = (len*pixel)/255;
        o = (o >= len || o < 0) ? len-1:o;
        printf("\rON ITTERATION => %d", itter);
        //printf("%c",brightness[o]);
        fprintf(outputFile, "%c", brightness[o]);
        itter++;
	}
	//printf ("\n");
    fprintf(outputFile, "\n");
    }
    printf("\n");
    fclose(outputFile);
}