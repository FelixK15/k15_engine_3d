#ifndef _K15_Render_Config_h_
#define _K15_Render_Config_h_

//comment to disable per command debug logs
#define K15_SHOW_PER_COMMAND_DEBUG_LOG

//comment to disable the check if a dispatched render command queue has been dispatched before
#define K15_CHECK_FOR_ALREADY_DISPATCHED_RENDER_COMMAND_QUEUE

//comment to disable loading shader from resource context (load from blob instead) - WIP
#define K15_LOAD_SHADER_FROM_RESOURCE_CONTEXT

//comment to disable checks for invalid gpu handles (added for runtime resource reloading because render resource
//would have an invalid handle for one frame. Shouldn't occur in the final product)
#define K15_TOLERATE_INVALID_GPU_HANDLES

#define K15_ENABLE_DEBUG_RENDERING

//comment to disable render pipeline checks
#define K15_ENABLE_RENDER_PIPELINE_EVALUATION

#define K15_MAX_MIP_MAPS 20

#endif //_K15_Render_Config_h_
