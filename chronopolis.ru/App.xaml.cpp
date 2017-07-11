//
// App.xaml.cpp
// Реализация класса приложения.
//

#include "pch.h"
#include "MainPage.xaml.h"

using namespace chronopolis_ru;

/// <summary>
/// Инициализирует одноэлементный объект приложения.  Это первая выполняемая строка разрабатываемого
/// кода; поэтому она является логическим эквивалентом main() или WinMain().
/// </summary>
App::App()
{
    InitializeComponent();
		InitWebSockets();
		SendWebSocketMessage(m_messageWebSocket, L"Kyrill");
    Suspending += ref new SuspendingEventHandler(this, &App::OnSuspending);

		Uri^ uri = ref new Uri("http://chronopolis.ru/phone");

		HttpClient^ httpClient = ref new HttpClient();

		// Always catch network exceptions for async methods
		create_task(httpClient->GetStringAsync(uri)).then([this](task<void> previousTask)
		{
			try
			{
				previousTask.get();
			}
			catch (Platform::COMException^ ex)
			{
				// Details in ex.Message and ex.HResult.       
			}
			//httpclient->Dispose();
		});

		// Once your app is done using the HttpClient object call dispose to 
		// free up system resources (the underlying socket and memory used for the object)

}

void App::InitWebSockets()
{
	// Create a new web socket
	m_messageWebSocket = ref new MessageWebSocket();

	// Set the message type to UTF-8
	m_messageWebSocket->Control->MessageType = Windows::Networking::Sockets::SocketMessageType::Utf8;

	// Register callbacks for notifications of interest
	m_messageWebSocket->MessageReceived +=
		ref new TypedEventHandler<MessageWebSocket^, MessageWebSocketMessageReceivedEventArgs^>(this, &App::WebSocketMessageReceived);
	m_messageWebSocket->Closed += ref new TypedEventHandler<IWebSocket^, WebSocketClosedEventArgs^>(this, &App::WebSocketClosed);

	// This test code uses the websocket.org echo service to illustrate sending a string and receiving the echoed string back
	// Note that wss: makes this an encrypted connection.
	m_serverUri = ref new Uri("ws://chronopolis.ru/ws");

	// Establish the connection, and set m_socketConnected on success
	create_task(m_messageWebSocket->ConnectAsync(m_serverUri)).then([this](task<void> previousTask)
	{
		try
		{
			// Try getting all exceptions from the continuation chain above this point.
			previousTask.get();

			// websocket connected. update state variable
			m_socketConnected = true;
			OutputDebugString(L"Successfully initialized websockets\n");
			SendWebSocketMessage(m_messageWebSocket, L"Kyrill");
		}
		catch (Platform::COMException^ ex)
		{
			// Add code here to handle any exceptions
			OutputDebugString(ex->Message->Data());

		}
	});
}
void App::WebSocketMessageReceived(MessageWebSocket^ sender, MessageWebSocketMessageReceivedEventArgs^ args)
{
	DataReader^ messageReader = args->GetDataReader();
	messageReader->UnicodeEncoding = Windows::Storage::Streams::UnicodeEncoding::Utf8;

	String^ readString = messageReader->ReadString(messageReader->UnconsumedBufferLength);
	// Data has been read and is now available from the readString variable.
	swprintf(m_debugBuffer, 511, L"WebSocket Message received: %s\n", readString->Data());
	OutputDebugStringW(m_debugBuffer);
}
void App::WebSocketClosed(IWebSocket^ sender, WebSocketClosedEventArgs^ args)
{
	// The method may be triggered remotely by the server sending unsolicited close frame or locally by Close()/delete operator.
	// This method assumes we saved the connected WebSocket to a variable called m_messageWebSocket
	if (m_messageWebSocket != nullptr)
	{
		delete m_messageWebSocket;
		m_messageWebSocket = nullptr;
		OutputDebugString(L"Socket was closed\n");
	}
	m_socketConnected = false;
}
void App::SendWebSocketMessage(Windows::Networking::Sockets::MessageWebSocket^ sendingSocket, Platform::String^ message)
{
	if (m_socketConnected)
	{
		// WebSocket is connected, so send a message
		m_messageWriter = ref new DataWriter(sendingSocket->OutputStream);

		m_messageWriter->WriteString(message);

		// Send the data as one complete message
		create_task(m_messageWriter->StoreAsync()).then([this](unsigned int)
		{
			// Send Completed
			m_messageWriter->DetachStream();    // give the stream back to m_messageWebSocket
			OutputDebugString(L"Sent websocket message\n");
		})
			.then([this](task<void> previousTask)
		{
			try
			{
				// Try getting all exceptions from the continuation chain above this point.
				previousTask.get();
			}
			catch (Platform::COMException ^ex)
			{
				// Add code to handle the exception
				OutputDebugString(ex->Message->Data());
			}
		});
	}
}
/// <summary>
/// Вызывается при обычном запуске приложения пользователем. Будут использоваться другие точки входа,
/// например, если приложение запускается для открытия конкретного файла.
/// </summary>
/// <param name="e">Сведения о запросе и обработке запуска.</param>
void App::OnLaunched(Windows::ApplicationModel::Activation::LaunchActivatedEventArgs^ e)
{
    auto rootFrame = dynamic_cast<Frame^>(Window::Current->Content);

    // Не повторяйте инициализацию приложения, если в окне уже имеется содержимое,
    // только обеспечьте активность окна
    if (rootFrame == nullptr)
    {
        // Создайте фрейм, используемый в качестве контекста навигации, и свяжите его с
        // ключом SuspensionManager
        rootFrame = ref new Frame();

        rootFrame->NavigationFailed += ref new Windows::UI::Xaml::Navigation::NavigationFailedEventHandler(this, &App::OnNavigationFailed);

        if (e->PreviousExecutionState == ApplicationExecutionState::Terminated)
        {
            // TODO: Восстановление сохраненного состояния сеанса только при необходимости и планирование
            // финальных шагов запуска после завершения восстановления

        }

        if (e->PrelaunchActivated == false)
        {
            if (rootFrame->Content == nullptr)
            {
                // Если стек навигации не восстанавливается для перехода к первой странице,
                // настройка новой страницы путем передачи необходимой информации в качестве параметра
                // навигации
                rootFrame->Navigate(TypeName(MainPage::typeid), e->Arguments);
            }
            // Размещение фрейма в текущем окне
            Window::Current->Content = rootFrame;
            // Обеспечение активности текущего окна
            Window::Current->Activate();
        }
    }
    else
    {
        if (e->PrelaunchActivated == false)
        {
            if (rootFrame->Content == nullptr)
            {
                // Если стек навигации не восстанавливается для перехода к первой странице,
                // настройка новой страницы путем передачи необходимой информации в качестве параметра
                // параметр
                rootFrame->Navigate(TypeName(MainPage::typeid), e->Arguments);
            }
            // Обеспечение активности текущего окна
            Window::Current->Activate();
        }
    }
}

/// <summary>
/// Вызывается при приостановке выполнения приложения.  Состояние приложения сохраняется
/// без учета информации о том, будет ли оно завершено или возобновлено с неизменным
/// содержимым памяти.
/// </summary>
/// <param name="sender">Источник запроса приостановки.</param>
/// <param name="e">Сведения о запросе приостановки.</param>
void App::OnSuspending(Object^ sender, SuspendingEventArgs^ e)
{
    (void) sender;  // Неиспользуемый параметр
    (void) e;   // Неиспользуемый параметр

    //TODO: Сохранить состояние приложения и остановить все фоновые операции
}

/// <summary>
/// Вызывается в случае сбоя навигации на определенную страницу
/// </summary>
/// <param name="sender">Фрейм, для которого произошел сбой навигации</param>
/// <param name="e">Сведения о сбое навигации</param>
void App::OnNavigationFailed(Platform::Object ^sender, Windows::UI::Xaml::Navigation::NavigationFailedEventArgs ^e)
{
    throw ref new FailureException("Failed to load Page " + e->SourcePageType.Name);
}