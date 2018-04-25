/* 
   This contains a program that uses the Monte Carlo method in order to 
   compute the volume of a set of spheres and the overlapping volume between 
   them.
   
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
# include <cmath>
# include <cassert>

# include <iostream>
# include <fstream>
# include <sstream>
# include <random>
# include <chrono>
# include <tuple>
# include <stdexcept>
# include <limits>
# include <list>

using namespace std;

struct Point
{
  double x;
  double y;
  double z;

  string to_string() const
  {
    stringstream sstr;
    sstr << "(" << x << ", " << y << ", " << z << ")";
    return sstr.str();
  }
};

struct Sphere
{
  Point  c;
  double r;

  string to_string() const
  {
    stringstream sstr;
    sstr << "Center: " << c.to_string() << "; " << "Radius: " << r;
    return sstr.str();
  }
};


/* Finds the minimum and maximum x, y, z values in the whole set of points. 
   These values will be two points in order to define a parallelepiped.
*/
tuple<Point, Point> find_min_and_max(const list<Sphere> & spheres)
{
  const double inf = numeric_limits<double>::max();
  
  Point min { inf,  inf,  inf};
  Point max {-inf, -inf, -inf};
  
  for (const Sphere & s : spheres)
    {
      if (s.c.x - s.r < min.x)
        min.x = s.c.x - s.r;

      if (s.c.y - s.r < min.y)
        min.y = s.c.y - s.r;

      if (s.c.z - s.r < min.z)
        min.z = s.c.z - s.r;

      if (s.c.x + s.r > max.x)
        max.x = s.c.x + s.r;

      if (s.c.y + s.r > max.y)
        max.y = s.c.y + s.r;

      if (s.c.z + s.r > max.z)
        max.z = s.c.z + s.r;
    }

  return make_tuple(min, max);
}

/* Checks whether a point p is inside of some of the spheres in the set.

   This function returns a true value only if p is inside of at least 
   the amount of spheres defined by limit.
*/
bool is_inside_region(const Point & p, const list<Sphere> & spheres,
		      unsigned limit)
{
  size_t counter = 0;

  for (const Sphere & s : spheres)
    if ((p.x - s.c.x) * (p.x - s.c.x) +
        (p.y - s.c.y) * (p.y - s.c.y) +
        (p.z - s.c.z) * (p.z - s.c.z) <= s.r * s.r)
      ++counter;

  return counter >= limit;
}

list<Sphere> read_spheres_file(const string & file_name)
{
  ifstream file(file_name);

  if (not file)
    throw logic_error("Cannot open file");

  list<Sphere> spheres;

  while (not file.eof())
    {
      Sphere sphere;

      file >> sphere.c.x >> sphere.c.y >> sphere.c.z >> sphere.r;

      if (not file.eof())
        spheres.push_back(sphere);
    }

  file.close();

  return spheres;
}

double compute_spheres_theoretical_volume(const list<Sphere> & spheres)
{
  double volume = 0.0;

  for (const Sphere & s : spheres)
    volume += 4 * M_PI * s.r * s.r * s.r / 3;

  return volume;
}

int main(int argc, char * argv[])
{
  if (argc < 2)
    {
      cout << "Usage: " << argv[0] << " file-name [num-points]\n";
      return 0;
    }

  list<Sphere> spheres;

  try
  {
    spheres = read_spheres_file(argv[1]);
  }
  catch(...)
  {
    cout << "Cannot open file: " << argv[1];
    return 1;
  }

  auto begin = chrono::system_clock::now();

  size_t num = argc < 3 ? 1E+06 : atol(argv[2]);

  auto t = find_min_and_max(spheres);

  Point & minp = get<0>(t);
  Point & maxp = get<1>(t);

  size_t hits = 0;
  size_t overlapping_hits = 0;

  unsigned seed = chrono::system_clock::now().time_since_epoch().count();

  mt19937 rng(seed);

  using unif_dist_t = uniform_real_distribution<double>;

  unif_dist_t unif_x(minp.x, maxp.x);
  unif_dist_t unif_y(minp.y, maxp.y);
  unif_dist_t unif_z(minp.z, maxp.z);

  for (size_t i = 0; i < num; ++i)
    {
      Point rp = {unif_x(rng), unif_y(rng), unif_z(rng)};

      if (is_inside_region(rp, spheres, 1))
        ++hits;

      if (is_inside_region(rp, spheres, 2))
        ++overlapping_hits;
    }

  double parallelepiped_side_a = maxp.x - minp.x;
  double parallelepiped_side_b = maxp.y - minp.y;
  double parallelepiped_side_c = maxp.z - minp.z;

  double parallelepiped_volume = parallelepiped_side_a *
                                 parallelepiped_side_b *
                                 parallelepiped_side_c;

  double res = parallelepiped_volume * double(hits) / double(num);

  double overlapping_res = parallelepiped_volume *
                           double(overlapping_hits) / double(num);

  auto end = chrono::system_clock::now();

  auto diff = end - begin;

  auto s = chrono::duration_cast<chrono::seconds>(diff).count();

  auto ms = chrono::duration_cast<chrono::milliseconds>(diff).count();

  ms -= s * 1000;

  cout << "Monte Carlo for computing volume of a set of spheres\n";
  cout << "====================================================\n";
  cout << "Set of spheres\n";
  for (const Sphere & sphere : spheres)
    cout << "- " << sphere.to_string() << "\n";
  cout << "\nBounding parallelepiped:\n"
            << "  Minimum point: " << minp.to_string() << "\n"
            << "  Maximum point: " << maxp.to_string() << "\n"
            << "  Dimensions: " << parallelepiped_side_a << " x "
                                << parallelepiped_side_b << " x "
                                << parallelepiped_side_c << "\n\n";
  cout << "Total volume of all spheres (Theoretical): "
            << compute_spheres_theoretical_volume(spheres) << "\n\n";
  cout << "Number of generated random points: " << num << "\n\n";
  cout << "Volume of all spheres = " << res << "\n";
  cout << "Overlapping volume = " << overlapping_res << "\n\n";
  cout << "Execution time: " << s << " secs " << ms << " msecs\n";
  return 0;
}

