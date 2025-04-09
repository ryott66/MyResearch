#ifndef GRID_2DIM_HPP
#define GRID_2DIM_HPP

#include <vector>
#include <memory>
#include <stdexcept>
#include <algorithm>
#include <string>

using namespace std;

template <typename Element>
class Grid2D
{
public:
    Grid2D(int rows, int cols);

    shared_ptr<Element> getElement(int row, int col) const;
    void setElement(int row, int col, const shared_ptr<Element> &element);

    void updateGridVn();
    void updateGriddE();
    bool gridminwt(const double dt);
    void updateGridQn(const double dt);

    vector<vector<shared_ptr<Element>>> &getGrid();
    int numRows() const;
    int numCols() const;

    shared_ptr<Element> getTunnelPlace() const;
    string getTunnelDirection() const;
    double getMinWT() const;

private:
    vector<vector<shared_ptr<Element>>> grid;
    int rows_, cols_;
    shared_ptr<Element> tunnelplace;
    string tunneldirection;
    double minwt;
};

template <typename Element>
Grid2D<Element>::Grid2D(int rows, int cols)
    : rows_(rows), cols_(cols), grid(rows, vector<shared_ptr<Element>>(cols))
{
    if (rows <= 0 || cols <= 0)
    {
        throw invalid_argument("Grid size must be positive");
    }
    for (int i = 0; i < rows; ++i)
    {
        for (int j = 0; j < cols; ++j)
        {
            grid[i][j] = make_shared<Element>();
        }
    }
}

template <typename Element>
shared_ptr<Element> Grid2D<Element>::getElement(int row, int col) const
{
    return grid.at(row).at(col);
}

template <typename Element>
void Grid2D<Element>::setElement(int row, int col, const shared_ptr<Element> &element)
{
    grid.at(row).at(col) = element;
}

template <typename Element>
void Grid2D<Element>::updateGridVn()
{
    for (auto &row : grid)
    {
        for (auto &elem : row)
        {
            elem->setSurroundingVoltages();
        }
    }

    for (auto &row : grid)
    {
        for (auto &elem : row)
        {
            elem->setPcalc();
        }
    }
}

template <typename Element>
void Grid2D<Element>::updateGriddE()
{
    for (auto &row : grid)
    {
        for (auto &elem : row)
        {
            elem->setdEcalc();
        }
    }
}

template <typename Element>
bool Grid2D<Element>::gridminwt(const double dt)
{
    minwt = dt;
    for (auto &row : grid)
    {
        for (auto &elem : row)
        {
            if (elem->calculateTunnelWt())
            {
                double tmpwt = max(elem->getWT()["up"], elem->getWT()["down"]);
                tunneldirection = (tmpwt == elem->getWT()["up"]) ? "up" : "down";
                tunnelplace = elem;
                minwt = min(minwt, tmpwt);
            }
        }
    }
    return minwt < dt;
}

template <typename Element>
void Grid2D<Element>::updateGridQn(const double dt)
{
    for (auto &row : grid)
    {
        for (auto &elem : row)
        {
            elem->setNodeCharge(dt);
        }
    }
}

template <typename Element>
vector<vector<shared_ptr<Element>>> &Grid2D<Element>::getGrid()
{
    return grid;
}

template <typename Element>
int Grid2D<Element>::numRows() const
{
    return rows_;
}

template <typename Element>
int Grid2D<Element>::numCols() const
{
    return cols_;
}

template <typename Element>
shared_ptr<Element> Grid2D<Element>::getTunnelPlace() const
{
    return tunnelplace;
}

template <typename Element>
string Grid2D<Element>::getTunnelDirection() const
{
    return tunneldirection;
}

template <typename Element>
double Grid2D<Element>::getMinWT() const
{
    return minwt;
}

#endif // GRID_2DIM_HPP
