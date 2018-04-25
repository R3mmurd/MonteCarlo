/* 
   This file contains a generic Monte Carlo algorithm to compute the definite
   integral of a function.
   
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

# ifndef MC_INTEGRATION_H
# define MC_INTEGRATION_H

# include <random>
# include <functional>

template <class Function>
class MC_Integration
{
  Function & f;

public:
  MC_Integration(Function & _f)
    : f(_f)
  {
    // Empty
  }

  MC_Integration(Function && _f = Function())
    : f(_f)
  {
    // Empty
  }

  double operator () (int a, int b, size_t num_it, unsigned seed = 1)
  {
    std::mt19937_64 rng(seed);

    std::uniform_real_distribution<double> unif_dist(a, b);

    auto unif = std::bind(unif_dist, rng);

    double sum = 0.0;

    for (size_t i = 0; i < num_it; ++i)
      {
        double x = unif();
        sum += f(x);
      }

    double ret_val = sum * (b - a) / num_it;

    return ret_val;
  }
};

# endif // MC_INTEGRATION_H

