//
//  utils.cpp
//  TraceARay01
//
//  Created by Riccardo Gigante on 09/01/2019.
//

#include "utils.hpp"

float schlick(float cosine, float ref_idx)
{
	float r0 = (1 - ref_idx) / (1 + ref_idx);
	r0 = r0 * r0;
	return r0 + (1 - r0 ) * pow((1 - cosine), 5);
}
