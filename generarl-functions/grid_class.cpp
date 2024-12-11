#include "grid_class.hpp"

int Grid::toFlatIndex(const std::vector<int>& indices) const {
    if (indices.size() != dimensions.size()) {
        throw std::invalid_argument("Invalid number of indices.");
    }
    int flatIndex = 0;
    int multiplier = 1;
    for (int i = dimensions.size() - 1; i >= 0; --i) {
        if (indices[i] < 0 || indices[i] >= dimensions[i]) {
            throw std::out_of_range("Index out of bounds.");
        }
        flatIndex += indices[i] * multiplier;
        multiplier *= dimensions[i];
    }
    return flatIndex;
}

Grid::Grid(const std::vector<int>& dims) : dimensions(dims) {
    if (dims.empty()) {
        throw std::invalid_argument("Dimensions cannot be empty.");
    }
    int totalSize = 1;
    for (int dim : dims) {
        if (dim <= 0) {
            throw std::invalid_argument("All dimensions must be greater than zero.");
        }
        totalSize *= dim;
    }
    grid.resize(totalSize);
    for (int i = 0; i < totalSize; ++i) {
        grid[i] = std::make_shared<SEO>();
    }
}

std::shared_ptr<SEO> Grid::getSEO(const std::vector<int>& indices) const {
    return grid[toFlatIndex(indices)];
}

void Grid::setSEO(const std::vector<int>& indices, const std::shared_ptr<SEO>& seo) {
    grid[toFlatIndex(indices)] = seo;
}

void Grid::configureConnections() {
    int totalSize = grid.size();
    for (int i = 0; i < totalSize; ++i) {
        std::vector<int> indices(dimensions.size());
        int temp = i;
        for (int j = dimensions.size() - 1; j >= 0; --j) {
            indices[j] = temp % dimensions[j];
            temp /= dimensions[j];
        }

        std::vector<std::shared_ptr<SEO>> connections;
        for (int d = 0; d < dimensions.size(); ++d) {
            for (int offset : {-1, 1}) {
                std::vector<int> neighbor = indices;
                neighbor[d] += offset;
                if (neighbor[d] >= 0 && neighbor[d] < dimensions[d]) {
                    connections.push_back(getSEO(neighbor));
                }
            }
        }
        getSEO(indices)->setConnections(connections);
    }
}

void Grid::updateGrid(double dt) {
    for (auto& seo : grid) {
        seo->setNodeCharge(dt);
    }
}

std::vector<int> Grid::getDimensions() const {
    return dimensions;
}