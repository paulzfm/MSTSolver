#include "Prim.h"

namespace mst {


void Prim::generate(float** G, int n)
{
	float* lowcost = new float[n];
	int* closest = new int[n];	// the closest point (-1 for searched)
	
	// init
	for (int i = 0; i < n; ++i)
		lowcost[i] = INF;
	for (int i = 1; i < n; ++i)
		closest[i] = 0;

	// add the first point
	closest[0] = -1;

	int curr = 0; // latest point added

	for (int k = 1; k < n; ++k)
	{
		// std::cout << "round " << num << "\n";
		int min = -1;
		float mincost = INF;
		for (int i = 0; i < n; ++i)
		{
			if (closest[i] != -1)	// select a unsearched point
			{
				float temp = G[curr][i];
				if (temp < lowcost[i])
				{
					lowcost[i] = temp;
					closest[i] = curr;
				}
				if (lowcost[i] < mincost)
					mincost = lowcost[min = i];
			}
		}

		weight_ += mincost;
		edges_.push_back(closest[min]);
		closest[min] = -1;	// searched
		edges_.push_back(min);
		curr = min;
	}
}

float Prim::get_weight()
{
	return weight_;
}

std::vector<int> Prim::get_edges()
{
	return edges_;
}


} // namespace mst