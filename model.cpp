#include <iostream>
#include <windows.h>
#include <wininet.h>
#include <string>

#pragma comment(lib, "wininet.lib")

// Function to make a POST request to an API
std::string sendPostRequest(const std::string& url, const std::string& data, const std::string& auth_token) {
    HINTERNET hInternet, hConnect;
    DWORD bytesRead;
    CHAR buffer[1024];

    // Initialize WinINet API
    hInternet = InternetOpen("HTTP Client", INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0); // Use ANSI string here
    if (hInternet == NULL) {
        std::cerr << "InternetOpen failed" << std::endl;
        return "";
    }

    // Connect to the API server
    hConnect = InternetOpenUrlA(hInternet, url.c_str(), NULL, 0, INTERNET_FLAG_RELOAD, 0);
    if (hConnect == NULL) {
        std::cerr << "InternetOpenUrlA failed" << std::endl;
        InternetCloseHandle(hInternet);
        return "";
    }

    // Set the request headers
    const std::string headers = "Content-Type: application/json\r\nAuthorization: Bearer " + auth_token + "\r\n";

    // Send the POST request
    BOOL sent = HttpSendRequestA(hConnect, headers.c_str(), -1, (LPVOID)data.c_str(), data.size());
    if (!sent) {
        std::cerr << "HttpSendRequestA failed" << std::endl;
        InternetCloseHandle(hConnect);
        InternetCloseHandle(hInternet);
        return "";
    }

    // Read the response
    std::string response;
    while (InternetReadFile(hConnect, buffer, sizeof(buffer), &bytesRead) && bytesRead > 0) {
        response.append(buffer, bytesRead);
    }

    // Clean up
    InternetCloseHandle(hConnect);
    InternetCloseHandle(hInternet);

    return response;
}

int main() {
    std::string url = "https://api-inference.huggingface.co/models/gpt2";
    std::string auth_token = "YOUR_HUGGING_FACE_API_KEY"; // Replace with your Hugging Face API key

    // Example input to send as JSON to the API
    std::string data = "{\"inputs\": \"What is the capital of France?\"}";

    // Send the POST request
    std::string response = sendPostRequest(url, data, auth_token);

    // Output the response
    std::cout << "Response: " << response << std::endl;

    return 0;
}
