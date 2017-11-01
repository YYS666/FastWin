

target:
	g++ -std=c++1y -o FastWin FastWin.cpp partition.cpp order.cpp WindowFunctionHandle.cpp WindowFunctionSum.cpp WindowFunctionAvg.cpp WindowFunctionMax.cpp WindowFunctionMin.cpp WindowFunctionRank.cpp -fopenmp -D_GLIBCXX_PARALLEL -pthread

test:
	
