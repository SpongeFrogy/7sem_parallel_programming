#include <iostream>
#include <tbb/flow_graph.h>

void solution(const int &v1, const int &v2) {
  tbb::flow::graph g;
  // multiply by 2
  tbb::flow::function_node<int, int> my_node(g, tbb::flow::unlimited,
                                             [](const int &in) -> int {
                                               // std::cout << "first node received:" << in << std::endl;
                                               return 2 * in;
                                             });

  // multiply by 3
  tbb::flow::function_node<int, int> my_other_node(g, tbb::flow::unlimited,
                                                   [](const int &in) -> int {
                                                     // std::cout << "second node received:" << in << std::endl;
                                                     return 3 * in;
                                                   });

  typedef tbb::flow::join_node<std::tuple<int, int>, tbb::flow::queueing>
      join_type;

  join_type my_join_node(g);

  // print received values
  tbb::flow::function_node<std::tuple<int, int>, std::string> my_final_node(
      g, tbb::flow::unlimited,
      [](const std::tuple<int, int> &t_in) -> std::string {
        std::cout << "final node received " << std::get<0>(t_in) << " and "
                  << std::get<1>(t_in) << std::endl;
        return std::to_string(std::get<0>(t_in)).append(std::to_string(std::get<1>(t_in)));
      });

  tbb::flow::make_edge(my_node, std::get<0>(my_join_node.input_ports()));
  tbb::flow::make_edge(my_other_node, std::get<1>(my_join_node.input_ports()));
  tbb::flow::make_edge(my_join_node, my_final_node);

  my_node.try_put(v1);
  my_other_node.try_put(v2);
  g.wait_for_all();
}

int main() {
  solution(1, 2);
  return 0;
}
/*
final node received 2 and 6
*/