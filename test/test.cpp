#include <iostream>
#include <thread>
#include <filesystem>
#include <string>
#include <vector>
#include <semaphore>
#include "gtest/gtest.h"

namespace fs = std::filesystem;
namespace{
	int num = 0;
	int checksum = 0;
	std::counting_semaphore<1> files(0);
	std::counting_semaphore<1> counts(0);
}
using namespace std::chrono;


void print(fs::path pathToFind)
{
	std::vector<std::string> result(0);
	while (true) {
	checksum = 0;
	result.clear();
			for (auto& x : fs::directory_iterator(pathToFind))
				{
					if (x.is_regular_file())
					{
						std::string tmp = x.path();
						result.push_back(tmp);
						++checksum;
					}
				}
	files.release();
	counts.acquire();
	if(num == checksum) 
	{
	break;
	} 	
	}	
	std::cout << "Path to files: \n";
	for (auto x : result) 
	{
		std::cout << x << "\n";
	}
}

void print2(fs::path pathToFind)
{
	std::vector<std::string> result(0);
	while (true) {
	checksum = 0;
	result.clear();
			for (auto& x : fs::directory_iterator(pathToFind))
				{
					if (x.is_regular_file())
					{
						std::string tmp = x.path();
						result.push_back(tmp);
						++checksum;
					}
				}
	std::this_thread::sleep_for(3000ms);
	files.release();
	counts.acquire();
	if(num == checksum) 
	{
	break;
	} 	
	}	
	std::cout << "Path to files: \n";
	for (auto x : result) 
	{
		std::cout << x << "\n";
	}
}



void count(fs::path pathToFind)
{
	while (true) {
		files.acquire();
		auto it = fs::directory_iterator(pathToFind);
		for (auto& x: it)
		{
		if (x.is_regular_file())
			{
			++num;
			}
		}
		if(num == checksum)
		{
			std::cout << "Number of files: " << num << "\n";
			counts.release();
			break;
		} else 
		{
			counts.release();
		}

}
}
/*
TEST(Ordinary, Check_General)
{
	std::string s = "./../check1";
	fs::path input(s);
	::testing::internal::CaptureStdout();
	std::thread first(print, input);
	std::thread second(count, input);
	first.join();
	second.join();
	std::string expected = "Number of files: 4\nPath to files: \n./../check1/file1.txt\n./../check1/file2.txt\n./../check1/file3.txt\n./../check1/file4.txt\n";
	std::string obtained = testing::internal::GetCapturedStdout();
	EXPECT_EQ(expected, obtained);
}
*/
TEST(Ordinary, Check_synchronization)
{
	std::string s = "./../check2";
	fs::path input(s);
	std::string s1 = "./../check2/file1.txt";
	fs::path from(s1);
	std::string s2 = "./../check2/file5.txt";
	fs::path to(s2);
	fs::copy(from, to);
	::testing::internal::CaptureStdout();
	std::thread first(print2, input);
	fs::remove(to);
	std::thread second(count, input);
	first.join();
	second.join();
	std::string expected = "Number of files: 4\nPath to files: \n./../check2/file1.txt\n./../check2/file2.txt\n./../check2/file3.txt\n./../check2/file4.txt\n";
	std::string obtained = testing::internal::GetCapturedStdout();
	EXPECT_EQ(expected, obtained);
}
int main()
{
	::testing::InitGoogleTest();
	return RUN_ALL_TESTS();
	return 0;
}
