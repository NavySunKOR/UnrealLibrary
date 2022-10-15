// Fill out your copyright notice in the Description page of Project Settings.


#include "PerformanceLibrary.h"

PerformanceTimer::PerformanceTimer(bool p_IsAutoStart)
{
	m_IsAutostart = p_IsAutoStart;
	if(m_IsAutostart)
	{
		StartTimer();
	}
}

PerformanceTimer::~PerformanceTimer()
{
	if (m_IsAutostart)
	{
		EndTimer();
	}
}


void PerformanceTimer::StartTimer()
{

	m_StartTimer = high_resolution_clock::now();
}
void PerformanceTimer::EndTimer()
{

	time_point<high_resolution_clock> endTimepoint = high_resolution_clock::now();
	long long start = time_point_cast<microseconds>(m_StartTimer).time_since_epoch().count();
	long long end = time_point_cast<microseconds>(endTimepoint).time_since_epoch().count();

	long long duration = end - start;
	double ms = duration * 0.001;

	UE_LOG(LogTemp, Warning, TEXT("Time to take : %f ms"), ms)
	UE_LOG(LogTemp, Warning, TEXT("Time to take : %d us"), duration)
}