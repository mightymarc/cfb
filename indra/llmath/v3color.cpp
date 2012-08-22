/** 
 * @file v3color.cpp
 * @brief LLColor3 class implementation.
 *
 * $LicenseInfo:firstyear=2000&license=viewergpl$
 * 
 * Copyright (c) 2000-2009, Linden Research, Inc.
 * 
 * Second Life Viewer Source Code
 * The source code in this file ("Source Code") is provided by Linden Lab
 * to you under the terms of the GNU General Public License, version 2.0
 * ("GPL"), unless you have obtained a separate licensing agreement
 * ("Other License"), formally executed by you and Linden Lab.  Terms of
 * the GPL can be found in doc/GPL-license.txt in this distribution, or
 * online at http://secondlifegrid.net/programs/open_source/licensing/gplv2
 * 
 * There are special exceptions to the terms and conditions of the GPL as
 * it is applied to this Source Code. View the full text of the exception
 * in the file doc/FLOSS-exception.txt in this software distribution, or
 * online at
 * http://secondlifegrid.net/programs/open_source/licensing/flossexception
 * 
 * By copying, modifying or distributing this software, you acknowledge
 * that you have read and understood your obligations described above,
 * and agree to abide by those obligations.
 * 
 * ALL LINDEN LAB SOURCE CODE IS PROVIDED "AS IS." LINDEN LAB MAKES NO
 * WARRANTIES, EXPRESS, IMPLIED OR OTHERWISE, REGARDING ITS ACCURACY,
 * COMPLETENESS OR PERFORMANCE.
 * $/LicenseInfo$
 */

#include "linden_common.h"

#include "v3color.h"
#include "v4color.h"
#include "v4math.h"

LLColor3 LLColor3::white(1.0f, 1.0f, 1.0f);
LLColor3 LLColor3::black(0.0f, 0.0f, 0.0f);
LLColor3 LLColor3::grey (0.5f, 0.5f, 0.5f);

LLColor3::LLColor3(const LLColor4 &a)
{
	mV[0] = a.mV[0];
	mV[1] = a.mV[1];
	mV[2] = a.mV[2];
}

LLColor3::LLColor3(const LLVector4 &a)
{
	mV[0] = a.mV[0];
	mV[1] = a.mV[1];
	mV[2] = a.mV[2];
}

LLColor3::LLColor3(const LLSD &sd)
{
	setValue(sd);
}

const LLColor3& LLColor3::operator=(const LLColor4 &a) 
{
	mV[0] = a.mV[0];
	mV[1] = a.mV[1];
	mV[2] = a.mV[2];
	return (*this);
}

std::ostream& operator<<(std::ostream& s, const LLColor3 &a) 
{
	s << "{ " << a.mV[VX] << ", " << a.mV[VY] << ", " << a.mV[VZ] << " }";
	return s;
}

static F32 hueToRgb ( F32 val1In, F32 val2In, F32 valHUeIn )
{
	if ( valHUeIn < 0.0f ) valHUeIn += 1.0f;
	if ( valHUeIn > 1.0f ) valHUeIn -= 1.0f;
	if ( ( 6.0f * valHUeIn ) < 1.0f ) return ( val1In + ( val2In - val1In ) * 6.0f * valHUeIn );
	if ( ( 2.0f * valHUeIn ) < 1.0f ) return ( val2In );
	if ( ( 3.0f * valHUeIn ) < 2.0f ) return ( val1In + ( val2In - val1In ) * ( ( 2.0f / 3.0f ) - valHUeIn ) * 6.0f );
	return ( val1In );
}

void LLColor3::setHSL ( F32 hValIn, F32 sValIn, F32 lValIn)
{
	if ( sValIn < 0.00001f )
	{
		mV[VRED] = lValIn;
		mV[VGREEN] = lValIn;
		mV[VBLUE] = lValIn;
	}
	else
	{
		F32 interVal1;
		F32 interVal2;

		if ( lValIn < 0.5f )
			interVal2 = lValIn * ( 1.0f + sValIn );
		else
			interVal2 = ( lValIn + sValIn ) - ( sValIn * lValIn );

		interVal1 = 2.0f * lValIn - interVal2;

		mV[VRED] = hueToRgb ( interVal1, interVal2, hValIn + ( 1.f / 3.f ) );
		mV[VGREEN] = hueToRgb ( interVal1, interVal2, hValIn );
		mV[VBLUE] = hueToRgb ( interVal1, interVal2, hValIn - ( 1.f / 3.f ) );
	}
}

void LLColor3::calcHSL(F32* hue, F32* saturation, F32* luminance) const
{
	F32 var_R = mV[VRED];
	F32 var_G = mV[VGREEN];
	F32 var_B = mV[VBLUE];

	F32 var_Min = ( var_R < ( var_G < var_B ? var_G : var_B ) ? var_R : ( var_G < var_B ? var_G : var_B ) );
	F32 var_Max = ( var_R > ( var_G > var_B ? var_G : var_B ) ? var_R : ( var_G > var_B ? var_G : var_B ) );

	F32 del_Max = var_Max - var_Min;

	F32 L = ( var_Max + var_Min ) / 2.0f;
	F32 H = 0.0f;
	F32 S = 0.0f;

	if ( del_Max == 0.0f )
	{
	   H = 0.0f;
	   S = 0.0f;
	}
	else
	{
		if ( L < 0.5 )
			S = del_Max / ( var_Max + var_Min );
		else
			S = del_Max / ( 2.0f - var_Max - var_Min );

		F32 del_R = ( ( ( var_Max - var_R ) / 6.0f ) + ( del_Max / 2.0f ) ) / del_Max;
		F32 del_G = ( ( ( var_Max - var_G ) / 6.0f ) + ( del_Max / 2.0f ) ) / del_Max;
		F32 del_B = ( ( ( var_Max - var_B ) / 6.0f ) + ( del_Max / 2.0f ) ) / del_Max;

		if ( var_R >= var_Max )
			H = del_B - del_G;
		else
		if ( var_G >= var_Max )
			H = ( 1.0f / 3.0f ) + del_R - del_B;
		else
		if ( var_B >= var_Max )
			H = ( 2.0f / 3.0f ) + del_G - del_R;

		if ( H < 0.0f ) H += 1.0f;
		if ( H > 1.0f ) H -= 1.0f;
	}

	if (hue) *hue = H;
	if (saturation) *saturation = S;
	if (luminance) *luminance = L;
}
