/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2009 University of Strasbourg
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation;
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * Author: Pierre Weiss <3weissp@gmail.com>
 */

/**
 * \file drag-widget.cpp
 * \brief The drag'n'drop zone file.
 * \author Pierre Weiss
 * \date 2009
 */

#include <QtGui>
#include <iostream>
#include <stdexcept>

#include "main-window.h"
#include "drag-widget.h"

#include "drag-object.h"
#include "drag-lines.h"

#include "utils.h"

#include "emu.h"
#include "tap.h"

DragWidget::DragWidget(QWidget *parent) : QWidget(parent)
{
  setMinimumSize(400, 400);
  setAcceptDrops(true);
  setMouseTracking(true);

  this->m_traceNetworkHardware = false;

  this->m_networkHardwareBegin = "";
  this->m_networkHardwareEnd = "";
  this->m_networkHardwareType = "";

  this->m_appsEnable = false;
  this->m_appsPing = false;
  this->m_appsUdpEcho = false;
  this->m_appsTcp = false;
  this->m_appsServer = "";
  this->m_appsClient = "";
  this->m_startTime = -1;
  this->m_endTime = -1;
  this->m_port = -1;
}

DragWidget::~DragWidget()
{
  delete this->m_timer;
}

void DragWidget::SetMainWindow(MainWindow *mw)
{
  this->m_mw = mw;
}

void DragWidget::CreateObject(const std::string &type, const std::string &name)
{
  DragObject *label = new DragObject(this);
  label->SetName(name);
  std::string new_toolTip = this->UpdateToolTip(name);
  label->SetToolTipText(QString(new_toolTip.c_str()));
  label->setToolTip(QString(new_toolTip.c_str()));

  if(type == "Pc")
  {
    label->setPixmap(QPixmap(":/Ico/Pc.png"));
  }
  else if(type == "Pc-group")
  {
    label->setPixmap(QPixmap(":/Ico/Pc-group.png"));
  }
  else if(type == "Emu")
  {
    label->setPixmap(QPixmap(":/Ico/Emu.png"));
  }
  else if(type == "Tap")
  {
    label->setPixmap(QPixmap(":/Ico/Tap.png"));
  }
  else if(type == "Ap")
  {
    label->setPixmap(QPixmap(":/Ico/Ap-Wifi.png"));
  }
  else if(type == "Station")
  {
    label->setPixmap(QPixmap(":/Ico/StationWifi.png"));
  }
  else if(type == "Hub")
  {
    label->setPixmap(QPixmap(":/Ico/Hub.png"));
  }
  else if(type == "Switch")
  {
    label->setPixmap(QPixmap(":/Ico/Switch.png"));
  }
  else if(type == "Router")
  {
    label->setPixmap(QPixmap(":/Ico/Router.png"));
  } 
  
  label->move(10, 10);
  label->show();
  label->setAttribute(Qt::WA_DeleteOnClose);

  this->m_timer = new QTimer();
  this->m_timer->start(100);
  connect(this->m_timer, SIGNAL(timeout()), this, SLOT(update()));
}

void DragWidget::dragEnterEvent(QDragEnterEvent *event)
{
  if (event->mimeData()->hasFormat("application/x-dnditemdata")) 
  {
    if (event->source() == this) 
    {
      event->setDropAction(Qt::MoveAction);
      event->accept();
    } 
    else 
    {
      event->acceptProposedAction();
    }
  } 
  else 
  {
    event->ignore();
  }
}

void DragWidget::dropEvent(QDropEvent *event)
{
  if (event->mimeData()->hasFormat("application/x-dnditemdata")) 
  {
    QByteArray itemData = event->mimeData()->data("application/x-dnditemdata");
    QDataStream dataStream(&itemData, QIODevice::ReadOnly);

    QPixmap pixmap;
    QPoint offset;
    dataStream >> pixmap >> offset;

    DragObject *child = static_cast<DragObject*>(childAt( this->m_lastPosition ));
    DragObject *label = new DragObject(this);
    if(child)
    {
      label->SetName(child->GetName());
      std::string new_tooltip = this->UpdateToolTip(child->GetName());
      label->SetToolTipText(QString(new_tooltip.c_str()));
      label->setToolTip(QString(new_tooltip.c_str()));
    }
    label->setPixmap(pixmap);
    label->move(event->pos() - offset);
    label->show();
    label->setAttribute(Qt::WA_DeleteOnClose);

    update();

    if (event->source() == this) 
    {
      event->setDropAction(Qt::MoveAction);
      event->accept();
    } 
    else 
    {
      event->acceptProposedAction();
    }
  } 
  else 
  {
    event->ignore();
  }
}

