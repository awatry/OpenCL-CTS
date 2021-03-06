//
// Copyright (c) 2017 The Khronos Group Inc.
// 
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//    http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
#ifndef _kernelHelpers_h
#define _kernelHelpers_h

#include "compat.h"

#include <stdio.h>
#include <stdlib.h>

#if defined (__MINGW32__)
#include <malloc.h>
#endif

#include <string.h>

#ifdef __APPLE__
    #include <OpenCL/opencl.h>
#else
    #include <CL/opencl.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

/*
 *  The below code is intended to be used at the top of kernels that appear inline in files to set line and file info for the kernel:
 *
 *  const char *source = {
 *      INIT_OPENCL_DEBUG_INFO
 *      "__kernel void foo( int x )\n"
 *      "{\n"
 *      "   ...\n"
 *      "}\n"
 *  };
 */
#define INIT_OPENCL_DEBUG_INFO                      SET_OPENCL_LINE_INFO( __LINE__, __FILE__ )
#define SET_OPENCL_LINE_INFO(_line, _file)          "#line " STRINGIFY(_line) " " STRINGIFY(_file) "\n"
#ifndef STRINGIFY_VALUE
    #define STRINGIFY_VALUE(_x)                     STRINGIFY(_x)
#endif
#ifndef STRINGIFY
    #define STRINGIFY(_x)                           #_x
#endif

/* Helper that creates a single program and kernel from a single-kernel program source */
extern int create_single_kernel_helper( cl_context context, cl_program *outProgram, cl_kernel *outKernel, unsigned int numKernelLines, const char **kernelProgram, const char *kernelName );

/* Helper to obtain the biggest fit work group size for all the devices in a given group and for the given global thread size */
extern int get_max_common_work_group_size( cl_context context, cl_kernel kernel, size_t globalThreadSize, size_t *outSize );

/* Helper to obtain the biggest fit work group size for all the devices in a given group and for the given global thread size */
extern int get_max_common_2D_work_group_size( cl_context context, cl_kernel kernel, size_t *globalThreadSize, size_t *outSizes );

/* Helper to obtain the biggest fit work group size for all the devices in a given group and for the given global thread size */
extern int get_max_common_3D_work_group_size( cl_context context, cl_kernel kernel, size_t *globalThreadSize, size_t *outSizes );

/* Helper to get major/minor number for a device */
extern int get_device_version( cl_device_id id, size_t* major, size_t* minor);

/* Helper to obtain the biggest allowed work group size for all the devices in a given group */
extern int get_max_allowed_work_group_size( cl_context context, cl_kernel kernel, size_t *outSize, size_t *outLimits );

/* Helper to determine if an extension is supported by a device */
extern int is_extension_available( cl_device_id device, const char *extensionName );

/* Helper to determine if a device supports an image format */
extern int is_image_format_supported( cl_context context, cl_mem_flags flags, cl_mem_object_type image_type, const cl_image_format *fmt );

/* Helper to get pixel size for a pixel format */
size_t get_pixel_bytes( const cl_image_format *fmt );

/* Verify the given device supports images. 0 means you're good to go, otherwise an error */
extern int verifyImageSupport( cl_device_id device );

/* Checks that the given device supports images. Same as verify, but doesn't print an error */
extern int checkForImageSupport( cl_device_id device );
extern int checkFor3DImageSupport( cl_device_id device );

/* Checks that a given queue property is supported on the specified device. Returns 1 if supported, 0 if not or an error. */
extern int checkDeviceForQueueSupport( cl_device_id device, cl_command_queue_properties prop );

/* Helper for aligned memory allocation */
void * align_malloc(size_t size, size_t alignment);
void   align_free(void *);

/* Helper to obtain the min alignment for a given context, i.e the max of all min alignments for devices attached to the context*/
size_t get_min_alignment(cl_context context);

/* Helper to obtain the default rounding mode for single precision computation. (Double is always CL_FP_ROUND_TO_NEAREST.) Returns 0 on error. */
cl_device_fp_config get_default_rounding_mode( cl_device_id device );

#define PASSIVE_REQUIRE_IMAGE_SUPPORT( device )    \
    if( checkForImageSupport( device ) )    \
    {    \
        log_info( "\n\tNote: device does not support images. Skipping test...\n" );    \
        return 0;    \
    }

#define PASSIVE_REQUIRE_3D_IMAGE_SUPPORT( device )    \
    if( checkFor3DImageSupport( device ) )    \
    {    \
        log_info( "\n\tNote: device does not support 3D images. Skipping test...\n" );    \
        return 0;    \
    }

/* Prints out the standard device header for all tests given the device to print for */
extern int printDeviceHeader( cl_device_id device );

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // _kernelHelpers_h
