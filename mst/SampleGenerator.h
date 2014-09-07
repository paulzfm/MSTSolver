#ifndef MST_SAMPLE_GENERATOR_H_
#define MST_SAMPLE_GENERATOR_H_

#include "util.h"
#include <ctime>
#include <string>

namespace mst {

class SampleGenerator
{
public:
    SampleGenerator(bool is_integer, int size, std::string path)
        : size_(size), path_(path)
	{
		std::srand(std::time(0));
		generate(is_integer);
	}
	virtual ~SampleGenerator() {}

	std::vector< Point<int> > get_samples(int);
	std::vector< Point<float> > get_samples();

private:
    std::string path_;

	std::vector< Point<int> > samples_int_;
	std::vector< Point<float> > samples_float_;
	int max_;							// maximum value can exist
	int size_;							// sample size

	Point<int> get_random_point(int);	// return a point with type int
	Point<float> get_random_point();	// return a point with type float

	bool repeated(int current, int);	// check whether the point is repeated with the previous
	bool repeated(int current);			// first for int and the last for float

	void generate(bool is_integer);

	void save_to_file();

    DISABLE_COPYING(SampleGenerator)
};

} // namespace mst

#endif // MST_SAMPLE_GENERATOR_H_
