/*
   mkvmerge -- utility for splicing together matroska files
   from component media subtypes

   Distributed under the GPL
   see the file COPYING for details
   or visit http://www.gnu.org/copyleft/gpl.html

   $Id$

   class definition for the VC1 video output module

   Written by Moritz Bunkus <moritz@bunkus.org>.
*/

#ifndef __P_VC1_H
#define __P_VC1_H

#include "os.h"

#include "common.h"
#include "pr_generic.h"
#include "vc1_common.h"


class vc1_video_packetizer_c: public generic_packetizer_c {
protected:
  vc1::es_parser_c m_parser;
  vc1::sequence_header_t m_seqhdr;
  memory_cptr m_raw_headers;

  int64_t m_previous_timecode;

public:
  vc1_video_packetizer_c(generic_reader_c *n_reader, track_info_c &n_ti);

  virtual int process(packet_cptr packet);
  virtual void set_headers();

  virtual void flush();

  virtual const char *get_format_name() {
    return "video";
  };

  virtual connection_result_e can_connect_to(generic_packetizer_c *src, string &error_message);

protected:
  virtual void flush_frames();
  virtual void headers_found();
};

#endif // __P_VC1_H