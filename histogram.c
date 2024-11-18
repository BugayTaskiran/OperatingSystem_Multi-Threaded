/**
 *
 * CENG305 Homework-1
 *
 * Histogram equalization with pthreads
 *
 * Usage:  main <input.jpg> <numthreads> 
 *
 * @author  your name
 *
 * @version 1.1, 02 November 2024
 */

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION

#include "stb_image.h"
#include "stb_image_write.h"
#define CHANNEL_NUM 1

// Global Variables - Do not touch them
int hist[256];
int cumhistogram[256];
int alpha[256];

void seq_histogram_equalizer(uint8_t* rgb_image,int width, int height);
void par_histogram_equalizer(uint8_t* rgb_image,int width, int height);

int main(int argc,char* argv[]) 
{		
    int width, height, bpp;
    int num_threads = atol(argv[2]);

    clock_t start, end;
    double elapsed_time;
	
	// Reading the image in grey colors
    uint8_t* rgb_image_for_seq = stbi_load(argv[1], &width, &height, &bpp, CHANNEL_NUM);
    uint8_t* rgb_image_for_par = stbi_load(argv[1], &width, &height, &bpp, CHANNEL_NUM);
	
    printf("Width: %d  Height: %d \n",width,height);
	printf("Input: %s , threads: %d  \n",argv[1],num_threads);
	
    ////////////////////////////////////////////////////////////////
    //
    // Sequential Code Starts
    //
    ////////////////////////////////////////////////////////////////

	// start the timer
    start = clock();
		
	seq_histogram_equalizer(rgb_image_for_seq,width, height);
    
	end = clock();
    elapsed_time = ((double)(end - start)) / CLOCKS_PER_SEC * 1000; // Convert to milliseconds
	printf("Sequential elapsed time: %lf \n",elapsed_time);

    ////////////////////////////////////////////////////////////////
    //
    // Multithreaded Code Starts
    //
    ////////////////////////////////////////////////////////////////

    start = clock();
		
	par_histogram_equalizer(rgb_image_for_par,width, height);
    
	end = clock();
    elapsed_time = ((double)(end - start)) / CLOCKS_PER_SEC * 1000; // Convert to milliseconds
	printf("Multi-thread elapsed time with %d threads: %lf \n",num_threads,elapsed_time);	

    // Check correctness of outputs here
    // ??
	
	// Storing the image 
    stbi_write_jpg("sequential_output.jpg", width, height, CHANNEL_NUM, rgb_image_for_seq, 100);
    stbi_write_jpg("multithread_output.jpg", width, height, CHANNEL_NUM, rgb_image_for_par, 100);
    stbi_image_free(rgb_image_for_seq);
    stbi_image_free(rgb_image_for_par);

    return 0;
}


void par_histogram_equalizer(uint8_t* rgb_image,int width, int height)
{

};

void seq_histogram_equalizer(uint8_t* rgb_image,int width, int height)
{			
	for(int i=0; i<height ; i++){
		for(int j=0; j<width; j++){
			hist[rgb_image[i*width + j]]++;
		}
	}	
	double size = width * height;
   
     //cumulative sum for histogram values    
	cumhistogram[0] = hist[0];
    for(int i = 1; i < 256; i++)
    {
        cumhistogram[i] = hist[i] + cumhistogram[i-1];
    }    
	    
    for(int i = 0; i < 256; i++)
    {
        alpha[i] = round((double)cumhistogram[i] * (255.0/size));
    }
			
    // histogram equlized image		
    for(int y = 0; y <height ; y++){
        for(int x = 0; x < width; x++){
            rgb_image[y*width + x] = alpha[rgb_image[y*width + x]];
		}
	}
	
}

