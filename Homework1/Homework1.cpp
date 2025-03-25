/*
Реализуйте алгоритм сортировки выбором.
Сортировка выбором состоит в том, что в неупорядоченном списке находитсянаименьший элемент. 
Выбранный в исходном списке минимальный элементзаписывается на i-е место исходного списка (i=1,2,…,п), 
а элемент с i-го места— на место выбранного. При этом очевидно, что уже упорядоченныеэлементы, 
а они будут расположены, начиная с первого места, исключаются издальнейшей сортировки, поэтому длина списка, участвующего в каждом

Поиск минимального элемента реализуйте в виде асинхронной задачи.

Результат должен возвращаться в основной поток через связку std::promise-std::future.
*/

#include <iostream>
#include <future>
//#include <thread>

constexpr size_t arrSize = 5;
std::atomic<int> arr[arrSize]{ 2, 4, 1, 0, 3 };     // Сделал массив глобальным, чтобы уменьшить число аргументов std::thread

/// <summary>
/// Поиск индекса минимального элемента
/// </summary>
/// <param name="index">Начальный индекс поиска минимума</param>
/// <param name="promiseMin">std::promise</param>
void findMin(size_t index, std::promise<size_t>& promiseMin)
{
    size_t minIndex{ index };
    for (size_t i{index + 1}; i < arrSize; ++i)
    {
        if (arr[i].load() < arr[minIndex].load())
        {
            minIndex = i;
        }
    }
    promiseMin.set_value(minIndex);
}

/// <summary>
/// Перестановка элементов в массиве
/// </summary>
/// <param name="index1">Индекс первого элемента</param>
/// <param name="index2">Индекс второго элемента</param>
void swapElements(size_t index1, size_t index2)
{
    if (index1 == index2)
    {
        return;     // Ничего не меняем, если минимальный элемент находится в начале
    }
    int temp{ arr[index1].load()};
    arr[index1].store(arr[index2].load());
    arr[index2].store(temp);
}

/// <summary>
/// Сортировка выбором части массива
/// </summary>
/// <param name="index">Начальный индекс поиска минимума</param>
void asyncChoiseSort(size_t index)
{
    if (index >= arrSize)
    {
        return;
    }

    std::promise<size_t> promiseMin{};
    std::future<size_t> futureMin{ promiseMin.get_future() };
    std::thread myThread(findMin, index, std::ref(promiseMin));
    //futureMin.wait();
    swapElements(index, futureMin.get());
    myThread.join();
    asyncChoiseSort(index + 1);
}

/// <summary>
/// Сортировка выбором всего массива
/// </summary>
void asyncChoiseSort()
{
    asyncChoiseSort(0);
}

/// <summary>
/// Вывод массива на экран
/// </summary>
void printArray()
{
    for (size_t i{}; i < arrSize; ++i)
    {
        std::cout << arr[i].load() << "  ";
    }
    std::cout << std::endl;
}

int main()
{
    

    std::cout << "Array before sorting: ";
    printArray();

    asyncChoiseSort();

    std::cout << "Array after sorting:  ";
    printArray();

    return EXIT_SUCCESS;
}