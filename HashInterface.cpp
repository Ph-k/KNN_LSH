#include "HashInterface.hpp"

HashInterface::HashInterface(int w, int k, int TableSize, int vecSize)
:w(w), k(k), TableSize(TableSize), vecSize(vecSize) {}

HashInterface::~HashInterface() {}