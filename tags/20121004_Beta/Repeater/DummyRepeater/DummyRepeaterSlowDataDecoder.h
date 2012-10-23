/*
 *	Copyright (C) 2009,2010,2011 by Jonathan Naylor, G4KLX
 *
 *	This program is free software; you can redistribute it and/or modify
 *	it under the terms of the GNU General Public License as published by
 *	the Free Software Foundation; version 2 of the License.
 *
 *	This program is distributed in the hope that it will be useful,
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *	GNU General Public License for more details.
 */

#ifndef	DummyRepeaterSlowDataDecoder_H
#define	DummyRepeaterSlowDataDecoder_H

#include "DummyRepeaterMessageData.h"

#include <wx/wx.h>

enum SDD_STATE {
	SDD_FIRST,
	SDD_SECOND
};

class CDummyRepeaterSlowDataDecoder {
public:
	CDummyRepeaterSlowDataDecoder();
	~CDummyRepeaterSlowDataDecoder();

	void addData(const unsigned char* data);

	CDummyRepeaterMessageData* getMessageData();

	void sync();
	void reset();

private:
	unsigned char*             m_buffer;
	SDD_STATE                  m_state;
	unsigned char*             m_message;
	CDummyRepeaterMessageData* m_messageData;

	void processMessage();
};

#endif