void DragWidget::mousePressEvent(QMouseEvent *event)
{
  DragObject *child = static_cast<DragObject*>(childAt(event->pos()));
  if (!child)
  {
    this->m_mw->GetDelQAction()->setDisabled(true);
    return;
  }
  if(child->GetName() == "deleted")
  {
    return;
  }
  this->m_mw->GetDelQAction()->setDisabled(false);

  this->m_lastPosition = event->pos();

  /* draw lines */
  DragObject *child2 = NULL;
  if(this->m_traceNetworkHardware)
  {
    if(this->m_networkHardwareBegin == "")
    {
      this->m_networkHardwareBegin = child->GetName();
    }
    else if(this->m_networkHardwareEnd == "")
    {
      child2 = this->GetChildFromName(this->m_networkHardwareBegin);
      if(child2)
      {
        if( child->GetName() != child2->GetName())
        {
          this->m_networkHardwareEnd = child->GetName();
          DragLines lig;
          lig.SetFirst(this->m_networkHardwareBegin);
          lig.SetSecond(this->m_networkHardwareEnd);
          lig.SetNetworkHardwareType(this->m_networkHardwareType);
          this->m_drawLines.push_back(lig);

          this->m_mw->ValidNetworkHardware();
        }
      }
    }
  } 

  QPixmap pixmap = *child->pixmap();

  QByteArray itemData;
  QDataStream dataStream(&itemData, QIODevice::WriteOnly);
  dataStream << pixmap << QPoint(event->pos() - child->pos());

  QMimeData *mimeData = new QMimeData;
  mimeData->setData("application/x-dnditemdata", itemData);

  QDrag *drag = new QDrag(this);
  drag->setMimeData(mimeData);
  drag->setPixmap(pixmap);
  drag->setHotSpot(event->pos() - child->pos());

  QPixmap tempPixmap = pixmap;
  QPainter painter;
  painter.begin(&tempPixmap);
  painter.fillRect(pixmap.rect(), QColor(127, 127, 127, 127));
  painter.end();

  child->setPixmap(tempPixmap);

  if (drag->start(Qt::CopyAction | Qt::MoveAction) == Qt::MoveAction)
  {
    child->close();
  }
  else 
  {
    child->show();
    child->setPixmap(pixmap);
  }

  /* application. */
  if(this->m_appsEnable)
  {
    if(this->m_appsServer == "")
    {
      this->m_appsServer = child->GetName();
    }
    else
    {
      if(this->m_appsClient == "" && child->GetName() != this->m_appsServer)
      {
        this->m_appsClient = child->GetName();
      }
    }
  }

  if(this->m_appsPing)
  {
    ShowGuiPing();
  }

  if(this->m_appsUdpEcho)
  {
    ShowGuiUdpEcho();
  }

  if(this->m_appsTcp)
  {
    ShowGuiTcp();
  }


  std::string new_tooltip = this->UpdateToolTip(child->GetName());
  child->SetToolTipText(QString(new_tooltip.c_str()));
  child->setToolTip(QString(new_tooltip.c_str()));
  if(child2)
  {
    std::string new_tooltip = this->UpdateToolTip(child2->GetName());
    child2->SetToolTipText(QString(new_tooltip.c_str()));
    child2->setToolTip(QString(new_tooltip.c_str()));
  }
}

void DragWidget::mouseMoveEvent(QMouseEvent * /*event*/)
{
  update();
}

