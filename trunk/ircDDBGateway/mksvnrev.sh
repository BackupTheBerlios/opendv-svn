#! /bin/sh

SVNVERSION=/usr/bin/svnversion
SVN_H=Common/SVN.h

# If SVN is installed then assume that it's in use
if [ -x $SVNVERSION ]; then
	echo Creating new SVN.h

	SVNREV=$($SVNVERSION -n .)

	echo "/*" > $SVN_H
	echo " *   Copyright (C) 2012 by Jonathan Naylor G4KLX" >> $SVN_H
	echo " *" >> $SVN_H
	echo " *   This program is free software; you can redistribute it and/or modify" >> $SVN_H
	echo " *   it under the terms of the GNU General Public License as published by" >> $SVN_H
	echo " *   the Free Software Foundation; either version 2 of the License, or" >> $SVN_H
	echo " *   (at your option) any later version." >> $SVN_H
	echo " *" >> $SVN_H
	echo " *   This program is distributed in the hope that it will be useful," >> $SVN_H
	echo " *   but WITHOUT ANY WARRANTY; without even the implied warranty of" >> $SVN_H
	echo " *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the" >> $SVN_H
	echo " *   GNU General Public License for more details." >> $SVN_H
	echo " *" >> $SVN_H
	echo " *   You should have received a copy of the GNU General Public License" >> $SVN_H
	echo " *   along with this program; if not, write to the Free Software" >> $SVN_H
	echo " *   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA." >> $SVN_H
	echo " */" >> $SVN_H
	echo >> $SVN_H
	echo "#ifndef	SVN_H" >> $SVN_H
	echo "#define	SVN_H" >> $SVN_H
	echo >> $SVN_H
	echo "#include <wx/wx.h>" >> $SVN_H
	echo >> $SVN_H
	echo "const wxString SVNREV = wxT(\"SVN revision: $SVNREV\");" >> $SVN_H
	echo >> $SVN_H
	echo "#endif" >> $SVN_H
fi
