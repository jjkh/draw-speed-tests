// Copyright (c) Microsoft Corporation and Contributors.
// Licensed under the MIT License.

#include "pch.h"
#include "MainWindow.xaml.h"
#if __has_include("MainWindow.g.cpp")
#include "MainWindow.g.cpp"
#endif

using namespace winrt;
using namespace Microsoft::UI::Xaml;
using namespace Microsoft::UI::Xaml::Input;
using namespace Microsoft::UI::Windowing;
using namespace Windows::Foundation;
using namespace Windows::System;

// To learn more about WinUI, the WinUI project structure,
// and more about our project templates, see: http://aka.ms/winui-project-info.

namespace winrt::winui3_test::implementation
{
    MainWindow::MainWindow()
    {
        AppWindow().SetPresenter(AppWindowPresenterKind::FullScreen);
        InitializeComponent();
    }

    void MainWindow::OnKeyUp(IInspectable const&, KeyRoutedEventArgs const& args)
    {
        if (args.Key() == VirtualKey::Escape)
            Close();
    }
}
