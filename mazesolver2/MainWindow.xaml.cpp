#include "pch.h"
#include "MainWindow.xaml.h"
#include "BacktrackingSolver.h"
#if __has_include("MainWindow.g.cpp")
#include "MainWindow.g.cpp"
#endif

#include <random>
#include <chrono>
#include <string>

using namespace winrt;
using namespace Windows::Foundation;
using namespace Microsoft::UI;
using namespace Microsoft::UI::Xaml;
using namespace Microsoft::UI::Xaml::Controls;
using namespace Microsoft::UI::Xaml::Media;
using namespace Microsoft::UI::Xaml::Input;

namespace winrt::mazesolver2::implementation
{
    MainWindow::MainWindow()
    {
        InitializeComponent();
        DrawGrid();
    }

    void MainWindow::SetStartButton_Click(IInspectable const&, RoutedEventArgs const&)
    {
        choosingStart = true;
        choosingEnd = false;
    }

    void MainWindow::SetEndButton_Click(IInspectable const&, RoutedEventArgs const&)
    {
        choosingEnd = true;
        choosingStart = false;
    }

    void MainWindow::DrawGrid()
    {
        MazeGrid().RowDefinitions().Clear();
        MazeGrid().ColumnDefinitions().Clear();
        MazeGrid().Children().Clear();

        for (int i = 0; i < ROW; i++)
        {
            MazeGrid().RowDefinitions().Append(RowDefinition());
            MazeGrid().ColumnDefinitions().Append(ColumnDefinition());
        }

        for (int i = 0; i < ROW; i++)
        {
            for (int j = 0; j < COL; j++)
            {
                Border border;
                border.BorderThickness(Thickness{ 1,1,1,1 });
                border.BorderBrush(SolidColorBrush(Colors::Black()));
                border.Background(SolidColorBrush(Colors::White()));
                border.Tag(box_value(i * COL + j));

                border.Tapped({ this, &MainWindow::Cell_Tapped });

                Grid::SetRow(border, i);
                Grid::SetColumn(border, j);
                MazeGrid().Children().Append(border);

                cells[i][j] = border;
            }
        }
    }

    void MainWindow::Cell_Tapped(IInspectable const& sender, TappedRoutedEventArgs const&)
    {
        Border border = sender.as<Border>();
        int value = unbox_value<int>(border.Tag());
        int row = value / COL;
        int col = value % COL;

        if (choosingStart)
        {
            if (maze[row][col] == 1) return;
            if (startRow != -1)
            {
                maze[startRow][startCol] = 0;
                cells[startRow][startCol].Background(SolidColorBrush(Colors::White()));
            }
            startRow = row; startCol = col;
            maze[row][col] = 2;
            border.Background(SolidColorBrush(Colors::Green()));
            choosingStart = false;
            return;
        }

        if (choosingEnd)
        {
            if (maze[row][col] == 1) return;
            if (endRow != -1)
            {
                maze[endRow][endCol] = 0;
                cells[endRow][endCol].Background(SolidColorBrush(Colors::White()));
            }
            endRow = row; endCol = col;
            maze[row][col] = 3;
            border.Background(SolidColorBrush(Colors::Red()));
            choosingEnd = false;
            return;
        }

        if (maze[row][col] == 2 || maze[row][col] == 3) return;

        if (maze[row][col] == 0)
        {
            maze[row][col] = 1;
            border.Background(SolidColorBrush(Colors::Black()));
        }
        else
        {
            maze[row][col] = 0;
            border.Background(SolidColorBrush(Colors::White()));
        }
    }

    void MainWindow::ResetButton_Click(IInspectable const&, RoutedEventArgs const&)
    {
        startRow = -1; startCol = -1;
        endRow = -1; endCol = -1;

        for (int i = 0; i < ROW; i++)
        {
            for (int j = 0; j < COL; j++)
            {
                maze[i][j] = 0;
                cells[i][j].Background(SolidColorBrush(Colors::White()));
            }
        }
    }

    void MainWindow::DrawMaze(const std::vector<std::vector<int>>& data)
    {
        ResetButton_Click(nullptr, nullptr);
        size_t rows = data.size();
        size_t cols = rows ? data[0].size() : 0;

        for (size_t i = 0; i < rows; i++)
        {
            for (size_t j = 0; j < cols; j++)
            {
                maze[i][j] = data[i][j];
                switch (data[i][j])
                {
                case 0:
                    cells[i][j].Background(SolidColorBrush(Colors::White()));
                    break;
                case 1:
                    cells[i][j].Background(SolidColorBrush(Colors::Black()));
                    break;
                case 2:
                    cells[i][j].Background(SolidColorBrush(Colors::Green()));
                    startRow = static_cast<int>(i); startCol = static_cast<int>(j);
                    break;
                case 3:
                    cells[i][j].Background(SolidColorBrush(Colors::Red()));
                    endRow = static_cast<int>(i); endCol = static_cast<int>(j);
                    break;
                }
            }
        }
    }

