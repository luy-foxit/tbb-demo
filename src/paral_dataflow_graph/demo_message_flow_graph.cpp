#include <iostream>
#include <stdlib.h>
#include "common/tbb_time.h"
#include "common/chrono_time.h"
#include "tbb/flow_graph.h"

using namespace tbb::flow;

struct square {
  int operator()(int v) { return v*v; }
};

struct cube {
  int operator()(int v) { return v*v*v; }
};

class sum {
  int &my_sum;
public:
  sum( int &s ) : my_sum(s) {}
  int operator()( tuple< int, int > v ) {
    my_sum += get<0>(v) + get<1>(v);
    return my_sum;
  }
};

int main() {
  int result = 0;

  graph g;
  broadcast_node<int> input(g);
  function_node<int,int> squarer( g, unlimited, square() );
  function_node<int,int> cuber( g, unlimited, cube() );
  join_node< tuple<int,int>, queueing > join( g );
  function_node<tuple<int,int>,int>
      summer( g, serial, sum(result) );

  make_edge( input, squarer );
  make_edge( input, cuber );
  make_edge( squarer, get<0>( join.input_ports() ) );
  make_edge( cuber, get<1>( join.input_ports() ) );
  make_edge( join, summer );

  for (int i = 1; i <= 10; ++i)
      input.try_put(i);
  g.wait_for_all();

  printf("Final result is %d\n", result);
  return 0;
}
