#include "utils.h"
#include "config.h"

bool isValidPartName(std::string part)
{
	for (auto str : config::part_names) {
		if (part == str)
			return true;
	}
	return false;
}

bool isValidVDirectionName(std::string direction)
{
	for (auto str : config::v_directions) {
		if (direction == str)
			return true;
	}
	return false;
}

bool isValidHDirectionName(std::string direction)
{
	for (auto str : config::h_directions) {
		if (direction == str)
			return true;
	}
	return false;
}

// check whether this folder is a valid teeth scan folder
bool isValidFolder(QString fd_path)
{
	for (auto str : config::valid_folder_files) {
		const std::string tmp_str = '/' + str;
		QString file_path = fd_path + QString::fromStdString(tmp_str);
		if (!isValidFile(file_path))
			return false;
	}
	return true;
}

bool isValidFile(QString path) {
	QFileInfo check_file(path);
	return check_file.exists() && check_file.isFile();
}

// return true if the dir exists, but always delete all the things in the dir
bool mkdirIfMissing(QString abs_path)
{
	if (QDir(abs_path).exists())
	{
		QDir dir(abs_path);
		dir.setNameFilters(QStringList() << "*.*");
		dir.setFilter(QDir::Files);
		foreach(QString dirFile, dir.entryList())
		{
			dir.remove(dirFile);
		}
		return true;
	}
	else
	{
		QDir().mkdir(abs_path);
		return false;
	}
}

bool mkdirIfMissing(std::string abs_path)
{
	return mkdirIfMissing(QString::fromStdString(abs_path));
}

QString getResultFolder(QString base_fd)
{
	if (!base_fd.endsWith("/"))
	{
		base_fd += "/";
	}
	base_fd += "cropped_mesh/";
	return base_fd;
}