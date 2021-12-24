#include <iostream>
#include <thread>
#include <filesystem>
#include <string>
#include <vector>
#include <semaphore>

namespace fs = std::filesystem;
namespace{
	int num = 0;
	int checksum = 0;
	std::counting_semaphore<1> files(0);
	std::counting_semaphore<1> counts(0);
	std::mutex m;
}



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
int main()
{
	std::string s = ".";
	fs::path input(s);
	while (true) {
	std::cin >> s;
	input = fs::path(s);
	if(fs::exists(input) &&fs::is_directory(input)){
	break;
	}
	}
	std::thread first(print, input);
	std::thread second(count, input);
first.join();
second.join();

	return 0;
}
