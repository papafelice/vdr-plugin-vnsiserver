/*
 *      vdr-plugin-vnsi - KODI server plugin for VDR
 *
 *      Copyright (C) 2005-2012 Team XBMC
 *      Copyright (C) 2015 Team KODI
 *
 *      http://kodi.tv
 *
 *  This Program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2, or (at your option)
 *  any later version.
 *
 *  This Program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with KODI; see the file COPYING.  If not, see
 *  <http://www.gnu.org/licenses/>.
 *
 */

#pragma once

#include <list>
#include "parser.h"

struct sStreamPacket;
class cTSStream;
class cChannel;
class cPatPmtParser;
class cVideoBuffer;

struct sStreamInfo
{
  int pID;
  eStreamType type;
  eStreamContent content;
  char language[MAXLANGCODE2];
  int subtitlingType;
  int compositionPageId;
  int ancillaryPageId;
  bool handleRDS;
  void SetLanguage(const char* lang)
  {
    language[0] = lang[0];
    language[1] = lang[1];
    language[2] = lang[2];
    language[3] = 0;
  }
};

class cVNSIDemuxer
{
public:
  cVNSIDemuxer(bool bAllowRDS);
  virtual ~cVNSIDemuxer();
  int Read(sStreamPacket *packet, sStreamPacket *packet_side_data);
  cTSStream *GetFirstStream();
  cTSStream *GetNextStream();
  void Open(const cChannel &channel, cVideoBuffer *videoBuffer);
  void Close();
  bool SeekTime(int64_t time);
  uint32_t GetSerial() { return m_MuxPacketSerial; }
  void SetSerial(uint32_t serial) { m_MuxPacketSerial = serial; }
  void BufferStatus(bool &timeshift, uint32_t &start, uint32_t &end);
  uint16_t GetError();

protected:
  bool EnsureParsers();
  void ResetParsers();
  void SetChannelStreams(const cChannel *channel);
  void SetChannelPids(cChannel *channel, cPatPmtParser *patPmtParser);
  cTSStream *FindStream(int Pid);
  void AddStreamInfo(sStreamInfo &stream);
  bool GetTimeAtPos(off_t *pos, int64_t *time);
  std::list<cTSStream*> m_Streams;
  std::list<cTSStream*>::iterator m_StreamsIterator;
  std::list<sStreamInfo> m_StreamInfos;
  cChannel m_CurrentChannel;
  cPatPmtParser m_PatPmtParser;
  bool m_WaitIFrame;
  cVideoBuffer *m_VideoBuffer;
  cMutex m_Mutex;
  uint32_t m_MuxPacketSerial;
  sPtsWrap m_PtsWrap;
  uint16_t m_Error;
  bool m_SetRefTime;
  time_t m_refTime, m_endTime, m_wrapTime;
  bool m_bAllowRDS;
};