void DragWidget::DeleteSelected()
{
  DragObject *child = static_cast<DragObject*>(childAt(this->m_lastPosition));
  if (!child)
  {
    this->m_mw->GetDelQAction()->setDisabled(true);
    return;
  }

  /* Generator part */
  /* delete equipement. */
  try
  {
    this->m_mw->GetGenerator()->RemoveNode(child->GetName());
  }
  catch(const std::exception)
  {}

  /* delete link. */
  try
  {
    this->m_mw->GetGenerator()->RemoveNetworkHardware(child->GetName());
  }
  catch(const std::exception)
  {}

  /* delete connections */
  std::vector<std::string> objDelLink;
  for(size_t i = 0; i < this->m_mw->GetGenerator()->GetNNetworkHardwares(); i++)
  {
    std::vector<std::string> nodes = this->m_mw->GetGenerator()->GetNetworkHardware(i)->GetInstalledNodes();
    for(size_t j = 0; j < nodes.size(); j++)
    {
      /* if the child to be deleted is connected ... we must remove it. */
      if(child->GetName() == nodes.at(j))
      {
        objDelLink.push_back(this->m_mw->GetGenerator()->GetNetworkHardware(i)->GetNetworkHardwareName());
        try
        {
          this->m_mw->GetGenerator()->GetNetworkHardware(i)->removeInstalledNode(j);
        }
        catch(const std::out_of_range &e)
        {
          this->m_mw->GetDelQAction()->setDisabled(true);
        }
      }
    }
  }

  /* delete hide hub for until two Pc for example. */
  bool isHide = true;
  for(size_t i = 0; i < objDelLink.size(); i++)
  {
    for(size_t j = 0; j < this->m_mw->GetGenerator()->GetNNetworkHardwares(); j++)
    {
      if( objDelLink.at(i) == this->m_mw->GetGenerator()->GetNetworkHardware(j)->GetNetworkHardwareName() )
      {
        if(this->m_mw->GetGenerator()->GetNetworkHardware(j)->GetInstalledNodes().size() <= 1)
        {
          // the link where the deleted object 
          // check if the link is hide. 
          isHide = true;
          for(size_t k = 0; k < (size_t)this->children().size(); k++)
          {
            if(dynamic_cast<DragObject*>((this->children().at(k))))
            {
              if( dynamic_cast<DragObject*>((this->children().at(k)))->GetName() == this->m_mw->GetGenerator()->GetNetworkHardware(j)->GetNetworkHardwareName())
              {
                isHide = false;
              }
            }
          }
          if(isHide)
          {
            this->m_mw->GetGenerator()->RemoveNetworkHardware(this->m_mw->GetGenerator()->GetNetworkHardware(j)->GetNetworkHardwareName());
          }
        }
      }
    }
  }

  /* remove from link part ... */
  if(child->GetName() == this->GetChildFromName(this->m_networkHardwareBegin)->GetName())
  {
    this->m_networkHardwareBegin = "";
  }
  if(child->GetName() == this->GetChildFromName(this->m_networkHardwareEnd)->GetName())
  {
    this->m_networkHardwareEnd = "";
  }
  for(size_t i = 0; i < this->m_drawLines.size(); i++)
  {
    if(child->GetName() == this->GetChildFromName(this->m_drawLines.at(i).GetFirst())->GetName() || child->GetName() == this->GetChildFromName(this->m_drawLines.at(i).GetSecond())->GetName())
    {
      try
      {
        this->m_drawLines.erase(this->m_drawLines.begin() + i);
      }
      catch(const std::out_of_range &e)
      {
        this->m_mw->GetDelQAction()->setDisabled(true);
      }
    }
  }

  /* remove application. */
  std::string sender("");
  std::string receiver("");
  for(size_t i = 0; i < this->m_mw->GetGenerator()->GetNApplications(); i++)
  {
    if((child->GetName()).find("nodesGroup_") == 0)
    {
      sender = this->m_mw->GetGenerator()->GetApplication(i)->GetSenderNode();
      receiver = this->m_mw->GetGenerator()->GetApplication(i)->GetReceiverNode();

      if(sender.find("NodeContainer") == 0)
      {
        sender = sender.substr((sender.find_first_of("(") + 1), (sender.find_first_of(".") - sender.find_first_of("(") - 1));
      }
      if(receiver.find("NodeContainer") == 0)
      {
        receiver = receiver.substr(receiver.find_first_of("(") + 1, (receiver.find_first_of(".") - receiver.find_first_of("(") - 1));
      }
    }
    else
    {
      sender = this->m_mw->GetGenerator()->GetApplication(i)->GetSenderNode();
      receiver = this->m_mw->GetGenerator()->GetApplication(i)->GetReceiverNode(); 
    }

    if(child->GetName() == sender ||
        child->GetName() == receiver )
    {
      this->m_mw->GetGenerator()->RemoveApplication(this->m_mw->GetGenerator()->GetApplication(i)->GetAppName());
    }
  }

  /* Gui part */
  child->clear();
  child->Destroy();

  this->m_mw->GetDelQAction()->setDisabled(true);
}

std::vector<std::string> DragWidget::GetLastSelected()
{
  std::vector<std::string> res;
  DragObject *child = this->GetChildFromName(this->m_networkHardwareBegin);
  DragObject *child2 = this->GetChildFromName(this->m_networkHardwareEnd);
  if (child)
  {
    res.push_back(child->GetName());
  }
  else
  {
    res.push_back("");
  }

  if (child2)
  {
    res.push_back(child2->GetName());
  }
  else
  {
    res.push_back("");
  }

  res.push_back(this->m_networkHardwareType);

  return res;
}

void DragWidget::DrawLines()
{
  update();
}

void DragWidget::ResetSelected()
{
  this->m_networkHardwareBegin = "";
  this->m_networkHardwareEnd = "";
  this->m_networkHardwareType = "";
}

