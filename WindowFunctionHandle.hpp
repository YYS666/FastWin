#include <vector>
#include <unordered_map>
#include <string>
#include "partition.hpp"
#include "order.hpp"
#include "WindowFunctionSum.hpp"
#include "WindowFunctionAvg.hpp"
#include "WindowFunctionMin.hpp"
#include "WindowFunctionMax.hpp"
#include "WindowFunctionRank.hpp"

namespace FastWin{
  void WindowFunctionHandle(
                          std::string fun_name,
                          std::string fun_attribute,
                          std::string rangeOrRow,
                          std::string preceding,
                          std::string folowing,
                          std::unordered_map<int, std::vector<std::vector<int>>>& combined_dataset
  );
}
