// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Utility.generated.h"

USTRUCT()
struct FDoOnce
{
	GENERATED_USTRUCT_BODY()

		bool m_bisDoOnce;

	FORCEINLINE FDoOnce();
	explicit FORCEINLINE FDoOnce(bool bisStartClosed);

	FORCEINLINE void Reset() { m_bisDoOnce = true; }

	FORCEINLINE bool Execute()
	{
		if (m_bisDoOnce)
		{
			m_bisDoOnce = false;
			return true;
		}

		return false;
	}
};

USTRUCT()
struct FGate
{
	GENERATED_USTRUCT_BODY()
public:
	FORCEINLINE FGate();
	explicit FORCEINLINE FGate(bool bisStartClosed);

	FORCEINLINE void Open() { m_bisGateOpen = true; }
	FORCEINLINE void Closed() { m_bisGateOpen = false; }
	FORCEINLINE void Togle() { m_bisGateOpen = !m_bisGateOpen; }
	FORCEINLINE bool Execute() { return m_bisGateOpen; }
private:
	bool m_bisGateOpen;
};

USTRUCT()
struct FFlipFlop
{
	GENERATED_USTRUCT_BODY()
public:
	FORCEINLINE FFlipFlop();
	explicit FORCEINLINE FFlipFlop(bool bisStartClosed);
	FORCEINLINE bool IsA() { return m_bisFilpFlop; }
	FORCEINLINE bool Execute()
	{
		bool val = m_bisFilpFlop;
		m_bisFilpFlop = !m_bisFilpFlop;

		return val;
	}
private:
	bool m_bisFilpFlop;
};

FORCEINLINE FDoOnce::FDoOnce() : m_bisDoOnce(false)
{}

FORCEINLINE FDoOnce::FDoOnce(bool bisStartClosed) : m_bisDoOnce(!bisStartClosed)
{}

FORCEINLINE FGate::FGate() : m_bisGateOpen(false)
{}

FORCEINLINE FGate::FGate(bool bisStartClosed) : m_bisGateOpen(!bisStartClosed)
{}

FORCEINLINE FFlipFlop::FFlipFlop() : m_bisFilpFlop(false)
{}

FORCEINLINE FFlipFlop::FFlipFlop(bool bisStartClosed) : m_bisFilpFlop(!bisStartClosed)
{}

