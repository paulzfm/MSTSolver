#include "SampleReader.h"
#include <cstdlib>
#include <map>

// type checking with the help of RE
#include "../qutil.h"
#include <QString>

namespace mst {


SampleReader::SampleReader(std::string file_name)
{
    exc_ = 0;
    if (read(file_name))
    {
        if (repeated())
            exc_ = 2;
    }
    else
        exc_ = 1;
}

bool SampleReader::read(std::string file_name)
{
    std::ifstream in(file_name.c_str());
	if (!in)
        return false;

	// read sample size
	std::string buf;
	while (getline(in, buf))
	{
		std::string tmp;
		for (int i = 0; i < buf.length(); ++i)
		{
			if (buf[i] == '#')
				break;

			if (buf[i] != ' ' && buf[i] != '\t')
				tmp += buf[i];
		}
		if (tmp.length() != 0)	// get the size number
		{
			size_ = atoi(tmp.c_str());
			break;
		}
	}

    // read points
    for (int i = 0; i < size_; ++i)
    {
        float x, y;
        in >> x >> y;
        samples_.push_back(Point<float>(x, y));
    }

	in.close();
    return true;
}

bool SampleReader::repeated()
{
    for (int i = 0; i < size_; ++i)
        for (int j = i + 1; j < size_; ++j)
            if (samples_[i] == samples_[j])
                return true;
    return false;
}

void SampleReader::show()
{
    for (int i = 0; i < size_; ++i)
        std::cout << (i + 1) << " - " << samples_[i] << "\n";
}


}   // namespace mst
