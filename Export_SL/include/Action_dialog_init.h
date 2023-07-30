// Example of a plugin action

#pragma once

#include <vector>
#include <qstring.h>

#include "ccPointCloud.h"
#include "Grid3D.h"

class ccMainAppInterface;

struct Pose_line
{
	QString entID;
	QString entName;
	QString pose_coord_x;
	QString pose_coord_y;
	QString pose_coord_z;
};

namespace ExportSL
{
	void	initDialog( ccMainAppInterface *appInterface );




	//ccPointCloud* pointCloud = ...; //test

	//ccGrid* grid = pointCloud->gridCount(); //test
}
