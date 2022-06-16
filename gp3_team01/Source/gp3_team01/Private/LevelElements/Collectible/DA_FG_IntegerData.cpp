// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelElements/Collectible/DA_FG_IntegerData.h"

void UDA_FG_IntegerData::Reset()
{
	this->Data = 0;
}

int32 UDA_FG_IntegerData::Get() const
{
	return this->Data;
}

void UDA_FG_IntegerData::Set(int32 Value)
{
	this->Data = Value;
}

void UDA_FG_IntegerData::Increment(int32 Value)
{
	this->Data += Value;
}

void UDA_FG_IntegerData::Decrement(int32 Value)
{
	this->Data -= Value;
}
