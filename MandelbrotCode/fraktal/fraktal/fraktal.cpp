#include "pch.h"
#include <omp.h>
#include <iostream>
#include <chrono>
#include <sstream>
#include <ctime>  
#include <string>  
const char backgroundColor[3] = { 255, 255, 255};
const char threadColors[4][3] = {
	{ 255, 0, 0 }, // red
	{ 0, 255, 0 }, // green
	{ 0, 0, 255 }, // blue
	{ 255, 255, 0 }, // yellow
};
int WIDTH = 1000;
int HEIGHT = 1000;
char colorsMatrix[5000][5000];
void fraktal(const double CxMin, const double CxMax, const double CyMin, const double CyMax, const int threads, const char* name)
{
	/* screen ( integer) coordinate */
	int iX, iY;
	const int iXmax = WIDTH;
	const int iYmax = HEIGHT;
	/* world ( double) coordinate = parameter plane*/
	double Cx, Cy;
	double PixelWidth = (CxMax - CxMin) / iXmax;
	double PixelHeight = (CyMax - CyMin) / iYmax;
	/* color component ( R or G or B) is coded from 0 to 255 */
	/* it is 24 bit color RGB file */
	const int MaxColorComponentValue = 255;
	FILE * fp;
	const char *filename = name;
	const char *comment = "# ";/* comment should start with # */
	static unsigned char color[3];
	/* Z=Zx+Zy*i ; Z0 = 0 */
	double Zx, Zy;
	double Zx2, Zy2; /* Zx2=Zx*Zx; Zy2=Zy*Zy */
	/* */
	int Iteration;
	const int IterationMax = 10000;
	/* bail-out value , radius of circle ; */
	const double EscapeRadius = 2;
	double ER2 = EscapeRadius * EscapeRadius;
	/*create new file,give it a name and open it in binary mode */
	fp = fopen(filename, "wb"); /* b - binary mode */
	/*write ASCII header to the file*/
	fprintf(fp, "P6\n %s\n %d\n %d\n %d\n", comment, iXmax, iYmax, MaxColorComponentValue);

	/* compute and write image data bytes to the file*/
	double start = omp_get_wtime();
	omp_set_num_threads(threads);
#pragma omp parallel private(iY, iX, Cy, Cx, Zx, Zy, Zx2, Zy2, Iteration)
	{
#pragma omp for schedule(guided)
		for (iY = 0; iY < iYmax; iY++)
		{
			Cy = CyMin + iY * PixelHeight;
			if (fabs(Cy) < PixelHeight / 2) Cy = 0.0; /* Main antenna */
			for (iX = 0; iX < iXmax; iX++)
			{
				Cx = CxMin + iX * PixelWidth;
				/* initial value of orbit = critical point Z= 0 */
				Zx = 0.0;
				Zy = 0.0;
				Zx2 = Zx * Zx;
				Zy2 = Zy * Zy;
				/* */
				for (Iteration = 0; Iteration < IterationMax && ((Zx2 + Zy2) < ER2); Iteration++)
				{
					Zy = 2 * Zx*Zy + Cy;
					Zx = Zx2 - Zy2 + Cx;
					Zx2 = Zx * Zx;
					Zy2 = Zy * Zy;
				};
				/* compute pixel color (24 bit = 3 bytes) */
				if (Iteration == IterationMax)
				{ /* interior of Mandelbrot set = black */
					colorsMatrix[iY][iX] = omp_get_thread_num();
				}
				else
				{ /* exterior of Mandelbrot set = white */
					colorsMatrix[iY][iX] = -1;
				};

			}
		}
	}

	char threadNum;
	for (iY = 0; iY < iYmax; iY++) {
		for (iX = 0; iX < iXmax; iX++) {
			threadNum = colorsMatrix[iY][iX];
			if (threadNum == -1) {
				fwrite(backgroundColor, 1, 3, fp);
			}
			else {
				fwrite(threadColors[threadNum], 1, 3, fp);
			}
		}
	}

	fclose(fp);
	double end = omp_get_wtime();

	//fprintf(stderr, "Czas: %f\n", end - start);
}

int main(int argc, char *argv[])
{
	auto now = std::chrono::system_clock::now();
	auto now_ms = std::chrono::time_point_cast<std::chrono::milliseconds>(now);

	auto value = now_ms.time_since_epoch();
	long duration = value.count();
	int threads = 15;
	if (argc > 1) {
		WIDTH = strtol(argv[1], NULL, 0);
		HEIGHT = WIDTH;
	}
	if (argc > 2) {
		threads = strtol(argv[2], NULL, 0);
	}
	int i, j;
	/*for (i = 10000 - 1; i >= 0; i--) {
		for (j = 10000 - 1; j >= 0; j--) {
			colorsMatrix[i][j] = -1;
		}
	}*/
	char* fileName = new char[20];
	std::string id = std::to_string(duration);
	char* fileTemp = new char[20];
	strcpy(fileTemp, "fraktal_");
	strcat(fileTemp, id.c_str());

	strcpy(fileName, "fraktals/");
	strcat(fileName, fileTemp);
	strcat(fileName, ".ppm");
	omp_set_dynamic(0);
	fraktal(-2.5, 1.5, -2, 2, threads, fileName);
	std::cout << fileTemp;
	exit(0);
}