    void MainWindow::Maze1_Click(IInspectable const&, RoutedEventArgs const&)
    {
        std::vector<std::vector<int>> map = {
            {2,0,0,1,0}, {1,1,0,1,0}, {0,0,0,0,0}, {0,1,1,1,0}, {0,0,0,3,0}
        };
        DrawMaze(map);
    }

    void MainWindow::Maze3_Click(IInspectable const&, RoutedEventArgs const&)
    {
        std::vector<std::vector<int>> map = {
            {2,0,0,0,0}, {1,1,0,1,0}, {0,0,0,0,0}, {0,1,0,1,0}, {0,0,0,3,0}
        };
        DrawMaze(map);
    }

    void MainWindow::Maze4_Click(IInspectable const&, RoutedEventArgs const&)
    {
        ResetButton_Click(nullptr, nullptr);
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(0, 99);

        for (int i = 0; i < ROW; i++)
        {
            for (int j = 0; j < COL; j++)
            {
                if (dis(gen) < 30)
                {
                    maze[i][j] = 1;
                    cells[i][j].Background(SolidColorBrush(Colors::Black()));
                }
            }
        }
        startRow = 0; startCol = 0;
        endRow = ROW - 1; endCol = COL - 1;
        maze[startRow][startCol] = 2; maze[endRow][endCol] = 3;
        cells[startRow][startCol].Background(SolidColorBrush(Colors::Green()));
        cells[endRow][endCol].Background(SolidColorBrush(Colors::Red()));
    }

    void MainWindow::Maze5_Click(IInspectable const&, RoutedEventArgs const&)
    {
        ResetButton_Click(nullptr, nullptr);
        startRow = 0; startCol = 0;
        endRow = ROW - 1; endCol = COL - 1;
        maze[startRow][startCol] = 2; maze[endRow][endCol] = 3;
        cells[startRow][startCol].Background(SolidColorBrush(Colors::Green()));
        cells[endRow][endCol].Background(SolidColorBrush(Colors::Red()));

        for (int i = 1; i < ROW - 1; i++)
        {
            for (int j = 1; j < COL - 1; j++)
            {
                if (i % 2 == 1)
                {
                    maze[i][j] = 1;
                    cells[i][j].Background(SolidColorBrush(Colors::Black()));
                }
            }
        }
    }

    void MainWindow::FindPath_Click(IInspectable const&, RoutedEventArgs const&)
    {
        if (startRow == -1 || endRow == -1) return;

        // Xóa các vết đường đi màu vàng cũ (chừa lại điểm Start/End và tường)
        for (int i = 0; i < ROW; i++) {
            for (int j = 0; j < COL; j++) {
                if (maze[i][j] == 0) {
                    cells[i][j].Background(SolidColorBrush(Colors::White()));
                }
            }
        }

        std::vector<std::vector<int>> graph(ROW, std::vector<int>(COL));
        for (int i = 0; i < ROW; i++)
        {
            for (int j = 0; j < COL; j++)
            {
                graph[i][j] = (maze[i][j] == 1) ? 1 : 0;
            }
        }

        std::vector<std::pair<int, int>> path;
        int visitedCount = 0;

        // Gọi trực tiếp UI Element sinh ra từ file XAML: AlgoComboBox()
        int selectedAlgo = AlgoComboBox().SelectedIndex();

        if (selectedAlgo == 0) // Lựa chọn 1: Backtracking
        {
            path = ::mazesolver2::BacktrackingSolver::FindPath(graph, startRow, startCol, endRow, endCol);
        }
        else // Lựa chọn 2: BFS
        {
            path = ::mazesolver2::BacktrackingSolver::FindPathBFS(graph, startRow, startCol, endRow, endCol, visitedCount);
        }

        ContentDialog dialog;
        dialog.Title(box_value(L"Kết quả"));

        if (path.empty())
        {
            dialog.Content(box_value(L"Không tìm thấy đường đi."));
        }
        else
        {
            // Tô màu vàng cho đường đi ngắn nhất (Bỏ qua ô đầu tiên và ô cuối cùng)
            for (size_t i = 1; i < path.size() - 1; i++)
            {
                cells[path[i].first][path[i].second].Background(SolidColorBrush(Colors::Yellow()));
            }

            std::wstring message = L"Tìm thấy đường đi gồm " + std::to_wstring(path.size()) + L" ô.";
            if (selectedAlgo == 1)
            {
                message += L"\nSố ô BFS đã duyệt qua: " + std::to_wstring(visitedCount);
            }
            dialog.Content(box_value(message));
        }

        dialog.CloseButtonText(L"OK");
        dialog.XamlRoot(Content().XamlRoot());
        dialog.ShowAsync();
    }
}