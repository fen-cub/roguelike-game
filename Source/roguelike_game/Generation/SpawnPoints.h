// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

using RoomPoint = TPair<int, int>;

TArray<TArray<RoomPoint>>
	DefaultTemplatesSet({{RoomPoint(4, 5), RoomPoint(13, 6), RoomPoint(15, 10), RoomPoint(2, 16)},
		{RoomPoint(7, 7), RoomPoint(11, 3), RoomPoint(14, 9), RoomPoint(14, 7)},
		{RoomPoint(11, 16), RoomPoint(13, 9), RoomPoint(3, 10), RoomPoint(7, 3)},
		{RoomPoint(14, 11), RoomPoint(2, 3), RoomPoint(8, 16), RoomPoint(16, 9)},
		{RoomPoint(7, 16), RoomPoint(3, 6), RoomPoint(10, 3), RoomPoint(13, 8)},
		{RoomPoint(3, 8), RoomPoint(10, 4), RoomPoint(15, 8), RoomPoint(11, 13)},
		{RoomPoint(4, 15), RoomPoint(10, 12), RoomPoint(7, 4), RoomPoint(13, 3)},
		{RoomPoint(10, 3), RoomPoint(3, 9), RoomPoint(15, 10), RoomPoint(11, 13)},
		{RoomPoint(3, 11), RoomPoint(6, 3), RoomPoint(12, 8), RoomPoint(12, 16)},
		{RoomPoint(4, 12), RoomPoint(8, 7), RoomPoint(10, 16), RoomPoint(14, 5)},
		{RoomPoint(15, 7), RoomPoint(4, 9), RoomPoint(13, 12), RoomPoint(10, 15)},
		{RoomPoint(9, 5), RoomPoint(5, 8), RoomPoint(15, 10), RoomPoint(14, 15)},
		{RoomPoint(11, 3), RoomPoint(6, 5), RoomPoint(15, 9), RoomPoint(11, 14)},
		{RoomPoint(6, 6), RoomPoint(8, 3), RoomPoint(14, 16), RoomPoint(15, 11)},
		{RoomPoint(12, 10), RoomPoint(3, 9), RoomPoint(8, 16), RoomPoint(12, 3)},
		{RoomPoint(11, 16), RoomPoint(5, 7), RoomPoint(12, 3), RoomPoint(14, 8)},
		{RoomPoint(4, 16), RoomPoint(14, 3), RoomPoint(15, 11), RoomPoint(7, 4)},
		{RoomPoint(15, 3), RoomPoint(4, 12), RoomPoint(14, 12), RoomPoint(11, 16)},
		{RoomPoint(3, 9), RoomPoint(6, 15), RoomPoint(9, 3), RoomPoint(14, 11)},
		{RoomPoint(4, 3), RoomPoint(13, 12), RoomPoint(4, 10), RoomPoint(9, 16)},
		{RoomPoint(11, 17), RoomPoint(16, 10), RoomPoint(16, 6), RoomPoint(6, 4)},
		{RoomPoint(8, 3), RoomPoint(3, 11), RoomPoint(7, 16), RoomPoint(13, 12)},
		{RoomPoint(14, 4), RoomPoint(3, 12), RoomPoint(9, 16), RoomPoint(16, 6)},
		{RoomPoint(3, 16), RoomPoint(5, 7), RoomPoint(12, 13), RoomPoint(14, 3)},
		{RoomPoint(3, 10), RoomPoint(15, 4), RoomPoint(16, 13), RoomPoint(7, 14)},
		{RoomPoint(14, 6), RoomPoint(4, 12), RoomPoint(15, 15), RoomPoint(4, 5)},
		{RoomPoint(14, 15), RoomPoint(8, 3), RoomPoint(4, 11), RoomPoint(15, 5)},
		{RoomPoint(3, 15), RoomPoint(11, 16), RoomPoint(14, 10), RoomPoint(8, 4)},
		{RoomPoint(4, 3), RoomPoint(16, 3), RoomPoint(12, 16), RoomPoint(5, 11)},
		{RoomPoint(7, 16), RoomPoint(6, 8), RoomPoint(11, 3), RoomPoint(14, 10)}});

TArray<TArray<RoomPoint>>
HorizontalAdditionsTemplatesSet({{RoomPoint(16, 1), RoomPoint(11, 3)},
	{RoomPoint(4, 2), RoomPoint(11, 4)},
	{RoomPoint(3, 3), RoomPoint(9, 5)},
	{RoomPoint(2, 4), RoomPoint(10, 1)},
	{RoomPoint(4, 1), RoomPoint(6, 4)},
	{RoomPoint(3, 2), RoomPoint(13, 4)},
	{RoomPoint(5, 3), RoomPoint(15, 2)},
	{RoomPoint(2, 4), RoomPoint(10, 5)},
	{RoomPoint(16, 1), RoomPoint(13, 4)},
	{RoomPoint(14, 1), RoomPoint(5, 3)},
	{RoomPoint(5, 1), RoomPoint(12, 4)},
	{RoomPoint(11, 2), RoomPoint(15, 5)},
	{RoomPoint(4, 1), RoomPoint(13, 3)},
	{RoomPoint(15, 2), RoomPoint(7, 4)},
	{RoomPoint(5, 2), RoomPoint(14, 4)},
	{RoomPoint(5, 4), RoomPoint(12, 2)},
	{RoomPoint(14, 1), RoomPoint(7, 4)},
	{RoomPoint(8, 1), RoomPoint(12, 4)},
	{RoomPoint(5, 1), RoomPoint(11, 4)},
	{RoomPoint(5, 1), RoomPoint(12, 3)}});

TArray<TArray<RoomPoint>>
	VerticalAdditionsTemplatesSet({{RoomPoint(2, 6), RoomPoint(3, 14)},
		{RoomPoint(2, 15), RoomPoint(4, 4)},
		{RoomPoint(5, 4), RoomPoint(2, 17)},
		{RoomPoint(0, 3), RoomPoint(6, 10)},
		{RoomPoint(4, 7), RoomPoint(1, 15)},
		{RoomPoint(1, 3), RoomPoint(4, 8)},
		{RoomPoint(1, 2), RoomPoint(1, 12)},
		{RoomPoint(2, 5), RoomPoint(4, 10)},
		{RoomPoint(5, 17), RoomPoint(1, 5)},
		{RoomPoint(2, 6), RoomPoint(4, 16)},
		{RoomPoint(3, 15), RoomPoint(4, 7)},
		{RoomPoint(1, 13), RoomPoint(3, 4)},
		{RoomPoint(4, 7), RoomPoint(2, 16)},
		{RoomPoint(2, 3), RoomPoint(4, 6)},
		{RoomPoint(1, 16), RoomPoint(3, 7)},
		{RoomPoint(2, 4), RoomPoint(4, 16)},
		{RoomPoint(1, 6), RoomPoint(4, 15)},
		{RoomPoint(4, 4), RoomPoint(2, 10)},
		{RoomPoint(2, 8), RoomPoint(3, 15)},
		{RoomPoint(3, 4), RoomPoint(0, 12)}});

TArray<TArray<RoomPoint>>
	HorizontalCorridorsTemplatesSet({{}});

TArray<TArray<RoomPoint>>
	VerticalCorridorsTemplatesSet({{}});