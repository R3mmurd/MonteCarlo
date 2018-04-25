/* 
   This file contains a program with the classical example of Monte Carlo
   method for computing the value of PI.
   
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

# include <cstdlib>

# include <iostream>
# include <chrono>
# include <random>
# include <functional>

using namespace std;

int main(int argc, char * argv[])
{
  size_t n = argc < 2 ? 1E+7 : atol(argv[1]);

  unsigned seed = chrono::system_clock::now().time_since_epoch().count();

  mt19937_64 rng(seed);

  using unif_dist_t = uniform_real_distribution<double>;

  unif_dist_t unif_dist(0.0, 1.0);

  auto unif = bind(unif_dist, rng);

  size_t hits = 0;

  auto begin = chrono::system_clock::now();

  for (size_t i = 0; i < n; ++i)
    {
      double x = unif();
      double y = unif();

      if (x * x + y * y <= 1)
        ++hits;
    }

  auto end = chrono::system_clock::now();

  auto diff = end - begin;

  auto s = chrono::duration_cast<chrono::seconds>(diff).count();

  auto ms = chrono::duration_cast<chrono::milliseconds>(diff).count();

  ms -= s * 1000;

  double pi = 4.0L * double(hits) / double(n);

  cout << "Monte Carlo for computing the value of PI\n";
  cout << "=====================================\n";
  cout << "Number of generated random points: " << n << "\n";
  cout << "PI = " << pi << "\n\n";
  cout << "Execution time: " << s << " secs " << ms << " msecs\n";

  return 0;
}


