//
// App.xaml.h
// Объявление класса приложения.
//

#pragma once
#include "App.g.h"
using namespace Platform;
using namespace Windows::ApplicationModel;
using namespace Windows::ApplicationModel::Activation;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Controls;
using namespace Windows::UI::Xaml::Controls::Primitives;
using namespace Windows::UI::Xaml::Data;
using namespace Windows::UI::Xaml::Input;
using namespace Windows::UI::Xaml::Interop;
using namespace Windows::UI::Xaml::Media;
using namespace Windows::UI::Xaml::Navigation;
using namespace Windows::Networking::Sockets;
using namespace Windows::Storage::Streams;
using namespace Windows::Data::Json;
using namespace Windows::Web::Http;
namespace chronopolis_ru
{
	/// <summary>
	/// Обеспечивает зависящее от конкретного приложения поведение, дополняющее класс Application по умолчанию.
	/// </summary>
	ref class App sealed
	{
	protected:
		virtual void OnLaunched(Windows::ApplicationModel::Activation::LaunchActivatedEventArgs^ e) override;
	internal:
		App();
		void InitWebSockets();
		void WebSocketMessageReceived(MessageWebSocket^ sender, MessageWebSocketMessageReceivedEventArgs^ args);
		void WebSocketClosed(IWebSocket^ sender, WebSocketClosedEventArgs^ args);
		void SendWebSocketMessage(MessageWebSocket^ sendingSocket, Platform::String^ message);
	private:
		void OnSuspending(Platform::Object^ sender, Windows::ApplicationModel::SuspendingEventArgs^ e);
		void OnNavigationFailed(Platform::Object ^sender, Windows::UI::Xaml::Navigation::NavigationFailedEventArgs ^e);
		MessageWebSocket^ m_messageWebSocket;
		Windows::Foundation::Uri^ m_serverUri;
		bool m_socketConnected = false;
		DataWriter^ m_messageWriter;
		WCHAR m_debugBuffer[512];
	};
}
