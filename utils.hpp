//
//  utils.hpp
//  TraceARay01
//
//  Created by Riccardo Gigante on 09/01/2019.
//

#ifndef utils_hpp
#define utils_hpp

#include <math.h>

// Linear interpolation template function
template <class T>T Lerp(T start, T end, float blend);
template <class T>T Lerp(T start, T end, float blend)
{
	return start * (1.0 - blend) + end * blend;
}

// clipping template function
template <typename T>
T clip(T in, T low, T high);
template <typename T>
T clip(T in, T low, T high)
{
	return fmin(fmax(in, low), high);
}


// Schlick formula approximation
float schlick(float cosine, float ref_idx);

#endif /* utils_hpp */
