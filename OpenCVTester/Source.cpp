#include <future>
#include <thread>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
using namespace cv;
using namespace std;
#define CHECK_BIT(var,pos) ((var) & (1<<(pos)))
#define CHECK_BITNOTSET(var,pos) (!((var) & (1<<(pos))))
bool PatternChecker(unsigned __int64 number, unsigned __int64 gridCount, int threadNumber, int gridSize)
{
	auto isFilled = false;
	auto gridHeight = static_cast<int>(sqrt(gridCount));
	auto _tempNumber = number;
	int blockSize = 20;

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
		unsigned __int64 cellCount = i%gridHeight;
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
		stringstream ss;
		ss << "C:\\temp\\amazon\\" << number << "_" << threadNumber << ".jpg";
		cvSaveImage(ss.str().c_str(), img);
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
		if (PatternChecker(number, gridCount, threadNumber, gridSize))
		{		
			counter++;
		}
	}
	return counter;
}
int main()
{
	// gridSize = n
	auto gridSize = 5;
	unsigned __int64 gridCount = gridSize * gridSize;
	unsigned __int64 maxCount = pow(2, gridCount);
	clock_t begin_time = clock();
	unsigned __int64 n = maxCount / 4;
	// Thread it
	std::future<unsigned __int64> ret1 = std::async(&runner, 0 * n, 1 * n, gridCount, maxCount, 1, gridSize);
	std::future<unsigned __int64> ret2 = std::async(&runner, 1 * n, 2 * n, gridCount, maxCount, 2, gridSize);
	std::future<unsigned __int64> ret3 = std::async(&runner, 2 * n, 3 * n, gridCount, maxCount, 3, gridSize);
	std::future<unsigned __int64> ret4 = std::async(&runner, 3 * n, 4 * n, gridCount, maxCount, 4, gridSize);

	auto counter = ret1.get() +ret2.get() + ret3.get() + ret4.get();
	std::cout << endl << float(clock() - begin_time) / CLOCKS_PER_SEC << "s Threaded" << endl;
	cout << "Result: " << counter << endl;
	
	int x;
	cin >> x;
}