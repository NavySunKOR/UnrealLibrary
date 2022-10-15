// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include <chrono>

/**
 * 
 */
using namespace std::chrono;
class PerformanceTimer
{
protected:
	time_point<high_resolution_clock> m_StartTimer;
	bool m_IsAutostart = false;
public:
	PerformanceTimer(bool p_AutoStart);
	~PerformanceTimer();
	void StartTimer();
	void EndTimer();
};
