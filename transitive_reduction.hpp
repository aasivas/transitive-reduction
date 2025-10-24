#ifndef TRANSITIVE_REDUCTION_HPP
#define TRANSITIVE_REDUCTION_HPP

/* Zero-Clause BSD
 * =============
 *
 * Permission to use, copy, modify, and/or distribute this software for
 * any purpose with or without fee is hereby granted.
 *
 * THE SOFTWARE IS PROVIDED “AS IS” AND THE AUTHOR DISCLAIMS ALL
 * WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE
 * FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY
 * DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN
 * AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT
 * OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */


#include <iostream>
#include <vector>
#include <algorithm>
#include <stack>
#include <utility>
#include <cmath>

void dfs_csc4(int current_node, int n, const std::vector<int>& col_pointers, const std::vector<int>& col_ends, const std::vector<int>& row_indices, std::vector<bool>& visited, int max_depth) {
    std::vector<std::pair<int, int>> s;
    s.reserve(n);

    s.push_back({current_node, 0});
    visited[current_node] = true;

    while (!s.empty()) {
        auto [u, depth] = s.back();
        s.pop_back();

        if (depth >= max_depth) {
            continue;
        }

        for (int i = col_pointers[u]; i < col_ends[u]; ++i) {
            int neighbor = row_indices[i];
            if (neighbor != -1 && !visited[neighbor]) {
                visited[neighbor] = true;
                s.push_back({neighbor, depth + 1});
            }
        }
    }
}

// Function to find the approximate transitive reduction of a directed acyclic graph
// Approximate transitive reduction has all properties of the transitive reduction, except that it is not minimal.
// If the dfs max depth is set to infinity, then this function computes the transitive reduction
// This function (assuming that the max depth is not modified between calls) is idempotent, e.g.
//   transitive_reduction(G) = transitive_reduction(transitive_reduction(G))
std::tuple<std::vector<int>, std::vector<int>, std::vector<int>> transitive_reduction(int n, const std::vector<int>& col_pointers, const std::vector<int>& row_indices) {
    std::vector<int> reduced_col_pointers(n);
    std::vector<int> reduced_col_ends(n);
    std::vector<int> reduced_row_indices = row_indices;

    std::copy_n(col_pointers.begin(), n, reduced_col_pointers.begin());
    std::copy_n(col_pointers.begin() + 1, n, reduced_col_ends.begin());

    for (int i = n - 1; i > -1 ; --i) {
        for (int k_idx = reduced_col_pointers[i]; k_idx < reduced_col_ends[i]; ++k_idx) {
            int k = reduced_row_indices[k_idx];
            if (k == -1) continue;

            bool is_redundant = false;
            std::vector<bool> visited(n, false);
            for (int j_idx = reduced_col_pointers[i]; j_idx < reduced_col_ends[i]; ++j_idx) {
                if (k_idx == j_idx) continue;
                int j = reduced_row_indices[j_idx];
                if (j == -1) continue;

		dfs_csc4(j, n, reduced_col_pointers, reduced_col_ends, reduced_row_indices, visited, std::max(5, static_cast<int>(sqrt(n)/10)));

                if (visited[k]) {
                    is_redundant = true;
                    break;
                }
            }

            if (is_redundant) {
                reduced_row_indices[k_idx] = -1;
            }
        }

	// Compress the column
	int beg_col = reduced_col_pointers[i];
	for (int j = reduced_col_pointers[i]; j < reduced_col_ends[i]; ++j) {
	    if (reduced_row_indices[j] != -1) {
	        reduced_row_indices[beg_col] = reduced_row_indices[j];
		++beg_col;
	    }
	}
	reduced_col_ends[i] = beg_col;
    }
    
    return {reduced_col_pointers, reduced_col_ends, reduced_row_indices};
}
#endif // TRANSITIVE_REDUCTION_HPP
