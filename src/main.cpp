#include <chrono>
#include <functional>
#include <iostream>
#include <thread>

#include "draw.h"
#include "maze.h"
#include "mazeGen.h"
#include "mazeSolve.h"

using namespace std;

/**
 * @brief Timer class to easily time code execution
 * Highly inspired (if not entirely copied) from
 * https://www.learncpp.com/cpp-tutorial/timing-your-code/
 *
 */
struct Timer {
private:
  // Type aliases to make accessing nested type easier
  using clock_t = chrono::high_resolution_clock;
  using second_t = chrono::duration<double, ratio<1>>;

  chrono::time_point<clock_t> m_beg;

public:
  /**
   * @brief Construct a new Timer object with the time of calling
   *
   */
  Timer() : m_beg{clock_t::now()} {}
  /**
   * @brief Resets the timer
   *
   */
  void reset() { m_beg = clock_t::now(); }

  /**
   * @brief Returns the elapsed time in proper units
   *
   * @return double
   */
  double elapsed() const {
    return chrono::duration_cast<second_t>(clock_t::now() - m_beg).count();
  }
};

int main() {

  int start = 9999;
  int finish = 250;
  // start = 5;
  // finish = 99;

  Maze m(100, 100); // width and height
  m.initGrid(0, finish);

  srand(time(NULL));
  thread drawMaze(draw, ref(m));

  Timer t;
  DFSGenNoRecursion(m, 0);
  cout << "maze generated in " << t.elapsed() << endl << endl;

  m.addRandomLoops(1000); // dim/10
                          // m.print();
  m.setWeight();

  t.reset();
  DFSsolve(m, start);
  cout << "maze solved in " << t.elapsed() << endl << endl;
  this_thread::sleep_for(chrono::milliseconds(1000));

  t.reset();
  BFSsolve(m, start);
  cout << "maze solved in " << t.elapsed() << endl << endl;
  this_thread::sleep_for(chrono::milliseconds(1000));

  t.reset();
  AStarSolve(m, start, finish);
  cout << "maze solved in " << t.elapsed() << endl << endl;
  this_thread::sleep_for(chrono::milliseconds(1000));

  t.reset();
  DijkstraSolve(m, start);
  cout << "maze solved in " << t.elapsed() << endl << endl;

  cout << "Finished solving ";

  drawMaze.join();
}
