#include "pch.h"
#include "BacktrackingSolver.h"
#include <queue>
#include <algorithm>

using namespace std;

namespace mazesolver2
{
    namespace
    {
        std::vector<std::vector<bool>> visited;
        std::vector<std::pair<int, int>> path;
        std::vector<std::vector<std::pair<int, int>>> allPaths;

        constexpr int dx[4] = { -1, 1, 0, 0 };
        constexpr int dy[4] = { 0, 0, -1, 1 };
    }

    bool BacktrackingSolver::IsValid(
        int x,
        int y,
        const std::vector<std::vector<int>>& maze,
        const std::vector<std::vector<bool>>& visited)
    {
        return x >= 0 &&
            x < (int)maze.size() &&
            y >= 0 &&
            y < (int)maze[0].size() &&
            maze[x][y] == 0 &&
            !visited[x][y];
    }

    void BacktrackingSolver::Backtrack(
        int x,
        int y,
        int endX,
        int endY,
        const std::vector<std::vector<int>>& maze,
        std::vector<std::vector<bool>>& visited,
        std::vector<std::pair<int, int>>& path,
        std::vector<std::vector<std::pair<int, int>>>& allPaths)
    {
        if (x == endX && y == endY)
        {
            allPaths.push_back(path);
            return;
        }

        for (int i = 0; i < 4; i++)
        {
            int nx = x + dx[i];
            int ny = y + dy[i];

            if (IsValid(nx, ny, maze, visited))
            {
                visited[nx][ny] = true;
                path.push_back({ nx, ny });

                Backtrack(
                    nx,
                    ny,
                    endX,
                    endY,
                    maze,
                    visited,
                    path,
                    allPaths);

                path.pop_back();
                visited[nx][ny] = false;
            }
        }
    }

    std::vector<std::pair<int, int>>
        BacktrackingSolver::FindPath(
            const std::vector<std::vector<int>>& maze,
            int startRow,
            int startCol,
            int endRow,
            int endCol)
    {
        visited.assign(
            maze.size(),
            std::vector<bool>(maze[0].size(), false));

        path.clear();
        allPaths.clear();

        visited[startRow][startCol] = true;
        path.push_back({ startRow, startCol });

        Backtrack(
            startRow,
            startCol,
            endRow,
            endCol,
            maze,
            visited,
            path,
            allPaths);

        if (allPaths.empty())
        {
            return {};
        }

        int minIndex = 0;
        for (int i = 1; i < (int)allPaths.size(); i++)
        {
            if (allPaths[i].size() < allPaths[minIndex].size())
            {
                minIndex = i;
            }
        }

        return allPaths[minIndex];
    }

    std::vector<std::pair<int, int>>
        BacktrackingSolver::FindPathBFS(
            const std::vector<std::vector<int>>& maze,
            int startRow,
            int startCol,
            int endRow,
            int endCol,
            int& visitedCount)
    {
        int rows = (int)maze.size();
        int cols = (int)maze[0].size();

        std::queue<std::pair<int, int>> q;
        std::vector<std::vector<bool>> vis(rows, std::vector<bool>(cols, false));
        std::vector<std::vector<std::pair<int, int>>> parent(rows, std::vector<std::pair<int, int>>(cols, { -1, -1 }));

        q.push({ startRow, startCol });
        vis[startRow][startCol] = true;
        visitedCount = 1;

        bool found = false;
        while (!q.empty())
        {
           
            auto const& cur = q.front();
            int x = cur.first;
            int y = cur.second;
            q.pop(); 

            if (x == endRow && y == endCol)
            {
                found = true;
                break;
            }
            

            for (int i = 0; i < 4; i++)
            {
                int nx = x + dx[i];
                int ny = y + dy[i];

                if (nx >= 0 && nx < rows && ny >= 0 && ny < cols && maze[nx][ny] == 0 && !vis[nx][ny])
                {
                    vis[nx][ny] = true;
                    parent[nx][ny] = { x, y };
                    q.push({ nx, ny });
                    visitedCount++;
                }
            }
        }

        if (!found || parent[endRow][endCol].first == -1)
        {
            return {};
        }

        std::vector<std::pair<int, int>> pathBFS;
        int x = endRow, y = endCol;
        while (!(x == startRow && y == startCol))
        {
            pathBFS.push_back({ x, y });
            auto p = parent[x][y];
            x = p.first; y = p.second;
        }
        pathBFS.push_back({ startRow, startCol });
        std::reverse(pathBFS.begin(), pathBFS.end());

        return pathBFS;
    }
}