#include "cue/audiofile.hpp"

#include <QFileInfo>
#include <QTextStream>

namespace Khopper {
	
	AudioFile::AudioFile() {
	}
	
	AudioFile::AudioFile( const QString & filePath, FileType fileType ) : path_( filePath ), type_( fileType ), format_( QFileInfo( path_ ).suffix().toLower() ) {
	}

	const QString & AudioFile::getFilePath() const {
		return path_;
	}

	const QString & AudioFile::getFormat() const {
		return format_;
	}
	
	QString AudioFile::toString() const {
		QTextStream sout;
		sout << "File Path:\t" << path_ << "\n";
		sout << "FileType:\t" << type_ << "\n";
		return sout.readAll();
	}
	
}
