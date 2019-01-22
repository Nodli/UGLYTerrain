#include <Vegetation/VegetationLayerMap.hpp>


int VegetationLayerMap::count_ID_at(const int i, const int j, const int ID) const
{
    auto cell = _cells.at(index(i, j));
    int res = 0;
    for(int i = 0; i < cell.size(); ++i)
    {
        res += cell[i]->_ID == ID;
    }
    return res;
}