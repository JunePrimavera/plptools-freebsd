/*-*-c++-*-
 * $Id$
 *
 * This file is part of plptools.
 *
 *  Copyright (C) 1999-2001 Fritz Elfert <felfert@to.com>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */
#include "plpdirent.h"
#include <stream.h>
#include <iomanip>

using namespace std;

PlpUID::PlpUID() {
    memset(uid, 0, sizeof(uid));
}

PlpUID::PlpUID(const u_int32_t u1, const u_int32_t u2, const u_int32_t u3) {
    uid[0] = u1; uid[1] = u2; uid[2] = u3;
}

u_int32_t PlpUID::
operator[](int idx) {
    assert ((idx > -1) && (idx < 3));
    return uid[idx];
}

PlpDirent::PlpDirent()
    : size(0), attr(0), name(""), time(0L), attrstr("") {
}

PlpDirent::PlpDirent(const PlpDirent &e) {
    size    = e.size;
    attr    = e.attr;
    time    = e.time;
    UID     = e.UID;
    name    = e.name;
    attrstr = e.attrstr;
}

PlpDirent::PlpDirent(const u_int32_t _size, const u_int32_t _attr,
		     const u_int32_t tHi, const u_int32_t tLo,
		     const char * const _name) {
    size = _size;
    attr = _attr;
    time = PsiTime(tHi, tLo);
    UID  = PlpUID();
    name = _name;
    attrstr = "";
}

u_int32_t PlpDirent::
getSize() {
    return size;
}

u_int32_t PlpDirent::
getAttr() {
    return attr;
}

u_int32_t PlpDirent::
getUID(int uididx) {
    if ((uididx >= 0) && (uididx < 4))
	return UID[uididx];
    return 0;
}

PlpUID &PlpDirent::
getUID() {
    return UID;
}

const char *PlpDirent::
getName() {
    return name.c_str();
}

PsiTime PlpDirent::
getPsiTime() {
    return time;
}

void PlpDirent::
setName(const char *str) {
    name = str;
}

PlpDirent &PlpDirent::
operator=(const PlpDirent &e) {
    size    = e.size;
    attr    = e.attr;
    time    = e.time;
    UID     = e.UID;
    name    = e.name;
    attrstr = e.attrstr;
    return *this;
}

ostream &
operator<<(ostream &o, const PlpDirent &e) {
    ostream::fmtflags old = o.flags();

    o << e.attrstr << " " << dec << setw(10)
      << setfill(' ') << e.size << " " << e.time
      << " " << e.name;
    o.flags(old);
    return o;
}

PlpDrive::PlpDrive() {
}

PlpDrive::PlpDrive(const PlpDrive &other) {
}

void PlpDrive::
setMediaType(u_int32_t type) {
    mediatype = type;
}

void PlpDrive::
setDriveAttribute(u_int32_t attr) {
    driveattr = attr;
}

void PlpDrive::
setMediaAttribute(u_int32_t attr) {
    mediaattr = attr;
}

void PlpDrive::
setUID(u_int32_t attr) {
    uid = attr;
}

void PlpDrive::
setSize(u_int32_t sizeLo, u_int32_t sizeHi) {
    size = ((unsigned long long)sizeHi << 32) + sizeLo;
}

void PlpDrive::
setSpace(u_int32_t spaceLo, u_int32_t spaceHi) {
    space = ((unsigned long long)spaceHi << 32) + spaceLo;
}

void PlpDrive::
setName(char drive, const char * const volname) {
    drivechar = drive;
    name = "";
    name += volname;
}

u_int32_t PlpDrive::
getMediaType() {
    return mediatype;
}

static const char * const media_types[] = {
    N_("Not present"),
    N_("Unknown"),
    N_("Floppy"),
    N_("Disk"),
    N_("CD-ROM"),
    N_("RAM"),
    N_("Flash Disk"),
    N_("ROM"),
    N_("Remote"),
};

void PlpDrive::
getMediaType(string &ret) {
    ret = media_types[mediatype];
}

u_int32_t PlpDrive::
getDriveAttribute() {
    return driveattr;
}

static void
appendWithDelim(string &s1, const char * const s2) {
    if (!s1.empty())
	s1 += ',';
    s1 += s2;
}

void PlpDrive::
getDriveAttribute(string &ret) {
    ret = "";
    if (driveattr & 1)
	appendWithDelim(ret, _("local"));
    if (driveattr & 2)
	appendWithDelim(ret, _("ROM"));
    if (driveattr & 4)
	appendWithDelim(ret, _("redirected"));
    if (driveattr & 8)
	appendWithDelim(ret, _("substituted"));
    if (driveattr & 16)
	appendWithDelim(ret, _("internal"));
    if (driveattr & 32)
	appendWithDelim(ret, _("removable"));
}

u_int32_t PlpDrive::
getMediaAttribute() {
    return mediaattr;
}

void PlpDrive::
getMediaAttribute(string &ret) {
    ret = "";

    if (mediaattr & 1)
	appendWithDelim(ret, _("variable size"));
    if (mediaattr & 2)
	appendWithDelim(ret, _("dual density"));
    if (mediaattr & 4)
	appendWithDelim(ret, _("formattable"));
    if (mediaattr & 8)
	appendWithDelim(ret, _("write protected"));
}

u_int32_t PlpDrive::
getUID() {
    return uid;
}

u_int64_t PlpDrive::
getSize() {
    return size;
}

u_int64_t PlpDrive::
getSpace() {
    return space;
}

string PlpDrive::
getName() {
    return name;
}

char PlpDrive::
getDrivechar() {
    return drivechar;
}

/*
 * Local variables:
 * c-basic-offset: 4
 * End:
 */
