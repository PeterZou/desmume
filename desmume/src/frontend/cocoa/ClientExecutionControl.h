/*
	Copyright (C) 2017 DeSmuME team
 
	This file is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 2 of the License, or
	(at your option) any later version.
 
	This file is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.
 
	You should have received a copy of the GNU General Public License
	along with the this software.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef _CLIENT_EXECUTION_CONTROL_H_
#define _CLIENT_EXECUTION_CONTROL_H_

#include <pthread.h>
#include <string>

#include "../../slot1.h"
#undef BOOL

#define SPEED_SCALAR_QUARTER						0.25	// Speed scalar for quarter execution speed.
#define SPEED_SCALAR_HALF							0.5		// Speed scalar for half execution speed.
#define SPEED_SCALAR_THREE_QUARTER					0.75	// Speed scalar for three quarters execution speed.
#define SPEED_SCALAR_NORMAL							1.0		// Speed scalar for normal execution speed.
#define SPEED_SCALAR_DOUBLE							2.0		// Speed scalar for double execution speed.
#define SPEED_SCALAR_MIN							0.005	// Lower limit for the speed multiplier.

#define DS_FRAMES_PER_SECOND						59.8261	// Number of DS frames per second.
#define DS_SECONDS_PER_FRAME						(1.0 / DS_FRAMES_PER_SECOND) // The length of time in seconds that, ideally, a frame should be processed within.

#define FRAME_SKIP_AGGRESSIVENESS					9.0		// Must be a value between 0.0 (inclusive) and positive infinity.
															// This value acts as a scalar multiple of the frame skip.
#define FRAME_SKIP_BIAS								0.1		// May be any real number. This value acts as a vector addition to the frame skip.
#define MAX_FRAME_SKIP								(DS_FRAMES_PER_SECOND / 2.98)

#define INPUT_STATES_CLEAR_VALUE					0xFFFFFFFF00FF03FFUL

enum ExecutionBehavior
{
	ExecutionBehavior_Pause = 0,
	ExecutionBehavior_Run,
	ExecutionBehavior_FrameAdvance,
	ExecutionBehavior_FrameJump
};

enum FrameJumpBehavior
{
	FrameJumpBehavior_Forward		= 0,
	FrameJumpBehavior_ToFrame		= 1,
	FrameJumpBehavior_NextMarker	= 2
};

enum CPUEmulationEngineID
{
	CPUEmulationEngineID_Interpreter			= 0,
	CPUEmulationEngineID_DynamicRecompiler		= 1
};

enum NDSInputID
{
	NDSInputID_A = 0,
	NDSInputID_B,
	NDSInputID_Select,
	NDSInputID_Start,
	NDSInputID_Right,
	NDSInputID_Left,
	NDSInputID_Up,
	NDSInputID_Down,
	NDSInputID_R,
	NDSInputID_L,
	
	NDSInputID_X,
	NDSInputID_Y,
	NDSInputID_Debug,
	NDSInputID_Touch,
	NDSInputID_Lid,
	
	NDSInputID_Microphone,
	
	NDSInputID_GuitarGrip_Green,
	NDSInputID_GuitarGrip_Red,
	NDSInputID_GuitarGrip_Yellow,
	NDSInputID_GuitarGrip_Blue,
	
	NDSInputID_Piano_C,
	NDSInputID_Piano_CSharp,
	NDSInputID_Piano_D,
	NDSInputID_Piano_DSharp,
	NDSInputID_Piano_E,
	NDSInputID_Piano_F,
	NDSInputID_Piano_FSharp,
	NDSInputID_Piano_G,
	NDSInputID_Piano_GSharp,
	NDSInputID_Piano_A,
	NDSInputID_Piano_ASharp,
	NDSInputID_Piano_B,
	NDSInputID_Piano_HighC,
	
	NDSInputID_Paddle,
	
	NDSInputID_InputCount
};

typedef union
{
	uint64_t value;
	
	struct
	{
		uint16_t gbaKeys;
		uint16_t ndsKeysExt;
		uint16_t easyPianoKeys;
		uint8_t guitarGripKeys;
		uint8_t miscKeys;
	};
	
	struct
	{
#ifndef MSB_FIRST
		uint8_t A:1;
		uint8_t B:1;
		uint8_t Select:1;
		uint8_t Start:1;
		uint8_t Right:1;
		uint8_t Left:1;
		uint8_t Up:1;
		uint8_t Down:1;
		
		uint8_t R:1;
		uint8_t L:1;
		uint8_t :6;
		
		uint8_t X:1;
		uint8_t Y:1;
		uint8_t :1;
		uint8_t Debug:1;
		uint8_t :2;
		uint8_t Touch:1;
		uint8_t Lid:1;
		
		uint8_t :8;
		
		uint8_t PianoC:1;
		uint8_t PianoCSharp:1;
		uint8_t PianoD:1;
		uint8_t PianoDSharp:1;
		uint8_t PianoE:1;
		uint8_t PianoF:1;
		uint8_t PianoFSharp:1;
		uint8_t PianoG:1;
		
		uint8_t PianoGSharp:1;
		uint8_t PianoA:1;
		uint8_t PianoASharp:1;
		uint8_t :2;
		uint8_t PianoB:1;
		uint8_t PianoHighC:1;
		uint8_t :1;
		
		uint8_t :3;
		uint8_t GuitarGripBlue:1;
		uint8_t GuitarGripYellow:1;
		uint8_t GuitarGripRed:1;
		uint8_t GuitarGripGreen:1;
		uint8_t :1;
		
		uint8_t Paddle:1;
		uint8_t Microphone:1;
		uint8_t Reset:1;
		uint8_t :5;
#else
		uint8_t Down:1;
		uint8_t Up:1;
		uint8_t Left:1;
		uint8_t Right:1;
		uint8_t Start:1;
		uint8_t Select:1;
		uint8_t B:1;
		uint8_t A:1;
		
		uint8_t :6
		uint8_t L:1;
		uint8_t R:1;
		
		uint8_t Lid:1;
		uint8_t Touch:1;
		uint8_t :2;
		uint8_t Debug:1;
		uint8_t :1;
		uint8_t Y:1;
		uint8_t X:1;
		
		uint8_t :8;
		
		uint8_t PianoG:1;
		uint8_t PianoFSharp:1;
		uint8_t PianoF:1;
		uint8_t PianoE:1;
		uint8_t PianoDSharp:1;
		uint8_t PianoD:1;
		uint8_t PianoCSharp:1;
		uint8_t PianoC:1;
		
		uint8_t :1;
		uint8_t PianoHighC:1;
		uint8_t PianoB:1;
		uint8_t :2;
		uint8_t PianoASharp:1;
		uint8_t PianoA:1;
		uint8_t PianoGSharp:1;
		
		uint8_t :1;
		uint8_t GuitarGripGreen:1;
		uint8_t GuitarGripRed:1;
		uint8_t GuitarGripYellow:1;
		uint8_t GuitarGripBlue:1;
		uint8_t :3;
		
		uint8_t :5;
		uint8_t Reset:1;
		uint8_t Microphone:1;
		uint8_t Paddle:1;
#endif
	};
} NDSInputState; // Each bit represents the Pressed/Released state of a single input. Pressed=0, Released=1

struct ClientExecutionControlSettings
{
	CPUEmulationEngineID cpuEngineID;
	uint8_t JITMaxBlockSize;
	NDS_SLOT1_TYPE slot1DeviceType;
	std::string filePathARM9BIOS;
	std::string filePathARM7BIOS;
	std::string filePathFirmware;
	std::string filePathSlot1R4;
	
	std::string cpuEmulationEngineName;
	std::string slot1DeviceName;
	
	bool enableAdvancedBusLevelTiming;
	bool enableRigorous3DRenderingTiming;
	bool enableGameSpecificHacks;
	bool enableExternalBIOS;
	bool enableBIOSInterrupts;
	bool enableBIOSPatchDelayLoop;
	bool enableExternalFirmware;
	bool enableFirmwareBoot;
	bool enableDebugConsole;
	bool enableEnsataEmulation;
	
	bool enableCheats;
	
	bool enableExecutionSpeedLimiter;
	double executionSpeed;
	
	bool enableFrameSkip;
	uint64_t frameJumpTarget;
	
	ExecutionBehavior execBehavior;
	FrameJumpBehavior jumpBehavior;
};

struct NDSFrameInfo
{
	std::string cpuEmulationEngineName;
	std::string slot1DeviceName;
	std::string rtcString;
	
	uint64_t frameIndex;
	uint32_t render3DFPS;
	uint32_t lagFrameCount;
	uint32_t cpuLoadAvgARM9;
	uint32_t cpuLoadAvgARM7;
	
	NDSInputState inputStatesPending;
	uint8_t touchLocXPending;
	uint8_t touchLocYPending;
	
	NDSInputState inputStatesApplied;
	uint8_t touchLocXApplied;
	uint8_t touchLocYApplied;
	
	void clear()
	{
		this->cpuEmulationEngineName	= std::string();
		this->slot1DeviceName			= std::string();
		this->rtcString					= std::string();
		this->frameIndex				= 0;
		this->render3DFPS				= 0;
		this->lagFrameCount				= 0;
		this->cpuLoadAvgARM9			= 0;
		this->cpuLoadAvgARM7			= 0;
		
		this->inputStatesPending.value	= INPUT_STATES_CLEAR_VALUE;
		this->touchLocXPending			= 0;
		this->touchLocYPending			= 0;
		
		this->inputStatesApplied.value	= INPUT_STATES_CLEAR_VALUE;
		this->touchLocXApplied			= 0;
		this->touchLocYApplied			= 0;
	}
	
	void copyFrom(const NDSFrameInfo &fromObject)
	{
		this->cpuEmulationEngineName	= fromObject.cpuEmulationEngineName;
		this->slot1DeviceName			= fromObject.slot1DeviceName;
		this->rtcString					= fromObject.rtcString;
		this->frameIndex				= fromObject.frameIndex;
		this->render3DFPS				= fromObject.render3DFPS;
		this->lagFrameCount				= fromObject.lagFrameCount;
		this->cpuLoadAvgARM9			= fromObject.cpuLoadAvgARM9;
		this->cpuLoadAvgARM7			= fromObject.cpuLoadAvgARM7;
		
		this->inputStatesPending		= fromObject.inputStatesPending;
		this->touchLocXPending			= fromObject.touchLocXPending;
		this->touchLocYPending			= fromObject.touchLocYPending;
		
		this->inputStatesApplied		= fromObject.inputStatesApplied;
		this->touchLocXApplied			= fromObject.touchLocXApplied;
		this->touchLocYApplied			= fromObject.touchLocYApplied;
	}
};

class ClientExecutionControl
{
private:
	ClientExecutionControlSettings _settingsPending;
	ClientExecutionControlSettings _settingsApplied;
	
	NDSFrameInfo _ndsFrameInfo;
	
	bool _newSettingsPendingOnReset;
	bool _newSettingsPendingOnExecutionLoopStart;
	bool _newSettingsPendingOnNDSExec;
	
	bool _needResetFramesToSkip;
	
	double _frameTime;
	uint8_t _framesToSkip;
	ExecutionBehavior _prevExecBehavior;
	
	std::string _cpuEmulationEngineNameOut;
	std::string _slot1DeviceNameOut;
	std::string _rtcStringOut;
	
	pthread_mutex_t _mutexSettingsPendingOnReset;
	pthread_mutex_t _mutexSettingsPendingOnExecutionLoopStart;
	pthread_mutex_t _mutexSettingsPendingOnNDSExec;
	pthread_mutex_t _mutexOutputPostNDSExec;
	
public:
	ClientExecutionControl();
	~ClientExecutionControl();
	
	CPUEmulationEngineID GetCPUEmulationEngineID();
	const char* GetCPUEmulationEngineName();
	void SetCPUEmulationEngineByID(CPUEmulationEngineID engineID);
	
	uint8_t GetJITMaxBlockSize();
	void SetJITMaxBlockSize(uint8_t blockSize);
	
	NDS_SLOT1_TYPE GetSlot1DeviceType();
	const char* GetSlot1DeviceName();
	void SetSlot1DeviceByType(NDS_SLOT1_TYPE type);
	
	const char* GetARM9ImagePath();
	void SetARM9ImagePath(const char *filePath);
	
	const char* GetARM7ImagePath();
	void SetARM7ImagePath(const char *filePath);
	
	const char* GetFirmwareImagePath();
	void SetFirmwareImagePath(const char *filePath);
	
	const char* GetSlot1R4Path();
	void SetSlot1R4Path(const char *filePath);
	
	bool GetEnableAdvancedBusLevelTiming();
	void SetEnableAdvancedBusLevelTiming(bool enable);
	
	bool GetEnableRigorous3DRenderingTiming();
	void SetEnableRigorous3DRenderingTiming(bool enable);
	
	bool GetEnableGameSpecificHacks();
	void SetEnableGameSpecificHacks(bool enable);
	
	bool GetEnableExternalBIOS();
	void SetEnableExternalBIOS(bool enable);
	
	bool GetEnableBIOSInterrupts();
	void SetEnableBIOSInterrupts(bool enable);
	
	bool GetEnableBIOSPatchDelayLoop();
	void SetEnableBIOSPatchDelayLoop(bool enable);
	
	bool GetEnableExternalFirmware();
	void SetEnableExternalFirmware(bool enable);
	
	bool GetEnableFirmwareBoot();
	void SetEnableFirmwareBoot(bool enable);
	
	bool GetEnableDebugConsole();
	void SetEnableDebugConsole(bool enable);
	
	bool GetEnableEnsataEmulation();
	void SetEnableEnsataEmulation(bool enable);
	
	bool GetEnableCheats();
	void SetEnableCheats(bool enable);
	
	bool GetEnableSpeedLimiter();
	void SetEnableSpeedLimiter(bool enable);
	
	double GetExecutionSpeed();
	void SetExecutionSpeed(double speedScalar);
	
	bool GetEnableFrameSkip();
	bool GetEnableFrameSkipApplied();
	void SetEnableFrameSkip(bool enable);
	
	uint8_t GetFramesToSkip();
	void SetFramesToSkip(uint8_t numFrames);
	void ResetFramesToSkip();
	
	uint64_t GetFrameJumpTarget();
	uint64_t GetFrameJumpTargetApplied();
	void SetFrameJumpTarget(uint64_t newJumpTarget);
	
	ExecutionBehavior GetPreviousExecutionBehavior();
	ExecutionBehavior GetExecutionBehavior();
	ExecutionBehavior GetExecutionBehaviorApplied();
	void SetExecutionBehavior(ExecutionBehavior newBehavior);
	
	FrameJumpBehavior GetFrameJumpBehavior();
	void SetFrameJumpBehavior(FrameJumpBehavior newBehavior);
	
	void ApplySettingsOnReset();
	void ApplySettingsOnExecutionLoopStart();
	void ApplySettingsOnNDSExec();
	
	void FetchOutputPostNDSExec();
	const NDSFrameInfo& GetNDSFrameInfo();
	
	double GetFrameTime();
	uint8_t CalculateFrameSkip(double startAbsoluteTime, double frameAbsoluteTime);
	
	virtual double GetCurrentAbsoluteTime();
	virtual double CalculateFrameAbsoluteTime(double frameTimeScalar);
	virtual void WaitUntilAbsoluteTime(double deadlineAbsoluteTime);
};

#endif // _CLIENT_EXECUTION_CONTROL_H_
