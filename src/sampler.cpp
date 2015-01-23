#include "sampler.h"

namespace opencl {

#ifndef CL_VERSION_2_0
// /* Sampler APIs */
// extern CL_API_ENTRY cl_sampler CL_API_CALL
// clCreateSampler(cl_context          /* context */,
//                 cl_bool             /* normalized_coords */,
//                 cl_addressing_mode  /* addressing_mode */,
//                 cl_filter_mode      /* filter_mode */,
//                 cl_int *            /* errcode_ret */) CL_API_SUFFIX__VERSION_1_0;
NAN_METHOD(CreateSampler) {
  NanScope();
  REQ_ARGS(4);

  if(!isOpenCLObj(args[0])) {
    return NanThrowError(JS_INT(CL_INVALID_CONTEXT));
  }
  cl_context context=Unwrap<cl_context>(args[0]);
  cl_bool normalized_coords = args[1]->BooleanValue() ? CL_TRUE : CL_FALSE;
  cl_addressing_mode addressing_mode = args[2]->Uint32Value();
  cl_filter_mode filter_mode = args[3]->Uint32Value();

  cl_int ret=CL_SUCCESS;
  cl_sampler sw = ::clCreateSampler(
              context,
              normalized_coords,
              addressing_mode,
              filter_mode,
              &ret);
  CHECK_ERR(ret);

  NanReturnValue(Wrap(sw));
}

#else
// extern CL_API_ENTRY cl_sampler CL_API_CALL
// clCreateSamplerWithProperties(cl_context                     /* context */,
//                               const cl_sampler_properties *  /* normalized_coords */,
//                               cl_int *                       /* errcode_ret */) CL_API_SUFFIX__VERSION_2_0;
#endif

// extern CL_API_ENTRY cl_int CL_API_CALL
// clRetainSampler(cl_sampler /* sampler */) CL_API_SUFFIX__VERSION_1_0;
NAN_METHOD(RetainSampler) {
  NanScope();
  REQ_ARGS(1);

  if(!isOpenCLObj(args[0])) {
    return NanThrowError(JS_INT(CL_INVALID_SAMPLER));
  }

  cl_sampler sampler = Unwrap<cl_sampler>(args[0]);
  cl_int count=clRetainSampler(sampler);

  NanReturnValue(JS_INT(count));
}

// extern CL_API_ENTRY cl_int CL_API_CALL
// clReleaseSampler(cl_sampler /* sampler */) CL_API_SUFFIX__VERSION_1_0;
NAN_METHOD(ReleaseSampler) {
  NanScope();
  REQ_ARGS(1);

  if(!isOpenCLObj(args[0])) {
    return NanThrowError(JS_INT(CL_INVALID_SAMPLER));
  }

  cl_sampler sampler = Unwrap<cl_sampler>(args[0]);
  cl_int count=clReleaseSampler(sampler);

  NanReturnValue(JS_INT(count));
}

// extern CL_API_ENTRY cl_int CL_API_CALL
// clGetSamplerInfo(cl_sampler         /* sampler */,
//                  cl_sampler_info    /* param_name */,
//                  size_t             /* param_value_size */,
//                  void *             /* param_value */,
//                  size_t *           /* param_value_size_ret */) CL_API_SUFFIX__VERSION_1_0;
NAN_METHOD(GetSamplerInfo) {
  NanScope();
  REQ_ARGS(2);

  if(!isOpenCLObj(args[0])) {
    return NanThrowError(JS_INT(CL_INVALID_SAMPLER));
  }
  cl_sampler sampler=Unwrap<cl_sampler>(args[0]);
  cl_sampler_info param_name = args[1]->Uint32Value();

  switch(param_name) {
    case CL_SAMPLER_REFERENCE_COUNT:
    {
      cl_uint val;
      CHECK_ERR(::clGetSamplerInfo(sampler,param_name,sizeof(cl_uint), &val, NULL))
      NanReturnValue(JS_INT(val));
    }
    case CL_SAMPLER_CONTEXT:
    {
      cl_context val;
      CHECK_ERR(::clGetSamplerInfo(sampler,param_name,sizeof(cl_context), &val, NULL))
      // TODO NanReturnValue(JS_INT(val));
      break;
    }
    case CL_SAMPLER_NORMALIZED_COORDS:
    {
      cl_bool val;
      CHECK_ERR(::clGetSamplerInfo(sampler,param_name,sizeof(cl_bool), &val, NULL))
      NanReturnValue(JS_BOOL(val));
    }
    case CL_SAMPLER_ADDRESSING_MODE:
    {
      cl_addressing_mode val;
      CHECK_ERR(::clGetSamplerInfo(sampler,param_name,sizeof(cl_addressing_mode), &val, NULL))
      NanReturnValue(JS_INT(val));
    }
    case CL_SAMPLER_FILTER_MODE:
    {
      cl_filter_mode val;
      CHECK_ERR(::clGetSamplerInfo(sampler,param_name,sizeof(cl_filter_mode), &val, NULL))
      NanReturnValue(JS_INT(val));
    }
  }
  return NanThrowError(JS_INT(CL_INVALID_VALUE));
}

namespace Sampler {
void init(Handle<Object> exports)
{
#ifndef CL_VERSION_2_0
  NODE_SET_METHOD(exports, "createSampler", CreateSampler);
#else
  NODE_SET_METHOD(exports, "createSamplerWithProperties", CreateSamplerWithProperties);
#endif
  NODE_SET_METHOD(exports, "retainSampler", RetainSampler);
  NODE_SET_METHOD(exports, "releaseSampler", ReleaseSampler);
  NODE_SET_METHOD(exports, "getSamplerInfo", GetSamplerInfo);
}
} // namespace Sampler

} // namespace opencl