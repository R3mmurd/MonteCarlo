/* 
   This file contains a program to compute the integral between 0 and 2 for
   the function e^(-x^2) through Monte Carlo method.
   
   Copyright (C) 2014 by Alejandro J. Mujica
   
   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.
   
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.
   
   Any user request of this software, write to 
   
   Alejandro Mujica
   
   aledrums@gmail.com 
*/

# include <cmath>

# include <iostream>
# include <chrono>

using namespace std;

# include <MC_integration.H>

struct Function
{
  double operator () (double x)
  {
    return exp(-x * x);
  }

  static string to_string()
  {
    return "e^(-x^2)";
  }
};

int main (int argc, char * argv[])
{
  size_t n = argc < 2 ? 1E+6 : atol(argv[1]);

  unsigned seed = chrono::system_clock::now().time_since_epoch().count();

  size_t a = 0;
  size_t b = 2;

  auto begin = chrono::system_clock::now();

  double integration = MC_Integration<Function>()(a, b, n, seed);

  auto end = chrono::system_clock::now();

  auto diff = end - begin;

  auto s = chrono::duration_cast<chrono::seconds>(diff).count();

  auto ms = chrono::duration_cast<chrono::milliseconds>(diff).count();

  ms -= s * 1000;

  cout << "Monte Carlo Integration\n";
  cout << "=======================\n";
  cout << "Number of generated random points: " << n << "\n";
  cout << "Function: " << Function::to_string() << "\n";
  cout << "integrate(" << Function::to_string() << ", 0, 2) = "
            << integration << "\n\n";
  cout << "Execution time: " << s << " secs " << ms <<  " msecs\n";

  return 0;
}