void DragWidget::paintEvent(QPaintEvent * /*event*/)
{
  QPainter paint(this);

  QPen pen(Qt::black);
  pen.setWidth(2);
  pen.setCapStyle(Qt::RoundCap);

  QPen point(Qt::black);
  point.setWidth(2);
  point.setStyle(Qt::DotLine);
  point.setCapStyle(Qt::RoundCap);


  QPen p2p(Qt::green);
  p2p.setWidth(2);
  p2p.setCapStyle(Qt::RoundCap);


  for(size_t i = 0; i < this->m_drawLines.size(); i++)
  {
    if(this->GetChildFromName(this->m_drawLines.at(i).GetFirst())->GetName() != "" && this->GetChildFromName(this->m_drawLines.at(i).GetSecond())->GetName() != "")
    {
      std::string type = this->m_drawLines.at(i).GetNetworkHardwareType();

      if(type == "WiredLink")
      {
        paint.setPen(pen);
      }
      else if(type == "WifiLink")
      {
        paint.setPen(point);
      }
      else if(type == "P2pLink")
      {
        paint.setPen(p2p);
      }
      DragObject *begin = this->GetChildFromName(this->m_drawLines.at(i).GetFirst());
      DragObject *end = this->GetChildFromName(this->m_drawLines.at(i).GetSecond());
      paint.drawLine((begin->pos().x() + (begin->width() / 2)), (begin->pos().y() + (begin->height() / 2)),
                     (end->pos().x() + (end->width() / 2)), (end->pos().y() + (end->height() / 2)));
    }
  }
  if(this->m_traceNetworkHardware)
  {
    if(this->m_networkHardwareBegin != "" && this->m_networkHardwareEnd == "")
    {
      if(this->m_networkHardwareType == "WiredLink")
      {
        paint.setPen(pen);
      }
      else if(this->m_networkHardwareType == "WifiLink")
      {
        paint.setPen(point);
      }
      else if(this->m_networkHardwareType == "P2pLink")
      {
        paint.setPen(p2p);
      }
      paint.drawLine((this->GetChildFromName(this->m_networkHardwareBegin))->pos().x() + ((this->GetChildFromName(this->m_networkHardwareBegin))->width() / 2),
          (this->GetChildFromName(this->m_networkHardwareBegin))->pos().y() + ((this->GetChildFromName(this->m_networkHardwareBegin))->height() / 2),
          mapFromGlobal(QCursor::pos()).x(),  mapFromGlobal(QCursor::pos()).y());
    }
  }
  paint.setPen(pen);
}

DragObject* DragWidget::GetChildFromName(const std::string &name)
{
  for(size_t i = 0; i < (size_t)this->children().size(); i++)
  {
    DragObject *child = dynamic_cast<DragObject*>(this->children().at(i));
    if(child)
    {
      if( child->GetName() == name)
      {
        return child;
      }
    }
  }
  return new DragObject(this);
}


void DragWidget::ShowGuiPing()
{
  this->m_appsEnable = false;

  this->m_dialog = new QDialog(this);
  this->m_dialog->setWindowTitle("Ping");

  QGridLayout *layout = new QGridLayout;

  QDialogButtonBox *buttonBox = new QDialogButtonBox(this->m_dialog);
  buttonBox->setStandardButtons(QDialogButtonBox::Ok);//QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

  connect(buttonBox, SIGNAL(accepted()), this->m_dialog, SLOT(accept()));
  connect(buttonBox, SIGNAL(rejected()), this->m_dialog, SLOT(reject()));

  /* MACHINE LEFT */
  std::string label_machineLeft("");
  if(this->m_appsServer != "")
  {
    label_machineLeft = "Sender : " + this->m_appsServer;
  }
  else
  {
    label_machineLeft = "Sender : ___________";
  }

  QLabel *machineLeft = new QLabel(QString(label_machineLeft.c_str()), this->m_dialog);
  layout->addWidget(machineLeft, 2, 0);

  QPushButton *button_machineLeft = new QPushButton("Choose", this->m_dialog);
  connect(button_machineLeft, SIGNAL( clicked() ), this, SLOT( ChooseServer() ) );

  layout->addWidget(button_machineLeft, 2, 1);

  /* MACHINE RIGHT */
  std::string label_machineRight("");
  if(this->m_appsClient != "")
  {
    label_machineRight = "Target : " + this->m_appsClient;
  }
  else
  {
    label_machineRight = "Target : ___________";
  }

  QLabel *machineRight = new QLabel(QString(label_machineRight.c_str()), this->m_dialog);
  layout->addWidget(machineRight, 3, 0);

  QPushButton *button_machineRight = new QPushButton("Choose", this->m_dialog);
  connect(button_machineRight, SIGNAL( clicked() ), this, SLOT( ChooseClient() ) );
  layout->addWidget(button_machineRight, 3, 1);

  /* PARAMS. */
  QLabel *lab_startTime = new QLabel("Start time (s):", this->m_dialog);
  layout->addWidget(lab_startTime, 5, 0);

  QLineEdit *line_startTime = new QLineEdit(this->m_dialog);
  if(this->m_startTime != (size_t) - 1)
  {
    line_startTime->insert(utils::integerToString(this->m_startTime).c_str());
  }
  layout->addWidget(line_startTime, 5, 1);

  QLabel *lab_endTime = new QLabel("End time (s):", this->m_dialog);
  layout->addWidget(lab_endTime, 6, 0);

  QLineEdit *line_endTime = new QLineEdit(this->m_dialog);
  if(this->m_endTime != (size_t) - 1)
  {
    line_endTime->insert(utils::integerToString(this->m_endTime).c_str());
  }
  layout->addWidget(line_endTime, 6, 1);

  /* add OK, CANCEL button */
  QPushButton *cancel = new QPushButton("Cancel", this->m_dialog);
  connect(cancel, SIGNAL( clicked() ), this, SLOT( Cancel() ) );
  layout->addWidget(cancel, 8, 3);
  layout->addWidget(buttonBox, 8, 4);

  this->m_dialog->setLayout(layout);
  this->m_dialog->exec();

  if(this->m_dialog->result() == 1)
  {
    this->m_startTime = line_startTime->text().toInt();
    this->m_endTime = line_endTime->text().toInt();

    if(this->m_appsServer != "" && this->m_appsClient != "" && this->m_startTime != (size_t) - 1 && this->m_startTime != (size_t) - 1)
    {
      if((this->m_startTime < this->m_endTime))
      {
        this->m_mw->ValidApps();
        this->m_appsPing = false;
      }
      else
      {
        QMessageBox::about(this, "Ping", "Start time can't be greather than end time.");
        this->ShowGuiPing();
      }
    }
    else
    {
      if(this->m_appsServer == "" || this->m_appsClient == "")
      {
        QMessageBox::about(this, "ping", "Sorry, you must choose all machines.");
        this->ShowGuiPing();
      }
      else
      {
        if(this->m_startTime == (size_t) - 1 || this->m_endTime == (size_t) - 1)
        {
          QMessageBox::about(this, "Ping", "Sorry, start time and end time can't be null.");
          this->ShowGuiPing();
        }
      }
    }
  }

}

