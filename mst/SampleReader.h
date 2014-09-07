#ifndef MST_SAMPLE_READER_H_
#define MST_SAMPLE_READER_H_

#include "util.h"

namespace mst {

class SampleReader
{
//
// This reader can analyze the sample files into a point list,
// which are in the proper format as the following:
// 
//   1. The first line, except the empty lines and commented lines
//      (starting with a '#') should be an integer that shows the
//      sample size.
//
//   2. In the following lines, a serial of numbers should be contained
//      and separated by spaces. Though they don't need to be on separate
//      lines, we recommand you to put each point in separated lines. For
//      each line, the x-coordinate goes first, then a space, then the 
//      y-coordinate. However, we don't allow comments after the "first 
//      line" mentioned before. Also, no characters except the numbers
//      and the dot(.) are allowed among the data.
//	 
//	 3. You may use numbers with a dot(.) for none-integers, like 1.2 or 30.45.
//      Otherwise, they will be regarded as integers. You'd better not contain
//      both integers and none-integers on the same sample file.
//
//   4. The reader will be complained if there is a syntax error on your file.
//      You may check it carefully to make sure it matches our rules, which
//      is rather easy to follow.
//
//   5. File extended name is NOT important. Select the ones you are familiar with. 
//

public:
    SampleReader(std::string file_name);
	virtual ~SampleReader() {}

    int get_exceptions() { return exc_; }
    const static int FILE_OPEN_FAILED = 1;
    const static int SAMPLE_REPEATED = 2;
    const static int NO_ERROR = 0;

    std::vector< Point<float> > get_samples() { return samples_; }

	void show();

private:
    bool read(std::string file_name);
	bool repeated();

    std::vector< Point<float> > samples_;
	int size_;

    int exc_;
    DISABLE_COPYING(SampleReader)
};

}	// namespace mst

#endif // MST_SAMPLE_READER_H_
