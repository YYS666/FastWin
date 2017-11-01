#include <vector>
#include <string>
namespace FastWin{
void WindowFunctionMax(
                        std::string funAttribute,
                        std::string rangeOrRow,
                        std::string preceding,
                        std::string following,
                        std::vector<std::vector<int>>& dataset,
                        std::vector<std::vector<int>>& max_levels

);
}
