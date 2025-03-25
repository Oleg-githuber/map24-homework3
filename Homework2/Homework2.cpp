/*
Реализуйте параллельный вариант оператора for_each.

Функция должна принимать итераторы на начало и конец контейнера иссылку, обрабатывающую функцию.

При реализации нужно рекурсивно разделить контейнер на несколько блокови для каждого запустить отдельную задачу, 
применяющую обрабатывающуюфункцию к части контейнера.
*/

#include <iostream>
#include <future>
#include <array>

std::array<int, 5> arr{ 4, 2, 1, 0, 3 };    // Глобальный массив

/// <summary>
/// Вычисление суммы элементов массива
/// </summary>
/// <typeparam name="T">Тип элементов массива</typeparam>
/// <typeparam name="Iterator">Итератор</typeparam>
/// <param name="first">Начало массива</param>
/// <param name="last">Конец массива</param>
/// <returns>Сумма</returns>
template<typename T, typename Iterator>
T calculateSum(Iterator first, Iterator last)
{
	T sum{};
	Iterator iterator{ first };
	while (iterator != last)
	{
		sum += *(iterator++);
	}
	return sum;
}

/// <summary>
/// Параллельный foreach
/// </summary>
/// <typeparam name="T">Тип элементов массива</typeparam>
/// <typeparam name="Iterator">Итератор</typeparam>
/// <param name="first">Начало массива</param>
/// <param name="last">Конец массива</param>
/// <param name="func">Указатель на функцию вычисления суммы</param>
/// <returns>Возврат суммы элементов</returns>
template<typename T, typename Iterator>
T parallel_foreach(Iterator first, Iterator last, T (*func)(Iterator, Iterator))
{
	int64_t size{ std::distance(first, last) };
	if (size > 2)
	{
		Iterator iterator{ first };
		std::advance(iterator, size / 2);
		std::future<T> left{ std::async(parallel_foreach<T, Iterator>, first, iterator, func) };
		std::future<T> right{ std::async(parallel_foreach<T, Iterator>, iterator, last, func) };
		return left.get() + right.get();
	}
	else
	{
		return func(first, last);
	}
}

template<typename T, typename Iterator>
void printArray(Iterator first, Iterator last)
{
	std::cout << "Array: ";
	Iterator iterator{ first };
	while (iterator != last)
	{
		std::cout << *(iterator++) << "  ";
	}
	std::cout << std::endl;
}

int main()
{
	printArray<int, std::array<int, 5>::iterator>(arr.begin(), arr.end());
	std::cout << "sum = " << parallel_foreach<int, std::array<int, 5>::iterator>(arr.begin(), arr.end(), &calculateSum);

    return EXIT_SUCCESS;
}
