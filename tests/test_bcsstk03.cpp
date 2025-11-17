#include <iostream>
#include <vector>
#include <cassert>
#include <tuple>
#include <algorithm>
#include <utility>
#include <fstream>
#include <string>
#include <sstream>
#include <omp.h>

#include "../transitive_reduction.hpp"

// Function to read a matrix from a Matrix Market file
std::tuple<int, std::vector<int>, std::vector<int>> read_matrix_market(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error opening file: " << filename << std::endl;
        exit(1);
    }

    // Skip header lines
    std::string line;
    while (std::getline(file, line) && line[0] == '%') {}

    // Read matrix dimensions
    std::stringstream ss(line);
    int n, m, nnz;
    ss >> n >> m >> nnz;

    // Read matrix data
    std::vector<std::tuple<int, int, double>> coordinates;
    coordinates.reserve(nnz);
    while (std::getline(file, line)) {
        std::stringstream line_ss(line);
        int u, v;
        double w;
        line_ss >> u >> v >> w;
	if (u == v) continue;
	coordinates.emplace_back(u - 1, v - 1, w);
    }

    // Convert to CSC - We are assuming indices are sorted
    std::vector<int> col_pointers(n + 1, 0);
    std::vector<int> row_indices;
    row_indices.reserve(nnz);

    for (const auto& coord : coordinates) {
        row_indices.push_back(std::get<0>(coord));
        col_pointers[std::get<1>(coord) + 1]++;
    }

    for (int i = 0; i < n; ++i) {
        col_pointers[i + 1] += col_pointers[i];
    }

    return {n, col_pointers, row_indices};
}

void test_bcsstk03()
{
    // Read the matrix
    auto [n, col_pointers, row_indices] = read_matrix_market("tests/bcsstk03/bcsstk03.mtx");

    // Compute the transitive reduction
    auto reduced_graph_csc = transitive_reduction(n, col_pointers, row_indices);
    
    std::vector<int> new_col_pointers = std::get<0>(reduced_graph_csc);
    std::vector<int> new_col_ends = std::get<1>(reduced_graph_csc);
    std::vector<int> new_row_indices = std::get<2>(reduced_graph_csc);

    // Assert that the number of edges is less than or equal to the original graph
    int original_edges = row_indices.size();
    int reduced_edges = 0;
    for (int i = 0; i < n; ++i) {
        reduced_edges += new_col_ends[i] - new_col_pointers[i];
    }

    assert(reduced_edges <= original_edges);

    std::cout << "Test passed!" << std::endl;
    std::cout << "Original edges: " << original_edges << std::endl;
    std::cout << "Reduced edges: " << reduced_edges << std::endl;
}

void test_bcsstk01()
{
    // Read the matrix
    auto [n, col_pointers, row_indices] = read_matrix_market("tests/bcsstk01/bcsstk01.mtx");

    for (int i = 0; i < n; ++i) {
	std::cout << "Node " << i << ": ";
    	for (int j = col_pointers[i]; j < col_pointers[i+1]; ++j){
	    std::cout << row_indices[j] << ", ";
	}
	std::cout << std::endl << std::flush;
    }

    // Compute the transitive reduction
    auto reduced_graph_csc = transitive_reduction(n, col_pointers, row_indices);
    
    std::vector<int> new_col_pointers = std::get<0>(reduced_graph_csc);
    std::vector<int> new_col_ends = std::get<1>(reduced_graph_csc);
    std::vector<int> new_row_indices = std::get<2>(reduced_graph_csc);

    std::cout << std::endl << std::endl << std::flush;
    for (int i = 0; i < n; ++i) {
	std::cout << "Node " << i << ": ";
    	for (int j = new_col_pointers[i]; j < new_col_ends[i]; ++j){
	    std::cout << new_row_indices[j] << ", ";
	}
	std::cout << std::endl << std::flush;
    }

    // Assert that the number of edges is less than or equal to the original graph
    int original_edges = row_indices.size();
    int reduced_edges = 0;
    for (int i = 0; i < n; ++i) {
        reduced_edges += new_col_ends[i] - new_col_pointers[i];
    }

    assert(reduced_edges <= original_edges);

    std::cout << "Test passed!" << std::endl;
    std::cout << "Original edges: " << original_edges << std::endl;
    std::cout << "Reduced edges: " << reduced_edges << std::endl;
}
int main() {
    test_bcsstk01();
    test_bcsstk03();

    return 0;
}
