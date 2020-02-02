#include <future>
#include <thread>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>

#include <fstream>
#include <string>

using namespace cv;
using namespace std;
#define CHECK_BIT(var,pos) ((var) & (1<<(pos)))
#define CHECK_BITNOTSET(var,pos) (!((var) & (1<<(pos))))
bool PatternChecker(unsigned __int64 number, unsigned __int64 gridCount, int threadNumber, int gridSize)
{
	auto isFilled = false;
	auto gridHeight = static_cast<int>(sqrt(gridCount));
	auto _tempNumber = number;
	int blockSize = 7;

	auto img = cvCreateImage(cvSize(gridHeight * blockSize, gridHeight * blockSize + blockSize * 2), 8, 3);
	cvZero(img); // zero image to make it black
	auto thickness = 2; // Ensure that no gaps and that flood fill won't calculate incorrectly
	auto lineType = 8;
	auto start = cvPoint(0, 0);
	auto end = cvPoint(gridHeight * blockSize, gridHeight * blockSize + blockSize * 2);
	auto color = Scalar(255, 255, 255);
	cvRectangle(img, start, end, color, thickness, lineType);

	for (unsigned __int64 i = 0; i < gridCount; i++)
	{
		unsigned __int64 x = _tempNumber & 0x01;
		_tempNumber = _tempNumber >> 1;
		unsigned __int64 cellCount = i % gridHeight;
		unsigned __int64 rowCount = i / gridHeight;
		if (x == 1)
		{
			cvLine(img, cvPoint((cellCount)* blockSize, rowCount * blockSize + blockSize), cvPoint(((cellCount)+1) * blockSize, rowCount * blockSize + blockSize * 2), color, thickness, lineType);
		}
		else
		{
			cvLine(img, cvPoint((cellCount)* blockSize, rowCount * blockSize + blockSize * 2), cvPoint(((cellCount)+1) * blockSize, rowCount * blockSize + blockSize), color, thickness, lineType);
		}
	}
	cvFloodFill(img, cvPoint(blockSize / 2, blockSize / 2), cvScalar(120, 200, 50)); // Flood fill from top pixel (10,10)
	CvScalar s = cvGet2D(img, gridHeight * blockSize + blockSize + blockSize / 2, gridHeight * blockSize / 2);	// middle pixel in bottom block

	if ((s.val[2] > 0)) // if the colour is not black then floodFill could reach here
	{
		isFilled = true;
		/*
		stringstream ss;
		ss << "C:\\temp\\amazon\\" << number << "_" << threadNumber << ".jpg";
		cvSaveImage(ss.str().c_str(), img);
		*/
	}
	cvReleaseImage(&img);
	return isFilled;
}

unsigned __int64 runner(unsigned __int64 start, unsigned __int64 end, unsigned __int64 gridCount, unsigned __int64 maxCount, int threadNumber, int gridSize)
{
	auto counter = 0;

	vector<int> listTest;
	for (unsigned __int64 number = start; number < end; number++)
	{
		// 6x6
		
		// 2886218022912 101010 000000 000000 000000 000000 000000 000000
		// 1443109011456 010101 000000 000000 000000 000000 000000 000000

		// 68719476736

		// 6
		// 45097156608		101010 000000 000000 000000 000000 000000
		// 22548578304		010101 000000 000000 000000 000000 000000
		// 5
		// 704643072			101010 000000 000000 000000 000000
		// 352321536			010101 000000 000000 000000 000000
		// 4
		// 11010048				101010 000000 000000 000000
		// 5505024				010101 000000 000000 000000
		// 3
		// 172032					101010 000000 000000
		// 86016					010101 000000 000000
		// 2
		// 2688						101010 000000
		// 1344						010101 000000
		// 1
		// 42							101010
		// 21							010101		

		if (number % 45097156608 == 0 || number % 22548578304 == 0)
		{
			number += 46656;
			continue;
		}
		if (number % 704643072 == 0 || number % 352321536 == 0)
		{
			number += 7776;
			continue;
		}
		if (number % 11010048 == 0 || number % 5505024 == 0)
		{
			number += 1296;
			continue;
		}
		if (number % 172032 == 0 || number % 86016 == 0)
		{
			number += 216;
			continue;
		}
		if (number % 2688 == 0 || number % 1344 == 0)
		{
			number += 36;
			continue;
		}
		if (number % 42 == 0 || number % 21 == 0)
		{
			number += 1;
			continue;
		}
		if (PatternChecker(number, gridCount, threadNumber, gridSize))
		{
			counter++;
		}
	}
	return counter;
}
__int64 minFunction(__int64 x, __int64 y)
{
	if (x > y)
		return y;
	return x;
}
__int64 saver()
{
	__int64 numPaths = 0;
	__int64 start = 0;

	ifstream myfile("example.txt");
	string line;

	std::string item;
	std::vector<std::string> splittedStrings;

	if (myfile.is_open())
	{
		while (std::getline(myfile, line))
		{
			std::stringstream ss(line);
			while (std::getline(ss, item, ','))
			{
				splittedStrings.push_back(item);
			}
		}
		myfile.close();
		auto x = splittedStrings[1].c_str();
		start = std::stoull(x);
		numPaths = std::stoull(splittedStrings[2].c_str());
	}

	__int64 counter = 16777216;
	__int64 end = start + counter;

	auto gridSize = 6;
	unsigned __int64 gridCount = gridSize * gridSize;
	unsigned __int64 maxCount = pow(2, gridCount);

	end = minFunction(end, maxCount);
	unsigned __int64 n = counter / 4;

	ofstream myfile2;
	while (true)
	{
		clock_t begin_time = clock();
		if (start == end)
		{
			cout << "start == end" << endl;
			break;
		}
		// Thread it			
		std::future<unsigned __int64> ret1 = std::async(&runner, start + 0 * n, start + 1 * n, gridCount, maxCount, 1, gridSize);
		std::future<unsigned __int64> ret2 = std::async(&runner, start + 1 * n, start + 2 * n, gridCount, maxCount, 2, gridSize);
		std::future<unsigned __int64> ret3 = std::async(&runner, start + 2 * n, start + 3 * n, gridCount, maxCount, 3, gridSize);
		std::future<unsigned __int64> ret4 = std::async(&runner, start + 3 * n, start + 4 * n, gridCount, maxCount, 4, gridSize);

		numPaths += ret1.get() + ret2.get() + ret3.get() + ret4.get();
		std::cout << start << " " << end << " " << numPaths << endl;
		myfile2.open("example.txt", ios::trunc);
		myfile2 << std::to_string(start) + "," + std::to_string(end) + "," + std::to_string(numPaths);
		myfile2.close();
		start += counter;
		end = start + counter;

		end = minFunction(end, maxCount);

		if (start > maxCount)
		{
			cout << "start > maxCount" << endl;
			break;
		}
		std::cout << endl << float(clock() - begin_time) / CLOCKS_PER_SEC << "s Threaded, togo: " << maxCount - end << endl;
	}

	return numPaths;

}
int main()
{	
	__int64 numPaths = saver();	
	cout << "Result: " << numPaths << endl;
	int x;
	cin >> x;
}