/*
 * Copyright (C) 2012 by Ian Wraith
 * Copyright (C) 2015,2016 by Jonathan Naylor G4KLX
 *
 *   Ported to OpenGD77 by Roger Clark VK3KYY / G4KYF
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */
#include "hotspot/BPTC19696.h"
#include "hotspot/dmrDefines.h"
#include "hotspot/dmrUtils.h"
#include "hotspot/DMRFullLC.h"
#include "hotspot/RS129.h"


bool DMRFullLC_decode(const unsigned char *data, unsigned char type, DMRLC_T *lc)
{
	//unsigned char lcData[12U];

	BPTC19696_init();
	BPTC19696_decode(data, lc->rawData);

	switch (type)
	{
		case DT_VOICE_LC_HEADER:
			lc->rawData[9U]  ^= VOICE_LC_HEADER_CRC_MASK[0U];
			lc->rawData[10U] ^= VOICE_LC_HEADER_CRC_MASK[1U];
			lc->rawData[11U] ^= VOICE_LC_HEADER_CRC_MASK[2U];
			break;

		case DT_TERMINATOR_WITH_LC:
			lc->rawData[9U]  ^= TERMINATOR_WITH_LC_CRC_MASK[0U];
			lc->rawData[10U] ^= TERMINATOR_WITH_LC_CRC_MASK[1U];
			lc->rawData[11U] ^= TERMINATOR_WITH_LC_CRC_MASK[2U];
			break;

		default:
			return false;// unsupported frame type
	}

	if (!RS129_check(lc->rawData))
	{
		return false;
	}

	DMRLCfromBytes(lc->rawData, lc);//

	return true;
}

bool DMRFullLC_encode(DMRLC_T *lc, unsigned char *data, unsigned char type)
{
	unsigned char lcData[12U];

	DMRLC_getDataFromBytes(lcData, lc);

	unsigned char parity[4U];
	RS129_encode(lcData, 9U, parity);

	switch (type)
	{
		case DT_VOICE_LC_HEADER:
			lcData[9U]  = parity[2U] ^ VOICE_LC_HEADER_CRC_MASK[0U];
			lcData[10U] = parity[1U] ^ VOICE_LC_HEADER_CRC_MASK[1U];
			lcData[11U] = parity[0U] ^ VOICE_LC_HEADER_CRC_MASK[2U];
			break;

		case DT_TERMINATOR_WITH_LC:
			lcData[9U]  = parity[2U] ^ TERMINATOR_WITH_LC_CRC_MASK[0U];
			lcData[10U] = parity[1U] ^ TERMINATOR_WITH_LC_CRC_MASK[1U];
			lcData[11U] = parity[0U] ^ TERMINATOR_WITH_LC_CRC_MASK[2U];
			break;

		default:
			return false;
	}

	BPTC19696_init();
	BPTC19696_encode(lcData,data);

	return true;
}
