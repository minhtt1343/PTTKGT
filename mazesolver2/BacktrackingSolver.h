#pragma once

#include <vector>
#include <utility>

namespace mazesolver2
{
    class BacktrackingSolver
    {
    public:
        static std::vector<std::pair<int, int>> FindPath(
            const std::vector<std::vector<int>>& maze,
            int startRow,
            int startCol,
            int endRow,
            int endCol);

        // ĐẢM BẢO DÒNG KHAI BÁO NÀY NẰM TRONG PUBLIC CỦA CLASS
        static std::vector<std::pair<int, int>> FindPathBFS(
            const std::vector<std::vector<int>>& maze,
            int startRow,
            int startCol,
            int endRow,
            int endCol,
            int& visitedCount);

    private:
        static bool IsValid(
            int x,
            int y,
            const std::vector<std::vector<int>>& maze,
            const std::vector<std::vector<bool>>& visited);

        static void Backtrack(
            int x,
            int y,
            int endX,
            int endY,
            const std::vector<std::vector<int>>& maze,
            std::vector<std::vector<bool>>& visited,
            std::vector<std::pair<int, int>>& path,
            std::vector<std::vector<std::pair<int, int>>>& allPaths);
    };
}