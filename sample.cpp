#include <iostream>
#include <fstream>
#include "FixedLenFileIO.h"
struct SampleData
{
	int n;
	int j;
};
int main(int argc ,char** argv)
{
	FixedLenFileIO<SampleData> testdata("sample.dat");
	SampleData buf = { 0 };
	buf.j = 10;
	buf.n = 20;
	testdata[0] = buf;
	SampleData buf2 = (SampleData)testdata[0];

	std::cout << buf2.j << " " << buf2.n << std::endl;
}