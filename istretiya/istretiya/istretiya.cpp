
#include <iostream>
#include <ctime> 
#include <string> 


// Параметры нейронной сети
const double a = 0.5, speed = 0.005;

const std::string str_months[12] = { "Январь", "Февраль", "Март", "Апрель", "Май", "Июнь", "Июль", "Август", "Сентябрь", "Октябрь", "Ноябрь", "Декабрь" };
const std::string str_zz[13] = { "Козерог", "Водолей", "Рыбы", "Овен", "Телец", "Близнецы", "Рак", "Лев", "Дева", "Весы", "Скорпион", "Стрелец", "Козерог" };

// Обучающая выборка
const int Learn_Examples = 41;
const int Learn_Date[Learn_Examples] = { 1,2,3,4,5,6,7,8,9,10,8,21,14,16,7,9,25,11,5,16,31,5,1,19,21,8,22,30,9,25,1,1,29,4,11,12,26,13,7,18,23 };
const int Learn_Month[Learn_Examples] = { 1,2,3,4,5,6,7,8,9,10,9,6,5,1,11,3,8,7,7,12,12,1,2,6,6,3,11,11,4,4,5,6,6,10,11,12,8,2,10,7,7 };
const int Learn_Ans[Learn_Examples] = { 1,2,3,4,5,6,7,8,9,10,9,7,5,2,11,3,9,7,7,12,13,1,2,6,7,3,12,12,4,5,5,6,7,10,11,12,9,2,10,7,8};

// Тестовая выборка
const int Test_Examples = 12;
const int Test_Date[Test_Examples]  = { 1, 12, 7, 27, 20, 12, 26, 13, 7, 2, 2, 30 };
const int Test_Month[Test_Examples] = { 1, 9, 12, 12, 2, 12, 8, 2, 10, 11, 8, 12 };
const int Test_Ans[Test_Examples]   = { 1, 9, 12, 13, 3, 12, 9, 2, 10, 11, 8, 13 };

double rnd()
{
	return ((-100 + rand() % 201) / 1000.0);
}

int main()
{
	setlocale(LC_ALL, "Russian");
	srand(time(0));

	// Нейронная сеть
	double*** nn;
	nn = new double** [2];

	// Указатели на нейроны скрытого слоя 
	nn[0] = new double* [1];

	// Указатели на нейроны выходного слоя 
	nn[1] = new double* [1];

	nn[0][0] = new double[3];	nn[1][0] = new double[2];

	// Активационные функции нейронов
	double** f;
	f = new double* [2];
	f[0] = new double[2];		f[1] = new double[1];

	// Данные входных нейронов
	double* ff;
	ff = new double[2];

	// Ошибки всех нейронов
	double** errnet;
	errnet = new double* [2];
	errnet[0] = new double[1]; 	errnet[1] = new double[1];


	// Первичная инициализация весов
	for (int i = 0; i < 3; i++)
		nn[0][0][i] = rnd();

	// Первичная инициализация смещений
	for (int i = 0; i < 2; i++)
		nn[1][0][i] = rnd();

	int Iteration = 0;
	double Err_Koef;

	//	{-{-{ Обучение нейронной сети }-}-}
	for (int Repeats = 0; Repeats < 200; Repeats++)
		for (int Example = 0; Example < Learn_Examples; Example++)
		{
			Err_Koef = 1;
			Iteration = 0;

			while (Iteration < 50000 && abs(Err_Koef) > 0.1)
			{
				f[0][0] = Learn_Month[Example];	f[0][1] = Learn_Date[Example];

				ff[0] = Learn_Month[Example];	ff[1] = Learn_Date[Example];


				// Функция активации нейронной сети
				f[0][0] = f[0][0] * nn[0][0][1] + a * f[0][1] * nn[0][0][2] + nn[0][0][0];
				f[1][0] = f[0][0] * nn[1][0][1] + nn[1][0][0];

				// Коэффициент ошибки
				Err_Koef = f[1][0] - Learn_Ans[Example];

				// Ошибки всех нейронов
				errnet[1][0] = Err_Koef;
				errnet[0][0] = nn[1][0][1] * errnet[1][0];

				// Корректировка весов 
				nn[0][0][1] = nn[0][0][1] - speed * errnet[0][0] * ff[0];
				nn[0][0][2] = nn[0][0][2] - a * speed * errnet[0][0] * ff[1];
				nn[1][0][1] = nn[1][0][1] - speed * errnet[1][0] * f[0][0];

				// Корректировка смещений
				nn[0][0][0] = nn[0][0][0] - speed * errnet[0][0];
				nn[1][0][0] = nn[1][0][0] - speed * errnet[1][0];

				Iteration++;
			}
		}

	int Correct = 0, prepos = 0;
	double temp = 0;

	//	{-{-{ Проверка работы нейронной сети }-}-}
	for (int Test_Num = 0; Test_Num < Test_Examples; Test_Num++) {

		// Вычистление ответа
		f[0][0] = Test_Month[Test_Num];
		f[0][1] = Test_Date[Test_Num];

		std::cout << "Дата рождения: " << f[0][1] << " " << str_months[Test_Month[Test_Num] - 1] << std::endl;

		f[0][0] = f[0][0] * nn[0][0][1] + a * f[0][1] * nn[0][0][2] + nn[0][0][0];
		f[1][0] = f[0][0] * nn[1][0][1] + nn[1][0][0]; // Ответ

		// Обработка полученного нейронной сетью ответа

		prepos = f[1][0];	temp = prepos;

		if (f[1][0] - temp >= 0.5) temp++;

		std::cout << "Ответ нейросети: " << str_zz[int(temp) - 1]
			<< ", правильный ответ: " << str_zz[Test_Ans[Test_Num] - 1]
			<< std::endl << std::endl;

		if (temp == Test_Ans[Test_Num]) Correct++;
	}

	std::cout << std::endl << "Точность: " << Correct << " из " << Test_Examples << std::endl;

	int month1, date1;

	std::cout << "Введите месяц рождения: " << std::endl;
	std::cin >> month1;
	std::cout << "Введите число рождения: " << std::endl;
	std::cin >> date1;

	// Вычистление ответа
	f[0][0] = month1;
	f[0][1] = date1;

	std::cout << "Дата рождения: " << f[0][1] << " " << str_months[month1 - 1] << std::endl;

	f[0][0] = f[0][0] * nn[0][0][1] + a * f[0][1] * nn[0][0][2] + nn[0][0][0];
	f[1][0] = f[0][0] * nn[1][0][1] + nn[1][0][0]; // Ответ

	// Обработка полученного нейронной сетью ответа

	prepos = f[1][0];	temp = prepos;

	if (f[1][0] - temp >= 0.5) temp++;

	std::cout << "Ответ: " << str_zz[int(temp) - 1] << std::endl;
}