void DragWidget::ChooseServer()
{
  this->m_appsEnable = true;
  this->m_dialog->hide();
  this->m_appsServer = "";
}

void DragWidget::ChooseClient()
{
  this->m_appsEnable = true; 
  this->m_dialog->hide();
  this->m_appsClient = "";
}

void DragWidget::Cancel()
{
  this->m_dialog->hide();
  this->m_appsServer = "";
  this->m_appsClient = "";

  this->m_appsEnable = false;
  this->m_appsPing = false;
  this->m_appsUdpEcho = false;
  this->m_appsTcp = false;
}

void DragWidget::ShowGuiUdpEcho()
{
  this->m_appsEnable = false;

  this->m_dialog = new QDialog(this);
  this->m_dialog->setWindowTitle("Udp Echo");

  QGridLayout *layout = new QGridLayout;

  QDialogButtonBox *buttonBox = new QDialogButtonBox(this->m_dialog);
  buttonBox->setStandardButtons(QDialogButtonBox::Ok);

  connect(buttonBox, SIGNAL(accepted()), this->m_dialog, SLOT(accept()));
  connect(buttonBox, SIGNAL(rejected()), this->m_dialog, SLOT(reject()));

  /* MACHINE LEFT */
  std::string label_machineLeft("");
  if(this->m_appsServer != "")
  {
    label_machineLeft = "Server : " + this->m_appsServer;
  }
  else
  {
    label_machineLeft = "Server : ___________";
  }

  QLabel *machineLeft = new QLabel(QString(label_machineLeft.c_str()), this->m_dialog);
  layout->addWidget(machineLeft, 2, 0);

  QPushButton *button_machineLeft = new QPushButton("Choose", this->m_dialog);
  connect(button_machineLeft, SIGNAL( clicked() ), this, SLOT( ChooseServer() ) );

  layout->addWidget(button_machineLeft, 2, 1);

  /* MACHINE RIGHT */
  std::string label_machineRight("");
  if(this->m_appsClient != "")
  {
    label_machineRight = "Client : " + this->m_appsClient;
  }
  else
  {
    label_machineRight = "Client : ___________";
  }

  QLabel *machineRight = new QLabel(QString(label_machineRight.c_str()), this->m_dialog);
  layout->addWidget(machineRight, 3, 0);

  QPushButton *button_machineRight = new QPushButton("Choose", this->m_dialog);
  connect(button_machineRight, SIGNAL( clicked() ), this, SLOT( ChooseClient() ) );
  layout->addWidget(button_machineRight, 3, 1);

  /* PARAMS. */
  QLabel *lab_startTime = new QLabel("Start time (s):", this->m_dialog);
  layout->addWidget(lab_startTime, 5, 0);

  QLineEdit *line_startTime = new QLineEdit(this->m_dialog);
  if(this->m_startTime != (size_t) - 1)
  {
    line_startTime->insert(utils::integerToString(this->m_startTime).c_str());
  }
  layout->addWidget(line_startTime, 5, 1);

  QLabel *lab_endTime = new QLabel("End time (s):", this->m_dialog);
  layout->addWidget(lab_endTime, 6, 0);

  QLineEdit *line_endTime = new QLineEdit(this->m_dialog);
  if(this->m_endTime != (size_t) - 1)
  {
    line_endTime->insert(utils::integerToString(this->m_endTime).c_str());
  }
  layout->addWidget(line_endTime, 6, 1);

  QLabel *lab_port = new QLabel("Port :", this->m_dialog);
  layout->addWidget(lab_port, 7, 0);

  QLineEdit *line_port = new QLineEdit(this->m_dialog);
  if(this->m_port != (size_t) - 1)
  {
    line_port->insert(utils::integerToString(this->m_port).c_str());
  }
  layout->addWidget(line_port, 7, 1);

  /* add OK, CANCEL button */
  QPushButton *cancel = new QPushButton("Cancel", this->m_dialog);
  connect(cancel, SIGNAL( clicked() ), this, SLOT( Cancel() ) );
  layout->addWidget(cancel, 8, 3);
  layout->addWidget(buttonBox, 8, 4);

  this->m_dialog->setLayout(layout);
  this->m_dialog->exec();

  if(this->m_dialog->result() == 1)
  {
    this->m_startTime = line_startTime->text().toInt();
    this->m_endTime = line_endTime->text().toInt();
    this->m_port = line_port->text().toInt();

    if(this->m_appsServer != "" && this->m_appsClient != "" && this->m_startTime != (size_t) - 1 && this->m_startTime != (size_t) - 1 && this->m_port != (size_t) - 1)
    {
      if((this->m_startTime < this->m_endTime))
      {
        this->m_mw->ValidApps();
        this->m_appsPing = false;
      }
      else
      {
        QMessageBox::about(this, "Udp Echo", "Start time can't be greather than end time.");
        this->ShowGuiUdpEcho();
      }
    }
    else
    {
      if(this->m_appsServer == "" || this->m_appsClient == "")
      {
        QMessageBox::about(this, "Udp Echo", "Sorry, you must choose all machines.");
        this->ShowGuiUdpEcho();
      }
      else
      {
        if(this->m_startTime == (size_t) - 1 || this->m_endTime == (size_t) - 1)
        {
          QMessageBox::about(this, "Udp Echo", "Sorry, start time and end time can't be null.");
          this->ShowGuiUdpEcho();
        }
        else
        {
          if(this->m_port == (size_t) - 1)
          {
            QMessageBox::about(this, "Udp Echo", "Sorry, port field can't be null.");
            this->ShowGuiUdpEcho();
          }
        }
      }
    }
  }

}

