#include <vector>
#include <string>

namespace FastWin{
void WindowFunctionRank(
                        std::string funAttribute,
                        std::string rangeOrRow,
                        std::string preceding,
                        std::string following,
                        std::vector<std::vector<int>>& dataset,
                        std::vector<std::vector<int>>& rank_levels

);
}
