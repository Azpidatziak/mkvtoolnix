#ifndef MTX_MMGQT_MUX_CONFIG_H
#define MTX_MMGQT_MUX_CONFIG_H

#include "common/common_pch.h"

// #include "mmg-qt/attachment.h"
#include "mmg-qt/source_file.h"
#include "mmg-qt/track.h"

#include <QObject>
#include <QList>
#include <QSettings>
#include <QString>

class MuxConfig;
typedef std::shared_ptr<MuxConfig> MuxConfigPtr;

class MuxConfig : public QObject {
  Q_OBJECT;

public:
  enum SplitMode {
    DoNotSplit = 1,
    SplitAfterSize,
    SplitAfterDuration,
    SplitAfterTimecodes,
    SplitByParts,
  };

public:
  QString m_configFileName;

  QList<SourceFilePtr> m_files;
  QList<Track *> m_tracks;
  // QList<AttachmentPtr> m_attachments;

  QString m_title, m_destination, m_globalTags, m_segmentinfo, m_splitAfterSize, m_splitAfterDuration, m_splitAfterTimecodes, m_splitByParts;
  QString m_segmentUIDs, m_previousSegmentUID, m_nextSegmentUID, m_chapters, m_chapterLanguage, m_chapterCharacterSet, m_chapterCueNameFormat, m_userDefinedOptions;
  SplitMode m_splitMode;
  unsigned int m_splitMaxFiles;
  bool m_linkFiles, m_webmMode;

public:
  MuxConfig();
  virtual ~MuxConfig();

  virtual void load(QSettings const &settings);
  virtual void save(QSettings &settings) const;
  virtual void save(QString const &fileName) const;
  virtual void reset() const;

public:
  static MuxConfigPtr load(QString const &fileName);
};

#endif  // MTX_MMGQT_MUX_CONFIG_H
