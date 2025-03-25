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
/// Вывод массива в консоль
/// </summary>
/// <typeparam name="T">Тип значений</typeparam>
/// <typeparam name="Iterator">Итератор</typeparam>
/// <param name="first">Начало массива</param>
/// <param name="last">Конец массива</param>
template<typename T, typename Iterator>
void printArray(Iterator first, Iterator last)
{
    int64_t size{ std::distance(first, last) };
    Iterator iterator{ first };
    while (iterator != last)
    {
        std::cout << *(iterator++) << "  ";
    }
    std::cout << std::endl;
}

/// <summary>
/// Функция параллельного суммирования элементов массива
/// </summary>
/// <typeparam name="Iterator">Итератор</typeparam>
/// <typeparam name="T">Тип значений</typeparam>
/// <param name="first">Начало массива</param>
/// <param name="last">Конец массива</param>
/// <returns>Сумма элементов массива</returns>
template<typename Iterator, typename T>
T parallel_fun(Iterator first, Iterator last)
{
    int64_t size{ std::distance(first, last) };
    if (size > 2)
    {
        Iterator iterator{ first };
        std::advance(iterator, size / 2);
        std::future<T> leftInit{ std::async(parallel_fun<Iterator, T>,first, iterator) };
        std::future<T> rightInit{ std::async(parallel_fun<Iterator, T>, iterator, last) };
        return leftInit.get() + rightInit.get();
    }
    else
    {
        T sum{};
        Iterator iterator = first;
        while (iterator != last)
        {
            sum += *(iterator++);
        }
        return sum;
    }
}

int main()
{
    
    printArray<int, std::array<int, 5>::iterator>(arr.begin(), arr.end());

    std::cout << "sum = " << parallel_fun<std::array<int, 5>::iterator, int>(arr.begin(), arr.end());

    return EXIT_SUCCESS;
}
