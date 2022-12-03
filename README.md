# OOOSTC_Problem2
All Problems u can find in "задачи.docs"

2.6	Реализовать простой синтезатор частот.
Использовать алгоритм Карплуса-Стронга
(https://en.wikipedia.org/wiki/Karplus%E2%80%93Strong_string_synthesis).
Входом является набор частот в звуковом диапазоне, частота дискретизации, длительность в секундах.
Выходом является сигнал, который допустимо записать в файл либо вывести на звуковую карту.
При записи в файл привести отсчеты к формату int16.
Любые модификации, дополнительные алгоритмы, параметры (коэффициент затухания и т.д.) и интерфейс остаются на усмотрение соискателя и приветствуются.

Решение:

Hardware: i5-10300H

OS: Ubuntu 18.04

Compiler: CMake 3.8, Python 3.10.8 (Графики для удобства строил в Python)

Файлы: "StringSynthesis.cpp", "StringSynthesisFun.h", "main.py"
Для создания файла в формате .wav используется python-скрипт

![image](https://user-images.githubusercontent.com/22713938/205453841-f3aad17b-019d-4aca-8500-a6ae498ae65d.png)

Pick-Direction Lowpass Filter
![image](https://user-images.githubusercontent.com/22713938/205453924-d8f0458b-7c2a-4add-866a-12e14534426f.png)

Pick-Position Comb Filter
![image](https://user-images.githubusercontent.com/22713938/205453939-a07bc7a5-fbf8-4382-a9af-9ef6b532be61.png)

One-Zero String Damping Filter
![image](https://user-images.githubusercontent.com/22713938/205453948-bcae49dc-f7bb-4e70-bab9-6fedbaef8849.png)

Allpass filter used for tuning
![image](https://user-images.githubusercontent.com/22713938/205453980-5dabf697-3b1b-4134-8145-9f505e7ed6f8.png)

Allpass filter used to simulate string stiffness
![image](https://user-images.githubusercontent.com/22713938/205454058-1a1d137e-a642-45df-9686-82b663528ab6.png)

Для упрощения использовалось свойтсво последовательных фильтров:
![image](https://user-images.githubusercontent.com/22713938/205454149-df7287f9-bece-4448-89d9-01eafe0a7e40.png)
Тем самым была создана функция MergedFilter:
![image](https://user-images.githubusercontent.com/22713938/205454376-6a4edd64-24f8-4228-997e-21d85a21d323.png)



