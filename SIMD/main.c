#include <stdlib.h>
#include <stdio.h>
#include <time.h>

// SIMD (lab 6)

// Pseudo-code to implement:
/*
    for (all_images)
    {
        read_image_from_file
        start_time = get_time ()
        for (all_pixels_in_the_image)
        {
            if (curent_pixel_value < threshold) then new_pixel_value = 0
            else new_pixel_value = 255
        }
        end_time = get_time ()
        out >> "Computing time " end_time - start_time
        write_image_result
    }
*/

// each greyscale pixel is 0-255(0xFF)
// convert to pure B&W image by setting a threshold (e.g., 128), and change pixel to 0 or 255

static const int threshold = 128;

int main()
{
    // for each image in current directory, read image from file, and write result to output file
    void images[] = {};
    
    for (int i=0; i<len(images); i++)
    {
        // read image from file

        clock_t start, end;
        start = clock();

        // process image using SIMD instructions
        asm_func();
        
        end = clock();
        float time = (float)(end - start)/CLOCKS_PER_SEC;
        printf("Time spent: %f\n", time);

        // write result to output file
        
    }
    return EXIT_SUCCESS;
}

void asm_func()
{

    __asm__
    {
        /* ASM code */
        "sub $1, %%rdx\n"
    }
}