void DragWidget::ShowGuiTcp()
{
  this->m_appsEnable = false;

  this->m_dialog = new QDialog(this);
  this->m_dialog->setWindowTitle("Tcp Large Transfer");

  QGridLayout *layout = new QGridLayout;

  QDialogButtonBox *buttonBox = new QDialogButtonBox(this->m_dialog);
  buttonBox->setStandardButtons(QDialogButtonBox::Ok);

  connect(buttonBox, SIGNAL(accepted()), this->m_dialog, SLOT(accept()));
  connect(buttonBox, SIGNAL(rejected()), this->m_dialog, SLOT(reject()));

  /* MACHINE LEFT */
  std::string label_machineLeft("");
  if(this->m_appsServer != "")
  {
    label_machineLeft = "Server : " + this->m_appsServer;
  }
  else
  {
    label_machineLeft = "Server : ___________";
  }

  QLabel *machineLeft = new QLabel(QString(label_machineLeft.c_str()), this->m_dialog);
  layout->addWidget(machineLeft, 2, 0);

  QPushButton *button_machineLeft = new QPushButton("Choose", this->m_dialog);
  connect(button_machineLeft, SIGNAL( clicked() ), this, SLOT( ChooseServer() ) );

  layout->addWidget(button_machineLeft, 2, 1);

  /* MACHINE RIGHT */
  std::string label_machineRight("");
  if(this->m_appsClient != "")
  {
    label_machineRight = "Client : " + this->m_appsClient;
  }
  else
  {
    label_machineRight = "Client : ___________";
  }

  QLabel *machineRight = new QLabel(QString(label_machineRight.c_str()), this->m_dialog);
  layout->addWidget(machineRight, 3, 0);

  QPushButton *button_machineRight = new QPushButton("Choose", this->m_dialog);
  connect(button_machineRight, SIGNAL( clicked() ), this, SLOT( ChooseClient() ) );
  layout->addWidget(button_machineRight, 3, 1);

  /* PARAMS. */
  QLabel *lab_startTime = new QLabel("Start time (s):", this->m_dialog);
  layout->addWidget(lab_startTime, 5, 0);

  QLineEdit *line_startTime = new QLineEdit(this->m_dialog);
  if(this->m_startTime != (size_t) - 1)
  {
    line_startTime->insert(utils::integerToString(this->m_startTime).c_str());
  }
  layout->addWidget(line_startTime, 5, 1);

  QLabel *lab_endTime = new QLabel("End time (s):", this->m_dialog);
  layout->addWidget(lab_endTime, 6, 0);

  QLineEdit *line_endTime = new QLineEdit(this->m_dialog);
  if(this->m_endTime != (size_t) - 1)
  {
    line_endTime->insert(utils::integerToString(this->m_endTime).c_str());
  }
  layout->addWidget(line_endTime, 6, 1);

  QLabel *lab_port = new QLabel("Port :", this->m_dialog);
  layout->addWidget(lab_port, 7, 0);

  QLineEdit *line_port = new QLineEdit(this->m_dialog);
  if(this->m_port != (size_t) - 1)
  {
    line_port->insert(utils::integerToString(this->m_port).c_str());
  }
  layout->addWidget(line_port, 7, 1);

  /* add OK, CANCEL button */
  QPushButton *cancel = new QPushButton("Cancel", this->m_dialog);
  connect(cancel, SIGNAL( clicked() ), this, SLOT( Cancel() ) );
  layout->addWidget(cancel, 8, 3);
  layout->addWidget(buttonBox, 8, 4);

  this->m_dialog->setLayout(layout);
  this->m_dialog->exec();

  if(this->m_dialog->result() == 1)
  {
    this->m_startTime = line_startTime->text().toInt();
    this->m_endTime = line_endTime->text().toInt();
    this->m_port = line_port->text().toInt();

    if(this->m_appsServer != "" && this->m_appsClient != "" && this->m_startTime != (size_t) - 1 && this->m_startTime != (size_t) - 1 && this->m_port != (size_t) - 1)
    {
      if((this->m_startTime < this->m_endTime))
      {
        this->m_mw->ValidApps();
        this->m_appsPing = false;
      }
      else
      {
        QMessageBox::about(this, "Tcp Large Transfer", "Start time can't be greather than end time.");
        this->ShowGuiTcp();
      }
    }
    else
    {
      if(this->m_appsServer == "" || this->m_appsClient == "")
      {
        QMessageBox::about(this, "Tcp Large Transfer", "Sorry, you must choose all machines.");
        this->ShowGuiTcp();
      }
      else
      {
        if(this->m_startTime == (size_t) - 1 || this->m_endTime == (size_t) - 1)
        {
          QMessageBox::about(this, "Tcp Large Transfer", "Sorry, start time and end time can't be null.");
          this->ShowGuiTcp();
        }
        else
        {
          if(this->m_port == (size_t) - 1)
          {
            QMessageBox::about(this, "Tcp Large Transfer", "Sorry, port field can't be null.");
            this->ShowGuiTcp();
          }
        }
      }
    }
  }
}

