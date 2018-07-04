#ifndef HLTYPES_TESTS_APP_XAML_H
#define HLTYPES_TESTS_APP_XAML_H

#include "App.g.h"

namespace libhltypes_tests
{
	ref class App sealed
	{
	protected:
		virtual void OnLaunched(Windows::ApplicationModel::Activation::LaunchActivatedEventArgs^ e) override;

	internal:
		App();

	private:
		void OnNavigationFailed(Platform::Object^ sender, Windows::UI::Xaml::Navigation::NavigationFailedEventArgs^ e);

	};
}

#endif
