#!/bin/bash

#
# Usage:
# ./ShapeGestureGenerator <ellipse|rectangle|triangle> <reduce_iterations> <abs_dispersion> <closed_contour> <delay>
#
# arg[1] <ellipse|rectangle|triangle> - Тип фигуры.
# arg[2] <reduce_iterations> - Количество итераций разрежения контура. На каждой итерации разрежается примерно на половину. Т.е. при 5 итерациях останется 2^5 (~32%) от изначального количества.
# arg[3] <abs_dispersion> - Абсолютная дисперсия точек по осям. Например, при дисперсии равной 10, точка будет подвергнута шуму по X и Y в направлениях от -10 до 10 пикселей.
# arg[4] <closed_contour> - Флаг замыкания контура. 0 или 1.
# arg[5] <delay> - Задержка в мс. Если задано 0 мс, то задержка бесконечная, до тех пор, пока не будет нажата какая-либо кнопка.
#
# [q] - Выход из приложения.
#

./build/Release/ShapeGestureGenerator ellipse 5 10 1 40
