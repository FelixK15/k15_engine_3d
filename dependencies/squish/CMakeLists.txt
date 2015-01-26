project(squish)

include_directories(./)

set(SQUISH_HEADER "alpha.h"
				  "clusterfit.h"
				  "colourblock.h"
				  "colourfit.h"
				  "colourset.h"
				  "config.h"
				  "maths.h"
				  "rangefit.h"
				  "simd.h"
				  "simd_float.h"
				  "simd_sse.h"
				  "simd_ve.h"
				  "singlecolourfit.h"
				  "squish.h")

set(SQUISH_SOURCE "alpha.cpp"
				  "clusterfit.cpp"
				  "colourblock.cpp"
				  "colourfit.cpp"
				  "colourset.cpp"
				  "maths.cpp"
				  "rangefit.cpp"
				  "singlecolourfit.cpp"
				  "squish.cpp")

source_group("include" FILES ${SQUISH_HEADER})
source_group("src" FILES ${SQUISH_SOURCE})

add_library(squish ${SQUISH_SOURCE} ${SQUISH_HEADER})