#ifndef _UWM_PGM_HPP_
#define _UWM_PGM_HPP_

#include <vector>
#include <iostream>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <malloc.h>
#include <memory.h>
#include <fstream>

namespace UWM
{

	class PGM{
	public:	 
		static void read(
			const std::string file_path,
			std::vector<unsigned char> &vector_image,
			int &x_dimension,
			int &y_dimension)
		{
			FILE *fp;
			if ((fp = fopen(file_path.c_str(), "rb")) == NULL)
			{
				printf("read error...\n");
				exit(-1);
			}
			int maxraw;
			int c;
			int i, j;
			int val;
			unsigned char *line;
			char buf[1024];
			unsigned char *image;

			while ((c = fgetc(fp)) == '#')
				fgets(buf, 1024, fp);
			ungetc(c, fp);
			if (fscanf(fp, "P%d\n", &c) != 1)
			{
				printf("read error 1 ....");
				exit(0);
			}
			if (c != 5 && c != 2)
			{
				printf("read error 2 ....");
				exit(0);
			}

			if (c == 5)
			{
				while ((c = fgetc(fp)) == '#')
					fgets(buf, 1024, fp);
				ungetc(c, fp);
				if (fscanf(fp, "%d%d%d", &x_dimension, &y_dimension, &maxraw) != 3)
				{
					printf("failed to read width/height/max\n");
					exit(0);
				}
				printf("Width=%d, Height=%d \nMaximum=%d\n", x_dimension, y_dimension, maxraw);
				vector_image.resize(x_dimension * y_dimension);
				image = (unsigned char *)malloc(sizeof(unsigned char) * x_dimension * y_dimension);
				getc(fp);

				line = (unsigned char *)malloc(sizeof(unsigned char) * x_dimension);
				for (j = 0; j < y_dimension; j++)
				{
					fread(line, 1, x_dimension, fp);
					for (i = 0; i < x_dimension; i++)
					{
						image[j * x_dimension + i] = line[i];
						vector_image.at(j * x_dimension + i) = line[i];
					}
				}
				free(line);
			}

			else if (c == 2)
			{
				while ((c = fgetc(fp)) == '#')
					fgets(buf, 1024, fp);
				ungetc(c, fp);
				if (fscanf(fp, "%d%d%d", &x_dimension, &y_dimension, &maxraw) != 3)
				{
					printf("failed to read width/height/max\n");
					exit(0);
				}
				printf("Width=%d, Height=%d \nMaximum=%d,\n", x_dimension, y_dimension, maxraw);

				image = (unsigned char *)malloc(sizeof(unsigned char) * x_dimension * y_dimension);
				getc(fp);

				for (j = 0; j < y_dimension; j++)
					for (i = 0; i < x_dimension; i++)
					{
						fscanf(fp, "%d", &val);
						image[j * x_dimension + i] = val;
						vector_image.at(j * x_dimension + i) = val;
					}
			}

			fclose(fp);
			

			free(image);
		}

		static void read(
			const std::string file_path,
			unsigned char * vector_image,
			int &x_dimension,
			int &y_dimension)
		{
			FILE *fp;
			if ((fp = fopen(file_path.c_str(), "rb")) == NULL)
			{
				printf("read error...\n");
				exit(-1);
			}
			int maxraw;
			int c;
			int i, j;
			int val;
			unsigned char *line;
			char buf[1024];
			unsigned char *image;

			while ((c = fgetc(fp)) == '#')
				fgets(buf, 1024, fp);
			ungetc(c, fp);
			if (fscanf(fp, "P%d\n", &c) != 1)
			{
				printf("read error 1 ....");
				exit(0);
			}
			if (c != 5 && c != 2)
			{
				printf("read error 2 ....");
				exit(0);
			}

			if (c == 5)
			{
				while ((c = fgetc(fp)) == '#')
					fgets(buf, 1024, fp);
				ungetc(c, fp);
				if (fscanf(fp, "%d%d%d", &x_dimension, &y_dimension, &maxraw) != 3)
				{
					printf("failed to read width/height/max\n");
					exit(0);
				}
				printf("Width=%d, Height=%d \nMaximum=%d\n", x_dimension, y_dimension, maxraw);		
				image = (unsigned char *)malloc(sizeof(unsigned char) * x_dimension * y_dimension);
				getc(fp);

				line = (unsigned char *)malloc(sizeof(unsigned char) * x_dimension);
				for (j = 0; j < y_dimension; j++)
				{
					fread(line, 1, x_dimension, fp);
					for (i = 0; i < x_dimension; i++)
					{
						image[j * x_dimension + i] = line[i];
					
					}
				}
				free(line);
			}

			else if (c == 2)
			{
				while ((c = fgetc(fp)) == '#')
					fgets(buf, 1024, fp);
				ungetc(c, fp);
				if (fscanf(fp, "%d%d%d", &x_dimension, &y_dimension, &maxraw) != 3)
				{
					printf("failed to read width/height/max\n");
					exit(0);
				}
				printf("Width=%d, Height=%d \nMaximum=%d,\n", x_dimension, y_dimension, maxraw);

				image = (unsigned char *)malloc(sizeof(unsigned char) * x_dimension * y_dimension);
				getc(fp);

				for (j = 0; j < y_dimension; j++)
					for (i = 0; i < x_dimension; i++)
					{
						fscanf(fp, "%d", &val);
						image[j * x_dimension + i] = val;
					
					}
			}

			fclose(fp);
			vector_image = image;

		
		}
		static void write(
			const std::string file_path,
			std::vector<unsigned char> &vector_image,
			const int &x_dimension,
			const int &y_dimension)
		{

			if (vector_image.empty())
				throw "no image found";
			FILE *fp;
			std::cout << "saving file to " << file_path << std::endl;
			if ((fp = fopen(file_path.c_str(), "wb")) == NULL)
			{
				printf("write pgm error....\n");
				exit(1);
			}
			int i, j;
			fprintf(fp, "P5\n%d %d\n%d\n", x_dimension, y_dimension, 255);
			for (j = 0; j < y_dimension; j++)
				for (i = 0; i < x_dimension; i++)
				{
					fputc(vector_image.at(j * x_dimension + i), fp);
				}
			fclose(fp);
		
		}

		static void write(
			const std::string file_path,
			unsigned char *vector_image,
			const int &x_dimension,
			const int &y_dimension)
		{

			
			FILE *fp;
			std::cout << "saving file to " << file_path << std::endl;
			if ((fp = fopen(file_path.c_str(), "wb")) == NULL)
			{
				printf("write pgm error....\n");
				exit(1);
			}
			int i, j;
			fprintf(fp, "P5\n%d %d\n%d\n", x_dimension, y_dimension, 255);
			for (j = 0; j < y_dimension; j++)
				for (i = 0; i < x_dimension; i++)
				{
					fputc(vector_image[j * x_dimension + i], fp);
				}
			fclose(fp);
		
		}
	};
}

#endif