/*
 * $Id: _temperature_common.c,v 1.3 2010/06/14 19:10:52 clivewebster Exp $
 *
 * Revision History
 * ================
 * $Log: _temperature_common.c,v $
 * Revision 1.3  2010/06/14 19:10:52  clivewebster
 * Add copyright license info
 *
 * Revision 1.2  2009/11/02 18:43:05  clivewebster
 * Added revision log
 *
 * ===========
 *
 * Copyright (C) 2010 Clive Webster (webbot@webbot.org.uk)
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * _temperature_common.c
 *
 *  Created on: 25-Sep-2009
 *      Author: Clive Webster
 */
#include "_temperature_common.h"
#include "../../core.h"

void _temperatureDumpTo(FILE* f, const TEMPERATURE* device){
	PRINTF(f,"(%d celsius)",device->celsius);
}
