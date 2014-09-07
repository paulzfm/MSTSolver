// This Prim algorithm is optimal in searching the minimal
// cost edge, which 
// 

#ifndef MST_PRIM_H_
#define MST_PRIM_H_

#include <vector>
#include "util.h"

namespace mst {

class Prim
{
public:
	Prim(float** weight_matrix, int size)
	{
		weight_ = 0;
		generate(weight_matrix, size);
	}
	virtual ~Prim() {}

	float get_weight();	// get the total weight
	std::vector<int> get_edges();

	static const float INF = 1e9;

private:
	void generate(float** G, int n);
	float weight_;
	std::vector<int> edges_;

    DISABLE_COPYING(Prim)
};


}

#endif // MST_PRIM_H_
