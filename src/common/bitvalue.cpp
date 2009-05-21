/*
   mkvmerge -- utility for splicing together matroska files
   from component media subtypes

   Distributed under the GPL
   see the file COPYING for details
   or visit http://www.gnu.org/copyleft/gpl.html

   The "bitvalue_c" class

   Written by Moritz Bunkus <moritz@bunkus.org>.
*/

#include "common/os.h"

#include "common/bitvalue.h"
#include "common/error.h"
#include "common/memory.h"
#include "common/random.h"
#include "common/strings/editing.h"

bitvalue_c::bitvalue_c(int bitsize) {
  assert((0 < bitsize) && (0 == (bitsize % 8)));

  m_value = memory_c::alloc(bitsize / 8);
  memset(m_value->get(), 0, m_value->get_size());
}

bitvalue_c::bitvalue_c(const bitvalue_c &src) {
  *this = src;
}

#define ishexdigit(c) (isdigit(c) || (((c) >= 'a') && ((c) <= 'f')))
#define hextodec(c)   (isdigit(c) ? ((c) - '0') : ((c) - 'a' + 10))

bitvalue_c::bitvalue_c(std::string s,
                       int allowed_bitlength) {
  if ((allowed_bitlength != -1) && ((allowed_bitlength % 8) != 0))
    throw error_c("wrong usage: invalid allowed_bitlength");

  int len                 = s.size();
  bool previous_was_space = true;
  s                       = downcase(s);
  std::string s2;

  int i;
  for (i = 0; i < len; i++) {
    // Space or tab?
    if (isblanktab(s[i])) {
      previous_was_space = true;
      continue;
    }
    previous_was_space = false;

    // Skip hyphens and curly braces. Makes copy & paste a bit easier.
    if ((s[i] == '-') || (s[i] == '{') || (s[i] == '}'))
      continue;

    // Space or tab followed by "0x"? Then skip it.
    if (s.substr(i, 2) == "0x") {
      i++;
      continue;
    }

    // Invalid character?
    if (!ishexdigit(s[i]))
      throw error_c(boost::format(Y("Not a hex digit at position %1%")) % i);

    // Input too long?
    if ((allowed_bitlength > 0) && ((s2.length() * 4) >= allowed_bitlength))
      throw error_c(boost::format(Y("Input too long: %1% > %2%")) % (s2.length() * 4) % allowed_bitlength);

    // Store the value.
    s2 += s[i];
  }

  // Is half a byte or more missing?
  len = s2.length();
  if (((len % 2) != 0)
      ||
      ((allowed_bitlength != -1) && ((len * 4) < allowed_bitlength)))
    throw error_c(Y("Missing one hex digit"));

  m_value = memory_c::alloc(len / 2);

  for (i = 0; i < len; i += 2)
    m_value->get()[i / 2] = hextodec(s2[i]) << 4 | hextodec(s2[i + 1]);
}

bitvalue_c::bitvalue_c(const EbmlBinary &elt)
  : m_value(clone_memory(elt.GetBuffer(), elt.GetSize()))
{
}

bitvalue_c &
bitvalue_c::operator =(const bitvalue_c &src) {
  m_value = clone_memory(src.m_value);

  return *this;
}

bitvalue_c::~bitvalue_c() {
}

bool
bitvalue_c::operator ==(const bitvalue_c &cmp)
  const {
  return (cmp.m_value->get_size() == m_value->get_size()) && (0 == memcmp(m_value->get(), cmp.m_value->get(), m_value->get_size()));
}

unsigned char
bitvalue_c::operator [](int index)
  const {
  assert((0 <= index) && (m_value->get_size() > index));
  return m_value->get()[index];
}

void
bitvalue_c::generate_random() {
  random_c::generate_bytes(m_value->get(), m_value->get_size());
}
