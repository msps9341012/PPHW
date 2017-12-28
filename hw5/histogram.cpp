#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include <fstream>
#include <iostream>
#include <CL/opencl.h>
#define MAX_SOURCE_SIZE (0x100000)

int main(int argc, char const *argv[])
{
	
	FILE *fp;
	size_t source_size;
	char *source_str;
	const char filename[]="./histogram.cl";	
	fp=fopen(filename,"r");
	source_str=(char *)malloc(MAX_SOURCE_SIZE);
	source_size=fread(source_str,1,MAX_SOURCE_SIZE,fp);
	fclose(fp);


	
	//unsigned int * histogram_results;
	unsigned int i=0, a, input_size;
	std::fstream inFile("input", std::ios_base::in);
	std::ofstream outFile("0553411.out", std::ios_base::out);
	inFile >> input_size;
	unsigned int *image = new unsigned int[input_size];
	while( inFile >> a ) {
		image[i++] = a;
	}

	unsigned int *result=(unsigned int *)malloc(256*3*sizeof(unsigned int));
	memset(result,0x0,256*3*sizeof(unsigned int));
	
	cl_int err;
	cl_uint num_devices,num_platforms;
	cl_device_id device;
	cl_platform_id platform;

	
	clGetPlatformIDs(1,&platform,&num_platforms);
	err=clGetDeviceIDs(platform,CL_DEVICE_TYPE_GPU,1,&device,&num_devices);
	cl_context myctx=clCreateContext(NULL,1,&device,NULL,NULL,&err);
	cl_command_queue myqueue=clCreateCommandQueue(myctx,device,0,&err);
	
	cl_mem img_gpu=clCreateBuffer(myctx,CL_MEM_READ_ONLY,sizeof(unsigned int)*input_size,NULL,&err);
	cl_mem result_gpu=clCreateBuffer(myctx,CL_MEM_WRITE_ONLY,sizeof(unsigned int)*256*3,NULL,&err);


	err=clEnqueueWriteBuffer(myqueue,img_gpu,CL_TRUE,0,sizeof(unsigned int)*input_size,image,0,NULL,NULL);
	clEnqueueWriteBuffer(myqueue,result_gpu,CL_TRUE,0,sizeof(unsigned int)*256*3,result,0,NULL,NULL);
	
	cl_program myprog=clCreateProgramWithSource(myctx,1,(const char **)&source_str,(const size_t *)&source_size,&err);
	err=clBuildProgram(myprog,1,&device,NULL,NULL,NULL);
	cl_kernel mykernel=clCreateKernel(myprog,"histogram",&err);
	
	clSetKernelArg(mykernel,0,sizeof(cl_mem),&img_gpu);
	clSetKernelArg(mykernel,1,sizeof(cl_mem),&result_gpu);
	clSetKernelArg(mykernel,2,sizeof(unsigned int),&input_size);
	
	size_t gsize=768;
	err=clEnqueueNDRangeKernel(myqueue,mykernel,1,0,&gsize,0,0,NULL,NULL);
	err=clEnqueueReadBuffer(myqueue,result_gpu,CL_TRUE,0,sizeof(unsigned int)*256*3,result,NULL,NULL,NULL);

	for(unsigned int i = 0; i < 256 * 3; ++i) {
		if (i % 256 == 0 && i != 0)
			outFile << std::endl;
		outFile << result[i]<< ' ';
	}

	inFile.close();
	outFile.close();

	return 0;
}
