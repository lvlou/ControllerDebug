#pragma once

#include <Windows.h>
#include <Xinput.h>

class CController
{
private:
	XINPUT_STATE _state;
	XINPUT_CAPABILITIES _capabilities;
	XINPUT_BATTERY_INFORMATION _batteryInfo;
	int _idx = -1;

	bool UpdateState()
	{
		ZeroMemory(&_state, sizeof(XINPUT_STATE));
		ZeroMemory(&_capabilities, sizeof(XINPUT_CAPABILITIES));
		ZeroMemory(&_batteryInfo, sizeof(XINPUT_BATTERY_INFORMATION));

		DWORD u1 = XInputGetState(_idx, &_state);
		DWORD u2 = XInputGetCapabilities(_idx, XINPUT_FLAG_GAMEPAD, &_capabilities);
		DWORD u3 = XInputGetBatteryInformation(_idx, BATTERY_DEVTYPE_GAMEPAD, &_batteryInfo);

		return (u1 == ERROR_SUCCESS and u2 == ERROR_SUCCESS and u3 == ERROR_SUCCESS);
	}
public:
	CController(int controllerIdx) : _idx(controllerIdx)
	{
		
	}

	void OnConnect()
	{
		UpdateState();
	}

	bool IsWireless()
	{
		UpdateState();
		return (_capabilities.Flags & XINPUT_CAPS_WIRELESS);
	}

	bool HasIntegratedVoiceDevice()
	{
		UpdateState();
		return (_capabilities.Flags & XINPUT_CAPS_VOICE_SUPPORTED);
	}

	bool IsFFBCapable()
	{
		UpdateState();
		return (_capabilities.Flags & XINPUT_CAPS_FFB_SUPPORTED);
	}

	std::string GetBatteryLevel()
	{
		UpdateState();

		switch (_batteryInfo.BatteryLevel)
		{
		case BATTERY_LEVEL_EMPTY:
			return "EMPTY";
		case BATTERY_LEVEL_LOW:
			return "LOW";
		case BATTERY_LEVEL_MEDIUM:
			return "MEDIUM";
		case BATTERY_LEVEL_FULL:
			return "FULL";
		default:
			return "UNKNOWN";
		}
	}

	std::string GetBatteryType()
	{
		UpdateState();

		switch (_batteryInfo.BatteryType)
		{
		case BATTERY_TYPE_DISCONNECTED:
			return "DISCONNECTED";
		case BATTERY_TYPE_ALKALINE:
			return "ALKALINE";
		case BATTERY_TYPE_WIRED:
			return "WIRED";
		case BATTERY_TYPE_NIMH:
			return "NICKEL_METAL_HYDRIDE";
		case BATTERY_TYPE_UNKNOWN:
			return "UNKNOWN";
		default:
			return "OTHER";
		}
	}

	std::string GetDeviceType()
	{
		UpdateState();
		
		switch (_capabilities.SubType)
		{
		case 0x00:
			return "UNKNOWN";
		case 0x01:
			return "GAMEPAD";
		case 0x02:
			return "WHEEL";
		case 0x03:
			return "ARCADE_STICK";
		case 0x04:
			return "FLIGHT_STICK";
		case 0x05:
			return "DANCE_PAD";
		case 0x06:
			return "GUITAR";
		case 0x07:
			return "GUITAR_ALT";
		case 0x08:
			return "DRUM_KIT";
		case 0x0B:
			return "GUITAR_BASS";
		case 0x13:
			return "ARCADE_PAD";
		default:
			return "OTHER";
		}
	}

	XINPUT_STATE* GetState()
	{
		UpdateState();
		return &_state;
	}

	bool Connected()
	{
		return (UpdateState() == TRUE);
	}

	bool Vibrate(float lowMotor, float highMotor)
	{
		if (lowMotor >= 0.0 and lowMotor <= 100.0 and highMotor >= 0.0 and highMotor <= 100.0)
		{
			XINPUT_VIBRATION vibration;
			ZeroMemory(&vibration, sizeof(XINPUT_VIBRATION));

			vibration.wLeftMotorSpeed = lowMotor * (65535 / 100.0f);
			vibration.wRightMotorSpeed = highMotor * (65535 / 100.0f);

			return (XInputSetState(_idx, &vibration) == ERROR_SUCCESS);
		}

		return false;
	}

	float GetTriggerLevel(bool leftTrigger)
	{
		UpdateState();

		auto value = (leftTrigger) ? _state.Gamepad.bLeftTrigger : _state.Gamepad.bRightTrigger;
		return value / (255 / 100.0f);
	}

	float GetThumbLevel(bool leftThumb, bool xAxis)
	{
		UpdateState();

		auto value = 0;
		if (leftThumb)
		{
			value = xAxis ? _state.Gamepad.sThumbLX : _state.Gamepad.sThumbLY;
		}
		else
		{
			value = xAxis ? _state.Gamepad.sThumbRX : _state.Gamepad.sThumbRY;
		}

		return value / (32767 / 100.0f);
	}

	bool IsButtonPressed(WORD btn)
	{
		UpdateState();
		return (_state.Gamepad.wButtons & btn);
	}
};