
#include <unordered_map>
#include <unordered_set>
#include <iostream>
#include "registration.h"
#include "util.h"

IMPLEMENT_DAY(12, Day12)

namespace {
    struct Graph {
        std::unordered_map<std::string, size_t> node_ids;
        std::vector<std::vector<size_t>> adjacent_nodes;
        std::unordered_set<size_t> big_caves;
    };
    
    Graph parse_input(const std::vector<std::string>& lines) {
        Graph result;
        const auto ensure_added = [&](const std::string& name) {
            const auto found = result.node_ids.find(name);
            if (found == result.node_ids.end()) {
                const size_t id = result.node_ids.size();
                result.node_ids[name] = id;
                result.adjacent_nodes.emplace_back();
                if (!name.empty() && name[0] >= 'A' && name[0] <= 'Z') {
                    result.big_caves.insert(id);
                }
                return id;
            } else {
                return found->second;
            }
        };
        
        for (const auto& line : lines) {
            const auto parts = util::split(line, "-");
            const size_t a = ensure_added(parts[0]);
            const size_t b = ensure_added(parts[1]);
            if (a == b) continue;
            result.adjacent_nodes[a].push_back(b);
            result.adjacent_nodes[b].push_back(a);
        }
        
        return result;
    }

    uint64_t count_paths_part1(const Graph& graph, std::unordered_set<size_t>& visited, size_t node, size_t end) {
        if (node == end) {
            return 1;
        }
        const bool is_big_cave = graph.big_caves.find(node) != graph.big_caves.end();
        uint64_t count = 0;
        if (!is_big_cave) {
            visited.insert(node);
        }
        for (const size_t adjacent : graph.adjacent_nodes[node]) {
            if (visited.find(adjacent) == visited.end()) {
                count += count_paths_part1(graph, visited, adjacent, end);
            }
        }
        if (!is_big_cave) {
            visited.erase(node);
        }
        return count;
    }

    uint64_t count_paths_part2(const Graph& graph, std::unordered_set<size_t>& visited, size_t node, size_t start, size_t end, std::optional<size_t> double_visited) {
        if (node == end) {
            return 1;
        }
        const bool is_big_cave = graph.big_caves.find(node) != graph.big_caves.end();
        uint64_t count = 0;
        if (!is_big_cave) {
            if (visited.find(node) == visited.end()) {
                visited.insert(node);
            } else {
                if (node == start || double_visited.has_value()) {
                    return 0;
                }
                double_visited = node;
            }
        }
        for (const size_t adjacent : graph.adjacent_nodes[node]) {
            count += count_paths_part2(graph, visited, adjacent, start, end, double_visited);
        }
        if (!is_big_cave && (!double_visited.has_value() || *double_visited != node)) {
            visited.erase(node);
        }
        return count;
    }
}

void Day12::part1(const std::vector<std::string> &lines) const {
    Graph graph = parse_input(lines);
    std::unordered_set<size_t> visited;
    uint64_t count = count_paths_part1(graph, visited, graph.node_ids["start"], graph.node_ids["end"]);
    std::cout << count << std::endl;
}

void Day12::part2(const std::vector<std::string> &lines) const {
    Graph graph = parse_input(lines);
    std::unordered_set<size_t> visited;
    uint64_t count = count_paths_part2(graph, visited, graph.node_ids["start"], graph.node_ids["start"], graph.node_ids["end"], {});
    std::cout << count << std::endl;
}
