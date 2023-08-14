//
// App.xaml.h
// Declaration of the App class.
//

#pragma once

#include "App.g.h"

namespace uwp_test
{
	/// <summary>
	/// Provides application-specific behavior to supplement the default Application class.
	/// </summary>
	ref class App sealed
	{
	protected:
		virtual void OnLaunched(Windows::ApplicationModel::Activation::LaunchActivatedEventArgs^ e) override;

	internal:
		App();
	};
}
