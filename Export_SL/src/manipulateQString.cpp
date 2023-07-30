#include "manipulateQString.h"
#include "Action_dialog_init.h"

/*----Functions for QString manipulation----*/

	// Entity Name contains path, this functions splits them (Ent_Name (Path) --> Ent_Name,Path)
QString formatPath(QString entName_f)
{
	entName_f.replace(" (", ",");
	entName_f.replace(")", "");
	return entName_f;
}
void writeDataToTextWindow(const Pose_line pose_line, QTextEdit* textEdit)
	{
		QString csvData = pose_line.entName + ","/* + entID + "," */ + pose_line.pose_coord_x + "," + pose_line.pose_coord_y + "," + pose_line.pose_coord_z;
		textEdit->append(csvData);
	}

void saveDataToCSV(const QString& fileName, const QString& data)
{
	QFile file(fileName);
	if (file.open(QIODevice::WriteOnly | QIODevice::Text))
	{
		QTextStream stream(&file);
		stream << data;
		file.close();
	}
	else
	{
		QMessageBox::warning(nullptr, "Error", "[Export_SL]: Failed to open the file for writing.");
	}
}
