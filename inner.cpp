#include <iostream> // std::cout
#include <functional> // std::minus, std::divides
#include <numeric> // std::inner_product
#include <vector>
#include "Hhashing.h"

int main()
{

	std::vector<int> series1;
	std::vector<int> series2;
	series1.push_back(1);
	series2.push_back(10);
	series1.push_back(2);
	series2.push_back(20);
	// int n = sizeof(series1) / sizeof(series1[0]);

	// Elements in series1
	std::cout << "First array contains :";
	for (int i = 0; i < series1.size(); i++)
		std::cout << " " << series1[i];
	std::cout << "\n";

	// // Elements in series2
	// std::cout << "Second array contains :";
	// for (int i = 0; i < n; i++)
	// 	std::cout << " " << series2[i];
	// std::cout << "\n\n";
	Hhash hello = Hhash(series1, 10, 3);
	int dp = hello.hash(series1);
	std::cout << dp << std::endl;

	return 0;
}

