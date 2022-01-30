#include <iostream>

#include "CController.hpp"

int main(int argc, char** argv)
{
	CController testCtr(0);

	while (not testCtr.Connected())
	{
		std::cout << "No controller found, waiting...\n";
		Sleep(500);
	}

	std::cout << "Connected!\nPlease do not disconnect your controller, or the application will exit.\n";

	testCtr.OnConnect();
	std::cout << "---\nType: " << testCtr.GetDeviceType() << "\nWireless: " << testCtr.IsWireless() << "\nSupports voice: " << testCtr.HasIntegratedVoiceDevice() << "\nFFB capable: " << testCtr.IsFFBCapable() << "\nBattery: " << testCtr.GetBatteryType() << "\t" << testCtr.GetBatteryLevel() << "\n---\nPress X on your controller to test the low frequency motor, or press B to test the high frequency one.\n";

	while (testCtr.Connected())
	{
		if (testCtr.IsButtonPressed(XINPUT_GAMEPAD_X))
		{
			testCtr.Vibrate(100.0f, 0.0f);
			Sleep(500);
			testCtr.Vibrate(0.0f, 0.0f);
		}

		if (testCtr.IsButtonPressed(XINPUT_GAMEPAD_B))
		{
			testCtr.Vibrate(0.0f, 100.0f);
			Sleep(500);
			testCtr.Vibrate(0.0f, 0.0f);
		}
	}

	return 0;
}