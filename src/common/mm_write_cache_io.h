/*
   mkvmerge -- utility for splicing together matroska files
   from component media subtypes

   Distributed under the GPL
   see the file COPYING for details
   or visit http://www.gnu.org/copyleft/gpl.html

   IO callback class definitions

   Written by Moritz Bunkus <moritz@bunkus.org>.
*/

#ifndef __MTX_COMMON_MM_WRITE_CACHE_IO_H
#define __MTX_COMMON_MM_WRITE_CACHE_IO_H

#include "common/common_pch.h"

#include "common/mm_io.h"

class MTX_DLL_API mm_write_cache_io_c: public mm_proxy_io_c {
protected:
  memory_cptr m_cache;
  int m_cache_pos;

public:
  mm_write_cache_io_c(mm_io_c *p_out, int cache_size, bool p_delete_out = true);

  virtual uint64 getFilePointer();
  virtual void setFilePointer(int64 offset, seek_mode mode = seek_beginning);
  virtual void flush();
  virtual void close();

protected:
  virtual uint32 _read(void *buffer, size_t size);
  virtual size_t _write(const void *buffer, size_t size);
  virtual void flush_cache();
};
typedef counted_ptr<mm_write_cache_io_c> mm_write_cache_io_cptr;

#endif // __MTX_COMMON_MM_WRITE_CACHED_FILE_IO_H
