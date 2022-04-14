//g++ -std=c++11 test_movie.cpp -o test_movie



/*
* script to track a movie file passed as arguments, along with a center and initial edge point, 
* and then find the contours along each (frame?). Saves the pixels into a text file called moviefile_contour_full.txt
* which can then be used by the python scripts for analysis.
* Supposedly every provided point within a certain tolerance will give the exact same contour out, so just need to be "good enough".
*/

#include "temika_header.h"
#include "get_movie_frame.h"
#include <string.h>
#include "contour_tracker-trackmovie_lib.h"
#include "contour_analyzer-trackmovie_lib.h"

#define SAMPLE 10

#define I_MAX 10000

#define VERBOSE 1
#define VERBOSE1 0

int main(int argc, char **argv)
{

	/*
	List of successful (attempted) arguments:
	../../data_remote/floppy.11Nov2021_11.18.23.movie (87 161 38 159)

	*/

	char *moviefile = argv[1];

	if(VERBOSE)
	{
		printf("Tracking %s\n", moviefile);
	}

	// useful for initialising
	double new_center[2];
	new_center[0]=atof(argv[2]);
	new_center[1]=atof(argv[3]);

	

	struct camera_frame_struct frame; //Temika frame struct
	long offset = 0; //Keep track of movie position
	int i = 0; //Frames opened so far
	int bad_frames = 0; //Frames which were untracked

	const char *movie_ext = ".movie";


	if(VERBOSE)
	{
		printf("about to generate filenames \n");
	}

	//Makes the seed contour_filename based on movie filename
	
	/*
	char contour_filename[1000];
	strcpy(contour_filename, moviefile);
	char *p = strstr(contour_filename, movie_ext);
	*p = '\0';
	strcat(contour_filename, "_contour.txt");*/

	//Makes the full contour_filename based on movie filename
	char contour_filename_new[1000];
	strcpy(contour_filename_new, moviefile);
	char *p_new = strstr(contour_filename_new, movie_ext);
	*p_new = '\0';
	strcat(contour_filename_new, "_contour_full.txt");


	if(VERBOSE)
	{
		printf("Filenames generated \n"); // not yet the files themselves -- is it hoping that the seed file is already created?
	}



	// Initialize the contour tracking struct -- for later use in the functions
	struct ct::ContourStruct ct_st;
	ct_st.max = 1000;
	ct_st.burn = 10;
	ct_st.chirality = 1;
	ct_st.contour_fine = (double*) malloc(2*ct_st.max*sizeof(double));
	ct_st.contour_px = (int*) malloc(2*ct_st.max*sizeof(int));
	ct::Vector position_vector(-1,-1);
	ct::Vector center(-1, -1);
	ct_st.position_vector = &position_vector;
	ct_st.center = &center;
	ct_st.horizontal_window = 5;
	ct_st.slope_window = 4;

	if(VERBOSE)
	{
		printf("ct_st initialised \n");
	}

	//This, I hope, can be gotten rid off
	double old_contour[2*SAMPLE];


	//double new_center[2]; // where do we assign values to this? - in contour_center function
	double new_position[2];
	new_position[0]=atof(argv[4]);
	new_position[1]=atof(argv[5]);

	struct Contour cs; // All it has is {double *contour; int max_i;}

	//For fancy printing
	int characters = 0;


	if(VERBOSE)
	{
		printf("about to open file...\n");
	}

	//Try opening the file
	FILE *file;
	if ( !( file = fopen(moviefile, "rb" ) ) )
	{
		printf( "Couldn't open movie file.\n" );
		exit( EXIT_FAILURE );
	}
        fseek(file, 0, SEEK_SET);

	while(((offset = get_next_frame(file, &frame)) != -1) && i < I_MAX) // not sure why this is currently only going to i=2030
	{
		if (i == 0) // init
		{


			if(VERBOSE1)
			{
				printf("First frame setup\n");
			}


			//allocate the image array based on first frame. This will break if frames can change (easy fix, but it never happens)
			ct_st.rows = frame.size_x;
			ct_st.cols = frame.size_y;
			ct_st.im_array = (double*) malloc(sizeof(double)*frame.size_x*frame.size_y);

			//load the seed contour (not all. just a subset of size SAMPLE). Write to old_contour (why?). Get the contour_center.
			//load_contour(contour_filename, &cs); // initialises the cs structure -- not working as ftpr is NULL currently




			/* // was useful for the function above, which seems the only place cs is used
			if(VERBOSE)
			{
				printf("load_contour successful\n");
				printf("The contour is: %f\n", cs.contour);
			}
			*/

			
			//for (int j = 0; j < SAMPLE; j++)
			//{
				/*
				if(VERBOSE)
				{
					printf("j=%i\n",j); // having some problems in here...
					printf("Should be giving %f", cs.contour[2*j*cs.max_i/SAMPLE]); // not sure if the cs.contour is actually given in every case
				}
				*/


				//old_contour[2*j] = cs.contour[2*j*cs.max_i/SAMPLE];  /// keep for i != 0
				//old_contour[2*j + 1] = cs.contour[2*j*cs.max_i/SAMPLE + 1];

				/*
				if(VERBOSE)
				{
					printf("j=%i iteration success\n", j);
				}
				*/
			//}



			//contour_center(&cs, new_center); // initialises new_center. cs initialised in load_contour. [function is in the analyzer header]
			ct_st.center->x = new_center[0];
			ct_st.center->y = new_center[1];
		
			//For testing purposes
			//ct_st.center->x = 112.;
			//ct_st.center->y = 112.;

		}

		//Copy the frame to the array. This is slow, but can be sped up if one cares to do it.
		image_array(&frame, ct_st.im_array); // function in get_movie_frame.h

		//Initialize position_vector, contour_px, contour_fine, done, etc
		//Uncommenting leads to seg fault
		for (int j = 0; j < SAMPLE; j++)
		{
			ct_st.position_vector->x = new_position[0]; 
			ct_st.position_vector->y = new_position[1]; 
			ct_st.contour_px[0] = (int) ct_st.position_vector->x;
			ct_st.contour_px[1] = (int) ct_st.position_vector->y;
			ct_st.contour_fine[0] = (double) ct_st.contour_px[0];
			ct_st.contour_fine[1] = (double) ct_st.contour_px[1];

			ct_st.done = 0;
			ct_st.max_i = 0;

			if(VERBOSE1)
			{
				printf("\nj=%i iteration, about to get_contour\n",j);
			}

			ct::get_contour(&ct_st); // currently problematic
			if (ct_st.max_i != -1)
			{
				break;
			}
		}

		if(VERBOSE1)
		{
			printf("ct_st parameters setup\n");
		}

		if (ct_st.max_i == -1) //Bad frame
		{
				bad_frames++;
		}
		else //Good frame
		{
			for (int j = 0; j < SAMPLE; j++) //Copy current contour into old_contour
			{
				old_contour[2*j] = ct_st.contour_fine[2*j*ct_st.max_i/SAMPLE];
				old_contour[2*j + 1] = ct_st.contour_fine[2*j*ct_st.max_i/SAMPLE + 1];
			}
		}


		if(VERBOSE1)
		{
			printf("Updated old_contour. About to save to file...\n");
			printf("Old contour: %f\n", old_contour);
		}
		save_contour(contour_filename_new, ct_st); //Save everything


		if(VERBOSE1)
		{
			printf("Saved. Now up to bit where we had printing before...\n");
		}
		if(VERBOSE)
		{
			//This is just to print stuff out
			if (i == 0)
			{
				printf("i = %d", i);
			}
			else if (i % 10 == 0)
			{
				for (int cc = 0; cc < characters; cc++)
				{
					printf("\b"); // removes the previous i and so prints every 10 frames
				}

				printf("%d", i);
			}
			fflush(stdout); // output formatting and choose characters to delete
			characters = (int) ((i == 0) ? 1 : (log10(i) + 1));
		}

		i++;

	}

	if (VERBOSE)
	{
		for (int cc = 0; cc < characters + 4; cc++)
		{
			printf("\b");
		}
		printf("Total frames = %d\n", i);
		printf("Frames dropped = %d\n", bad_frames); // should add this to the text file somewhere
		printf("\n");
	}

	free(ct_st.contour_fine);
	free(ct_st.contour_px);
	free(ct_st.im_array);

	return 0;
}
