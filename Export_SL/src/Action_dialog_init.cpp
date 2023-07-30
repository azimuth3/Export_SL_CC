// Plugin action to initialize UI window for Plugin Export_SL

//#include "ccMainAppInterface.h"
#include "ccMainAppInterface.h"
#include "ui_mainwindow.h"

#include "Action_dialog_init.h"
#include "manipulateQString.h"

#include <ccGBLSensor.h>
#include <ccPointCloud.h>
#include <ccSensor.h>

#include <QFile>
#include <QMainWindow>
#include <QMessageBox>

#include <QDialog>
#include <QFileDialog>
#include <QPushButton>
#include <QString.h>
#include <QStringList>
#include <QTextEdit>
#include <QVBoxLayout>


namespace ExportSL
{

/*----Functions for pose extraction depending on entity type----*/
//extract pose of PointCloud
void extrPosePC(ccHObject *objChild, ccMainAppInterface *appInterface, QTextEdit *textEdit, Pose_line pose_line)
{
    if (objChild != nullptr)
    {
        ccGBLSensor *gblSensor = dynamic_cast<ccGBLSensor *>(objChild);

        if (gblSensor != nullptr)
        {
            CCVector3 poseVec;
            bool testboolPose = gblSensor->getActiveAbsoluteCenter(poseVec);

            if (testboolPose == true)
            {
                pose_line.pose_coord_x = QString::number(poseVec.x);
                pose_line.pose_coord_y = QString::number(poseVec.y);
                pose_line.pose_coord_z = QString::number(poseVec.z);

                appInterface->dispToConsole("[Export_SL] " + pose_line.entName +
                                                ": Scan location (pose): X = " + pose_line.pose_coord_x +
                                                " Y = " + pose_line.pose_coord_y + " Z = " + pose_line.pose_coord_z,
                                            ccMainAppInterface::STD_CONSOLE_MESSAGE);

                pose_line.entName = formatPath(pose_line.entName); //Ent_Name (Path) --> Ent_Name,Path

                writeDataToTextWindow(pose_line, textEdit);
            }
            else
            {
                appInterface->dispToConsole("[Export_SL] Warning: Object appears to not contain pose data",
                                            ccMainAppInterface::WRN_CONSOLE_MESSAGE);
            }
        }
        else
        {
            appInterface->dispToConsole("[Export_SL] Warning: Object appears to not have a child of type ccGBLSensor",
                                        ccMainAppInterface::WRN_CONSOLE_MESSAGE);
        }
    }
    else
    {
        appInterface->dispToConsole("[Export_SL] Warning: Object position is invalid",
                                    ccMainAppInterface::WRN_CONSOLE_MESSAGE);
    }
}
// extract pose of sensor
void extrPoseSensor(ccHObject *selObject, ccMainAppInterface *appInterface, QTextEdit *textEdit, Pose_line pose_line)
{

    ccSensor *sensor = dynamic_cast<ccSensor *>(selObject);

    if (sensor != nullptr)
    {
        CCVector3 poseVec;
        bool testboolPose = sensor->getActiveAbsoluteCenter(poseVec);

        pose_line.pose_coord_x = QString::number(poseVec.x);
        pose_line.pose_coord_y = QString::number(poseVec.y);
        pose_line.pose_coord_z = QString::number(poseVec.z);

        appInterface->dispToConsole("[Export_SL] Scan location (pose): X = " + pose_line.pose_coord_x +
                                        " Y = " + pose_line.pose_coord_y + " Z = " + pose_line.pose_coord_z,
                                    ccMainAppInterface::STD_CONSOLE_MESSAGE);
    }
    else
    {
        appInterface->dispToConsole("[Export_SL] Warning: Object appears to be not of type ccSensor",
                                    ccMainAppInterface::WRN_CONSOLE_MESSAGE);
    }
}
// extract pose of GBL-sensor
void extrPoseGBL(ccHObject *objChild, ccMainAppInterface *appInterface, QTextEdit *textEdit, Pose_line pose_line)
{
    ccSensor *gblSensor = dynamic_cast<ccGBLSensor *>(objChild);

    if (gblSensor != nullptr)
    {
        CCVector3 poseVec;
        bool testboolPose = gblSensor->getActiveAbsoluteCenter(poseVec);

        pose_line.pose_coord_x = QString::number(poseVec.x);
        pose_line.pose_coord_y = QString::number(poseVec.y);
        pose_line.pose_coord_z = QString::number(poseVec.z);

        appInterface->dispToConsole("[Export_SL] Scan location (pose): X = " + pose_line.pose_coord_x +
                                        " Y = " + pose_line.pose_coord_y + " Z = " + pose_line.pose_coord_z,
                                    ccMainAppInterface::STD_CONSOLE_MESSAGE);
    }
    else
    {
        appInterface->dispToConsole("[Export_SL] Warning: Object appears to be not of type ccGBLSensor",
                                    ccMainAppInterface::WRN_CONSOLE_MESSAGE);
    }
}


/*----Pose extraction function----*/


//check entity type function to prevent errors
static bool checkEntityType(ccHObject *obj)
{
    auto type = obj->getClassID();
    if (obj != nullptr && (type == CC_TYPES::POINT_CLOUD || type == CC_TYPES::SENSOR || type == CC_TYPES::GBL_SENSOR))
    {
        return true;
    }
    else
        return false;
}

void extractPose(ccHObject *selObject, ccMainAppInterface *appInterface, QTextEdit *textEdit, Pose_line &pose_line)
{
    const int CHILD_COUNT = selObject->getChildCountRecursive();
    const auto CHILD_COUNT_Q = QString::number(selObject->getChildCountRecursive());

    appInterface->dispToConsole("[Export_SL] Count of Children (recursive): " + CHILD_COUNT_Q,
                                ccMainAppInterface::STD_CONSOLE_MESSAGE);
    auto entType = selObject->getClassID();


    if ((selObject != nullptr) && (entType != NULL) && (CHILD_COUNT != 0) &&
        (entType == CC_TYPES::POINT_CLOUD || entType == CC_TYPES::SENSOR || entType == CC_TYPES::GBL_SENSOR))
    {
        //pose_line.entID = QString::number(selObject->GetLastUniqueID());

        for (int i = 0; i < CHILD_COUNT; i++)
        {
            try
            {
                auto objChild_test = selObject->getChild(i);
            }
            catch (const std::exception &e)
            {
                appInterface->dispToConsole("[Export_SL] Warning: Entity appears to be of the wrong type",
                                            ccMainAppInterface::WRN_CONSOLE_MESSAGE);
            }

            auto objChild = selObject->getChild(i);

            //check entity Type of child before extracting the pose
            if (!checkEntityType(objChild))
            {
                break;
            }
            else
            {

                auto childType = objChild->getClassID();

                pose_line.entName = selObject->getParent()->getName();


                switch (selObject->getClassID())
                {
                case CC_TYPES::POINT_CLOUD:
                {
                    extrPosePC(objChild, appInterface, textEdit, pose_line);
                    break;
                }

                case CC_TYPES::SENSOR:
                {
                    extrPoseSensor(selObject, appInterface, textEdit, pose_line);
                    break;
                }

                case CC_TYPES::GBL_SENSOR:
                {
                    extrPoseGBL(selObject, appInterface, textEdit, pose_line);
                    break;
                }
                default:
                {
                    appInterface->dispToConsole("[Export_SL] Warning: Entity appears to not have a pose "
                                                "in its data structure or something went wrong",
                                                ccMainAppInterface::WRN_CONSOLE_MESSAGE);
                }
                }
            }
        }
    }
    else
    {
        appInterface->dispToConsole("[Export_SL] Warning: Entity appears to be of the wrong type",
                                    ccMainAppInterface::WRN_CONSOLE_MESSAGE);
    }
}

// START ("main" method of plugin --> Export_SL calls this method when clicking it)
void initDialog(ccMainAppInterface *appInterface)
{
    if (appInterface == nullptr)
    {
        // The application interface should have already been initialized when the plugin is loaded
        Q_ASSERT(false);

        return;
    }

    // dialog initialization
    QDialog dialog;
    QVBoxLayout layout(&dialog);

    // label name
    QLabel *nameLabel = new QLabel("Export_SL @v0.1");
    nameLabel->setAlignment(Qt::AlignLeft);
    layout.addWidget(nameLabel);

    // label order
    QLabel *orderLabel = new QLabel("Name,Path,X,Y,Z");
    orderLabel->setAlignment(Qt::AlignLeft);
    orderLabel->setFont(QFont("", -1, QFont::StyleOblique));
    layout.addWidget(orderLabel);

    QTextEdit textEdit;
    layout.addWidget(&textEdit);

    QPushButton saveButton("Save as CSV");
    layout.addWidget(&saveButton);

    int defaultWidth = 600;
    int defaultHeight = 230;
    dialog.setMinimumWidth(defaultWidth);
    dialog.setMinimumHeight(defaultHeight);
    dialog.setSizePolicy(QSizePolicy::MinimumExpanding, dialog.sizePolicy().verticalPolicy());

    QString csvData;


    //Pose extraction loop and function call

    ccHObject::Container selEnt =
        appInterface->getSelectedEntities(); // initialize selected point clouds/entities as object selPc

    for (ccHObject *object : selEnt)         // iterate through selected entitites (object_1 ... object_n)
    {
        Pose_line curr_pose_line{};          // holds the QStrings for CSV and is passed to all relevant functions

        extractPose(object, appInterface, &textEdit, curr_pose_line);

        csvData.append(QString("%1,%2,%3,%4\n")
                           //.arg(entID)  ((not finished yet))
                           .arg(curr_pose_line.entName)
                           .arg(curr_pose_line.pose_coord_x)
                           .arg(curr_pose_line.pose_coord_y)
                           .arg(curr_pose_line.pose_coord_z));
    }

    /* Remove comma-only lines (e.g. ",,,") which are generated when selected
		entities are not of type Point Cloud or Sensor or GBL-Sensor */

    QStringList lines = csvData.split('\n');
    for (int i = lines.size() - 1; i >= 0; --i)
    {
        QString line = lines.at(i).trimmed();
        if (line.isEmpty() || line.count(',') == line.length())
        {
            lines.removeAt(i);
        }
    }

    csvData = lines.join('\n'); //join the seperate lines to form a CSV-file

    QObject::connect(&saveButton, &QPushButton::clicked, [&]() {
        QString fileName = QFileDialog::getSaveFileName(nullptr, "Save CSV", "", "CSV Files (*.csv)");
        if (!fileName.isEmpty())
        {
            saveDataToCSV(fileName, csvData);
        }
    });

    //last thing: Dialog execution after setup and data extraction

    dialog.exec();
}
} // namespace ExportSL
////for (int i = 0; i < selPw.size(); i++) //iteriere durch selektierte Objekte
////{
////
//
////	//getName() returnt Qstring --> std::cout etc nicht nutzbar, deshalb Qstring initialisieren
////	QString enitityname = selPw[i]->getName();
//
////	//QString coordX = selPw[i]->find(pose);
//
////	//ccPointCloud::Grid cloudGrid =
//
////	//ccPointCloud::Grid meinGrid;
//
////	//ccHObject* find(unsigned uniqueID) const;
//
//
////	appInterface->dispToConsole("[Export_SL: ] Name of the entity: " + enitityname, ccMainAppInterface::STD_CONSOLE_MESSAGE);
//
////	//--alter Ansatz--
////	//std::cout << "Name of selected entity " << i + 1 << ": " << name.toStdString() << std::endl;
////	//std::cout << "Name des Objekts: " << selpw[i]->getName()<< std::endl;
////}
//
//
//		//QMainWindow* test1 = appInterface->getMainWindow();
//
//		//QMessageBox::information(test1, "TEST");
//
//		// This is how you can output messages
//		// Display a standard message in the console
//		//appInterface->dispToConsole( "[ExamplePlugin] Hello world!", ccMainAppInterface::STD_CONSOLE_MESSAGE );
//
//		// Display a warning message in the console
//		//appInterface->dispToConsole( "[ExamplePlugin] Warning: example plugin shouldn't be used as is", ccMainAppInterface::WRN_CONSOLE_MESSAGE );
//
//		// Display an error message in the console AND pop-up an error box
//		//appInterface->dispToConsole( "Example plugin shouldn't be used - it doesn't do anything!", ccMainAppInterface::ERR_CONSOLE_MESSAGE );
//
//				//ccSensor* sensor = dynamic_cast<ccSensor*>(object);
//				//if (sensor) {
//				//	// 'object' is of type ccSensor
//				//	sensors.push_back(sensor);
//				//	appInterface->dispToConsole("HAS SENSOR ", ccMainAppInterface::STD_CONSOLE_MESSAGE);
//				//}
//
//				//appInterface->dispToConsole("CC_TYPE: POINT_CLOUD ", ccMainAppInterface::STD_CONSOLE_MESSAGE);
//
//				//for (ccSensor* sensor : sensors)
//				//{
//				//	//const ccIndexedTransformationBuffer* pose = sensor->getPositions();
//				//	//appInterface->dispToConsole(pose->m_bBox, ccMainAppInterface::STD_CONSOLE_MESSAGE);
//				//	ccGLMatrix position_pw = sensor->getRigidTransformation();
//
//				//	Vector3Tpl<float> x_pos = position_pw.getTranslationAsVec3D();
//
//
//				//	QString x_pos_string = QString("%1, %2, %3")
//				//		.arg(QString::number(x_pos.x))
//				//		.arg(QString::number(x_pos.y))
//				//		.arg(QString::number(x_pos.z));
//
//				//	appInterface->dispToConsole("POS: "+x_pos_string, ccMainAppInterface::STD_CONSOLE_MESSAGE);
/*Ui::MainWindow ui;
		QMainWindow* mainWindow = new QMainWindow();
		ui.setupUi(mainWindow);

		mainWindow->setWindowTitle("Plugin: Export_SL");
		mainWindow->show();*/

/*** HERE ENDS THE ACTION ***/
