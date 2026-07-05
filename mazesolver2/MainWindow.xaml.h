#pragma once

#include "MainWindow.g.h"

#include <array>
#include <vector>

#include <winrt/Microsoft.UI.Xaml.h>
#include <winrt/Microsoft.UI.Xaml.Controls.h>
#include <winrt/Microsoft.UI.Xaml.Media.h>
#include <winrt/Microsoft.UI.Xaml.Input.h>

namespace winrt::mazesolver2::implementation
{
    struct MainWindow : MainWindowT<MainWindow>
    {
        MainWindow();

        static constexpr int ROW = 20;
        static constexpr int COL = 20;

        bool choosingStart{ false };
        bool choosingEnd{ false };

        std::array<
            std::array<Microsoft::UI::Xaml::Controls::Border, COL>,
            ROW> cells;

        std::array<
            std::array<int, COL>,
            ROW> maze{};

        int startRow{ -1 };
        int startCol{ -1 };

        int endRow{ -1 };
        int endCol{ -1 };

        // Buttons Click Handlers
        void Maze1_Click(
            Windows::Foundation::IInspectable const&,
            Microsoft::UI::Xaml::RoutedEventArgs const&);

        void Maze3_Click(
            Windows::Foundation::IInspectable const&,
            Microsoft::UI::Xaml::RoutedEventArgs const&);

        void Maze4_Click(
            Windows::Foundation::IInspectable const&,
            Microsoft::UI::Xaml::RoutedEventArgs const&);

        void Maze5_Click(
            Windows::Foundation::IInspectable const&,
            Microsoft::UI::Xaml::RoutedEventArgs const&);

        void FindPath_Click(
            Windows::Foundation::IInspectable const&,
            Microsoft::UI::Xaml::RoutedEventArgs const&);

        void ResetButton_Click(
            Windows::Foundation::IInspectable const&,
            Microsoft::UI::Xaml::RoutedEventArgs const&);

        void SetStartButton_Click(
            Windows::Foundation::IInspectable const&,
            Microsoft::UI::Xaml::RoutedEventArgs const&);

        void SetEndButton_Click(
            Windows::Foundation::IInspectable const&,
            Microsoft::UI::Xaml::RoutedEventArgs const&);

        void Cell_Tapped(
            Windows::Foundation::IInspectable const&,
            Microsoft::UI::Xaml::Input::TappedRoutedEventArgs const&);

    private:
        void DrawGrid();
        void DrawMaze(const std::vector<std::vector<int>>& data);
    };
}

namespace winrt::mazesolver2::factory_implementation
{
    struct MainWindow :
        MainWindowT<MainWindow, implementation::MainWindow>
    {
    };
}