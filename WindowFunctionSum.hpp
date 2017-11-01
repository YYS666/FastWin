#include <vector>
#include <string>
#include <unordered_map>
namespace FastWin{
extern std::vector<std::vector<int>> sum_levels;
typedef std::unordered_map<int, std::vector<int>> bound_rec;

void WindowFunctionSum(
			std::string funAttribute,
			std::string rangeOrRow,
			std::string preceding,
			std::string following,
			std::vector<std::vector<int>>& dataset,
			std::vector<std::vector<int>>& sum_levels,
                        bound_rec& sum_ref

);

void buildLevels(int fanout, 
                 int funAttribute, 
                 std::vector<std::vector<int>>& dataset,
                 std::vector<std::vector<int>>& sum_levels

);

int traverseTree(int begin, 
                 int end, 
                 int fanout,
                 std::vector<std::vector<int>>& sum_levels
);
}
