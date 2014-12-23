/*
 * statlib.c
 *
 *  Created on: Oct 27, 2014
 *      Author: justin
 */

#include "statlib.h"


/* calculate the mean value of the data in the list */
double mean(int totnum, double data[])
{
	double sum = 0.0;
	double meanval = 0;
	int i = 0;

	for(i=0; i<totnum; i++)
		sum += data[i];
	meanval = sum / totnum;

	return meanval;
}

double minimum(int totnum, double data[])
{
	int i = 0;
	int minIndex = 5;  //arbitrary val > max possible GPA

	for(i=0; i<totnum; i++)
	{
		if (data[i]<data[minIndex])
			minIndex = i;
	}

	return data[minIndex];
}

double maximum(int totnum, double data[])
{
	int i = 0;
	int maxIndex = 0;

	for(i=0; i<totnum; i++)
	{
		if(data[i]>data[maxIndex])
			maxIndex = i;
	}

	return data[maxIndex];
}

double median(int totnum, double data[])
{

	//ensure array is sorted for accuracy's sake
	sort(totnum, data);

	//n is odd
	if (totnum % 2)
		return data[(totnum)/2];

	//n is even
	else
		return (data[(totnum-1)/2] + data[((totnum-1)/2)+1]) / 2;

}

double variance(int totnum, double data[])
{
	int i = 0;
	double summation = 0;
	double variance = 0;
	double meanVal = mean(totnum, data);

	for(i=0; i<totnum; i++)
	{
		summation += pow(data[i] - meanVal, 2);
	}

	variance = summation / (totnum - 1);

	return variance;
}

double stdDeviation(int totnum, double data[])
{
	double stdDev = sqrt(variance(totnum, data));

	return stdDev;
}

int aboveVal(int totnum, double data[], double val)
{
	int i = 0;
	int count = 0;

	for(i=0; i<totnum; i++)
		if(data[i] >= val)
			count++;

	return count;
}

int belowVal(int totnum, double data[], double val)
{
	int i = 0;
	int count = 0;

	for(i=0; i<totnum; i++)
		if(data[i] <= val)
			count++;

	return count;
}

void sort(int totnum, double data[])
{
	int i = 0;
	int j = 0;
	double temp = 0;

	for(i=0; i<totnum-1; i++)
		for(j=0; j<totnum-1; j++)
			if(data[j] > data[j+1])
			{
				memcpy(&temp, &data[j], sizeof(data[j]));
				memcpy(&data[j], &data[j+1], sizeof(data[j+1]));
				memcpy(&data[j+1], &temp, sizeof(temp));
			}
}