void DragWidget::SetNetworkHardwareType(const std::string& linkType)
{
  this->m_networkHardwareType = linkType;
}

std::string DragWidget::GetNetworkHardwareType() const
{
  return this->m_networkHardwareType;
}

void DragWidget::SetLastPosition(const QPoint &pos)
{
  this->m_lastPosition = pos;
}

QPoint DragWidget::GetLastPosition()
{
  return this->m_lastPosition;
}

void DragWidget::SetNetworkHardwareBegin(const std::string &linkBegin)
{
  this->m_networkHardwareBegin = linkBegin;
}

std::string DragWidget::GetNetworkHardwareBegin()
{
  return this->m_networkHardwareBegin;
}

MainWindow* DragWidget::GetMainWindow()
{
  return m_mw;
}

void DragWidget::SetTraceNetworkHardware(const bool &state)
{
  this->m_traceNetworkHardware = state;
}

bool DragWidget::GetTraceNetworkHardware()
{
  return this->m_traceNetworkHardware;
}

void DragWidget::SetNetworkHardwareEnd(const std::string &link)
{
  this->m_networkHardwareEnd = link;
}

std::string DragWidget::GetNetworkHardwareEnd()
{
  return this->m_networkHardwareEnd;
}

void DragWidget::SetDragLine(const DragLines &lines)
{
  this->m_lines = lines;
}

DragLines DragWidget::GetDragLines()
{
  return this->m_lines;
}

