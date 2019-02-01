// expression_template.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "sarray.h"
#include "array_expr.h"
#include <iostream>
#include <string>
#include <vector>

template<typename Arr>
void printArray(const Arr& arr, const std::string& arr_name = "")
{
	if (!arr_name.empty())
	{
		std::cout << arr_name << ": ";
	}

	for (size_t i = 0; i < arr.size(); ++i)
	{
		std::cout << arr[i] << " ";
	}

	std::cout << endl;
}

int _tmain(int argc, _TCHAR* argv[])
{
	//typedef SArray<int> IntArr;
	typedef Array<int> IntArr;
	//typedef Array<int, std::vector<int>> IntArr;

	const size_t size = 10;

	{
		IntArr arr0(size), arr1(size), arr2(size);

		for (size_t i = 0; i < size; ++i)
		{
			arr0[i] = i;
			arr1[i] = i * 10;

			arr2[i] = (10 * arr0[i] + arr0[i] * arr1[i]) * 100;
		}

		auto arr3 = (10 * arr0 + arr0 * arr1) * 100;
		auto arr4 = static_cast<SArray<int>>(arr1 - arr0);

		printArray(arr0, "arr0");
		printArray(arr1, "arr1");
		printArray(arr2, "arr2");
		printArray(arr3, "arr3");
		printArray(arr4, "arr4");
	}

	system("pause");

	return 0;
}

