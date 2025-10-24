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

void print_graph(int n, const std::vector<int>& col_pointers, const std::vector<int>& col_ends, const std::vector<int>& row_indices) {
    std::cout << "Graph in CSC4 format:" << std::endl;
    std::cout << "col_pointers: ";
    for (int val : col_pointers) {
        std::cout << val << " ";
    }
    std::cout << std::endl;

    std::cout << "col_ends: ";
    for (int val : col_ends) {
        std::cout << val << " ";
    }
    std::cout << std::endl;

    std::cout << "row_indices: ";
    for (int val : row_indices) {
        std::cout << val << " ";
    }
    std::cout << std::endl;
}

// Helper function to convert CSC4 to a more readable format for comparison
std::vector<std::pair<int, int>> csc4_to_edges(int n, const std::vector<int>& col_pointers, const std::vector<int>& col_ends, const std::vector<int>& row_indices) {
    std::vector<std::pair<int, int>> edges;
    for (int i = 0; i < n; ++i) {
        for (int j = col_pointers[i]; j < col_ends[i]; ++j) {
            edges.push_back({i, row_indices[j]});
        }
    }
    std::sort(edges.begin(), edges.end());
    return edges;
}

int main() {
    // Test case: a simple graph with a transitive edge
    // 0 -> 1, 1 -> 2, 0 -> 2
    int n = 3;
    std::vector<int> col_pointers = {0, 2, 3, 3};
    std::vector<int> row_indices = {1, 2, 2};

    auto reduced_graph_csc = transitive_reduction(n, col_pointers, row_indices);
    
    std::vector<int> new_col_pointers = std::get<0>(reduced_graph_csc);
    std::vector<int> new_col_ends = std::get<1>(reduced_graph_csc);
    std::vector<int> new_row_indices = std::get<2>(reduced_graph_csc);

    print_graph(n, new_col_pointers, new_col_ends, new_row_indices);

    // Expected reduction: 0 -> 1, 1 -> 2
    std::vector<std::pair<int, int>> expected_edges = {{0, 1}, {1, 2}};
    std::vector<std::pair<int, int>> actual_edges = csc4_to_edges(n, new_col_pointers, new_col_ends, new_row_indices);

    assert(actual_edges == expected_edges);

    std::cout << "Test passed!" << std::endl;

    // Test case 2: a graph with a transitive edge
    // 0 -> 1, 0 -> 2, 0 -> 3, 1 -> 2
    n = 4;
    col_pointers = {0, 3, 4, 4, 4};
    row_indices = {1, 2, 3, 2};
    reduced_graph_csc = transitive_reduction(n, col_pointers, row_indices);
    new_col_pointers = std::get<0>(reduced_graph_csc);
    new_col_ends = std::get<1>(reduced_graph_csc);
    new_row_indices = std::get<2>(reduced_graph_csc);

    print_graph(n, new_col_pointers, new_col_ends, new_row_indices);

    // Expected reduction: 0 -> 1, 0 -> 3, 1 -> 2
    expected_edges = {{0, 1}, {0, 3}, {1, 2}};
    actual_edges = csc4_to_edges(n, new_col_pointers, new_col_ends, new_row_indices);
    
    assert(actual_edges == expected_edges);

    std::cout << "Test 2 passed!" << std::endl;

    // Test case 3: a diamond-shaped graph
    // 0 -> 1, 0 -> 2, 1 -> 3, 2 -> 3
    n = 4;
    col_pointers = {0, 2, 3, 4, 4};
    row_indices = {1, 2, 3, 3};

    reduced_graph_csc = transitive_reduction(n, col_pointers, row_indices);
    new_col_pointers = std::get<0>(reduced_graph_csc);
    new_col_ends = std::get<1>(reduced_graph_csc);
    new_row_indices = std::get<2>(reduced_graph_csc);

    print_graph(n, new_col_pointers, new_col_ends, new_row_indices);

    // Expected reduction: 0 -> 1, 0 -> 2, 1 -> 3, 2 -> 3
    expected_edges = {{0, 1}, {0, 2}, {1, 3}, {2, 3}};
    actual_edges = csc4_to_edges(n, new_col_pointers, new_col_ends, new_row_indices);

    assert(actual_edges == expected_edges);

    std::cout << "Test 3 passed!" << std::endl;

    // Test case 4: Wikipedia example
    // 0 -> 1, 0 -> 2, 0 -> 3, 0 -> 4, 1 -> 3, 2 -> 3, 2 -> 4, 3 -> 4
    n = 5;
    col_pointers = {0, 4, 5, 7, 8, 8};
    row_indices = {1, 2, 3, 4, 3, 3, 4, 4};

    reduced_graph_csc = transitive_reduction(n, col_pointers, row_indices);
    new_col_pointers = std::get<0>(reduced_graph_csc);
    new_col_ends = std::get<1>(reduced_graph_csc);
    new_row_indices = std::get<2>(reduced_graph_csc);

    print_graph(n, new_col_pointers, new_col_ends, new_row_indices);
    
    // Expected reduction: 0 -> 1, 0 -> 2, 1 -> 3, 2 -> 3, 3 -> 4
    expected_edges = {{0, 1}, {0, 2}, {1, 3}, {2, 3}, {3,4}};
    actual_edges = csc4_to_edges(n, new_col_pointers, new_col_ends, new_row_indices);

    assert(actual_edges == expected_edges);

    std::cout << "Test 4 passed!" << std::endl;

    // Test case 5: Dense example
    n = 5;
    col_pointers = {0, 4, 7, 9, 10, 10};
    row_indices = {1, 2, 3, 4, 2, 3, 4, 3, 4, 4};

    reduced_graph_csc = transitive_reduction(n, col_pointers, row_indices);
    new_col_pointers = std::get<0>(reduced_graph_csc);
    new_col_ends = std::get<1>(reduced_graph_csc);
    new_row_indices = std::get<2>(reduced_graph_csc);

    print_graph(n, new_col_pointers, new_col_ends, new_row_indices);
    
    // Expected reduction: 0 -> 1, 1 -> 2, 2 -> 3, 3 -> 4
    expected_edges = {{0, 1}, {1, 2}, {2, 3}, {3, 4}};
    actual_edges = csc4_to_edges(n, new_col_pointers, new_col_ends, new_row_indices);

    assert(actual_edges == expected_edges);

    std::cout << "Test 5 passed!" << std::endl;
    
    return 0;
}