void DragWidget::AddDrawLine(const DragLines &dl)
{
  this->m_drawLines.push_back(dl);
}

DragLines DragWidget::GetDrawLine(const size_t &index)
{
  return this->m_drawLines.at(index);
}

std::vector<DragLines> DragWidget::GetDrawLines()
{
  return this->m_drawLines;
}

void DragWidget::EraseDrawLine(const size_t &index)
{
  this->m_drawLines.erase(this->m_drawLines.begin() + index);
}

void DragWidget::SetTimer(QTimer *timer)
{
  this->m_timer = timer;
}

QTimer* DragWidget::GetTimer()
{
  return this->m_timer;
}
  
void DragWidget::SetAppsServer(const std::string &appsServ)
{
  this->m_appsServer = appsServ;
}

std::string DragWidget::GetAppsServer()
{
  return this->m_appsServer;
}

void DragWidget::SetAppsClient(const std::string &appsCli)
{
  this->m_appsClient = appsCli;
}

std::string DragWidget::GetAppsClient()
{
  return this->m_appsClient;
}

void DragWidget::SetAppsPing(const bool &appsPing)
{
  this->m_appsPing = appsPing;
}

bool DragWidget::GetAppsPing()
{
  return this->m_appsPing;
}

void DragWidget::SetAppsUdpEcho(const bool &appsUdp)
{
  this->m_appsUdpEcho = appsUdp;
}

bool DragWidget::GetAppsUdpEcho()
{
  return this->m_appsUdpEcho;
}

void DragWidget::SetAppsTcp(const bool &appsTcp)
{
  this->m_appsTcp = appsTcp;
}

bool DragWidget::GetAppsTcp()
{
  return this->m_appsTcp;
}

void DragWidget::SetStartTime(const size_t &start)
{
  this->m_startTime = start;
}

size_t DragWidget::GetStartTime()
{
  return this->m_startTime;
}

void DragWidget::SetEndTime(const size_t &end)
{
  this->m_endTime = end;
}

size_t DragWidget::GetEndTime()
{
  return this->m_endTime;
}

void DragWidget::SetPort(const size_t &port)
{
  this->m_port = port;
}

size_t DragWidget::GetPort()
{
  return this->m_port;
}

void DragWidget::SetDialog(QDialog *dialog)
{
  this->m_dialog = dialog;
}

QDialog* DragWidget::GetDialog()
{
  return this->m_dialog;
}

void DragWidget::SetAppsEnable(const bool &state)
{
  this->m_appsEnable = state;
}

bool DragWidget::GetAppsEnable()
{
  return this->m_appsEnable;
}

std::string DragWidget::UpdateToolTip(const std::string &childName)
{
  std::string  new_tooltip = childName;
  std::vector<std::string> connectedTo;
  bool isConnected = false;
  for(size_t i = 0; i < this->m_mw->GetGenerator()->GetNNetworkHardwares(); i++)
  {
    for(size_t j = 0; j < this->m_mw->GetGenerator()->GetNetworkHardware(i)->GetInstalledNodes().size(); j++)
    {
      if( this->m_mw->GetGenerator()->GetNetworkHardware(i)->GetInstalledNodes().at(j) == childName )
      {
        std::string iface("");
        isConnected = true;
        if(this->m_mw->GetGenerator()->GetNetworkHardware(i)->GetNetworkHardwareName().find("emu_") == 0) 
        {
          iface = " (" + dynamic_cast<Emu*>(this->m_mw->GetGenerator()->GetNetworkHardware(i))->GetIfaceName() + ")";
        }
        else if( this->m_mw->GetGenerator()->GetNetworkHardware(i)->GetNetworkHardwareName().find("tap_") == 0)
        {
          iface = " (" + dynamic_cast<Tap*>(this->m_mw->GetGenerator()->GetNetworkHardware(i))->GetIfaceName() + ")";
        }
        
        connectedTo.push_back("10.0." + utils::integerToString(i) + "." + utils::integerToString(j + 1) + iface);
      }
    }
  }
  if(isConnected)
  {
    for(size_t i = 0; i < connectedTo.size(); i++)
    {
      new_tooltip += "<br />"+connectedTo.at(i);
    }
  }

  for(size_t i = 0;  i < this->m_mw->GetGenerator()->GetNApplications(); i++)
  {
    if(this->m_mw->GetGenerator()->GetApplication(i)->GetSenderNode() == childName)
    {
      new_tooltip += "<br />Sender on " + this->m_mw->GetGenerator()->GetApplication(i)->GetAppName();
    }
    else if(this->m_mw->GetGenerator()->GetApplication(i)->GetReceiverNode() == childName)
    {
      new_tooltip += "<br />Receiver on " + this->m_mw->GetGenerator()->GetApplication(i)->GetAppName();
    }
  }
  return new_tooltip;
}

