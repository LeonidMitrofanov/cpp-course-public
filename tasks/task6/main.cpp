/**
 * @file main.cpp
 * @brief задача написать программу, которая будет высчитывать значение интеграла через метод трапеций
 * Особенность этой задачи: использовать потоки(threads) для ускорения вычислений (нужно разделить задачу для вычисления части интеграла на каждый поток)
 *
 * на вход программе при запуске(это в argv[]) подается 3 числа: a, b, n, tn где [a, b] - интервал (целочисленный, неотрицательный и не больше 50 для задачи), n - количество разбиений, tn - threads number - количество потоков для подсчёта
 * примечание: n - количество разбиений в тестах будет явно нацело делиться на tn - количество потоков.
 *
 * на выход программе выведите значение интеграла через метод трапеций (вывод в стандартный поток std::cout, в конце выведите '\n'), вывод в поток с точностью до 4 знака
 * в функции trapezoidalIntegral есть переменная const std::function<double (double)> &f, при вызове подсчётов передайте ссылку на функцию из задания
 * реализовать подсчёт интеграла S(a, b) = (1+e^x)^0.5 dx
 * 
 *
 * литература:
 * https://ru.wikipedia.org/wiki/Метод_трапеций
 * https://habr.com/ru/articles/420867/
 */

#include <cmath>
#include <functional>
#include <numeric>
#include <vector>
#include <thread>
#include <iostream>
#include <algorithm>
#include <iomanip>


class Integral {
private:
    int a, b, n, tn;

public:
    Integral(int& argc, char** argv) {
        if (argc != 5) {
            throw std::invalid_argument("Wrong number of arguments");
        }
        a  = std::stoi(argv[1]);
        b  = std::stoi(argv[2]);
        n  = std::stoi(argv[3]);
        tn = std::stoi(argv[4]);
    }

    static double integralFunction(double x) {
        return sqrt(1 + exp(x));
    }

    // Функция, считающая интеграл методом трапеций 
    // на отрезка [a, b] и n разбиениями
    double trapezoidalIntegral(double local_a, double local_b, int local_n) {
        double h = (local_b - local_a) / local_n;
        double sum = (integralFunction(local_a) + integralFunction(local_b)) / 2.0;

        for (int i = 1; i < local_n; ++i) {
            double x_i = local_a + i * h;
            sum += integralFunction(x_i);
        }
        return sum * h;
    }

    double calculateIntegral() {
        // в зависимости от количество потоков (tn) реализуйте подсчёт интеграла
        double total_integral = 0.0;
        std::vector<double> local_integrals(tn);
        std::vector<std::thread> threads;

        for (int i = 0; i < tn; ++i) {
            double local_a = a + i * (b - a) / tn;
            double local_b = a + (i + 1) * (b - a) / tn;
            int local_n = n / tn;

            threads.emplace_back([=, &local_integrals] {
                local_integrals[i] = trapezoidalIntegral(local_a, local_b, local_n);
            });
        }

        for (auto& thread : threads) thread.join(); // Ждем завершения выполнения потоков
        for (auto local_integral : local_integrals) total_integral += local_integral;
        return total_integral;
    }
};


int main(int argc, char** argv)
{
    auto i = Integral(argc, argv);
    std::cout << std::fixed << std::setprecision (4);
    std::cout << i.calculateIntegral() << std::endl;
    return 0;
}