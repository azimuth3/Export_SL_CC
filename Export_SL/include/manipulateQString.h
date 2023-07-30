#pragma once

#include <QStringList>
#include <QString.h>
#include <QTextEdit>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>

#include "Action_dialog_init.h"

QString formatPath(QString entName_f);

void writeDataToTextWindow(const Pose_line pose_line, QTextEdit* textEdit);
	// const QString& entID, ((not finished yet))



void saveDataToCSV(const QString& fileName, const QString& data);
