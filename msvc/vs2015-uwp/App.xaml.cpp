#include "pch.h"

using namespace Platform;
using namespace Windows::ApplicationModel;
using namespace Windows::ApplicationModel::Activation;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Controls;

namespace libhltypes_tests
{
	App::App()
	{
		this->InitializeComponent();
	}

	void App::OnLaunched(Windows::ApplicationModel::Activation::LaunchActivatedEventArgs^ e)
	{
		Frame^ rootFrame = dynamic_cast<Frame^>(Window::Current->Content);
		if (rootFrame == nullptr)
		{
			rootFrame = ref new Frame();
			rootFrame->NavigationFailed += ref new Windows::UI::Xaml::Navigation::NavigationFailedEventHandler(this, &App::OnNavigationFailed);
			rootFrame->Width = 1;
			rootFrame->Height = 1;
			Window::Current->Content = rootFrame;
		}
		Window::Current->Activate();
		Microsoft::VisualStudio::TestPlatform::TestExecutor::WinRTCore::UnitTestClient::Run(e->Arguments);
	}

	void App::OnNavigationFailed(Platform::Object^ sender, Windows::UI::Xaml::Navigation::NavigationFailedEventArgs^ e)
	{
		throw ref new FailureException("Failed to load Page " + e->SourcePageType.Name);
	}

}
