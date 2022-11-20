/**
 * "SimplePictureTool" Copyright (C) 2022 Patrice Coni
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <QFileDialog>
#include <QProcess>
#include <QDir>
#include <QFile>
#include <QMessageBox>
#include <QSettings>
#include <QCloseEvent>
#include <QProgressDialog>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QObject::connect(ui->pushButton, &QPushButton::clicked, this, &MainWindow::selectDir);
    QObject::connect(ui->pushButton_3, &QPushButton::clicked, this, &MainWindow::startProcess);
    QObject::connect(ui->checkBox_2, &QCheckBox::toggled, [this]()
    {
        ui->spinBox->setEnabled(ui->checkBox_2->checkState());
        ui->spinBox_2->setEnabled(ui->checkBox_2->checkState());
    });

    confFilePath = QDir::homePath()+"/.SimplePictureTool/ApplicationSettings.conf";
    QFile confFile = confFilePath;
    if(confFile.exists())
    {
        loadSettings();

        if(!ui->checkBox_2->isChecked())
        {
            ui->spinBox->setDisabled(true);
            ui->spinBox_2->setDisabled(true);
        }
    }
    else
    {
        ui->checkBox_2->setChecked(true);
        ui->spinBox->setValue(80);
        ui->spinBox_2->setValue(50);
    }
    selectDir();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::closeEvent(QCloseEvent * event)
{
    saveSettings();
}

void MainWindow::selectDir()
{
    ui->lineEdit->setText(QFileDialog::getExistingDirectory(this, tr("Select Directory"),
                                                            QDir::homePath(),
                                                            QFileDialog::ShowDirsOnly
                                                            | QFileDialog::DontResolveSymlinks));
}

void MainWindow::startProcess()
{
    QMessageBox _msg;

    switch(getStatus())
    {
    case 1:
        _msg.information(0, "Information", "Please select at least one action.");
        break;
    case 2:
        _msg.information(0, "Information", "Process completed.");
        break;
    case 3:
        _msg.information(0, "Information", "There are no .PNG or .JPG picture files in the selected directory.");
        break;
    case 4:
        _msg.information(0, "Information", "The selected folder is empty.");
        break;
    case 5:
        _msg.critical(0, "Error", "Directory does not exist.");
        break;
    case 6:
        _msg.information(0, "Information", "Please select a directory.");
        break;
    case 7:
        _msg.critical(0, "Error", "Error. Please check your ImageMagick installation.");
        break;
    }
}

int MainWindow::getStatus()
{
    if(!ui->lineEdit->text().isEmpty())
    {
        if(!ui->checkBox->isChecked() && !ui->checkBox_2->isChecked())
        {
            return 1;
        }
        else
        {
            QDir picDir(ui->lineEdit->text());

            if (picDir.exists())
            {
                QStringList _arguments;

                QString qualityValue = QString::number(ui->spinBox->value())+"%";
                QString scaleValue = QString::number(ui->spinBox_2->value())+"%";

                QDir pictureDir(ui->lineEdit->text());

                if(!pictureDir.isEmpty())
                {
                     QStringList pictureFiles = pictureDir.entryList(QStringList() << "*.jpg" << "*.JPG" << "*.png" << "*.PNG", QDir::Files);

                     QProgressDialog progstatus("Working in progress...", "Cancel", 0, pictureFiles.size(), this);
                     progstatus.setWindowModality(Qt::WindowModal);
                     progstatus.show();

                     if(!pictureFiles.isEmpty())
                     {
                         QString file;

                         for(int i=0; i<pictureFiles.size(); i++)
                         {
                             progstatus.setValue(i);

                             file=ui->lineEdit->text()+"/"+pictureFiles.at(i);
                             if(ui->checkBox->isChecked() == true)
                             {
                                 _arguments << "-strip" << file << file;

                                 if(doProcess(_arguments)==false)
                                 {
                                     return 7;
                                 }

                                 _arguments.clear();
                             }

                             if(ui->checkBox_2->isChecked()==true)
                             {
                                 _arguments << "-quality" << qualityValue+"%" << "-scale" << scaleValue+"%" << file << file;

                                 if(doProcess(_arguments)==false)
                                 {
                                     return 7;
                                 }

                                 _arguments.clear();
                             }
                         }

                         progstatus.setValue(pictureFiles.size());

                         return 2;

                         pictureFiles.clear();
                     }
                     else
                     {
                         return 3;
                     }
                }
                else
                {
                    return 4;
                }
            }
            else
            {
                return 5;
            }
        }
    }
    else
    {
        return 6;
    }
    return 8;
}

bool MainWindow::doProcess(QStringList arguments)
{
    QProcess proc;

    proc.start("convert", arguments);
    proc.waitForFinished();
    proc.close();

    if(proc.exitCode()==255)
    {
        return false;
    }
    else
    {
        return true;
    }
}

void MainWindow::saveSettings()
{
    QSettings settings(confFilePath, QSettings::NativeFormat);

    int qualityValue = ui->spinBox->value();
    settings.setValue("quality", qualityValue);

    int scaleValue = ui->spinBox_2->value();
    settings.setValue("scale", scaleValue);

    settings.setValue("checkBox1", (int)ui->checkBox->checkState());
    settings.setValue("checkBox2", (int)ui->checkBox_2->checkState());
}

void MainWindow::loadSettings()
{
    QSettings settings(confFilePath, QSettings::NativeFormat);

    ui->spinBox->setValue(settings.value("quality", "").toInt());
    ui->spinBox_2->setValue(settings.value("scale", "").toInt());

    ui->checkBox->setCheckState((Qt::CheckState)settings.value("checkBox1", 0).toInt());
    ui->checkBox_2->setCheckState((Qt::CheckState)settings.value("checkBox2", 0).toInt());
}
